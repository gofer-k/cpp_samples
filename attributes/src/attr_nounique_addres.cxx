module;


export module attr_nounique_address;

struct Empty{};

struct NoUniqueField {
  int field_{1};
  Empty field2_;
};

struct UniqueField {
  int field_{1};
  [[no_unique_address]] Empty field2_;
};

export void test_no_unique_addreess_attr() {
  // both members are dictinct addresses in the NoUniqueField
  static_assert(sizeof(NoUniqueField) > sizeof(int));

  // both members are unique addresses in the UniqueField
  static_assert(sizeof(UniqueField) == sizeof(int));
}