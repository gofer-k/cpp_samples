module;
#include <functional>
#include <print>
#include <type_traits>

export module callable_features;

struct Foo {
  Foo(int num) : num_(num) {}
  int add(int arg) const {
    const auto res = num_ + arg;
    std::println("Foo:::num + arg {}", res);
    return res;
  }

private:
  int num_{};
};

int add(int arg1, int arg2) {
  const auto res = arg1 + arg2;
  std::println("print_add: {}", res);
  return res;
}

struct PrintNum {
  void operator()(int arg) const { std::println("arg {}", arg); }
};

constexpr int multiply(int x, int y) { return x * y; }

export namespace callable_features {

void test_callable_features() {
  Foo obj{123};

  // Return type is same as the return type of the function
  {
    auto res = std::invoke(&Foo::add, obj, 100);
    using method_return_type =
        std::invoke_result_t<decltype(&Foo::add), Foo, int>;
    static_assert(std::is_same_v<method_return_type, decltype(res)>,
                  "Return type is same as the return type of the function");
  }
  {
    auto res = std::invoke(add, 100, 200);
    static_assert(std::is_same_v<std::invoke_result_t<decltype(add), int, int>,
                                 decltype(res)>,
                  "Return type is same as the return type of the function");
  }

  { std::invoke(PrintNum{}, 100); }

  {
    // Nothing is returned from the function
    std::invoke_r<void>(PrintNum{}, 200);
  }
  {
    // Enforce return type - ordinal function pointer
    auto res = std::invoke_r<double>(add, 100, 200);
    static_assert(not std::is_same_v<decltype(add), decltype(res)>);
  }
  {
    // Enforce return type - constexpr function
    auto res = std::invoke_r<float>(multiply, 100, 200);
  }
}
} // namespace callable_features