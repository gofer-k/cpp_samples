import async_features;
import barrier_samples;
import jthread_sample;
import free_lock_samples;
import latch_samples;
import mutex_samples;
import route_simulator;
import locks_samples;

int main() {
  // Your code here
  test_jthread_construct();
  test_jthread_interrupt();
  test_free_lock_queue();
  test_mutex_samples();
  test_scope_lock_sample();
  test_latch_sample();
  test_barrier_sample();
  async_features::test_async();
  simulate_route();
  return 0;
}