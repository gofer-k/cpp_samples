#include "std_ref_wrappes.hpp"
#include <functional>
#include <print>
#include <future>

int main() {
  vectorRefs();

  {
    /*
    From: https://www.sandordargo.com/blog/2024/08/21/std-ref
    In this example, f() takes all the integers by reference. The third one is also const.
    If we call func_refs() directly, we get the desired behaviour, n1 and n2 are “permanently” modified.
    
    On the other hand, the behaviour is different when we invoke func_refs() with the help of std::function.
    When we don’t use std::ref or std::cref to bind the arguments, they are simply copied.
    That’s why even though n1 is seemingly passed as a reference, in bound_f, 
    p1 still has the value it had when the binding was made. Likewise, it doesn’t increase the value of n1. 
    But if we call bound_f() the second time, we can see that p1’s value has changed.
    What that means is that std::function holds on a copy of n1 and that is updated between the calls.

    If we want p1 to be an actual reference of n1, we must pass it with the help of std::ref,
    just as we did it for n2/p2 and for n3/p3.
  */
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = 
    std::bind( // bind function with refermce arguments
      func_refs,
      n1,  
      std::ref(n2),
      std::cref(n3));

    n1 = 10;
    n2 = 11;
    n3 = 12;
    std::println("Before calling func_refs() directly: {}, {}, {}", n1, n2, n3);
    func_refs(n1, n2, n3);
    
    std::println("After calling func_refs() directly: {}, {}, {}", n1, n2, n3);
    std::println("=================");
    
    std::println("Before calling bound_func_refs(): {}, {}, {}",  n1, n2, n3);
    bound_f();
    bound_f();
    std::println("After calling bound_func_refs(): {}, {}, {}", n1, n2, n3);
  }
  
  {
    auto test = [value = 0]() mutable {
    std::println("Hello from lambda {}", ++value);
    };

    std::function<void()> move_func = std::move(test);
    move_func();
    move_func();

    std::packaged_task<double()> packaged_task([](){ return 3.14159; });
    std::future<double> future = packaged_task.get_future();
    auto lambda = [task = std::move(packaged_task)]() mutable { task(); };
    // std::function<void()> function = std::move(lambda); // Error
    std::move_only_function<void()> function = std::move(lambda); // OK
    function();
    std::println("Value from erasure function: {}", future.get());
  }
  
  return 0;
}
