
#include "co_awaitable_abstraction.hpp"
#include "co_generator.hpp"
#include "co_iterator.hpp"
#include "co_lazy_function.hpp"
#include "co_scheduler.hpp"
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

void fibonacci_custom_generatpr_enabled_range_interface(int n) {  
  auto fibonacci = [](int limit) -> CoIterator::CoGenerator<int> {
    int prev = 0;
    int next = 1;
    for (;limit > 0; --limit) {      
        co_yield std::exchange(prev, std::exchange(next, prev + next));
    }
    co_return;
  };
  
  std::println("Fibonacci algorithm with custom coroutine generator with range API: ");
  for (auto fib : fibonacci(10)) {
    std::print("{} ", fib);
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

void multi_tasks(int number_tasks) {
  std::println("Multi co-tasks");
  auto tasks = [number_tasks] ->  CoMultiTask::CoTask {
    auto countdown = number_tasks;
    for (; countdown > 1; --countdown) {
      std::println("The task ({})", countdown);
      co_await CoMultiTask::CoScheduler{}.wake_up();
    }
  };

  for(; number_tasks > 1; --number_tasks) {
    tasks().detach();
  }
  // Complete multiktask
  tasks().detach();
  
  CoMultiTask::CoScheduler().run();
  std::println();
}


void times_multi_tasks(int number_tasks) {
  std::println("Multi co-tasks");
  auto tasks = [number_tasks] ->  CoMultiTask::CoTaskTimed {
    auto countdown = number_tasks;
    auto time =  std::chrono::system_clock::now();
    for (; countdown > 1; --countdown) {
      std::println("The task ({} time: {})", countdown, std::chrono::system_clock::now());
      co_await (time);
      time += std::chrono::milliseconds(200);
    }
  };

  for(; number_tasks > 1; --number_tasks) {
    tasks().detach();
  }
  // Complete multiktask
  tasks().detach();
  
  CoMultiTask::CoScheduler().run();
  std::println();
}

void awaitable_abstration() {
  std::println("Awaitable abstraction");
  
  CoAwaitableInterface::parent().run_until_completion();
  CoAwaitableInterface::other_parent(234325).run_until_completion();
}

int main() {
  fibonaccii_std_generator(10);
  fibonacci_custom_generatpr(10);
  fibonacci_custom_generatpr_enabled_range_interface(10);
  co_tree_tranversal();
  custom_coroutine();
  lazy_coroutine();
  multi_tasks(3);
  times_multi_tasks(3);
  awaitable_abstration();
}