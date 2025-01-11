module;
#include <chrono>
#include <cstdint>
#include <memory>
#include <print>
#include <random>
#include <set>
#include <thread>
#include <vector>

export module route_simulator;

struct Light {
  using Interval = std::chrono::duration<std::int64_t, std::milli>;

  std::mutex mtx;
  std::chrono::steady_clock::time_point start_time_;
  Interval duration_{0}; // this ;light  duration in milliseconds
  std::size_t id_{0};
  bool is_green_{false};

  Light(std::size_t id, std::chrono::steady_clock::time_point start_time,
        std::int64_t duration)
      : id_(id), duration_(std::chrono::milliseconds(duration * 500)),
        start_time_(start_time) {}

  Light(Light &&other) noexcept
      : mtx{}, start_time_{other.start_time_}, duration_{other.duration_},
        id_{other.id_}, is_green_{other.is_green_} {}

  Light &operator=(Light &&other) noexcept {
    if (this != &other) {
      start_time_ = other.start_time_;
      duration_ = other.duration_;
      id_ = other.id_;
      is_green_ = other.is_green_;
    }
    return *this;
  }

  // Light(const Light &) = delete;
  // Light &operator=(const Light &) = delete;
  ~Light() { mtx.unlock(); }

  // change the state of the light
  void change() {
    std::lock_guard<std::mutex> lock(mtx);

    const auto current_time = std::chrono::steady_clock::now();
    const auto new_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        current_time - start_time_);
    if (new_time > duration_) {
      start_time_ += new_time;
      is_green_ = !is_green_;
    }
    std::println("Turning light {} green is {}", id_, is_green_);
  }

  bool is_green() {
    std::lock_guard<std::mutex> lock(mtx);
    return is_green_;
  }
};

struct Segment {
  std::size_t id_{0};
  std::size_t length_m_{0};
};

// TODO: Try to constraing the speed of the car to be between 0 and 200
struct Car {
  std::size_t id_{0};
  std::size_t speed_kmh_{0};
  void start() {
    std::chrono::steady_clock::time_point start_time =
        std::chrono::steady_clock::now();
    std::println("Starting car {}", id_);
  }
  void stop() { std::println("Stopping car {}", id_); }
};

class Route {
private:
  Car car_;
  std::vector<std::size_t> lights_;
  std::vector<Segment> segments_;

public:
  Route(std::size_t id, std::vector<std::size_t> lights,
        std::size_t number_segments)
      : car_(Car{.id_ = id, .speed_kmh_ = 50}), lights_(std::move(lights)) {

    auto init_segments = [this](std::size_t count) {
      std::random_device rd;  // a seed source for the random number engine
      std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
      std::uniform_int_distribution<std::size_t> distrib(1, count);
      segments_.reserve(count);
      for (std::size_t i = 0; i < count; ++i) {
        segments_.emplace_back(
            Segment{.id_ = i, .length_m_ = 200 * distrib(gen)});
      }
    };
    init_segments(number_segments);
  }

  Route(Route &&other) noexcept = default;
  Route &operator=(Route &&other) noexcept = default;

  void drive() {
    //  TODO: check out the code from the copilot snippet.
    car_.start();
    for (auto &segment : segments_) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(segment.length_m_ / car_.speed_kmh_));
      // for (auto &light : lights_) {
      //   light.change();
      //   while (not light.is_green()) {
      //     car_.stop();
      //   }
      // }
    }
    car_.stop();
  }

  ~Route() {
    // for (auto &light : lights_) {
    //   if (light != nullptr) {
    //     light.reset();
    //   }
    // }
  }
};

struct RouteSimulator {
private:
  std::vector<Light> lights_;
  std::vector<Route> routes_;
  std::vector<std::jthread> tasks_;

public:
  RouteSimulator() {
    auto init_lights = [this](std::size_t count) {
      std::random_device rd;  // a seed source for the random number engine
      std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
      std::uniform_int_distribution<std::size_t> distrib(1, count);
      std::chrono::steady_clock::time_point start_time =
          std::chrono::steady_clock::now();

      lights_.reserve(count);
      for (std::size_t i = 0; i < count; ++i) {
        lights_.emplace_back(i, start_time, 100ul * distrib(gen));
        // lights_.push_back(std::make_unique<Light>(i, start_time, 100ul *
        // distrib(gen)));
        start_time += std::chrono::seconds(1); // 1 second interval each lights
      }
    };

    auto generate_random_lights =
        [this](std::size_t count) -> std::vector<std::size_t> {
      std::random_device rd;  // a seed source for the random number engine
      std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
      std::uniform_int_distribution<std::size_t> distrib(1, count);
      std::set<std::size_t> lights;
      for (std::size_t i = 0; i < count;) {
        const auto light_id = distrib(gen);
        if (lights.insert(light_id).second == true) {
          ++i;
        }
      }
      return std::vector<std::size_t>(lights.begin(), lights.end());
    };

    const std::size_t number_lights = 10;
    init_lights(number_lights);

    const std::size_t number_routes = 1;
    const std::size_t number_segments = 4;
    for (std::size_t i = 0; i < number_routes; ++i) {
      auto route_lights = generate_random_lights(number_segments + 1);
      routes_.emplace_back(i, route_lights, number_segments);
    }

    for (auto &route : routes_) {
      tasks_.emplace_back(&Route::drive, &route);
    }
  }

  ~RouteSimulator() { simulate(); }

  void simulate() {
    std::println("--Simulating route");

    // 1. threads per route
    // 2. threads per light
    // 3. Main the simulation thread fire (1), (2)
    // 4. Stop routes having the red lights (not green)
    // 5. Continiue the routes having the green lights
    // 6. Main - Stop lights threads before are finished routes threads
    // 7. Main - Join all threads

    // for (auto &task : tasks_) {
    //   if (task.joinable()) {
    //     task.join();
    //   }
    // }
  }
};

export void simulate_route() {
  RouteSimulator route_simulator;
  // route_simulator.simulate();  // Manual simulate the route or let the
  // destructor do it
}