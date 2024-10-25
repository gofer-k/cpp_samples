#include <coroutine>
// std23
#include <generator>
#include <utility>

namespace CoGenerator {

/// @brief Fibonatcci algorithim with  std coroutine generator feature.
/// Compile C++23 onward.
/// @param n 
/// @return yield values utmost n range limit. 
std::generator<int> Fibonacci(int n) {
  int prev = 0;
  int next = 1;

  for (;n > 0; --n) {
    {
      // this algorithm work from C++20 to onward      
      // co_yield prev;
      // int cur = prev + next;
      // prev = next;
      // next = cur;
    }
    {
      // This work from C++23 to onward/ 
      co_yield std::exchange(prev, std::exchange(next, prev + next));
    }
  }
}

} // namespace