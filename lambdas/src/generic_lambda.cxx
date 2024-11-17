module;
#include <cstdint>
#include <type_traits>
#include <vector>

export module GenericLambda;

export void generic_lambda_parameter() {
  std::vector<std::uint32_t> vec = {1, 34, 54, 65};

  auto a = [](auto vec) {
    // Since C++20
    using T = typename decltype(vec)::value_type;
    static_assert(std::is_unsigned_v<T> == true);
  };

  // Since C++20
  auto b = []<typename T>(std::vector<T> vec) {
    static_assert(std::is_unsigned_v<T> == true);
  };

  a(vec);
  b(vec);
}

export void generic_access_argument() {
  std::vector<std::uint32_t> vec = {1, 34, 54, 65};

  // Until C+20
  auto a = [](const auto& arg) {
    using T = std::decay_t<decltype(arg)>;
    // using T = decltype(x); // without decay_t<> it would be const T&, so
    T copy = arg;             // copy would be a ref
    // T::static__func();        // not compile

    using Iter = typename T::iterator;
  };

  // Since C++20
  auto b = []<typename T>(const T& arg) {
    T copy = arg;
    // T::static__func();        // not compile

    using Iter = typename T::iterator;    
  };

  a(vec);
  b(vec);
}

