#include <iostream>
#include "compile_loop.hpp"
#include "constexpr.hpp"
#include "sieve_primes.hpp"

// template <int p, int i> struct is_prime {
//   enum {
//     prim = (p == 2) ||
//            (p % i) &&
//            is_prime<(i > 2 ? p : 0), (i - 1) > :: prim };
// };

// template<>
// struct is_prime<0, 0> { enum {prim = 1}; };

// template<>
// struct is_prime<0, 1> { enum {prim = 1}; };

// template <int i> struct D { D(void*); }; // not compile!!!

// template <int i> struct Prime_print {
//   Prime_print<i - 1> a;
//   enum { prim = is_prime<i, i - 1>::prim };
//   void f() { 
//     D<i> d = (prim ? 1 : 0);
//     a.f();
//     }
// };

// template<> struct Prime_print<1> {
//   enum {prim = 0};

//   void f() { 
//     D<1> d = (prim ? 1 : 0);
//     };
// };


int main() {
  // Prime_print<18> a;
  // a.f();

  factorial<10> fac;
  std::cout << "Factoral(10): " << fac.value << std::endl;

  std::cout << "Factoral2(10): " << factorial2<10>::value << std::endl;

  std::cout << "constexpr factorial_recursive(10): " << ConstExpr::factorial_recursive(10) << std::endl;

  std::cout << "constexpr factorial_iter(10): " << ConstExpr::factorial_iter(10) << std::endl;

  std::cout << "Primes: ";
  Primers::output(Primers::primes_to<100>());
  std::cout << std::endl;

  constexpr auto primes = ConstExpr::get_prime_array<100000>();
  std::cout << "constrexpr Primes: ";
  for (const auto& p : primes) {
        std::cout << p << "\t";
  }
  std::cout << std::endl;

  return 0;
}