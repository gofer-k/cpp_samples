module;
#include <print>
#include <tuple>

export module structured_binding;

struct BitField {
  int a : 1 {1}; // bit-field
  int b : 1 {1}; // bit-field
  int c{4};
};

struct Aggregate {
  int a{1}; // bit-field
  int b{2}; // bit-field
  int c{4};

  friend void foo();

private:
  int d{8};
};

export void test_structured_bindings() {
  static auto [A, B, C] = BitField{};

  // Structured bindings with filtered members
  {
    thread_local auto [a, b, c] = BitField();
    // Ok:
    auto capture_by_value = [=]() { return a + b + c; };
    std::println("Structured binding capture_capture by value in a lambda: {}",
                capture_by_value());

    // Error, can't capture bit-fields by-reference
    auto capture_by_ref = [&]() { return a + b; };
    std::println("Structured binding capture_capture by ref in a lambda: {}",
                capture_by_ref());
  }
  
  // Structured bindings filtered members
  {
    auto agg = Aggregate{};
    auto [a, b, c] = std::tie(agg.a, agg.b, agg.c);
  }
}

void foo() {
  Aggregate agg;
  auto binding_members = agg.d; // OK - access private member
  auto [x, y, z, d] = agg;      // OK - all members are bound
}

export void test_structured_bindings_members() { foo(); }