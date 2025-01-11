module;
#include <mutex>
#include <print>
#include <thread>

export module locks_samples;

struct Worker {
  std::mutex mtx;

  void print_data() {
    std::println("Worker {} print data", std::this_thread::get_id());
  }
  void print_another_data() {
    std::println("Worker {} print_another_data", std::this_thread::get_id());
  }
};
void printable_data(Worker &worker1, Worker &worker2) {

  std::scoped_lock lock(worker1.mtx, worker2.mtx);
  worker1.print_data();
  worker2.print_another_data();

  // Equivalent code 1 (using std::lock and std::lock_guard)
  {
  // std::lock(worker1.mtx, worker2.mtx);
  // std::lock_guard<std::mutex> lk1(worker1.mtx, std::adopt_lock);
  // std::lock_guard<std::mutex> lk2(worker2.mtx, std::adopt_lock);
    // worker1.print_data();
    // worker2.print_another_data();
  }

  // Equivalent code 2 (if unique_locks are needed, e.g. for condition variables)
  {
  // std::unique_lock<std::mutex> lk1(worker1.mtx, std::defer_lock);
  // std::unique_lock<std::mutex> lk2(worker2.mtx, std::defer_lock);
  // std::lock(lk1, lk2);
    // worker1.print_data();
    // worker2.print_another_data(); 
  }
}

export void test_scope_lock_sample() {
  // C++23: std::scoped_lock can be used to lock multiple mutexes at once.
  Worker worker1;
  Worker worker2;
  
  
  std::jthread thd1(printable_data, std::ref(worker1), std::ref(worker2));
  std::jthread thd2(printable_data, std::ref(worker1), std::ref(worker2));

  thd1.join();
  thd2.join();
}