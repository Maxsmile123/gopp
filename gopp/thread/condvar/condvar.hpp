#pragma once

#include <gopp/thread/sys/futex/wait.hpp>

#include <atomic>
#include <cstdint>

namespace gopp::thread::stdlike {

class CondVar {
 public:
  template <class Mutex>
  void Wait(Mutex& mutex) {
    auto ticket = counter_.load(std::memory_order_relaxed); // cause using in critical section
    mutex.unlock();
    gopp::thread::futex::Wait(counter_, ticket, std::memory_order_acquire);
    mutex.lock();
  }

  void NotifyOne() {
    auto wake_key = gopp::thread::futex::PrepareWake(counter_);
    counter_.fetch_add(1, std::memory_order_release);
    gopp::thread::futex::WakeOne(wake_key);
  }

  void NotifyAll() {
    auto wake_key = gopp::thread::futex::PrepareWake(counter_);
    counter_.fetch_add(1, std::memory_order_release);
    gopp::thread::futex::WakeAll(wake_key);
  }

 private:
  std::atomic<uint32_t> counter_{0};
};
} // namespace gopp::thread::stdlike