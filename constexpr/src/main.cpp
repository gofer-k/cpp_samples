#include <print>

import allocations_samples;
import consteval_samples;
import constexpr_samples;
import trivial_init;

int main() {
  // auto res = try_modify_value(2); // not compile
  {
    auto res = inc_value(23);
    std::println("inc_value = {}", res);
  }
  {
    constexpr auto val = 233; // OK
    // auto val = 23;  // not usable pass to consteval function
    auto res = inc_value(val);
    std::println("val = {}, inc_value = {}", val, res);
  }
  std::println("get_sum = {}", get_sum(2, 3));

  // constexpr auto str = get_string(); // Error, but it's non constexpr at
  // compile-time. std::println("constexpr string: {}", str);
  std::println("constexpr string: {}",
               get_string()); // OK pass result from consttexpr fun.

  trivial_initialization();

  test_constant_evaluation();
  auto res = is_in_constexpr(23);
  auto carray_size = get_carray_size(23);
  auto array_size = get_std_array_size(23);
  std::println("is_in_constexpr = {}, carray_size = {}, array_size = {}", res,
               carray_size, array_size);
  return 0;
}