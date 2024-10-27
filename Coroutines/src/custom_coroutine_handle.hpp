// https://simontoth.substack.com/p/daily-bite-of-c-coroutines-step-by
#include <coroutine>

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
private: 
  std::coroutine_handle<promise_type> handle_;
};
}