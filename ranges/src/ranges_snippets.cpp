#include <algorithm>
#include <print>
#include <vector>
#include "filters.hpp"
#include "ranges_init_loop.hpp"

template<typename Range>
void print_vector(const Range& rag) {
  std::ranges::for_each(rag, [](auto elem) {
    std::print("{} ", elem);
  });
  std::println();
}

template<typename Range>
void print_vector_with_index(const Range& rag) {
  auto elem_with_index = 
    // mutable allows change captured idx among calls
    [idx = std::size_t{0}](const auto& elem) mutable{
      // Thanks CTAD allows specific the following:
      // std::pair<std::size_t, std::ranges::range_value_t<decltype(Rag)>>(..)
      return std::pair(idx++, elem);
    };

  for(const auto& [idx, elem] : rag | std::ranges::views::transform(elem_with_index)) {
    std::print("[{}: {}] ", idx, elem);
  }
  std::println();
}

int main() {
  {    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto result = ranges_filters::filter_range(numbers, [](int num) {
      return num % 2 == 0; // Filter even numbers
    });
    std::println("---Filter range");
    print_vector(result);
  }
  
  test_init_range_loop();
  
  {
    std::vector<std::uint32_t> vec{1,24,5, 465, 3486, 79, 8};
    std::println("--Pipeline ranges views: Origin vec:");
    print_vector_with_index(vec);
    auto res = vec | std::ranges::views::drop(1) | std::ranges::views::take(4);
    std::println("Result vec:");
    print_vector_with_index(res);
  }  

  return 0;
}