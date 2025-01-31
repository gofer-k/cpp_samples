module;
#include <print>
#include <vector>

export module deducing_this;

export namespace deducing_this {

template<typename Elem, typename std::size_t size>
struct Vector {
    template<typename Self>
    auto& at(this Self&& self) {
        return std::forward<Self>(self).data.at(0);
    }

    std::vector<Elem> data{size};
};

template<typename Data=std::string>
struct Obj {
  template<typename Self>
  void print_obj(this Self&& self, Data input_data) {
    return std::forward<Self>(self).print(std::move(input_data));
  }

private:
  void print(std::string input_data) const {
    std::println("Input data: ", input_data);
  }
};

struct Obj2 {
  template<typename Self>
  void print_obj(this Self&& self, std::string input_data) {
    return std::forward<Self>(self).print(std::move(input_data));
  }

private:
  void print(std::string input_data) const {
    std::println("Input data: ", input_data);
  }
};

void test_deducing_this() {
  // This code is not complite untill deducing 'this' supported in the GCC nor clang
  /*
  using Sequence = Vector<std::int32_t, 3>;
  {
    Sequence vec;
    vec.at(0);  // OK - normal call
    auto wrapper_at = Sequence::at<Sequence&>;    // OK - wrapper call type. It's used with non-const Sequence object
    wrapper_at(vec, static_cast<std::size_t>(9))  // OK - same above
  }
  {
    auto const_wrapper_at = Sequence::at<const Sequence&>;  // OK - wrapper call type. It's used with const Sequence object    
    const Sequence vec;
    wrapper_at(vec, static_cast<std::size_t>(9));
  }
  {
    const Obj obj; // OK -template type with default template argument
    obj.print_obj("Nornal call object's methed");
    // auto obj_method_wrapper = Obj::print_obj<const Obj&>; // Error - default template argumetn is not allowed here
    auto obj_method_wrapper = Obj<std::string>::print_obj<const Obj<std::string>&>;
    obj_method_wrapper(obj, "Call object's method using wrapper"); 
  }
  {
    const Obj2 obj; // OK -ordinal type use with deducing this, too.
    obj.print_obj("Nornal call object's methed");
    auto obj_method_wrapper = Obj::print_obj<const Obj&>; // Ok - deducing this
    obj_method_wrapper(obj, "Call object's method using wrapper"); 
  }   
  */
}

} // namespace deducing_this
 
