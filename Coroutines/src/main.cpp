
#include "co_generator.hpp"
#include "co_tree_traversal.hpp"
#include "custom_coroutine.hpp"
#include <print>

int main() {
  {
    std::println("Fibonacci generator: ");
    for (int fib : CoGenerator::Fibonacci(10)) {
      std::println("{}", fib);
    }
  }
  {
    auto tree = CoTreeTranversal::BuildTree();
    auto root = tree.front().get();

    std::println("CoPreOrder: ");
    for (auto node : CoTreeTranversal::CoPreOrder(root)) {
      std::print("{} ", node->value);
    }
    std::println("");

    std::println("CoInOrder: ");
    for (auto node : CoTreeTranversal::CoInOrder(root)) {
      std::print("{} ", node->value);
    }
    std::println("");

    std::println("CoPostOrder: ");
    for (auto node : CoTreeTranversal::CoPostOrder(root)) {
      std::print("{} ", node->value);
    }
    std::println("");
  }  

  {
    // Custtom coroutine
    std::println("Custom coroutine");    
    auto coro = [] -> CustomCoroutine::Coroutine<int> {
      co_return 123;
    };    

    auto res = coro();
    std::println("Custom coroutine result: {}", res.get());

    // std::println("Custom coroutine result: {}", coro().get());

    //TODO: debug SIGFAULT at the end executng coroutine
  }
}