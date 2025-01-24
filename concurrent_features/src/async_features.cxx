module;
#include <future>
#include <print>
#include <thread>

export module async_features;
import user_utilities;

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

void concurrent_shared_value_one_to_one() {
	std::println("---Testing concurrent shared value between threads with one-to-one relationship using shared_future");
	// Create a shared value between threads
	std::shared_future<std::uint32_t> shared_value = std::async(std::launch::async, []() -> std::uint32_t {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		std::uint32_t value = 42;
		std::println("Inner worker producing shared value: {}, thread id: {}", value, std::this_thread::get_id());
		return value;
	}).share();

	// Create a lambda function that takes no arguments and returns void
	auto lambda = [shared_value]() -> void {
		// Get the shared value
		std::uint32_t value = shared_value.get();
		std::println("Outer worker consuming shared value: {}, thread id: {}", value, std::this_thread::get_id());
	};

	// Call std::async in the other thread
	auto future = std::async(std::launch::async, lambda);
	// Get the result from the future
	future.get();
	std::println("---End of concurrent shared value between threads");
}

void concurrent_shared_value_one_to_one_2() {
	std::println("---Testing concurrent shared value between threads with one-to-one relationship using promise and future");
	// Create a shared value between threads
	using shared_value = std::promise<std::uint32_t>;

	shared_value promise;
	auto future = promise.get_future();

	{
		auto inner_worker = [](shared_value value) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			value.set_value(42);
			std::println("Inner worker producing shared value, thread id: {}", std::this_thread::get_id());
		};
		std::jthread thread(inner_worker, std::move(promise));
	}
	{
		auto outer_worker = [&future]() {
			std::uint32_t value = future.get();
			std::println("Outer worker consuming shared value: {}, thread id: {}", value, std::this_thread::get_id());
		};
		std::jthread thread(outer_worker);
	}
	std::println("---End of concurrent shared value between threads");
}

void test_async() {
  std::println("---Testing std::async features");
  async_example();
	packaged_task_example();
	concurrent_shared_value_one_to_one();
	concurrent_shared_value_one_to_one_2();
	std::println("---End of std::async features");
}
} // namespace async_features