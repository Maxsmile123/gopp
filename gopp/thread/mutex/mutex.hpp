#pragma once


// Using this article: https://dept-info.labri.fr/~denis/Enseignement/2008-IR/Articles/01-futex.pdf


#include <atomic>
#include <gopp/thread/futex/wait.hpp>

#include <cstdint>

namespace gopp::thread::stdlike {

class Mutex {
private:
 enum MutexStates : uint32_t {
   Free, 
   LockedWithoutContention, 
   LockedWithContention
 };

public:
  void Lock();

  bool TryLock();

  void Unlock();

  // BasicLockable
  // https://en.cppreference.com/w/cpp/named_req/BasicLockable

  void lock(); // NOLINT

  void unlock(); // NOLINT

  bool try_lock(); // NOLINT


 private:
  std::atomic<uint32_t> phase_{0};
};

}  // namespace gopp::thread::std
