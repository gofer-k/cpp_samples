module;
#include <algorithm>
#include <print>
#include <ranges>

export module utilities;

export template <typename Range>
void print_vector(const Range &rng) {
  std::ranges::for_each(rng, [](auto elem) { std::print("{} ", elem); });
  std::println();
}

export template <typename Range>
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