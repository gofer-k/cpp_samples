module;
#include <map>
#include <print>
#include <string>

export module stateless_lambda;

auto less_compare = [](auto lhs, auto rhs) {
  return lhs < rhs;  
};

// Compile since C++20
export std::map<std::string, std::uint32_t, decltype(less_compare)> map_constructible_type; 
// Same above
export auto map_assignable = map_constructible_type;