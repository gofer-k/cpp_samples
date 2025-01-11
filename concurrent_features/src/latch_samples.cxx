module;
#include <algorithm>
#include <latch>
#include <numeric> // Add this line to include the <numeric> header
#include <print>
#include <ranges>
#include <vector>
#include <thread>

export module latch_samples;

using product_data = std::vector<int>;

void print_collection(const product_data& coll) {
  std::for_each(coll.begin(), coll.end(), [](const auto& elem) {
      std::print("{} ", elem); 
  });
  std::println();
}

struct chunk_data {
  product_data chunk_;
  chunk_data(product_data chunk) : chunk_(std::move(chunk)) {
  }

  void work_chunk(std::latch &latch) {    
    std::transform(chunk_.begin(), chunk_.end(), chunk_.begin(), [](int i) { return i * 2; });
    std::println("work_chunk thread {}", std::this_thread::get_id());
    latch.count_down();
  }
};

struct producer {
  product_data data_;  
  std::vector<std::jthread> thd_;

  producer(product_data data) : data_(std::move(data)) {       
  }

  void operator()() {
    std::println("---Latch sample");
    print_collection(data_);

    const auto data_size = std::size(data_);
    const auto num_chunks = (data_size >= 4) ?  data_size / 4 : data_size;
    
    std::latch latch(num_chunks);

    std::vector<chunk_data> chunks;    
    chunks.reserve(num_chunks);

    for (std::size_t i = 0; i < num_chunks; ++i) {
      product_data chunk;
      std::ranges::copy(data_ | std::views::drop(i * 4) | std::views::take(4), std::back_inserter(chunk));
      chunks.emplace_back(std::move(chunk));
    }

    //  latch.wait(); // Deadlock
    thd_.reserve(num_chunks);
    for (auto& chunk : chunks) {
      thd_.emplace_back(&chunk_data::work_chunk, &chunk, std::ref(latch));
    }
    latch.wait();   // OK here

    std::for_each(chunks.begin(), chunks.end(), [](const auto& chunk) {
      print_collection(chunk.chunk_);
    });   

    const auto total_reduces_values = std::transform_reduce(chunks.begin(), chunks.end(), 0, std::plus<int>(), [](const auto& chunk) {
      return std::reduce(chunk.chunk_.begin(), chunk.chunk_.end(), 0);
    });
    std::println("Total reduced elements's values: {}", total_reduces_values);
  }

};

export void test_latch_sample() {
  // C++20: std::latch is a synchronization primitive that can be used to block
  // a group of threads until a specified number of threads have reached a
  // certain point in their execution.
  product_data data = std::ranges::iota_view{0, 10} | std::ranges::to<product_data>();
  
  producer prod(std::move(data));
  prod();  
}