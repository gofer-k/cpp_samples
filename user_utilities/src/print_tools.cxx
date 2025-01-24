module;
#include <algorithm>
#include <print>
#include <ranges>
#include <cstdint>
#include <chrono>

// Export the module partition
export module user_utilities:print_tools;

export namespace user_utilities {

template <typename Range>
void print_vector(const Range &rng) {
  std::ranges::for_each(rng, [](auto elem) { std::print("{} ", elem); });
  std::println();
}

template <typename Range>
void print_vector_with_index(const Range &rng) {
  auto elem_with_index =
      // mutable allows change captured idx among calls
      [idx = std::size_t{0}](const auto &elem) mutable {
        // Thanks CTAD allows specific the following:
        // std::pair<std::size_t, std::ranges::range_value_t<decltype(rng)>>(..)
        return std::pair(idx++, elem);
      };

  for (const auto &[idx, elem] :
       rng | std::ranges::views::transform(elem_with_index)) {
    std::print("[{}: {}] ", idx, elem);
  }
  std::println();
}

// template<typename Func>
// void benchmark(Func func, std::size_t num_iterations) {
//   auto start = std::chrono::high_resolution_clock::now();
//   while (num_iterations-- > 0) 
//   {
//     func();
//   }
//   auto end = std::chrono::high_resolution_clock::now();
//   std::println("Benchmark took: ", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), "ms");     
// }
}  // namespace user_utilities