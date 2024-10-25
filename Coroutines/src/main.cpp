
#include "co_generator.hpp"
#include "co_tree_traversal.hpp"
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
}