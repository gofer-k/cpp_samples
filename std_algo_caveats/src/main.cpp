#include <cstdint>
#include <algorithm>
#include <print>
#include <vector>

template<typename T>
void print_vector(const std::vector<T>& vec) {
  std::for_each(vec.begin(), vec.end(), [](auto elem) {
    std::print("{} ", elem);
  });
  std::println();
}

int main() {
  std::vector<std::uint32_t> orig = {1, 2, 4, 2, 3, 1, 45, 3, 6, 1};
  print_vector(orig);

  {
    auto vec{orig};
    // erase algo move speficited elements to the end, trash then in thge end.
    // This is here, the vector first element is the reference, but
    // the second element origin vector will be the first after the first algo iteration
    std::erase(vec, vec.front());
    std::println("After erase elements");
    print_vector(vec);
  }
  {    
    auto vec{orig};
    auto copy = [](const auto& value) {
      return value;
    };

    std::println("--- erase elements with copied specfied value");
    // std::erase(vec, auto{vec.front()});
    std::erase(vec, copy(vec.front()));
    std::println("After erase elements works expected");
    print_vector(vec);
  }

  return 0;
}