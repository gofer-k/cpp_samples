#include <algorithm>
#include <vector>
#include <ranges>
#include <functional>

#include "converts.hpp"

namespace ranges_filters {

std::vector<int> filter_range(const std::vector<int> &v, std::function<bool(int)> filter_pred) {
  auto result_view = v | std::views::filter(filter_pred);
  return converts::to_vector(result_view);
}

}