#include "BoomDB.h"

namespace BoomDB {

void BoomDB::start() {
  while (true) {
    print_prompt();

    std::string input_str;
    std::getline(std::cin, input_str);

    if (parse_meta_commad(input_str)) {
      continue;
    }
  }
}

void BoomDB::print_prompt() { std::cout << "Boom > "; }

bool BoomDB::parse_meta_commad(std::string input_str) {
  if (input_str == ".exit") {
    std::cout << "Bye!" << std::endl;
    exit(EXIT_SUCCESS);
  } else {
    std::cout << "unrecognized command: " << input_str << std::endl;
    return true;
  }
  return false;
}

} // namespace BoomDB