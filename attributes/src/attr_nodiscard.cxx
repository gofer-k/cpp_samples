export module attr_nodiscard;

[[nodiscard("Remember to return value.")]]
int foo() {
  return 23;
}

struct Socket {
  // empty resource, no harm if discarded
  Socket() = default;
    
  [[nodiscard("not allowed to handle socket")]]
  Socket(int fd) : handle_(fd) {};

private: 
  int handle_{};
};

export void test_nodiscard() {
  // test whether it's supported
  static_assert(__has_cpp_attribute(nodiscard) == 201907L);

  foo();  // Warning with custom message
  
  auto res = foo(); // OK no warning

  Socket{};     // OK
  Socket{1};    // warning
}