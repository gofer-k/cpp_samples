module;
#include <print>
#include <memory>

export module inout_ptr;

struct Resource {
  Resource() {
    std::println("Resource acquired.");
  }
  
  Resource* operator()() {
    return this;
  }

  virtual ~Resource() noexcept{
    std::println("Resource released.");
  }
  
  void doSomething() {
    std::println("Doing something with the resource.");
  }
  
  void reset() {
    std::println("Reset the resource.");
  }
};

export void test_inout_ptr() {
  // Test case 1: Using std::out_ptr with unique_ptr
  auto deleter_resource = [](Resource* res){ res->reset(); }; 
  std::unique_ptr<Resource, decltype(deleter_resource)> res_ptr(new Resource);
  
  auto do_output_resource = [](auto res) {
    auto *ptr = *(res.operator Resource **());
    ptr->doSomething();
  };
  // Note: When std::out_ptr is instantiated with unique_ptr,
  // it will call reset() on the resource
  do_output_resource(std::out_ptr(res_ptr, deleter_resource));
}