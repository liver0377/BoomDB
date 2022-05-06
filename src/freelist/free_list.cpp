#include "freelist/free_list.h"
#include "const.h"

namespace BoomDB {
/**
 * @brief 将ids以及pending合并成一个有序序列
 *
 * @param dst[out] 结果序列
 */
void Freelist::copyAll(pgids &dst) {
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
  uint16 count = p->get_count();

  p->set_flags(PageFlags::FREE_LIST_PAGE);
  if (count == 0) {
    p->set_count(count);
  } else if (count < 0xFFFF) {
    p->set_count(count);
    // 将ids以及pending拷贝到Page的数据部分
    auto data = reinterpret_cast<std::vector<pgid> *>(p->get_ptr());
    copyAll(*data);
  } else {
    // 在data指向的第一个id区块记录元素个数
    p->set_count(0xFFFF);
    auto data = reinterpret_cast<std::vector<pgid> *>(
        static_cast<uint64>(p->get_ptr()) + 1);
    copyAll(*data);
  }
}

/**
 * @brief 将freelist page的内容写入到freelist结构体
 *
 * @param p 给定page
 */
void Freelist::Read(Page *p) {
  int count = p->get_count();

  if (count == 0xFFFF) {
    auto data = reinterpret_cast<pgid *>(static_cast<uint64>(p->get_ptr()) + 1);
    count = static_cast<int>(*data);
  } else if (count == 0) {
    ids = {};
  } else {
    for (int i = 0; i < count; i++) {
      auto data =
          reinterpret_cast<pgid *>(static_cast<uint64>(p->get_ptr()) + i);
      ids.push_back(*data);
      // 感觉没有必要
      std::sort(ids.begin(), ids.end());
    }
  }
}

/**
 * @brief 建立freelist的缓存
 *
 */
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
      return beg;
    }
  }
  return 0;
}
} // namespace BoomDB