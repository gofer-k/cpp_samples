#ifndef CO_SCHEDULER_HPP_
#define CO_SCHEDULER_HPP_

#include "custom_coroutine_handle.hpp"
#include <coroutine>
#include <list>

namespace CoMultiTask {    
  struct CoScheduler {
    struct WakaupAwaitable {    
      // TODO: Does it works?
      // Opportunity for early continuation, returning true
      // will let the coroutine continue without suspending.
      bool await_ready() { return false; }
      
      // Called after the coroutine is suspended, controls
      // what happens next. The void returning variant returns 
      // control to the caller.
      void await_suspend(std::coroutine_handle<> context) {
          // Re-schedule the suspended coroutine
          // context is a handle to the suspended coroutine
          CoScheduler{}.enqueue(context);
      }
      // Called as the last part of evaluating co_await,
      // the coroutine is resumed just before this call 
      // (if it was suspended in the first place).
      void await_resume() {}
    };

    void enqueue(std::coroutine_handle<> handle) {
        coroutines_.push_back(handle);
    }

    void run() const {
      while ( not coroutines_.empty()) {
        auto coroutine = coroutines_.front();        
        coroutines_.pop_front();

        coroutine.resume();
        if (coroutine.done()) {
          coroutine.destroy();
        }
      }
    }

    WakaupAwaitable wake_up() const { return WakaupAwaitable{}; }

  private:
    // Simgle CoScheduler object maintain the corroutines in this sample.
    static std::list<std::coroutine_handle<>> coroutines_;
  };
  std::list<std::coroutine_handle<>> CoScheduler::coroutines_{};

  struct CoTask {
    struct promise_type {
      using handle_t = std::coroutine_handle<promise_type>;

      CoTask get_return_object() {
        return CoTask{handle_t::from_promise(*this)};
      }
      std::suspend_always initial_suspend() noexcept { return {}; }
      std::suspend_always final_suspend() noexcept { return {}; }

      void return_void() {}
      void unhandled_exception() {}
    };

  explicit CoTask(promise_type::handle_t handle)
  : handle_(handle) {}

  void detach() {
    // Pass this coroutine handle to the scheduler
    CoScheduler{}.enqueue(handle_.detach());
  }
  
  private:
    CustomCoroutineHandle::CoroutineHandle<promise_type> handle_;
  };

} // namespace

#endif // CO_SCHEDULER_HPP_