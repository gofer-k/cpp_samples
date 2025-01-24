module;
#include <cstdint>
#include <chrono>
#include <print>

// Export the module partition
export module user_utilities:benchmark;

export template<typename Func>
void benchmark(Func func, std::size_t num_iterations, std::string name) {
  auto start = std::chrono::high_resolution_clock::now();
  while (num_iterations-- > 0) 
  {
    func();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::println("Benchmark {} took: {} ms", name, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());     
}