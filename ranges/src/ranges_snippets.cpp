#include <algorithm>
#include <print>
#include <vector>
#include "filters.hpp"
#include "ranges_init_loop.hpp"

template<typename Range>
void print_vector(const Range& rng) {
  std::ranges::for_each(rng, [](auto elem) {
    std::print("{} ", elem);
  });
  std::println();
}

template<typename Range>
void print_vector_with_index(const Range& rng) {
  auto elem_with_index = 
    // mutable allows change captured idx among calls
    [idx = std::size_t{0}](const auto& elem) mutable{
      // Thanks CTAD allows specific the following:
      // std::pair<std::size_t, std::ranges::range_value_t<decltype(rng)>>(..)
      return std::pair(idx++, elem);
    };

  for(const auto& [idx, elem] : rng | std::ranges::views::transform(elem_with_index)) {
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
    {
      // Ranges pipeline expresions is excuted from the left
      auto res = vec | std::ranges::views::drop(1) | std::ranges::views::take(4);
      std::println("Result vec | drop(1) | take(4): ");
      print_vector_with_index(res);
    }
    {
      // Ranges pipeline expresions is excuted from the left
      auto res = vec | std::ranges::views::take(4) | std::ranges::views::drop(1);
      std::println("Result vec | take(4) | drop(1): ");
      print_vector_with_index(res);
    }
    // composition views and algorithms {
    {
      auto print_elem = [](const auto& elem) {
        std::print("{} ", elem);
      };

      auto is_even = [](const auto& elem) {
        return elem % 2 == 0;
      };
      std::vector<std::uint32_t> vec{2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
      // Take the leading values as soon asDrop leading odd value and The rest of the vector 
      auto after_leading_view = std::ranges::views::drop_while(vec, is_even);

      // print the view
      std::ranges::for_each(after_leading_view, print_elem);
      std::println();
      // Take the leading event value as soon as odd number appeared
      auto leading_view = std::ranges::views::take_while(vec, is_even);

      // print the view
      std::ranges::for_each(leading_view, print_elem);
      std::println();

      auto print_chunks = [printer = print_elem](const auto& chunk) {
        std::ranges::for_each(chunk, printer);
        std::println("");
      };

      auto chunk_view = vec | std::views::chunk(4);
      std::ranges::for_each(chunk_view, print_chunks);
      std::println();
    }    
  }  

  return 0;
}