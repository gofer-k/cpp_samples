module;
#include <utility>
#include <tuple>

export module capture_pack_expansion;

// C++17
template<class F, class... Args>
auto delay_apply(F&& f, Args&&... args) {
  return [f = std::forward<F>(f),
          tup = std::make_tuple(std::forward<Args>(args)...)]() -> decltype(auto) {
    return std::apply(f, tup);
  };
}

// C++20
template<typename F, typename... Args>
auto delay_call(F&& f, Args&&... args) {
  return [f = std::forward<F>(f),
          ...f_args = std::forward<Args>(args)]() -> decltype(auto) {
    return f(f_args...);
  };
}

void functor(int, int){}

export void call_functor_with_pack_expasion() {
  delay_call(functor, 1, 2)();
  delay_apply(functor, 1, 2)();;
}