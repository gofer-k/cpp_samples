module;
#include <cassert>
#include <cstdint>

export module constexpr_samples;

struct Base
{
  constexpr Base(std::uint32_t val) : value_(val) {}

  constexpr virtual ~Base() = default;
  virtual int get() const = 0; // non-constexpr

protected:
  std::uint32_t value_;
};

struct Derived : Base
{
  constexpr Derived(std::uint32_t val) : Base(val) {}

  constexpr int get() const override
  {
    return value_;
  }
};

export constexpr auto get_sum(std::uint32_t a, std::uint32_t b)
{
  const Derived d1(a);
  const Derived d2(b);
  const Base *pb1 = &d1;
  const Base *pb2 = &d2;

  return pb1->get() + pb2->get();
}

static_assert(get_sum(1, 2) == 1 + 2); // evaluated at compile-time