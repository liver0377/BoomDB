#include "freelist/free_list.h"
#include "page/page.h"
#include "gtest/gtest.h"

namespace BoomDB {
TEST(FreeListTest, Free) {
  Freelist list;
  Page p(10, 0, 0, 0, 0);
  list.Free(100, &p);
  EXPECT_EQ(1, list.getCount());
  EXPECT_EQ(0, list.getfreeCount());
  EXPECT_EQ(1, list.getPendingCount());
}

TEST(FreeListTest, Overflow) {
  Freelist list;
  // 3个溢出页
  Page p(10, 0, 0, 3, 0);
  list.Free(100, &p);
  EXPECT_EQ(4, list.getPendingCount());
}

TEST(FreeListTest, Release) {
  Freelist list;
  Page p1(12, 0, 0, 1, 0);
  Page p2(9, 0, 0, 0, 0);
  Page p3(39, 0, 0, 0, 0);

  list.Free(100, &p1);
  list.Free(100, &p2);
  list.Free(102, &p3);
  list.Release(100);
  list.Release(101);

  pgids res;
  res = list.getIds();
  EXPECT_EQ(res, pgids({9, 12, 13}));

  list.Release(102);
  res = list.getIds();
  EXPECT_EQ(res, pgids({9, 12, 13, 39}));
}

TEST(FreelistTest, Allocate) {
  Freelist list;
  list.setIds(pgids({3, 4, 5, 6, 7, 9, 12, 13, 18}));
  EXPECT_EQ(3, list.Allocate(3));           // 3, 4, 5
  EXPECT_EQ(6, list.Allocate(1));           // 6
  EXPECT_EQ(0, list.Allocate(3));           // 0
  EXPECT_EQ(12, list.Allocate(2));          // 12, 13
  EXPECT_EQ(7, list.Allocate(1));           // 7
  EXPECT_EQ(0, list.Allocate(0));           // 0
  EXPECT_EQ(0, list.Allocate(0));           // 0
  EXPECT_EQ(pgids({9, 18}), list.getIds()); // 9, 18
  EXPECT_EQ(9, list.Allocate(1));
  EXPECT_EQ(18, list.Allocate(1)); // 18
  EXPECT_EQ(0, list.Allocate(1));  // 0
  EXPECT_EQ(pgids({}), list.getIds());
}

TEST(FreelistTest, Read) {
  auto p = reinterpret_cast<Page *>(malloc(4096));
  p->set_flags(FREE_LIST_PAGE);
  p->set_count(2);
  auto data = reinterpret_cast<pgid *>(&p->get_ptr());
  data[0] = 23;
  data[1] = 50;

  Freelist list;
  list.Read(p);
  EXPECT_EQ(pgids({23, 50}), list.getIds());
}

TEST(FreelistTest, CopyAll) {
  pgids dst;
  Freelist list;
  list.setIds({1, 3, 5, 7});
  list.setPending(100, {8, 2, 4});
  list.setPending(101, {6});
  pgid *p = new pgid[8];
  list.copyAll(p);
  pgids tmp;
  for (int i = 0; i < 8; i++) {
    tmp.push_back(p[i]);
  }
  delete p;
  EXPECT_EQ(pgids({1, 2, 3, 4, 5, 6, 7, 8}), tmp);
}

TEST(FreelistTest, Write) {
  auto p = reinterpret_cast<Page *>(malloc(4096));
  p->set_flags(FREE_LIST_PAGE);

  Freelist list;
  list.setIds(pgids{12, 32});
  list.setPending(100, pgids({28, 11}));
  list.setPending(101, pgids({3}));

  list.Write(p);
  Freelist new_list;
  auto data = reinterpret_cast<pgid *>(&p->get_ptr());

  new_list.Read(p);
  EXPECT_EQ(pgids({3, 11, 12, 28, 32}), new_list.getIds());
}

TEST(FreelistTest, Release) {
  Page p1(12, 0, 0, 1, 0);
  Page p2(9, 0, 0, 0, 0);
  Page p3(39, 0, 0, 0, 0);
  Freelist list;
  list.Free(100, &p1);
  list.Free(100, &p2);
  list.Free(102, &p3);
  list.Release(100);
  list.Release(101);

  EXPECT_EQ(pgids({9, 12, 13}), list.getIds());
  list.Release(102);
  EXPECT_EQ(pgids({9, 12, 13, 39}), list.getIds());
}
} // namespace BoomDB