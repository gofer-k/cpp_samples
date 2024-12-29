module;
#include <atomic>
#include <memory>
#include <cassert>
export module free_lock_samples;

// A lock-free queue implementation
// to be used in a single-producer, single-consumer scenario.
template <typename T> class spsc_lock_free_queue {
private:
  struct node {
    std::shared_ptr<T> data_;
    node *next_{nullptr};
  };
  node *pop_head() {
    node *old_head = head_.load();
    if (old_head == tail_.load()) {
      return nullptr;
    }
    head_.store(old_head->next_);
    return old_head;
  }

  std::atomic<node *> head_;
  std::atomic<node *> tail_;

public:
  spsc_lock_free_queue() : head_(new node), tail_(head_.load()) {}
  spsc_lock_free_queue(const spsc_lock_free_queue &) = delete;
  spsc_lock_free_queue &operator=(const spsc_lock_free_queue &) = delete;
  ~spsc_lock_free_queue() {
    while (node *const old_head = head_.load()) {
      head_.store(old_head->next_);
      delete old_head;
    }
  }
  std::shared_ptr<T> pop() {
    node *old_head = pop_head();
    if (!old_head) {
      return std::shared_ptr<T>();
    }
    std::shared_ptr<T> const res(old_head->data_);
    delete old_head;
    return res;
  }
  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
    node *p = new node;
    node *const old_tail = tail_.load();
    old_tail->data_ = new_data;
    old_tail->next_ = p;
    tail_.store(p);
  }
};

template <typename T> class mpsc_lock_free_queue {
private:
  struct node_counter {
    unsigned external_count : 30; // 30 bits for external count
    unsigned internal_count : 2;  // 2 bits for internal count
  };

  struct node;
  struct counted_node_ptr {
    std::atomic<std::size_t> external_count{0};
    node *ptr{nullptr};
  };
  std::atomic<counted_node_ptr *> head_;
  std::atomic<counted_node_ptr *> tail_;
  struct node {
    std::unique_ptr<T> data_;
    std::atomic<node_counter> count_{0};
    std::atomic<counted_node_ptr> next_;
    node(T const &data) : data_(std::make_unique<T>(data)) {
      node_counter new_count;
      new_count.external_count = 2; // Every new node starts out referenced
                                    // from the head and the next pointer.
      new_count.internal_count = 0;
      count_.store(new_count);
    }    
  };

public:
  mpsc_lock_free_queue() : head_(new node), tail_(head_.load()) {}
  mpsc_lock_free_queue(const mpsc_lock_free_queue &) = delete;
  mpsc_lock_free_queue &operator=(const mpsc_lock_free_queue &) = delete;
  ~mpsc_lock_free_queue() {
    // TODO: fix this
    // while (node *const old_head = head_.load()) {
    //   head_.store(old_head->next_);
    //   delete old_head;
    // }
  }
  std::unique_ptr<T> pop() {
    //TODO: Implement this
    return std::unique_ptr<T>();
  }

  void push(T new_value) {
    //TODO: Implement this    
  }
};

export void test_free_lock_queue() {
  spsc_lock_free_queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  assert(*q.pop() == 1);
  assert(*q.pop() == 2);
  assert(*q.pop() == 3);
  assert(q.pop() == nullptr);
  // mpsc_lock_free_queue<int> mpsc_q;
  // mpsc_q.push(1);
  // mpsc_q.push(2);
  // mpsc_q.push(3);
  // assert(*mpsc_q.pop() == 1);
  // assert(*mpsc_q.pop() == 2);
  // assert(*mpsc_q.pop() == 3);
  // assert(mpsc_q.pop() == nullptr);
}