#ifndef CO_LAZY_FUNCTION_HPP_
#define CO_LAZY_FUNCTION_HPP_

#include "custom_coroutine_handle.hpp"
#include <coroutine>
#include <exception>

namespace LazyCoro {

template<typename T>
struct LazyCoroutinePromise;

template<typename T>
struct LazyCoroutine {
  using promise_type = LazyCoroutinePromise<T>;

  explicit LazyCoroutine(promise_type::handle_ handle) : handle_(handle) {}

  T get() {
    // THrown exception in the coroutine execution
    if (handle_.promise().exp() != nullptr)  {      
      std::rethrow_exception(handle_.promise().exp()); 
    }
    // None an exception during the execution
    if (not handle_.done()) {            
      handle_.resume();
    }

    // Result increment value after suspendation to the completion coroutine.
    auto inc = 1;
    handle_.promise().chageValue(inc);
    return handle_.promise().result();
  }

  operator T() {
    return get();
  }

private: 
  CustomCoroutineHandle::CoroutineHandle<LazyCoroutinePromise<T>> handle_;
};

template<typename T>
struct LazyCoroutinePromise {
  using handle_ = std::coroutine_handle<LazyCoroutinePromise<T>>;

  // Customization coroutine point: : passed returned type to a caller.
  LazyCoroutine<T> get_return_object() {
    return LazyCoroutine<T>{handle_::from_promise(*this)};
  }

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

  // Customization coroutine point after coroutine executed co_return from resut_ value.
  template<std::convertible_to<T> Arg> 
  void return_value(Arg&& result) {
    result_ = std::forward<Arg>(result);
  }

  // Customization coroutine point after coroutine executed co_return void value
  // void return_void() {}

  // Customization coroutine point when there is an unhanded exception.
  void unhandled_exception() {
    // Optiopn 1: when an exception is thrown within a coroutine execution stack
    // std::terminate();
    
    // Option 2: evaluate the excep[tion,  re-emit the exception to the caller and
    // the caller can try again access the coroutine result. 
    exp_ = std::current_exception();
  }

  T result() const {
    return result_;
  }

  template<std::convertible_to<T> Arg> 
  void chageValue(Arg&& new_value) {
    result_ += std::forward<Arg>(new_value);
  }

  std::exception_ptr  exp() const {
    return exp_;
  }

private:
  // The result coroutine. 
  T result_;
  std::exception_ptr exp_;
};

} // namespace

#endif // CO_LAZY_FUNCTION_HPP_
