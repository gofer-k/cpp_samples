#include <cstdint>
#include <vector>

struct Collection {
  using Coll =  std::vector<std::uint32_t>;
  
  Coll& getItems() {
    return vec_;
  }

  Coll vec_;
};

Collection getCollection() {
  return {};
}

void test_init_range_loop() {
  // Dangling reference to getCollection return temporary object 
  for (auto elem : getCollection().getItems()) {
  }
  
  // Safe now
  for(auto vec = getCollection(); auto elem : vec.getItems()) {
  }
}