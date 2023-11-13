


namespace gopp::thread{
namespace std{

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



}
}