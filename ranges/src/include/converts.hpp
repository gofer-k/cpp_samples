#include <vector>
#include <ranges>

namespace converts {

template <typename R>
constexpr auto to_vector(R&& r) {
  using elem_t = std::decay_t<std::ranges::range_value_t<R>>;
  return std::vector<elem_t>{r.begin(), r.end()};
}


}