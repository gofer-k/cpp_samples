#ifndef CO_GENERATOR_HPP_
#define CO_GENERATOR_HPP_

#include "custom_coroutine_handle.hpp"
#include <coroutine>
// std23
#include <generator>
#include <utility>

namespace CoGenerator {

/// @brief Fibonatcci algorithim with  std coroutine generator feature.
/// Compile C++23 onward.
/// @param n 
/// @return yield values utmost n range limit. 
std::generator<int> Fibonacci(int n) {
  int prev = 0;
  int next = 1;

  for (;n > 0; --n) {
    {
      // this algorithm work from C++20 to onward      
      // co_yield prev;
      // int cur = prev + next;
      // prev = next;
      // next = cur;
    }
    {
      // This work from C++23 to onward/ 
      co_yield std::exchange(prev, std::exchange(next, prev + next));
    }
  }
}

template<typename T>
struct CustomGeneratorPromise;

template<typename T>
struct CustomGenerator {
  using promise_type = CustomGeneratorPromise<T>;

  explicit CustomGenerator(promise_type::handle_ handle) : handle_(handle) {}

  // Let coroutine generator yield next result, if the coroutine completetion. 
  bool exhausted() {
    if (not handle_.done()) {
      handle_.resume();
    }
    return handle_.done();
  }

  T get() {
    return handle_.promise().get();
  }
private:
  CustomCoroutineHandle::CoroutineHandle<promise_type> handle_;
};

template<typename T>
struct CustomGeneratorPromise {
  using handle_ = std::coroutine_handle<CustomGeneratorPromise<T>>;

  // Customization coroutine point: happend before the coroutine body starts.
  // Immediate return result to the caller.
  std::suspend_always initial_suspend() noexcept {
    return {};
  }

  // Customization coroutine point: happend after the coroutine body finishs.
  // std::suspend_always here is a caller to read the result,
  // the coroutine now cannot finish after and coroutine_handle.destroy leads
  // into SIGSEGV after completed executing block.
  std::suspend_always final_suspend() noexcept {
    return {};
  }

  CustomGenerator<T> get_return_object() {
    return CustomGenerator<T>(handle_::from_promise(*this));
  }

  template<std::convertible_to<T> Arg>
  std::suspend_always yield_value(Arg&& value) {
    result_ = std::forward<Arg>(value);
    return {};
  }

  // None return value supported in this coroutine.
  void return_void() {}
  void unhandled_exception() {}
  
  T get() const {
    return result_;
  }

private: 
  T result_;
};
  
} // namespace
#endif // CO_GENERATOR_HPP_
