#include "freelist/free_list.h"
#include "const.h"

namespace BoomDB {

Freelist::Freelist() {}

// 测试函数
pgids Freelist::getPending(txid tid) { return pending[tid]; }
pgids Freelist::getIds() { return ids; }
void Freelist::setIds(pgids page_ids) { ids = page_ids; }
void Freelist::setPending(txid tid, pgids pids) {
  pending.erase(tid);
  pending[tid] = pids;
}

/** @brief 返回freelist 中的page个数 */
int Freelist::getCount() { return getfreeCount() + getPendingCount(); }

/** @brief 返回freelist中真正可以分配的page个数 */
int Freelist::getfreeCount() { return ids.size(); }

/** @brief 返回freelist中处于pending 状态下的page个数 */
int Freelist::getPendingCount() {
  int ans = 0;
  for (auto iter = pending.begin(); iter != pending.end(); iter++) {
    ans += iter->second.size();
  }
  return ans;
}

/**
 * @brief 将ids以及pending合并成一个有序序列
 *
 * @param dst[out] 结果序列
 */
void Freelist::copyAll(pgid *dst) {
  pgids tmp;
  // pendings是一个pgids为value的map
  for (auto &list : pending) {
    for (auto &e : list.second) {
      tmp.push_back(e);
    }
  }
  std::sort(tmp.begin(), tmp.end());
  MergePgids(ids, tmp, dst);
}

/**
 * @brief 将freelist信息写入到page当中
 * @details
 * 当freelist中页的个数超过了0xFFFF时，就在data的第一个id条目记录空闲页个数
 *          同时将p.count设置为0xFFFF
 * @param p 指定page
 */
void Freelist::Write(Page *p) {
  uint16 count = getCount();

  p->set_flags(PageFlags::FREE_LIST_PAGE);

  if (count == 0) {
    p->set_count(count);
  } else if (count < 0xFFFF) {
    p->set_count(count);
    // 将ids以及pending拷贝到Page的数据部分
    auto data = reinterpret_cast<pgid *>(&(p->get_ptr()));
    copyAll(data);
  } else {
    // 在data指向的第一个id区块记录元素个数
    p->set_count(0xFFFF);
    auto data = reinterpret_cast<pgid *>(&(p->get_ptr()));
    data[0] = count;
    auto data_1 = reinterpret_cast<pgid *>(&(p->get_ptr())) + 1;
    copyAll(data_1);
  }
}

/**
 * @brief 将freelist page的内容写入到freelist结构体
 *        这里全部写入了freelist的ids字段当中
 * @param p 给定page
 */
void Freelist::Read(Page *p) {
  int count = p->get_count();

  if (count == 0xFFFF) {
    auto data = reinterpret_cast<pgid *>(&(p->get_ptr()));
    count = static_cast<int>(*data);
  }
  if (count == 0) {
    ids = {};
  } else {
    for (int i = 0; i < count; i++) {
      auto data = reinterpret_cast<pgid *>(&(p->get_ptr())) + i;
      ids.push_back(*data);
    }
    std::sort(ids.begin(), ids.end());
  }
}

/**
 * @brief
 * 将一个page中内容读入freelist结构，与read不同的时，其同时还会过滤掉处于pending
 * 状态的page
 *
 * @param p  待读入的page
 */
void Freelist::Reload(Page *p) {
  Read(p);

  std::map<pgid, bool> tmp;
  // 1. 为所有pending page建立到哈希表
  for (auto iter = pending.begin(); iter != pending.end(); iter++) {
    for (int j = 0; j < iter->second.size(); j++) {
      tmp[iter->second[j]] = true;
    }
  }
  // 2. 过滤掉pending page
  pgids new_ids;
  for (int i = 0; i < ids.size(); i++) {
    if (!tmp.count(ids[i])) {
      new_ids.push_back(ids[i]);
    }
  }
  ids = new_ids;

  // 3. 重新建立缓存
  Reindex();
}

/** @brief 建立freelist的缓存  */
void Freelist::Reindex() {
  for (auto &id : ids) {
    cache[id] = true;
  }
  for (auto &list : pending) {
    for (auto &id : list.second) {
      cache[id] = true;
    }
  }
}

/**
 * @brief 尝试分配一个连续的长度为n的块
 * @return 如果找到了，返回第一个块的pgid
 *         如果没有找到，返回0
 */
pgid Freelist::Allocate(int n) {
  if (ids.size() < n) {
    return 0;
  }

  int beg = 0, prev = 0;
  for (int i = 0; i < ids.size(); i++) {
    if (ids[i] <= 1) {
      throw std::logic_error("id can't <= 1\n");
    }
    if (prev == 0 || ids[i] - prev != 1) {
      // 遇到了不连续的id
      beg = ids[i];
    }
    if (ids[i] - beg + 1 == n) {
      // 找到了满足条件的n个块
      // 将i后面的所有元素前移n位
      for (int j = i + 1; j < ids.size(); j++) {
        ids[j - n] = ids[j];
      }
      int original = ids.size();
      ids.resize(original - n);
      // 更新缓存
      for (int i = 0; i < n; i++) {
        cache.erase(i + beg);
      }
      return beg;
    }
    prev = ids[i];
  }
  return 0;
}

/**
 * @brief 释放一个与某个事务相关的page以及其相关的溢出page到freelist中
 *        注意，这里并没有真正的释放到freelist的ids列表当中，而是释放到了pending[txid]中
 * @param tid 给定事务
 * @param p  待释放的page
 */
void Freelist::Free(txid tid, Page *p) {
  pgid page_id = p->get_page_id();
  if (page_id <= 1) {
    throw std::logic_error("id can't <= 1\n");
  }

  auto ids = pending[tid];
  for (pgid id = page_id; id <= page_id + p->get_overflow(); id++) {
    if (cache[id] == true) {
      throw std::logic_error("can't free a page twice\n");
    }

    ids.push_back(id);
    // 设置缓存
    cache[id] = true;
  }
  pending[tid] = ids;
}

/**
 * @brief 将某个事务本身以及之前的所有事务有关的所有page全部移除到freelist中
 *        该方法的实际操作是将pending[txid]的所有page全部移动到freelist的ids列表当中
 * @param tid 待释放的事务
 */
void Freelist::Release(txid tid) {
  pgids new_ids;
  pgids pending_ids;
  for (auto iter = pending.begin(); iter != pending.end();) {
    txid cur_tid = iter->first;
    pgids cur_list = iter->second;
    // 更老的事务
    if (cur_tid <= tid) {
      pending_ids.insert(pending_ids.end(), cur_list.begin(), cur_list.end());
      // erase删除指定元素后会返回下一个元素的迭代器
      iter = pending.erase(iter);
    } else {
      iter++;
    }
  }
  std::sort(pending_ids.begin(), pending_ids.end());
  // 有序合并旧ids与pending_ids
  pgid *p = new pgid[ids.size() + pending_ids.size()];
  MergePgids(ids, pending_ids, p);
  for (int i = 0; i < ids.size() + pending_ids.size(); i++) {
    new_ids.push_back(p[i]);
  }
  ids = new_ids;
  delete p;
}

/**
 * @brief 将一个处于pending的事务的有关page全部从freelist中移出
 *
 * @param tid
 */
void Freelist::Rollback(txid tid) {
  // 1. 将pending 事务的page从缓存中移出
  for (int i = 0; i < pending[tid].size(); i++) {
    cache.erase(pending[tid][i]);
  }
  // 2. 将pending 事务的page从pending list中移出
  pending.erase(tid);
}

/** @return 返回一个给定的page是否存在freelist当中 */
bool Freelist::freed(pgid pid) { return cache[pid]; }

} // namespace BoomDB