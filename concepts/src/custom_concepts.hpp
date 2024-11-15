#ifndef CUSTOM_CONCEPTS_HPP_
#define CUSTOM_CONCEPTS_HPP_

#include <print>

 namespace CustomConcepts
 {
  // Thnis is no-op concept, because it's not runtime expresion.
  template <typename T>
  concept Zero = requires(T a) {
    a == static_cast<decltype(a)>(0);
  };

  template <typename T>
  concept Addable = requires(T a, T b){
    a + b;
  };

  template <typename T>
  concept Subtractable = requires(T a, T b){
    a - b;
  };

  template <typename T>
  concept Mulltipliable = requires(T a, T b){
    a * b;
  };

  template <typename T>
  concept Divideable = requires(T a, T b  ) {
    // b == 0; // this is no-op to concept
    a / b;    
  };

  template <typename T>
  concept Divideable_not_zero = Zero<T> && Divideable<T>;


  // Contrainted (required concept Addable) overload if 'add' function.
  template <typename T>
  requires Addable<T>
  T add(T a, T b) {

    return a + b;
  }

  template <Addable T>  // same as add()
  T add2(T a, T b) {
    return a + b;
  }


  // Contrainted (required concept Divideable) overload if 'divide' function.
  template<typename T>
  requires Divideable<T>
  T divide(T a, T b) {
    return a / b;
  }

  // Contrainted overload 'divide' function with (required concept Divideable_not_zero) .
  template <typename T>
  requires Divideable_not_zero<T>
  auto divide(T a, T b) {
    return a / b;
  }

  template <Zero T>
  bool zero_value(T a) {
    return a ==  static_cast<T>(0);
  }

  template<typename Container, typename Range>
  void cont_assign(Container& cont, Range&& rng) {
    cont.clear();
    
    // if (rng.empty()) {
    //   return;
    // }

    // reserve a container if the one has
    if constexpr (requires {cont.reserve(std::ranges::size(rng)); }) {
      std::println("Reserve container if it's the one");
      cont.reserve(std::ranges::size(rng));
    }

    for (auto&& elem : std::forward<Range>(rng)) {
      if constexpr (requires {cont.push_back(elem); }) {
        std::println("A container with push_back method: std::vector" );
        cont.push_back(std::forward<decltype(elem)>(elem));
      }
      if constexpr (requires {cont.emplace(elem); }) {
        std::println("A container with emplace method i.e: std::set" );
        cont.emplace(std::forward<decltype(elem)>(elem));
      }
    }
  }
  } // namespace CustomConcepts
 

#endif // CUSTOM_CONCEPTS_HPP

