#include <algorithm>
#include <iostream>
#include <vector>
#include "filters.hpp"
#include "ranges_init_loop.hpp"

int main() {
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  auto result = ranges_filters::filter_range(numbers, [](int num) {
    return num % 2 == 0; // Filter even numbers
  });

  std::ranges::for_each(result, [](int num) {
    std::cout << num << " ";}
    );
  std::cout << std::endl;
  
  test_init_range_loop();
  
  return 0;
}