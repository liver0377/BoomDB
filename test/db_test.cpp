#include "BoomDB.h"
#include "gtest/gtest.h"
#include <iostream>
#include <limits.h>
#include <string>

namespace BoomDB {
// TEST(MetaCommandTest, Exit) {
//   BoomDB boom;
//   ::testing::internal::CaptureStdout();
//
//   bool ans = boom.parse_meta_command(".exit");
//   std::string str = ::testing::internal::GetCapturedStdout();
//   EXPECT_TRUE(ans);
//   EXPECT_EQ("bye\n", str);
// }

TEST(MetaCommandTest, Unrecognized) {
  BoomDB boom;
  ::testing::internal::CaptureStdout();

  bool ans = boom.parse_meta_command(".hh");
  std::string str = ::testing::internal::GetCapturedStdout();
  EXPECT_TRUE(ans);
  EXPECT_EQ("unrecognized command: .hh\n", str);
}

TEST(StatementTest, Insert) {
  BoomDB boom;
  PrepareResult prepare_result;
  bool ans;
  Statement statement;
  fflush(stdout);
  ::testing::internal::CaptureStdout();

  std::string input_str = "insert";
  std::string output_str;

  prepare_result = boom.prepare_statement(input_str, statement);
  EXPECT_EQ(prepare_result, PREPARE_SUCCESS);

  ans = boom.parse_statement(input_str, statement);
  EXPECT_TRUE(ans);

  boom.execute_statement(statement);
  output_str = ::testing::internal::GetCapturedStdout();
  EXPECT_EQ(output_str, "executing insert statement\n");
}

TEST(StatementTest, Select) {
  BoomDB boom;
  PrepareResult prepare_result;
  bool ans;
  Statement statement;
  fflush(stdout);
  ::testing::internal::CaptureStdout();

  std::string input_str = "select";
  std::string output_str;

  prepare_result = boom.prepare_statement(input_str, statement);
  EXPECT_EQ(prepare_result, PREPARE_SUCCESS);

  ans = boom.parse_statement(input_str, statement);
  EXPECT_TRUE(ans);

  boom.execute_statement(statement);
  output_str = ::testing::internal::GetCapturedStdout();
  EXPECT_EQ(output_str, "executing select statement\n");
}
// class BoomDBTest : public ::testing::Test {
// protected:
//   void SetUp() override {}
//
//   void TearDown() override {}
// };

// TEST_F(BoomDBTest, Start) {}
} // namespace BoomDB