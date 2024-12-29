module;
#include <mutex>
#include <print>
#include <string>
#include <thread>
#include <vector>

export module mutex_samples;

std::mutex mtx;
std::timed_mutex tmtx;

void synchronous_print_data(std::string data) {
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::lock_guard<std::mutex> lock(mtx);
  std::println("{}", data);
}

void timed_print_data(std::string data) {
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  if (tmtx.try_lock_for(std::chrono::milliseconds(200))) {
    std::lock_guard<std::timed_mutex> lock(tmtx, std::adopt_lock);
    std::println("Locked data: {}", data);
  } else {
    std::println("Failed to acquire lock for {}", data);
  }
}
export void test_mutex_samples() {
  auto printable = []<typename Print>(Print &&print, std::string data) {
    print(data);
  };

  std::vector<std::jthread> threads;
  {
    std::println("---Synchronouons print data");
    // Not alowed to initiate std::vector with j)thread object because
    // (j)threadd as only moveable objects
    threads.reserve(6);
    // Have to manualny puysh the objects into the vector
    for (int i = 0; i < 6; ++i) {
      threads.push_back(std::jthread(printable, synchronous_print_data, "Hello from t" + std::to_string(i)));
    }
    for (auto &thd : threads) {
      thd.join();
    }
  }
  {
    std::println("---Timed synchronouons print data");
    threads.clear();
    threads.reserve(6);
    // Have to manualny puysh the objects into the vector
    for (int i = 0; i < 6; ++i) {
      threads.push_back(std::jthread(printable, timed_print_data, "Hello from t" + std::to_string(i)));
    }
    for (auto &thd : threads) {
      thd.join();
    }
  }
  {
    struct RecursivePrint {
      std::string shared_data_;
      std::recursive_mutex rmtx;

      void pass_data(std::string data) {
        std::lock_guard<std::recursive_mutex> lock(rmtx);
        shared_data_ = data;
        std::println("Passed data {}", shared_data_);
      }

      void default_data() {
        std::lock_guard<std::recursive_mutex> lock(rmtx);
        shared_data_ = "Default shared data";
        std::println("In default_pass {}", shared_data_);
        pass_data("New data");
        std::println("Back to default_data {}", shared_data_);
      }
    };

    std::println("--- Recursive mutex printable data");
    RecursivePrint rp;
    std::jthread thd1(&RecursivePrint::pass_data, &rp, "Hello from t1");
    std::jthread thd2(&RecursivePrint::default_data, &rp);

    thd1.join();
    thd2.join();
  }
}