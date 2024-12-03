template<typename T>
using IntPair = std::pair<int, T>;

void test_ctad_alias() {
  double d_value{};
  IntPair<double> p0{1, d_value};  // C++17
  IntPair p1{1, d_value};          // CTAD: std::pair<int, double>
  IntPair p2{1, p1};               // CTAD: std::pair<int, std::pair<int, double>>
}
