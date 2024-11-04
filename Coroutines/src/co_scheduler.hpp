#ifndef CO_SCHEDULER_HPP_
#define CO_SCHEDULER_HPP_

#include "custom_coroutine_handle.hpp"
#include <chrono>
#include <coroutine>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

namespace CoMultiTask {    
  using time_point = std::chrono::time_point<std::chrono::system_clock>;

  struct CoScheduler {    
    using timed_coroutine = std::pair<time_point, std::coroutine_handle<>>;
    using scheduled_coroutines = 
      std::priority_queue<
        timed_coroutine,
        std::vector<timed_coroutine>,
        std::greater<>>;

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
          CoScheduler{}.enqueue(context, time_);
      }
      // Called as the last part of evaluating co_await,
      // the coroutine is resumed just before this call 
      // (if it was suspended in the first place).
      void await_resume() {}

      time_point time_;
    };

    void enqueue(
      std::coroutine_handle<> handle,
      time_point time = std::chrono::system_clock::now()) {
      
      timed_coroutines_.push(std::make_pair(time, handle));
    }

    void run() const {
      while ( not timed_coroutines_.empty()) {

        if (timed_coroutines_.top().first < std::chrono::system_clock::now()) {
          std::this_thread::sleep_until(timed_coroutines_.top().first);
        }

        auto coroutine = timed_coroutines_.top().second;        
        timed_coroutines_.pop();

        coroutine.resume();
        if (coroutine.done()) {
          coroutine.destroy();
        }
      }
    }

    WakaupAwaitable wake_up(time_point time) const { return WakaupAwaitable{time}; }
    WakaupAwaitable wake_up() const {
      return WakaupAwaitable{std::chrono::system_clock::now()};
    }
    
  private:
    // Single CoScheduler object maintain the corroutines in this sample.
    static scheduled_coroutines timed_coroutines_;
  };
  CoScheduler::scheduled_coroutines CoScheduler::timed_coroutines_{};

  template<typename T>
  struct CoTaskBase {
    // Note: promise_type alias in the coroutine type the compiler looks for it.
    using promise_type = T;
    explicit CoTaskBase(promise_type::handle_t handle)
    : handle_(handle) {}

    void detach() {
      // Pass this coroutine handle to the scheduler
      CoScheduler{}.enqueue(handle_.detach());
    }
    
  private:
    CustomCoroutineHandle::CoroutineHandle<promise_type> handle_;
  };

  template<typename promise_type>
  struct CoPromiseTypeBase {
    using handle_t = std::coroutine_handle<promise_type>;

    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }

    void return_void() {}
    void unhandled_exception() {}
  };

  struct CoPromiseType : public CoPromiseTypeBase<CoPromiseType> {
    CoTaskBase<CoPromiseType> get_return_object() {
      return CoTaskBase<CoPromiseType>{handle_t::from_promise(*this)};
    }
  };

  struct TimedPromiseType : public CoPromiseTypeBase<TimedPromiseType> {
    // Customzation point
    CoTaskBase<TimedPromiseType> get_return_object() {
      return CoTaskBase<TimedPromiseType>{handle_t::from_promise(*this)};
    } 

    auto await_transform(time_point time) const {
      return CoScheduler{}.wake_up(time);
    }  
  };

  using CoTask = CoTaskBase<CoPromiseType>;
  using CoTaskTimed = CoTaskBase<TimedPromiseType>;
} // namespace

#endif // CO_SCHEDULER_HPP_