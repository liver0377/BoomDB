#pragma once

#include "page/page.h"
#include "type.h"
#include <algorithm>
#include <map>
#include <vector>
#include <stdexcept>

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

  void Write(Page *p);
  void Read(Page *p);
  void Reindex();
  pgid Allocate(int n);

private:
  /** @brief 将freelist中的ids以及pending全部拷贝到dst中 */
  void copyAll(pgids &dst);

private:
  // 已经可以被分配的空闲页
  std::vector<pgid> ids;
  // 事务正在使用，使用完毕后马上就会被释放的页
  std::map<txid, pgids> pending;
  // 缓存页
  std::map<pgid, bool> cache;
};

/** @brief 有序合并a, b到dst */
void MergePgids(const pgids &a, const pgids &b, pgids &dst) {
  int i = 0, j = 0;

  while (i < a.size() && j < b.size()) {
    if (a[i] < b[j]) {
      dst.push_back(a[i]);
      i++;
    } else {
      dst.push_back(b[j]);
      j++;
    }
  }
  while (i < a.size()) {
    dst.push_back(a[i++]);
  }
  while (j < b.size()) {
    dst.push_back(b[j++]);
  }
}
} // namespace BoomDB