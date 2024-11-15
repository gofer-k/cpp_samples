#include "custom_concepts.hpp"
#include <print>
#include <set>
#include <vector>

int main() {
  int ia = 1, ib = 2, ic = 0;
  unsigned int ua = 3, ub = 5;
  float fa = 5.f, fb = 6.f;
  double da = 5.0, db = 8.0, dc = 0.0;

  std::println("ia + ib = {}", CustomConcepts::add(ia, ib));
  std::println("ia + ib = {}", CustomConcepts::add2(ia, ib)); // same as the above
  std::println("fa + fb = {}", CustomConcepts::add(fa, fb));
  std::println("da + db = {}", CustomConcepts::add(da, db));

  // std::println("ia / 0 = {}", CustomConcepts::divide(ia, ic));  // exception
  std::println("0 / ia = {}", CustomConcepts::divide(ic, ia));
  std::println("0.0 / db = {}", CustomConcepts::divide(dc, db));
  // std::println("da / 0.0 = {}", CustomConcepts::divide(da, dc));  // exception
  
  // CustomConcepts::divide_not_zero(ia, ic);  // exception
  std::println("zero_value(ia) = {}", CustomConcepts::zero_value(ia));
  std::println("zero_value(0) = {}", CustomConcepts::zero_value(ic));
  std::println("zero_value(0.0) = {}", CustomConcepts::zero_value(dc));
  // CustomConcepts::add(ia, ub);  // error!!
  // CustomConcepts::add(fa, ib);  /// error!!
  // CustomConcepts::add(fa, db);    // error!!


  {
    std::vector<std::int32_t> source_container = {1, 2, 3, 5, 8, 13, 21, 3435};    
    {
      std::vector<std::int32_t> dest_container;
      CustomConcepts::cont_assign(dest_container, source_container);
    }
    {
      std::set<std::int32_t> dest_container;
      CustomConcepts::cont_assign(dest_container, source_container);
    }
  }
  return 0;
}