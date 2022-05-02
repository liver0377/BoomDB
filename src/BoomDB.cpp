#include "BoomDB.h"
// #include <string>

namespace BoomDB {

void BoomDB::start() {
  while (true) {
    print_prompt();

    std::string input_str;
    std::getline(std::cin, input_str);

    if (parse_meta_command(input_str)) {
      continue;
    }
  }
}

/**
 * @brief 打印终端提示符
 *
 */
void BoomDB::print_prompt() { std::cout << "Boom > "; }

/**
 * @brief  meta_command指的是以'.'开头的命令
 *         该函数专门用于解析metadata
 *
 * @param input_str    输入命令
 * @return true        输入命令可识别
 * @return false       输入命令不可识别
 */
bool BoomDB::parse_meta_command(const std::string &input_str) {
  if (input_str == ".exit") {
    std::cout << "Bye!" << std::endl;
    exit(EXIT_SUCCESS);
  } else {
    std::cout << "unrecognized command: " << input_str << std::endl;
    return true;
  }
  return false;
}

/**
 * @brief 解析sql语句，转换成对应的statement
 *
 * @param input_str  输入命令
 * @param statement[out] sql statement
 * @return true  解析成功
 * @return false 解析失败
 */
bool BoomDB::parse_statement(const std::string &input_str,
                             Statement &statement) {
  switch (prepare_statement(input_str, statement)) {
  case PREPARE_SUCCESS:
    return true;
  case PREPARE_UNRECOGNIZED_STATEMENT:
    std::cout << "Uncorecognized command" << std::endl;
    return false;
  }
  return true;
}

/**
 * @brief 对sql语句进行预处理
 *
 * @param input_str  输入命令
 * @param statement  sql语句对应的statement类型
 * @return PrepareResult
 */
PrepareResult BoomDB::prepare_statement(const std::string &input_str,
                                        Statement &statement) {
  if (!input_str.compare(0, 6, "insert")) {
    statement.type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  } else if (!input_str.compare(0, 6, "select")) {
    statement.type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  } else {
    return PREPARE_UNRECOGNIZED_STATEMENT;
  }
}

/**
 * @brief 执行一个statement
 *
 * @param statement
 */
void BoomDB::execute_statement(const Statement &statement) {
  switch (statement.type) {
  case STATEMENT_INSERT:
    std::cout << "executing insert statement" << std::endl;
    break;
  case STATEMENT_SELECT:
    std::cout << "executing select statement" << std::endl;
    break;
  }
}
} // namespace BoomDB