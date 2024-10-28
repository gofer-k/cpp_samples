
#include "co_generator.hpp"
#include "co_lazy_function.hpp"
#include "co_tree_traversal.hpp"
#include "custom_coroutine.hpp"
#include <print>

void fibonaccii_std_generator(int n) {
std::println("Fibonacci generator: ");
  for (int fib : CoGenerator::Fibonacci(n)) {
    std::print("{} ", fib);
  }
  std::println();
}


void fibonacci_custom_generatpr(int n) {
  auto fibonacci = [](int limit) -> CoGenerator::CustomGenerator<int> {
    int prev = 0;
    int next = 1;
    for (;limit > 0; --limit) {      
        co_yield std::exchange(prev, std::exchange(next, prev + next));
    }
    co_return;
  };
  
  std::println("Fibonacci algorithm with custom coroutine generator: ");
  auto res = fibonacci(n);
  while (not res.exhausted()){
    std::print("{} ", res.get());
  }
  std::println();
}

void co_tree_tranversal() {
auto tree = CoTreeTranversal::BuildTree();
  auto root = tree.front().get();

  std::println("Tree traversal with coroutines");
  std::println("---CoPreOrder: ");
  for (auto node : CoTreeTranversal::CoPreOrder(root)) {
    std::print("{} ", node->value);
  }
  std::println("");

  std::println("---CoInOrder: ");
  for (auto node : CoTreeTranversal::CoInOrder(root)) {
    std::print("{} ", node->value);
  }
  std::println("");

  std::println("---CoPostOrder: ");
  for (auto node : CoTreeTranversal::CoPostOrder(root)) {
    std::print("{} ", node->value);
  }
  std::println();
}

void custom_coroutine() {
// Custtom coroutine
  std::println("Custom coroutine");    
  auto coro = [] -> CustomCoroutine::Coroutine<int> {
    throw std::runtime_error("Runtime exception...");
    co_return 123;
  };    

  try {
    auto res = coro();
    std::println("Custom coroutine result: {}", res.get());
    // The same above
    std::println("Custom coroutine result: {}\n", coro().get());
  }
  catch (const std::exception& e) {
  }
}

void lazy_coroutine(){
  // Lazy coroutine
  auto lazyCoro = [] -> LazyCoro::LazyCoroutine<int> {
    std::println("Running lazy coroutine...");
    co_return 345;
  };

  // Coroutine instantation and the result is pass to the caller 
  auto x = lazyCoro();
  auto res = x.get();
  auto res2 = x.get();
  std::println("res = {}, res2 = {}\n", res, res2);        
}

int main() {
  fibonaccii_std_generator(10);
  fibonacci_custom_generatpr(10);
  co_tree_tranversal();
  custom_coroutine();
  lazy_coroutine();
}