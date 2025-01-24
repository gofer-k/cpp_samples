import memory_samples;

int main() {
  // Your code here
  memory_samples::test_default_alloc();
  memory_samples::test_default_pmr_alloc();
  memory_samples::test_custom_pmr_alloc_with_buff();
  memory_samples::test_custom_no_buff();
  return 0;
}