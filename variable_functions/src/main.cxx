#include <print>
#include <iostream>

template<typename... Ts>
void print_args2(Ts... args) {
  if constexpr (sizeof...(args) > 0) {
    std::println("{}", args...);   // print only the first element
    
    // Print the all pack elements with print separator
    // Unary right fold (E op ...) becomes (E1 op (... op (EN-1 op EN)))
    // E = (std::cout << args << ", ")
    // op = ',' 
    // ... = pack
    ((std::cout << args << ", "), ...) << std::endl;
    return;
  }
  std::println("Empty arguments");
}

template<typename... Args>
void print_args(Args... args) { 
  print_args2(++args...);  // increment each numeric pack elements
  // auto args2 = ++args...; // not compile - it's not scalar valu  
}

int main() {
  // print_args(123, -2354.789,  true, "text");  // not compile because bool type is in the pack  
  print_args(123, -2354.789,  "text"); 
  print_args();
  return 0;
}