// Cudos from: https://oleksandrkvl.github.io/2021/04/02/cpp-20-overview.html#lambda-features
#include <memory>
#include <print>

template<typename T>
std::unique_ptr<T> move_unique_ptr(std::unique_ptr<T> && ptr) {
  return ptr; // copied in C++17(thus, error), moved in C++20, OK
}

std::string move_string(std::string && x) {
  return x;   // copied in C++17, moved in C++20
}

struct Widget{
  Widget() = default;
  Widget(const Widget&) {
    std::println("Copy Widget");
  }
  Widget(Widget&&) {
    std::println("Move Widget");
  }
};

void move_with_throw(Widget arg){
  throw arg;    // copied in C++17, moved in C++20
}

struct From {
  From(Widget const &) {
    std::println("Copy From(Widget)");
  }
  From(Widget&&) {
    std::println("Move From(Widget)");
  }
};

struct To {
  operator Widget() const & {
    std::println("Copy Widget from To type");
    Widget w;
    return w;
  };
  operator Widget() && {
    std::println("Move Widget from To type");
    Widget w;
    return w;
  }
};

From from_move() {
  Widget w;
  return w;  // moved (no NRVO because of different types)
}

Widget to_move() {
    To t;
    return t;// copied in C++17(conversions were not considered), moved in C++20
}

struct A{
  A(const Widget&) {
    std::println("Copy A(Widget)");
  }
  A(Widget&&) {
    std::println("Move A(Widget)");
  }
};

struct B{
  B(Widget) {
    std::println("B(Widget) c-tor");
  }
};

A test5() {
    Widget w;
    return w;  // moved
}

B test6() {
    Widget w;
    return w; // copied in C++17(because there's no B(Widget&&)), moved in C++20
}

struct Derived : Widget{
  Derived() {
    std::println("Derived c-tor");
  }

  Derived(const Derived&) {
    std::println("Copy Derived");
  }
  Derived( Derived&&) {
    std::println("Move Derived");
  }
};

std::shared_ptr<Widget> test7() {
  std::shared_ptr<Derived> result;
  return result;  // moved
}

Widget test8() {
  Derived result;
  // copied in C++17(because there's no Base(Derived)), moved in C++20
  return result;
}

int main() {
  {
    std::println("test8()");
    auto res = test8();
  }
  std::println("test7()");
  auto shared_arg = test7();
  {
    std::println("test6()");
    auto res = test6();
  }  
  {
    std::println("test5()");
    auto res = test5();
  }
  { 
    std::println("Test to_move()");
    auto res = to_move();
  }  
  {
    std::println("Test from_move()");
    auto res = from_move();
  }

  return 0;
}