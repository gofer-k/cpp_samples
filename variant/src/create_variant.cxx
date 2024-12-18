
module;
#include <cstdint>
#include <print>
#include <string>
#include <variant>
#include <utility>

export module create_variant;

struct Aggregate
{
  std::uint32_t num_{0};
  std::string str_{};
};

struct User
{
  User() = default;
  User(std::string arg) : str_(std::move(arg)) {
    std::println("User c_tor(arg)");
  }
  User(const User& other) : str_(std::move(other.str_)) {
    std::println("User c_tor copy");
  }
  User(User&& other) noexcept : str_(std::exchange(str_, other.str_)) {
    std::println("User c_tor move");
  }

  User& operator=(const User& other) { 
    std::println("User copy assignment");
    str_ = other.str_;
    return *this;
  }
  User& operator=(User&& other) noexcept { 
    std::println("User move assignment");
    str_ = std::exchange(str_, other.str_);
    return *this;
  }

  std::string str_{"User"};
};

export void test_create_variant() {
  {
    std::variant<Aggregate, User> var{Aggregate{.num_ = 1, .str_ = "abc"}};
    auto agg = std::get<Aggregate>(var);
    std::println("-- Variant<Aggregate>({}, {})", agg.num_, agg.str_);
  }
  {
    // The variant pass a variable in place allows to reduce construtible the variant.
    std::variant<Aggregate, User> var{std::in_place_type<Aggregate>, 1, "abc"};
    auto agg = std::get<Aggregate>(var);
    std::println("-- Variant(std::in_place_type<Aggregate>, {}, {})", agg.num_, agg.str_);
  }
  {
    std::variant<Aggregate, User> var{User{"abc"}};
    const auto& user = std::get<User>(var);
    std::println("-- Variant<User>({})", user.str_);
  }
  {
    // The variant pass a variable in place allows to reduce construtible the variant.
    std::variant<Aggregate, User> var{std::in_place_type<User>, "efg"};
    const auto& user = std::get<User>(var);
    std::println("-- Variant(std::in_place_type<User>, {})", user.str_);

    // The same the above
    std::variant<Aggregate, User> var_2 {std::in_place_index<1>, "efg"};
  }  
}