#include <vector>
#include <string>

void vectorRefs() {
  // This doesn't compile
  // std::vector<std::string&> vec;

  // This doesn't compile either
  // std::vector<const std::string> vec;

  std::string s1{"Hello"};
  std::string s2{","};
  std::string s3{"World!"};
  std::vector<std::reference_wrapper<std::string>> v {
    std::ref(s1), std::ref(s2), std::ref(s3)
  }; 
}

void func_refs(int& p1, int& p2, const int& p3) {    
    ++p1; // increments the copy of n1 stored in the function object
    ++p2; // increments the main()'s n2
    // ++p3; // compile error
}