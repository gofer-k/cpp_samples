module;
#include <exception>

export module trivial_init;

struct NonTrivial {
  bool field_ = false;
};

struct Trivial {
  bool field_;
};

template <typename T>
constexpr T copy(const T& other) {
    T t;        // default initialization
    t = other;
    return t;
}

template <typename T>
constexpr auto get_trivial(const T& other) {
    T t;
    if constexpr (requires(const T& obj) { obj.field_; } ) {
      return t.field_;
    }
    else {
      std::terminate();
    }
}

export void trivial_initialization(){
    constexpr auto a = copy(Trivial{});    // error in C++17, OK in C++20
    constexpr auto b = copy(NonTrivial{}); // OK

    // constexpr auto c = get_trivial(Trivial{});    // Error, uninitialized Trivial::b is used
    constexpr auto d = get_trivial(NonTrivial{}); // OK
}