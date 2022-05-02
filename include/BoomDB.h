#pragma once

#include <iostream>
#include <string>

namespace BoomDB {

enum MetaCommandResult { RECOGNIZED, UNRECOGNIZED };

// PrepareResult是prepare_statement()的返回类型
enum PrepareResult { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT };

enum StatementType { STATEMENT_INSERT, STATEMENT_SELECT };

struct Statement {
  StatementType type;
};

class BoomDB {
public:
  void start();
  void print_prompt();
  bool parse_meta_command(const std::string &);
  bool parse_statement(const std::string &, Statement &);

  MetaCommandResult do_meta_command(const std::string &);
  PrepareResult prepare_statement(const std::string &, Statement &);
  void execute_statement(const Statement &);
};

} // namespace BoomDB