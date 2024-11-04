#ifndef CO_AWAITABLE_ABSTRACTION_HPP_
#define CO_AWAITABLE_ABSTRACTION_HPP_

#include "custom_coroutine_handle.hpp"
#include <coroutine>
#include <exception>
#include <print>

namespace CoAwaitableInterface {
  struct ContinuationAwaitable {
    bool await_ready() noexcept { return false; }
    std::coroutine_handle<> await_suspend(std::coroutine_handle<>) noexcept {
        return continuation_;
    }
    void await_resume() noexcept {}
    std::coroutine_handle<> continuation_{std::noop_coroutine()};
};

template <typename T>
struct AwaitableFunction {
  struct promise_type {
    using handle_t = std::coroutine_handle<promise_type>;
    // Get the caller access to the handle
    AwaitableFunction get_return_object() {
      return AwaitableFunction{handle_t::from_promise(*this)};
    }

    std::suspend_always initial_suspend() noexcept { return {}; }
    ContinuationAwaitable final_suspend() noexcept {
      return { continuation_ };
    }

    // Same logic as in Function example
    template <std::convertible_to<T> Arg>
    void return_value(Arg&& result) {
        result_ = std::forward<Arg>(result);
    }
    void unhandled_exception() { std::terminate(); }

    std::coroutine_handle<> continuation_{std::noop_coroutine()};
    T result_;
  };

    // Store the coroutine handle
    explicit AwaitableFunction(std::coroutine_handle<promise_type> handle)
      : handle_(handle) {}
    
    // Awaitable interface
    bool await_ready() noexcept { return false; }
    // When we await on the result of calling a coroutine
    // the coroutine will start, but will also remember its caller
    std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
        // Remember the caller
        handle_.promise().continuation_ = caller;
        // Returning a coroutine handle will run the corresponding coroutine
        return handle_.raw_handle();
    }
    T await_resume() {
        return handle_.promise().result_;
    }

    void run_until_completion() {
        while (not handle_.done())
            handle_.resume();
    }

private:
    CustomCoroutineHandle::CoroutineHandle<promise_type> handle_;
};

struct AwaitableTask {
  struct promise_type {
    using handle_t = std::coroutine_handle<promise_type>;
    // Get the caller access to the handle
    AwaitableTask get_return_object() {
        return AwaitableTask{handle_t::from_promise(*this)};
    }

    std::suspend_always initial_suspend() noexcept { return {}; }
    ContinuationAwaitable final_suspend() noexcept {
        return { continuation_ };
    }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }

    std::coroutine_handle<> continuation_{std::noop_coroutine()};
  };

  // Store the coroutine handle
  explicit AwaitableTask(std::coroutine_handle<promise_type> handle)
    : handle_(handle) {}

  // Awaitable interface
  bool await_ready() noexcept { return false; }
  // When we await on the result of calling a coroutine
  // the coroutine will start, but will also remember its caller
  std::coroutine_handle<> await_suspend(std::coroutine_handle<> caller) noexcept {
    // Remember the caller
    handle_.promise().continuation_ = caller;
    // Returning a coroutine handle will run the corresponding coroutine
    return handle_.raw_handle();
  }
  void await_resume() {}

  void run_until_completion() {
    while (not handle_.done())
      handle_.resume();
  }

private:
  CustomCoroutineHandle::CoroutineHandle<promise_type> handle_;
};


auto child() -> CoAwaitableInterface::AwaitableTask {
    co_return;
}

auto parent() -> CoAwaitableInterface::AwaitableTask {
    // Start child and wait until it is completed
    co_await child();
    co_return;
}

auto child_func(int num) -> CoAwaitableInterface::AwaitableFunction<int> {
    co_return num;
}

auto other_parent(int num) -> CoAwaitableInterface::AwaitableTask {
  int v = co_await child_func(num);
  std::println("Child returned: {}", v);
  co_return;
}
} //anmespace

#endif // CO_AWAITABLE_ABSTRACTION_HPP_