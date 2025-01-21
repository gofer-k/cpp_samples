module;
#include <future>
#include <print>
#include <thread>

export module async_features;
import utilities;

void print_thread_id(std::string msg) {
  std::println("{} thread id: {}", msg, std::this_thread::get_id());
}

export namespace async_features {
// Function to demonstrate the use of std::async
void async_example() {
  // Create a lambda function that takes no arguments and returns void
  auto lambda = []() -> std::uint32_t {    
		auto future = std::async(std::launch::async, []() -> std::uint32_t {
			auto inner_task = []() -> std::uint32_t {
				// Simulate some work
				std::this_thread::sleep_for(std::chrono::seconds(1));
				print_thread_id("Most inner worker");
				return 4;
			};
			auto inner_future = std::async(std::launch::deferred, inner_task);

			std::this_thread::sleep_for(std::chrono::seconds(2));
			print_thread_id("Inner worker");
			// the most inner worker is return result after the parent worker due to std::launch::deferred
			return inner_future.get();
		});
		// Wait for the inner worker to finish
		future.wait();
		print_thread_id("Worker");
    return future.get();
  };

  // Call std::async in te other thread
  auto future = std::async(std::launch::async, lambda);

	// Get the result from the future
	std::println("Workers result: {}", future.get());
	  // Processing the main thread
  print_thread_id("Main");
}

void packaged_task_example() {
	std::println("---Testing std::packaged_task features");
	// Create a packaged_task in the main thread
	std::packaged_task<void(std::string)> task([](std::string msg) {
		print_thread_id(msg);
	});
	// Get the future from the packaged_task
	auto future = task.get_future();	
	// Fire the packaged_task
	task("Worker in the main thread");
	// Get the result from the future must be called after the worker thread has finished otherwise it will block.
	future.get();

	// Reset the packaged_task to be able to call it again	
	task.reset();
	// Call the packaged_task in a separate thread
	std::jthread thread(std::move(task), "Worker in the different thread");
	// Wait for the worker thread to finish
	thread.join();

	// Processing the main thread
	print_thread_id("Main");
}

void test_async() {
  std::println("---Testing std::async features");
  async_example();
	packaged_task_example();
	std::println("---End of std::async features");
}
} // namespace async_features