#include "gopp/thread/include/mutex.hpp"

namespace gopp::thread::sync {

 void Mutex::Lock() {
    auto free_state = static_cast<uint32_t>(MutexStates::Free);
    if(phase_.compare_exchange_strong(
        free_state, 
        MutexStates::LockedWithoutContention, 
        std::memory_order_release,
        std::memory_order_relaxed
    )) {
      return;
    }

    while(phase_.exchange(MutexStates::LockedWithContention, std::memory_order_release) != MutexStates::Free) {
        while(phase_.load(std::memory_order_acquire) != MutexStates::Free) {
            gopp::thread::futex::Wait(
                phase_, 
                MutexStates::LockedWithContention, 
                std::memory_order_acquire
            );
        }
    }
  }

  bool Mutex::TryLock() {
    auto free_state = static_cast<uint32_t>(MutexStates::Free);
    if(phase_.compare_exchange_strong(
        free_state, 
        MutexStates::LockedWithoutContention,
        std::memory_order_release,
        std::memory_order_relaxed
    )) {
      return true;
    }
    return false;
  }

  void Mutex::Unlock() {
    auto key = gopp::thread::futex::PrepareWake(phase_);
    if (phase_.exchange(MutexStates::Free, std::memory_order_release) == LockedWithContention) {
      gopp::thread::futex::WakeOne(key);
    }
  }

  // BasicLockable
  // https://en.cppreference.com/w/cpp/named_req/BasicLockable

  void Mutex::lock() {  // NOLINT
    Lock();
  }

  void Mutex::unlock() {  // NOLINT
    Unlock();
  }

  bool Mutex::try_lock() { // NOLINT
    return TryLock();
  }



} // namespace gopp::thread::sync
