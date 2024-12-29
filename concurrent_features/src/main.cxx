import jthread_sample;
import free_lock_samples;
import mutex_samples;

int main() {
  // Your code here
  test_jthread_construct();
  test_jthread_interrupt();
  test_free_lock_queue();
  test_mutex_samples();
  return 0;
}