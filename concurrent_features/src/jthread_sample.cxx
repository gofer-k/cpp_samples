module;
#include <chrono>
#include <condition_variable>
#include <print>
#include <thread>

export module jthread_sample;

export void test_jthread_construct() {
  // C+23: std::jthread can be used to create a thread and join it in arguments.
  std::jthread t(
      [](int arg) { std::println("Hello from jthread arg {}!", arg); }, 123);
  std::println("Here t is joinable: {}", t.joinable());
  t.join();
  std::println("Here t after join is joinable: {}", t.joinable());

  std::println("Thread hardware concurrency: {}",
               std::jthread::hardware_concurrency());
}

export void test_jthread_interrupt() {
  // {
  //   // C+23: std::jthread can be used to interrupt a thread.
  //   std::jthread t([]() {
  //     while (true) {
  //       std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //       std::println("Hello from jthread!");
  //     }
  //   });

  //   std::this_thread::sleep_for(std::chrono::seconds(3));
  //   // explicit request_stop() to interrupt the thread
  //   t.request_stop();
  //   t.join();
  // }
  {
    std::jthread sleepy_worker([](std::stop_token st) {
      while (!st.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::println("Not interrupting jthread!");
      }
      std::println("Interrupted jthread!");
    });

    std::jthread waiting_worker([](std::stop_token st) {
        std::mutex mtx;
        std::unique_lock<std::mutex> lck(mtx);
        // wait() will return when stop_token is requested to stop
        std::condition_variable_any().wait(lck, st, []{return false;});
        std::println("Waiting jthread is requested to stop  !");
    });

     // Sleep this thread to give threads time to spin
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    // std::jthread::request_stop();  // can be called explicitly:
    sleepy_worker.request_stop();
    sleepy_worker.join();
    std::println("Sleepy jthread is joined!");
  }
  {
    auto print_thread_source = [](std::stop_source source) {
      std::println("jthread stop source {}, requested {}", source.stop_possible(), source.stop_requested());
    };

    auto worker = std::jthread([](std::stop_token st) {
      while (!st.stop_requested()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::println("Not interrupting jthread!");
      }
      std::println("Interrupted jthread!");
    });

    std::stop_source source = worker.get_stop_source();
    print_thread_source(source);

    std::println("Pass stop source to the other jthread worker");
    auto stopable_worker = std::jthread([](std::stop_source source) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::println("Request stop for the worker via the source");      
    }, source);

    stopable_worker.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    print_thread_source(source);
  }
}