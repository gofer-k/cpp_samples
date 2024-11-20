#include <print>

import capture_pack_expansion;
import lambda_capture_this;
import generic_lambda;
import stateless_lambda;

int main() {
  CaptureThis x;

  x.inc_depred();

  std::println("Initial; x.field_ = {}", x.field_);
  {
    CaptureThis y = x;
    auto res = x.inc_by_copy();
    std::println("nc_by_copy = {} after x.field_ = {}, y.field_ = {}", res, x.field_, y.field_);
  }
  {
    CaptureThis y = x;
    auto res = x.inc_by_ref();
    std::println("inc_by_ref = {} after x.field_ = {}, y.field_ = {}", res, x.field_, y.field_);
  }
  
  {    
    generic_lambda_parameter();
    generic_access_argument();
  }
  
  {
    map_constructible_type.emplace("a", 1);
    map_constructible_type.emplace("b", 2);
  }
  {
    call_functor_with_pack_expasion();
  }
  return 0;
}