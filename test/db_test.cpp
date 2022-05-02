#include "BoomDB.h"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <limits.h>
#include <string>
// #include <strstream>

namespace BoomDB {
TEST(BoomTest, Start) {
  BoomDB boom;
  bool ans;
  ans = boom.parse_meta_commad("hello");
  EXPECT_TRUE(ans);
  //   ans = boom.parse_meta_commad(".exit");
  //   EXPECT_TRUE(ans);
}

class BoomDBTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(BoomDBTest, Start) {
  
}
} // namespace BoomDB