#ifndef CUSTOM_CONCEPTS_HPP_
#define CUSTOM_CONCEPTS_HPP_


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

  template <typename T>
  requires Addable<T>
  T add(T a, T b) {

    return a + b;
  }

  template <Addable T>  // same as add()
  T add2(T a, T b) {
    return a + b;
  }

  template<Divideable T>
  T divide(T a, T b) {
    return a / b;
  }

  template <Divideable_not_zero T>
  auto divide_not_zero(T a, T b) {
    return a / b;
  }

  template <Zero T>
  bool zero_value(T a) {
    return a ==  static_cast<T>(0);
  }
 } // namespace CustomConcepts
 

#endif // CUSTOM_CONCEPTS_HPP

