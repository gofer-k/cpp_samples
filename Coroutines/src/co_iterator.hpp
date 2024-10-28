#ifndef CO_ITERATOR_HPP_
#define CO_ITERATOR_HPP_

#include "custom_coroutine_handle.hpp"
#include <iterator>
#include <utility>

namespace CoIterator {

template<typename T>
struct CustomGeneratorPromise;

template<typename T>
struct CoGenerator {
  using promise_type = CustomGeneratorPromise<T>;

  CoGenerator(promise_type::handle_ handle) : handle_(handle) {}

  struct iterator {
    using value_type = T;
    using difference_type = std::ptrdiff_t;

    iterator(CustomCoroutineHandle::CoroutineHandle<promise_type> handle)
    : handle_(std::move(handle)) {}

    iterator(iterator&& other) noexcept
    : handle_(std::exchange(other.handle_, {})) {}

    iterator& operator=(iterator&& other) {
      handle_ = std::exchange(other.handle_, {});
      return *this;
    }

    T operator*() {
      return handle_.promise().get();
    }

    // Pre-increment
    iterator& operator++() {
      if(not handle_.done()){
        handle_.resume();        
      }
      return *this;
    }

    // Post-increment
    void operator++(int) {
      return ++(*this);
    }

    friend bool operator==(const iterator& other, std::default_sentinel_t) {
      return other.handle_.done();
    }
  private: 
    CustomCoroutineHandle::CoroutineHandle<promise_type> handle_;
  };

  iterator begin() {
    handle_.resume();
    return iterator{std::move(handle_)};
  }

  std::default_sentinel_t end() const noexcept { return {}; };

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

  CoGenerator<T> get_return_object() {
    return CoGenerator<T>(handle_::from_promise(*this));
  }

  template<std::convertible_to<T> Arg>
  std::suspend_always yield_value(Arg&& value) {
    result_ = std::forward<Arg>(value);
    return {};
  }

  // None return value supported in this coroutine.
  void return_void() {}
  void unhandled_exception() {}
  
  T get() {
    return result_;
  }

private: 
  T result_;
};

} // namespace
#endif // CO_ITERATOR_HPP_namespace CoIterator