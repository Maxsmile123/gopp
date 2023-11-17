#pragma once

#include "gopp/thread/include/mutex.hpp"
#include "gopp/thread/include/condvar.hpp"

#include <cstdlib>
#include <iostream>

namespace gopp::thread::sync {

class Semaphore {
 public:
  explicit Semaphore(size_t tokens): counter_(tokens) {}

  void Acquire() {
    std::unique_lock lock(mutex_);
    ++counter_;
    while(counter_ > 0){
      more_then_zero_.Wait(lock);
    }
  }

  void Release() {
    std::lock_guard lock(mutex_);
    --counter_;
    more_then_zero_.NotifyOne();
  }

 private:
  size_t counter_ = 0;
  CondVar more_then_zero_;
  Mutex mutex_;
};

} // namespace gopp::thread::sync