#include <coroutine>
#include <generator>
#include <memory>
#include <utility>
#include <vector>

namespace CoTreeTranversal {

struct Node {
    int value{};
    Node* left_{};
    Node* right_{};
};

std::generator<Node*> CoPreOrder(Node* root) {
  if (root == nullptr) {
    co_return;
  }

  co_yield root;  
  co_yield std::ranges::elements_of(CoPreOrder(root->left_));
  co_yield std::ranges::elements_of(CoPreOrder(root->right_));
}

std::generator<Node*> CoInOrder(Node* root) {
  if (root == nullptr) {
    co_return;
  }
  co_yield std::ranges::elements_of(CoInOrder(root->left_));
  co_yield root;
  co_yield std::ranges::elements_of(CoInOrder(root->right_));
}

std::generator<Node*> CoPostOrder(Node* root) {
  if (root == nullptr) {
    co_return;
  }
  co_yield std::ranges::elements_of(CoPostOrder(root->left_));  
  co_yield std::ranges::elements_of(CoPostOrder(root->right_));
  co_yield root;
}

using Tree = std::vector<std::unique_ptr<Node>>;
Tree BuildTree() {
  Tree tree;

  tree.push_back(std::make_unique<Node>(4));
  auto root = tree.back().get();

  tree.push_back(std::make_unique<Node>(2));
  auto left = tree.back().get();
  root->left_ = left;

  tree.push_back(std::make_unique<Node>(6));
  auto right = tree.back().get();
  root->right_ = right;

  tree.push_back(std::make_unique<Node>(1));
  left->left_ = tree.back().get();

  tree.push_back(std::make_unique<Node>(3));
  left->right_ = tree.back().get();

  tree.push_back(std::make_unique<Node>(5));
  right->left_ = tree.back().get();

  tree.push_back(std::make_unique<Node>(7));
  right->right_ = tree.back().get();

  return tree;
}
} // namespace