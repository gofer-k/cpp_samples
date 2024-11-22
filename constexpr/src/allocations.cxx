module;
#include <array>
#include <algorithm>
#include <string>

export module allocations_samples;

export constexpr auto get_string() {
  std::string str{"Hello"};
  std::string space{" "};
  std::string str2{"world"};

  return str + space + str2;
}

export constexpr auto get_array_strings() {
  // constexpr auto str = get_string(); // Error: copy from non constexpr value at compile-time.
  constexpr auto N = get_string().size();
  std::array<char, N> arr{};
  std::copy_n(get_string().data(), N, std::begin(arr));
  return arr;
}