#include "page/page.h"

namespace BoomDB {
uint64 Page::get_page_id() { return page_id_; }
uint16 Page::get_flags() { return flags_; }
uint16 Page::get_count() { return count_; }
uint32 Page::get_overflow() { return overflow_; }
uint32 Page::get_ptr() { return ptr_; }


} // namespace BoomDB