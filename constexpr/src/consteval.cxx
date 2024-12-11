module;
#include <cstdint>
#include <limits>
#include <type_traits>
#include <cassert>
#include <array>

export module consteval_samples;

export consteval std::uint32_t inc_value(std::uint32_t val) {
  if (val < std::numeric_limits<std::uint32_t>::max()) {
    return ++val;
  }
  return val;
}

export consteval std::uint32_t product(std::uint32_t val, std::uint32_t fact) {
  // consteval functions are allowed and return a result
  return inc_value(val) * inc_value(fact);
}

constexpr int get_number() {
  // if (std::is_constant_evaluated()) { // should not be `if constexpr` - C++20
  // The same as above 
  if consteval {  // C++23
    return 1;
  }
  return 2;
}

constexpr int get_number(int x) {
  if (std::is_constant_evaluated()) { // should not be `if constexpr`
    return x;
  }
  return x + 1;
}

export void test_constant_evaluation() {
  constexpr auto v1 = get_number();
  const auto v2 = get_number();

  // initialization of a non-const variable, not constant-evaluated
  auto v3 = get_number();

  assert(v1 == 1);
  assert(v2 == 1);
  assert(v3 == 2);

  constexpr auto v4 = get_number(1);
  int x = 1;

  // x is not a constant-expression, not constant-evaluated
  const auto v5 = get_number(x);

  assert(v4 == 1);
  assert(v5 == 2);

  static_assert(product(2, 4) == 15); // 2++ * 4++ -> 5
}

export constexpr bool is_in_constexpr(int) {
  if constexpr (std::is_constant_evaluated()) { // always `true`
    return true;
  }
  return false;
}

export constexpr std::size_t get_carray_size(int) {
  int arr[std::is_constant_evaluated()]; // always int arr[1];
  return sizeof(arr);
}

// always returns `1`
export constexpr std::size_t get_std_array_size(int) {
  std::array<int, std::is_constant_evaluated()> arr; // std::array<int, 1>
  return arr.size();
}