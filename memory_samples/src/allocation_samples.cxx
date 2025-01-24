module;
#include <array>
#include <cstdint>
#include <list>
#include <memory_resource>

export module memory_samples;
import user_utilities;

constexpr std::size_t iterations = 100;
constexpr std::size_t elems = 10'000;

export namespace memory_samples {

void test_default_alloc(const std::size_t num_nodes = iterations,
                        const std::size_t num_iterations = elems) {
  auto default_alloc = [&num_nodes]() {
    std::list<int> list;
    for (std::size_t i = 0; i < num_nodes; ++i) {
      list.push_back(i);
    }
  };

  benchmark(default_alloc, num_iterations, "std::list with standard allocator");
}

void test_default_pmr_alloc(const std::size_t num_nodes = iterations,
                            const std::size_t num_iterations = elems) {
  auto default_pmr_alloc = [&num_nodes]() {
    std::pmr::list<int> list;
    for (std::size_t i = 0; i < num_nodes; ++i) {
      list.push_back(i);
    }
  };
  benchmark(default_pmr_alloc, num_iterations,
            "pmr::list with allocator without monotonic_buffer_resource");
}
template <std::size_t Size = elems>
void test_custom_pmr_alloc_with_buff(const std::size_t num_iterations = elems) {
  auto custom_pmr_alloc_with_buff = [num_nodes = Size]() {
    std::array<std::byte, Size> buffer;
    std::pmr::monotonic_buffer_resource pool(buffer.data(), buffer.size());
    std::pmr::polymorphic_allocator<int> alloc(&pool);
    std::pmr::list<int> list{alloc};
    //  list.set_memory_resource(&pool);  // This is also valid

    for (std::size_t i = 0; i < num_nodes; ++i) {
      list.push_back(i);
    }
  };
  benchmark(custom_pmr_alloc_with_buff, num_iterations,
            "pmr::list with polymorphic allocator with custom default "
            "monotonic_buffer_resource");
}

void test_custom_no_buff(const std::size_t num_nodes = iterations,
                         const std::size_t num_iterations = elems) {
  auto custom_no_buff = [&num_nodes]() {
    std::pmr::monotonic_buffer_resource pool;
    std::pmr::polymorphic_allocator<int> alloc(&pool);
    std::pmr::list<int> list{alloc};
    for (std::size_t i = 0; i < num_nodes; ++i) {
      list.push_back(i);
    }
  };
  benchmark(custom_no_buff, num_iterations,
            "pmr::list with polymorphic allocator with default "
            "monotonic_buffer_resource");
}

} // namespace memory_samples