module;
#include <print>
#include <string>

export module aggredates_pack_expasion;

template <typename... Ts>
struct Overloads : Ts... {
  using Ts::operator()...;    // functior for all pac elements
};

export void test_pack_expasion() {
  Overloads functor{
    [](int){ std::println("Called with int"); },
    [](char){ std::print("Called with char"); },
    [](std::string arg){ std::println("{}", arg); }
  };

  functor(1);
  functor('c');
  functor("Called std::string");
}