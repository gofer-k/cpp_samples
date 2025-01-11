module;
#include <algorithm>
#include <barrier>
#include <numeric> // Add this line to include the <numeric> header
#include <print>
#include <ranges>
#include <vector>
#include <thread>

using product_data = std::vector<int>;

void print_collection(const product_data& coll) {
  std::for_each(coll.begin(), coll.end(), [](const auto& elem) {
      std::print("{} ", elem); 
  });
  std::println();
}

void on_completion() {
  std::println("on_completion thread {}", std::this_thread::get_id());
}
using barrier_t = std::barrier<decltype(on_completion)*>;


struct chunk_data {
  product_data chunk_;
  chunk_data(product_data chunk) : chunk_(std::move(chunk)) {
  }

  void work_chunk(barrier_t& sync_point) {    
    std::transform(chunk_.begin(), chunk_.end(), chunk_.begin(), [](int i) { return i * 2; });
    sync_point.arrive_and_wait();
  }
};

struct producer {
  product_data data_;  
  std::vector<std::jthread> thd_;

  producer(product_data data) : data_(std::move(data)) {       
  }

  void operator()() {
    std::println("---Barrier sample");
    print_collection(data_);

    const auto data_size = std::size(data_);
    const auto num_chunks = (data_size >= 2) ?  data_size / 2 : data_size;    

    std::vector<chunk_data> chunks;    
    chunks.reserve(num_chunks);

    for (std::size_t i = 0; i < num_chunks; ++i) {
      product_data chunk;
      std::ranges::copy(data_ | std::views::drop(i * num_chunks) | std::views::take(num_chunks), std::back_inserter(chunk));
      chunks.emplace_back(std::move(chunk));
    }

    barrier_t barrier(std::ssize(chunks), on_completion);

    thd_.reserve(num_chunks);
    for (auto& chunk : chunks) {
      thd_.emplace_back(&chunk_data::work_chunk, &chunk, std::ref(barrier));
    }
    
    std::for_each(chunks.begin(), chunks.end(), [](const auto& chunk) {
      print_collection(chunk.chunk_);
    });
    const auto total_reduces_values = std::transform_reduce(chunks.begin(), chunks.end(), 0, std::plus<int>(), [](const auto& chunk) {
      return std::reduce(chunk.chunk_.begin(), chunk.chunk_.end(), 0);
    });
    std::println("Total reduced elements's values: {}", total_reduces_values);
  } 
};

export module barrier_samples;

export void test_barrier_sample() {
  product_data data = std::ranges::iota_view{0, 10} | std::ranges::to<product_data>();
  
  producer prod(std::move(data));
  prod();
}