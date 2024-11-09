#include <print>  // include without a module

import root_module;

int main() {
  /// root_module
  {
    HellWorld exported_type;
    exported_type.print();
    print_fun();
  }
  std::println("Message from main");
  return 0;
}