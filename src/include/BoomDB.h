#pragma once

#include <iostream>
#include <string>

namespace BoomDB {
class BoomDB {
public:
  void start();
  void print_prompt();
  bool parse_meta_commad(std::string);
};


} // namespace BoomDB