#pragma once


// Using this article: https://dept-info.labri.fr/~denis/Enseignement/2008-IR/Articles/01-futex.pdf


#include <atomic>
#include <gopp/thread/futex/wait.hpp>

#include <cstdint>

namespace gopp::thread::std {



class Mutex {
private:
 enum MutexStates : uint32_t {
   Free, 
   LockedWithoutContention, 
   LockedWithContention
 };

public:
  void Lock() {
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

  bool TryLock() {
    auto free_state = static_cast<uint32_t>(MutexStates::Free);
    if(phase_.compare_exchange_strong(free_state, MutexStates::LockedWithoutContention)) {
      return true;
    } else {
      return false;
    }
  }

  void Unlock() {
    auto key = gopp::thread::futex::PrepareWake(phase_);
    if (phase_.exchange(MutexStates::Free) == LockedWithContention) {
      gopp::thread::futex::WakeOne(key);
    }
  }

  // BasicLockable
  // https://en.cppreference.com/w/cpp/named_req/BasicLockable

  void lock() {  // NOLINT
    Lock();
  }

  void unlock() {  // NOLINT
    Unlock();
  }

  bool try_lock() { // NOLINT
    return TryLock();
  }


 private:
  std::atomic<uint32_t> phase_{0};
};

}  // namespace gopp::thread::std
