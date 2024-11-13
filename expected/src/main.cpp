#include <cmath>
#include <expected>
#include <limits>
#include <print>

auto divide(double lhs, double rhs) -> std::expected<double, std::string> {  
  if (std::abs(rhs - 0.0) < 1e-6) {
    return std::unexpected("Error divide zero");
  }
  return lhs / rhs;
}

auto negative(double v) -> std::expected<double, std::string> {
  if (v < 0.0) {
    return std::unexpected("Error negative value");
  }
  return v;
}

struct Rectangle{
  auto operator()(double a, double b) -> std::expected<double, std::string> {
    return surface(a, b);
  }

  auto surface(double a, double b) -> std::expected<double, std::string> {
    return divide(a, b)
      .and_then([](double div) { return negative(div); })
      .and_then([b](double div) {
        return std::expected<double, std::string>(div * b);
      })
      .or_else([](std::string error) {
        return std::expected<double, std::string>(std::numeric_limits<double>{}.infinity());  // default value
      });
  }

  void print (std::expected<double, std::string> exp) {
    if (exp.has_value()) {
      std::println("Rectangle surface = {}", exp.value());
    }
    else {
      std::println("{}", exp.error());  
    }
  }
};

void print_expected_value(std::expected<double, std::string> exp) {
  if (exp.has_value()) {
    std::println("{}", exp.value());
  }
  else {
    std::println("{}", exp.error());  
  }
}

void print_divide(double lhs, double rhs) {  
  std::print("{} / {} = ", lhs, rhs);

  auto res = divide(lhs, rhs);
  print_expected_value(res); 
}

int main() {
  print_divide(23., 0.0);
  print_divide(342., -2345.0);

  Rectangle rec;
  rec.print(rec(2312.0, 23.7));
  rec.print(rec(2312.0, -23.7));
  rec.print(rec(-2312.0, 23.7));
  rec.print(rec(2312.0, 0.0));
  rec.print(rec(0.0, 23.7));

  return 0;
}