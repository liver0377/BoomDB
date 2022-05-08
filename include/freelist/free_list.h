#pragma once

#include "page/page.h"
#include "type.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

namespace BoomDB {
class Freelist {
public:
  Freelist();
  /** @return 串行化之后的page大小 */
  int size();
  /** @return 当前freelist中的page数目 */
  int getCount();
  /** @return free pages的个数 */
  int getfreeCount();
  /** @return pending 状态的page个数 */
  int getPendingCount();

  void Write(Page *);
  void Read(Page *);
  void Reindex();
  void Free(txid, Page *);
  void Release(txid);
  void Rollback(txid);
  void Reload(Page *);

  pgid Allocate(int);

  bool freed(pgid);

  // 测试函数
  pgids getPending(txid);
  pgids getIds();
  void setIds(pgids);
  void setPending(txid, pgids);

  // private:
  /** @brief 将freelist中的ids以及pending全部拷贝到dst中 */
  void copyAll(pgid *dst);

private:
  // 已经可以被分配的空闲页
  std::vector<pgid> ids;
  // 事务正在使用，使用完毕后马上就会被释放的页
  std::map<txid, pgids> pending;
  // 缓存页
  std::map<pgid, bool> cache;
};

/** @brief 有序合并a, b到dst */
void MergePgids(const pgids &a, const pgids &b, pgid *dst) {
  int i = 0, j = 0, k = 0;

  while (i < a.size() && j < b.size()) {
    if (a[i] < b[j]) {
      dst[k++] = a[i];
      i++;
    } else {
      dst[k++] = b[j];
      j++;
    }
  }
  while (i < a.size()) {
    dst[k++] = a[i];
    i++;
  }
  while (j < b.size()) {
    dst[k++] = b[j];
    j++;
  }
}
} // namespace BoomDB