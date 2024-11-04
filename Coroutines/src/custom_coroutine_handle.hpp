#ifndef CUSTOM_COROUTINE_HANDLE_HPP_
#define CUSTOM_COROUTINE_HANDLE_HPP_

// https://simontoth.substack.com/p/daily-bite-of-c-coroutines-step-by
#include <coroutine>
#include <utility>

namespace CustomCoroutineHandle {
template <typename promise_type>
struct CoroutineHandle {
  CoroutineHandle(): handle_() {}

  CoroutineHandle(std::nullptr_t) : handle_(nullptr) {}
  CoroutineHandle(std::coroutine_handle<promise_type> promise)
  : handle_(std::move(promise)) {}

  CoroutineHandle(const CoroutineHandle<promise_type>&) = delete;
  CoroutineHandle(CoroutineHandle<promise_type>&& other)
  : handle_(std::exchange(other.handle_, nullptr)) {}

  ~CoroutineHandle() {
    if (handle_ != nullptr) {
      handle_.destroy();
    }
  }

  CoroutineHandle& operator=(const CoroutineHandle<promise_type>& other) = delete;
  CoroutineHandle& operator=(CoroutineHandle<promise_type>&& other) {
    handle_ = std::exchange(other.handle_, nullptr);
    return *this;
  }

  promise_type& promise() const {    
    return handle_.promise();
  }

  bool done() const {
    if (handle_ == nullptr) {
      return false;
    }
    return handle_.done();
  }

  void resume() const {
    if (handle_ != nullptr) {
      handle_.resume();
    }
  }
  
  std::coroutine_handle<> detach() {
    return std::exchange(handle_, {});
  }

  std::coroutine_handle<> raw_handle() const {
    return handle_;
  }

private: 
  std::coroutine_handle<promise_type> handle_;
};
} // namespace
#endif // CUSTOM_COROUTINE_HANDLE_HPP_
