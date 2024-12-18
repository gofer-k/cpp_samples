module;
#include <print>
#include <string>
#include <variant>
#include <vector>
#include <iomanip>
export module visit_variant;

// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

export void test_visit_variant() {
  using var_t = std::variant<int, long, double, std::string>;

  var_t var{std::in_place_index<0>, 42};

  // 1. void visitor, only called for side-effects (here, for I/O)
  std::visit([](auto&& arg) { std::println("Variant visit: {}", arg); }, var);

  // 2. value-returning visitor, demonstrates the idiom of returning another variant
  var_t var_2{std::in_place_index<1>, 234256};
  var_t w = std::visit([](auto&& arg) -> var_t { return arg + arg; }, var_2);
  std::println("Variant visit: {}", std::get<long>(w));

  // 3. type-matching visitor: a lambda that handles each type differently
  std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, int>)
      std::println("int with value {}", arg);
    else if constexpr (std::is_same_v<T, long>)
      std::println("long with value {}", arg);
    else if constexpr (std::is_same_v<T, double>)
      std::println("double with value {}", arg);
    else if constexpr (std::is_same_v<T, std::string>)
      std::println("std::string with value {}", arg);
    else
      static_assert(false, "non-exhaustive visitor!");
  }, w);

  std::vector<var_t> vec{var_t{10}, var_t{15l}, var_t{1.5}, var_t{"hello"}};
  for (auto& v: vec)
  {
      // 4. another type-matching visitor: a class with 3 overloaded operator()'s
      // Note: The `(auto arg)` template operator() will bind to `int` and `long`
      //       in this case, but in its absence the `(double arg)` operator()
      //       *will also* bind to `int` and `long` because both are implicitly
      //       convertible to double. When using this form, care has to be taken
      //       that implicit conversions are handled correctly.
      std::visit(overloaded{
        [](auto arg) { std::println("{} ", arg); },
        [](double arg) { std::println("{:.3f} ", arg); },
        [](const std::string& arg) { std::println("\"{}\" ", arg); }
      }, v);
  }
}