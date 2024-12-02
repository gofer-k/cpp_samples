module;
#include <print>
#include <string>

export module aggregates_ctad;

template <typename T, typename U> struct User {
  T t_;
  U u_;
};

// user ctag deduction needed in C++17
template <typename T, typename U> User(T, U) -> User<T, U>;

template <typename T> struct Convert {
  T data_;
};
Convert(const char *) -> Convert<std::string>;

template <typename T, std::size_t N> struct Array {
  T data[N];
};

export void test_agg_ctad() {
  User x{1, 2.0}; // Ok since C++20 without user ctag deduction (lines 10-11)
  std::println("User({}, {})", x.t_, x.u_);

  Convert x1{"abc"}; // OK, Convert<std::string> using deduction guide
  std::println("Convert({})", x1.data_);

  Convert<int> x2{1}; // OK, explicit specialization
  std::println("Convert<T>({})", x1.data_);

  // Convert x3{1};      // Error, CTAD isn't involved

  Array<int, 3u> not_ctag{{1, 2, 3}}; // Ok below C++20
  Array a{{1, 2, 3}}; // Ok since C++20 without user ctag deduction
  Array str{"hello"}; // Ok as above
}