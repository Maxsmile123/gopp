#pragma once

#include <gopp/thread/include/mutex.hpp>
#include <gopp/thread/include/condvar.hpp>

#include <cstdlib>
#include <iostream>

namespace gopp::thread::sync {
class Barrier {
 public:
  explicit Barrier(size_t participants): cap(participants) {}

  void Wait() {
    std::unique_lock lock(mutex_);
    size_t cur_iter = iteration;
    ++calls;
    if (calls == cap){
      calls = 0;
      ++iteration;
      is_zero_.notify_all();
    } else{
      while (cur_iter == iteration){
        is_zero_.wait(lock);
      }
    }
  }

 private:
  const size_t cap;
  size_t calls = 0;
  size_t iteration = 0;
  Mutex mutex_;
  CondVar is_zero_;
};

} // gopp::thread::sync
