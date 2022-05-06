#pragma once

#include "type.h"

namespace BoomDB {
enum PageFlags { BRANCH_PAGE, LEAF_PAGE, META_PAGE, FREE_LIST_PAGE };

// 64 + 16 + 16 + 32 + 32 = 160 bit
class Page {
public:
  uint64 get_page_id();
  uint16 get_flags();
  uint16 get_count();
  uint32 get_overflow();
  uint32 get_ptr();

private:
  uint64 page_id_;  // 页号
  uint16 flags_;    // 表明该页属于哪一种page
  uint16 count_;    // page当中存储了多少个记录
  uint32 overflow_; // 溢出页
  uint32 ptr_;      // 指向数据
};

/**
 * @brief Meta存储相关元数据
 *
 */
struct Meta {
  uint32 magic;     // 用于表示该文件是一个BoomDB实例文件
  uint32 page_size; // Page的大小
  bucket root;      // 根节点
  pgid freelist;    // 空闲链表
  uint64 checksum;  // 检验和
};
} // namespace BoomDB