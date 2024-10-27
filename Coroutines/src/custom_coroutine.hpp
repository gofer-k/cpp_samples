// https://simontoth.substack.com/p/daily-bite-of-c-coroutines-step-by
#include "custom_coroutine_handle.hpp"
#include <coroutine>

namespace CustomCoroutine {

template<typename T>
struct CoroutinePromise;

template<typename T>
struct Coroutine {
  using promise_type = CoroutinePromise<T>;

  explicit Coroutine(promise_type::handle_ handle) : handle_(handle) {}

  T get() {    
    return handle_.promise().result();
  }

  operator T() {
    return get();
  }

private: 
  CustomCoroutineHandle::CoroutineHandle<CoroutinePromise<T>> handle_;
};

template<typename T>
struct CoroutinePromise {
  using handle_ = std::coroutine_handle<CoroutinePromise<T>>;

  // Customization coroutine point: : passed returned type to a caller.
  Coroutine<T> get_return_object() {
    return Coroutine<T>{handle_::from_promise(*this)};
  }

  // Customization coroutine point: happend before the coroutine body starts.
  std::suspend_never initial_suspend() noexcept {
    return {};
  }

  // Customization coroutine point: happend after the coroutine body finishs.
  // std::suspend_always here is a caller to read the result,
  // the coroutine now cannot finish after and coroutine_handle.destroy leads
  // into SIGSEGV after completed executing block.
  std::suspend_always final_suspend() noexcept {
    return {};
  }

  // Customization coroutine point after coroutine executed co_return from resut_ value.
  template<std::convertible_to<T> Arg> 
  void return_value(Arg&& result) {
    result_ = std::forward<Arg>(result);
  }

  // Customization coroutine point after coroutine executed co_return void value
  // void return_void() {}

  // Customization coroutine point when there is an unhanded exception.
  void unhandled_exception() {}

  T result() const {
    return result_;
  }
private:
  // The result coroutine. 
  T result_;
};

// Any coroutine returning a type.
// template <typename... Args>
// std::coroutine_traits<Coroutine<Args...>> {
//   using promise_type = CoroutinePromise<Args...>;
// };
  
} // namespace