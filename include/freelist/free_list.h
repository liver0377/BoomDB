#pragma once

#include "type.h"
#include <map>
#include <vector>

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
  
private:
  // 已经可以被分配的空闲页
  std::vector<pgid> ids;
  // 事务正在使用，使用完毕后马上就会被释放的页
  std::map<txid, std::vector<pgid>> pending;
  // 缓存页
  std::map<pgid, bool> cache;
};
