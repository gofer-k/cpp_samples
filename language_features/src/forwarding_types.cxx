module;
#include <type_traits>
#include <print>
#include <utility>

export module forwarding_types;

struct Foo {
  int x = 32;

  auto&& get(this auto&& self) {       
    if constexpr (std::is_lvalue_reference_v<decltype(self)>) {
      if constexpr(std::is_const_v<std::remove_reference_t<decltype(self)>>) {
        std::println("This type is const lvalue reference");
      } else {
        std::println("This type is lvalue reference");
      }
    } else {
      if constexpr ( std::is_const_v<std::remove_reference_t<decltype(self)>>) {
        std::println("This type is const rvalue reference");
      } else {
        std::println("This type is rvalue reference");
      }
    }
    return std::forward_like<decltype(self)>(self.x);
  }
};


export namespace forwarding_types {
  void test_forward_like() {
    Foo foo;    

    auto&& x = foo.get();
    static_assert(std::is_reference_v<decltype(x)>, "Expected int&"); 
    static_assert(std::is_lvalue_reference_v<decltype(foo.get())>, "Expected int&");
    static_assert(std::is_rvalue_reference_v<decltype(std::move(foo).get())>, "Expected int&");

    const Foo cfoo;
    auto&& cx = cfoo.get();
    static_assert(std::is_const_v< std::remove_reference_t<decltype(cx)> >, "Expected const int&");
  }

  void test_forwarding_types() {  
    int x = 42;
    auto&& ref_x = x;
    static_assert(std::is_reference_v<decltype(ref_x)>, "int&");        

    int& rx = x;
    auto&& ref_x2 = rx;
    static_assert(std::is_reference_v<decltype(ref_x2)>, "int&");   

    const int& crx = x;
    auto&& cref_x = crx;
    static_assert(std::is_const_v<std::remove_reference_t<decltype(cref_x)>>, "const int&");        

    int&& rrx = 42;
    auto&& rref_x = rrx;
    static_assert(std::is_reference_v<decltype(rref_x)>, "int&&");

    const int&& crrx = 42;
    auto&& crref_x = crrx;
    static_assert(std::is_const_v<std::remove_reference_t<decltype(crref_x)>>, "const int&&");

  }
} // namespace forwarding_types