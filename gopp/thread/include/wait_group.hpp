#pragma once

#include <gopp/thread/include/mutex.hpp>
#include <gopp/thread/include/condvar.hpp>

#include <atomic>
#include <cstdlib>
#include <iostream>

namespace gopp::thread::sync {

class WaitGroup {
 public:
  // += count
  void Add(size_t count) {
    std::unique_lock lock(mutex_);
    counter_.fetch_add(count, std::memory_order_release);
  }

  // =- 1
  void Done() {
    std::unique_lock lock(mutex_);
    if(counter_.fetch_sub(1, std::memory_order_release) == 1){
      is_zero.notify_all();
    }
  }

  // == 0
  // One-shot
  void Wait() {
    std::unique_lock lock(mutex_);
    while (counter_.load(std::memory_order_acquire) > 0){
      is_zero.wait(lock);
    }
  }

 private:
  std::atomic<uint32_t> counter_{0};
  CondVar is_zero;
  Mutex mutex_;
};

} // namespace gopp::thread::sync
