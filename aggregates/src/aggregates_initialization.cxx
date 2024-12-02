module;
#include <memory>
#include <print>
#include <string>

export module aggregates_initialization;

struct TopAggregate {
  int a;
  int b = 2;
  struct NestedAggregate {
    int a;
  } c;
};

struct Ref {
  const int &r;
};

int GetInt() { return 21; }

export void test_agg_init() {
  // TopAggregate{0.1};    // Error, narrowing doesn't work in the init brace
  {
    auto agg = TopAggregate(0.1); // OK works well (TopAggregate.a = 0)
    std::println(
        "Init TopAggregate(0.1) -> TopAggregate(a = {}, b = {}, c.a = {})",
        agg.a, agg.b, agg.c.a);
  }
  {
    auto agg = TopAggregate{.a = 1}; // OK
    std::string str = "Init TopAggregate{.a = 1}";
    std::println(      
        "{} -> TopAggregate(a = {}, b = {}, c.a = {})",
        str, agg.a, agg.b, agg.c.a);
  }
  
  // TopAggregate{.a = 0.1}; // Error, narrowing doesn't work here as well
  // TopAggregate(.a = 1);   // Error, no designated initializers
  {
    Ref r{GetInt()}; // OK, lifetime is extended
    std::string str = "Ref{arg}";
    std::println("{} = Ref.r = {}", str, r.r);
  }
  {
    Ref r(GetInt()); // dangling, lifetime is not extended
    
    std::string str = "Ref(arg)";
    std::println("{} = Ref.r = {}", str, r.r);
  }  

  TopAggregate{1, 2, 3}; // OK, brace elision, same as TopAggregate{1,2,{3}}
  // TopAggregate(1, 2, 3); // error, no brace elision

  // values without initializers take default values or value-initialized(T{})
  TopAggregate{1};
  TopAggregate(1);

  // make_unique works now
  auto ps =
      std::make_unique<TopAggregate>(1, 2, TopAggregate::NestedAggregate{3});

  // arrays are also supported
  int arr1[](1, 2, 3);
  int arr2[2](1); // {1, 0}
}