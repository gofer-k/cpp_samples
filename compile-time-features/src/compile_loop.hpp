
template <bool Condition, typename Then, typename Else>
struct conditional;

template <typename Then, typename Else>
struct conditional<true, Then, Else> {
  using type = Then;
};

template <typename Then, typename Else>
struct conditional<false, Then, Else> {
  using type = Else;
};

/// @brief Compile time loop
/// @tparam Body 
/// @tparam Condition 
template <bool Condition, typename Body>
struct loop_result_raw;

template <typename Body>
struct loop_result_raw<true, Body> {
  using type = typename loop_result_raw<
    Body::next_type::condition,
    typename Body::next_type>::type;
};

template <typename Body>
struct loop_result_raw<false, Body> {
  using type = typename Body::type;
};

template <typename Body>
struct loop_raw {
  using type =
    typename loop_result_raw<Body::condition,
                         Body>::type;
};

/// @brief Compile time loop with class template inheritance
/// @tparam Body 
/// @tparam Condition 
template <bool Condition, typename Body>
struct loop_result;

template <typename Body>
struct loop_result<true, Body>
  : loop_result<Body::next_type::condition,
                typename Body::next_type> {};

template <typename Body>
struct loop_result<false, Body> : Body {};

template <typename Body>
struct loop
  : loop_result<Body::condition, Body> {};

template <int N, int Last, int Result>
struct factorial_loop {
  static const bool condition = (N <= Last);
  using type = std::integral_constant<int, Result>;
  using next_type =
    factorial_loop<N + 1, Last, Result * N>;
};

template <int N>
struct factorial2
  : loop<factorial_loop<1, N, 1>>::type {};
  
template <int N>
struct factorial {
  static const int value =
    N * factorial<N - 1>::value;
};

template <>
struct factorial<0> {
  static const int value = 1;
};