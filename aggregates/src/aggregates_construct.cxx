module;
#include <cstdint>
#include <type_traits>
export module aggregates;
export void test_aggregate_construct();

struct CtorDeleted {
  CtorDeleted(std::uint32_t) = delete;
  std::uint32_t field_ {0};
};

struct CtorDefaulted {
  CtorDefaulted() = default;
  std::uint32_t field_ {0};
};

struct CtorUser {
  CtorUser(std::uint32_t arg) : field_(arg) {}
  std::uint32_t field_ {0};
};

struct Aggregate {
  std::uint32_t field_ {0};
};

void test_aggregate_construct() {
  static_assert(not std::is_aggregate_v<CtorDeleted>);
  static_assert(not std::is_aggregate_v<CtorDefaulted>);
  static_assert(not std::is_aggregate_v<CtorUser>);
  static_assert(std::is_aggregate_v<Aggregate>);

  // CtorDeleted x(2);      // error due to deleted c-tor
  // CtorDefaulted x1;      // OK in C++17, error since C++20
  // CtorDefaulted x2{static_cast<std::uint32_t>(3)};  // OK in C++17, error since C++20
  CtorUser x3{4};           // OK
  Aggregate x4{5};          // OK
}