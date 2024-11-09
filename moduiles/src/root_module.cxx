// Global module partintion
// Raw headers can br included here
module;
#include <print>  // OK

// first thing after the Global module fragment must be a module command
export module root_module;

// Exported function from this module (main_module)
export void print_fun() {
  std::println("Exported print_fun from main+_module");
}

// Exported custom user type from tis module  (main_modules)
export struct HellWorld {
  HellWorld() = default;
  ~HellWorld() = default;

  void print() {
    std::println("Hello world");
  }
};
