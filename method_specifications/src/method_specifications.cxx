module;
#include <print>
#include <vector>
#include <numeric>

export module method_specifications;

struct CtorNoExceptFalse {
  // noexcept(false) explicit spec.
  // This c-tor prevent move the type object, i.e. to a  collection,
  // also c-tor can throw.
  CtorNoExceptFalse(CtorNoExceptFalse&&) noexcept(false) = default;
  CtorNoExceptFalse() noexcept(false) = default;
  CtorNoExceptFalse(const CtorNoExceptFalse&) {
    std::println("Copying CtorNoExceptFalse object");
  }
};

struct CtorNoExceptTrue {
  // noexpect(true) <=> noexpect (implicit spec.)
  // The object its allows move, also no throw 
  CtorNoExceptTrue(CtorNoExceptTrue&&) noexcept(true) = default;
  CtorNoExceptTrue() noexcept(true) = default;
  // Version to C++17
  //CtorNoExceptTrue(CtorNoExceptTrue&& ) = default;

  CtorNoExceptTrue(const CtorNoExceptTrue&) {
    std::println("Copying CtorNoExceptTrue object");
  }
};

struct Destroying {
  Destroying(std::size_t count) : alloc_(count) {
  }

  ~Destroying() {
    std::println("Destroying object released {} bytes", alloc_);
  }

  std::size_t getAllocatingSize() const {
    return alloc_;
  }

  // Class scope deleting the this type object equivalent to global operator delete
  void operator delete(Destroying* ptr, std::destroying_delete_t) {
    // without destroying_delete_t object would have been destroyed here
    const std::size_t alloc_size = ptr->getAllocatingSize();
    // now we need to call the destructor by-hand
    ptr->~Destroying();
    // and free storage it occupies
    ::operator delete(ptr, alloc_size);
  }
  // If defined, called by the custom placement new with the matching signature
  // if allocating an object of type T.
  // Note: 
  // The first parameter is not used to custom allocation size otherwise
  // global 'new' operator allocate the sizeof(pthdiif_t)
  // alloc_size is used to custom allocation!
  void* operator new(std::size_t /* not used */, std::size_t alloc_size) {
    std::println("Placement Destroying object size {} bytes", alloc_size);
    return ::operator new(alloc_size);
  }

private:
  std::size_t alloc_{0UL};
};

template<typename T>
struct ConditionalExplicit {
  explicit (!std::is_convertible_v<T, int>)
  ConditionalExplicit(T){
    auto   str = typeid(T).name();
    std::println("Instanatiate ConditionalExplicit({})", str);
  }
};

export void test_noexpect_specification(){
  // Check out c-tr default exception specification
  static_assert(std::is_nothrow_move_constructible_v<CtorNoExceptFalse> == false);
  static_assert(std::is_nothrow_move_constructible_v<CtorNoExceptTrue> == true);

  auto check_move_to_vector = []<typename T>(std::vector<T>&& vec) {
    std::vector<int> seq(3);
    std::iota(seq.begin(), seq.end(), 0);
    for(auto i : seq) {
      vec.push_back(T{});
    }

    // Prevent copy vector with the movable noexpect elements
    if constexpr (requires { std::is_nothrow_move_constructible_v<T>(); } ) {
      auto copy{vec};
    }    
  };

  {
    std::vector<CtorNoExceptFalse> vec;
    check_move_to_vector(std::move(vec));
  }
  {
    std::vector<CtorNoExceptTrue> vec;
    check_move_to_vector(std::move(vec)); 
  }
}

export void test_deleting_operator() {
  // Need to passng allocation space t c-tor,
  // unless deallocation space is hold withing object.
  // Also allocation space to 'new' perator must be repeated t the c-tor,
  // unless deallocation not match and lead to the crash.
  Destroying* test = new (100UL)Destroying(100UL);
  delete test;
}

export void test_conditional_explicit() {
  ConditionalExplicit<char> cond_exp_c = 'x'; // OK
  // ConditionalExplicit<std::string> ss1 = "x"; // Error, constructor is explicit
  ConditionalExplicit<std::string> ss2{"x"};  // OK
}