#include "page/page.h"

namespace BoomDB {
Page::Page(uint64 page_id, uint16 flags, uint16 count, uint32 overflow,
           uint32 ptr)
    : page_id_(page_id), flags_(flags), count_(count), overflow_(overflow),
      ptr_(ptr) {}

/** 获取Page的内部成员 */
uint64 Page::get_page_id() { return page_id_; }
uint16 Page::get_flags() { return flags_; }
uint16 Page::get_count() { return count_; }
uint32 Page::get_overflow() { return overflow_; }
uint32 Page::get_ptr() { return ptr_; }

/** 设置Page的内部成员 */
void Page::set_count(uint16 count) { count_ = count; }
void Page::set_flags(PageFlags flag) { flags_ |= flag; }

} // namespace BoomDB