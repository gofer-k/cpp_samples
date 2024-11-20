module;
#include <cstdint>

export module lambda_capture_this;

export struct CaptureThis {
  [[deprecated("Since C++20. Implicity refecenced 'this'.") ]]
  std::uint32_t inc_depred() {
    auto a = [=] {
      return ++field_;
    };

    return a();
  }

  std::uint32_t inc_by_copy() {
    auto a = [=, *this]() mutable {
      // return ++field_;  // field_ is read-olnly
      return ++field_;
    };
    return a();
  }

  std::uint32_t inc_by_ref() {
    auto a = [=, this]() {
      return ++field_;
    };
    return a();
  }

std::uint32_t field_{};
};