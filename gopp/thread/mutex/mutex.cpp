#include "gopp/thread/mutex/mutex.hpp"

namespace gopp::thread::stdlike {

 void Mutex::Lock() {
    auto free_state = static_cast<uint32_t>(MutexStates::Free);
    if(phase_.compare_exchange_strong(free_state, MutexStates::LockedWithoutContention)) {
      return;
    }

    while(phase_.exchange(MutexStates::LockedWithContention) != MutexStates::Free) {
        while(phase_.load() != MutexStates::Free) {
            gopp::thread::futex::Wait(phase_, MutexStates::LockedWithContention);
        }
    }
  }

  bool Mutex::TryLock() {
    auto free_state = static_cast<uint32_t>(MutexStates::Free);
    if(phase_.compare_exchange_strong(free_state, MutexStates::LockedWithoutContention)) {
      return true;
    }
    return false;
  }

  void Mutex::Unlock() {
    auto key = gopp::thread::futex::PrepareWake(phase_);
    if (phase_.exchange(MutexStates::Free) == LockedWithContention) {
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



} // namespace gopp::thread::std
