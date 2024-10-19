
#include <array>

namespace ConstExpr {
constexpr int factorial_recursive(int n)
{
  return n == 0 ? 1 : n * factorial_recursive(n - 1);
}

constexpr int factorial_iter(int n)
{
  int result = 1;
  for (int i = 2; i <= n; ++i)
    result *= i;
  return result;
}


template <int N>
constexpr auto sieve_prime()
{
    std::array<bool, N + 1> sieve{};
    for (int i = 2; i <= N; ++i) {
        sieve[i] = true;
    }
    for (int p = 2; p * p <= N; p++) {
        if (sieve[p]) {
            for (int i = p * p; i <= N; i += p) {
                sieve[i] = false;
            }
        }
    }
    return sieve;
}

template <std::size_t N>
constexpr std::size_t prime_count(const std::array<bool, N>& sieve)
{
  std::size_t count = 0;
  for (std::size_t i = 2; i < sieve.size(); ++i)
    if (sieve[i])
      ++count;
  return count;
}

template <int N>
constexpr auto get_prime_array()
{
    constexpr auto sieve = sieve_prime<N>();
    std::array<int, prime_count(sieve)> result{};
    for (std::size_t i = 2, j = 0; i < sieve.size(); ++i) {
        if (sieve[i]) {
            result[j] = i;
            ++j;
        }
    }
    return result;
}


template <typename... Args>
constexpr bool is_any_null(const Args&... args)
{
  return (... || (args == nullptr));
}

template <typename T>
constexpr std::string printBool(const T& arg) {
  return arg ? "true" : "false";
}

} // ConstExpr