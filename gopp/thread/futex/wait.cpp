#include <gopp/thread/futex/wait.hpp>


namespace gopp::thread{
namespace futex{

static uint32_t* Addr(std::atomic<uint32_t>& atomic) {
  return (uint32_t*)&atomic;
}

void Wait(std::atomic<uint32_t>& atomic, uint32_t old, std::memory_order mo) {
  while (atomic.load(mo) == old) {
    SystemWait(Addr(atomic), old);
  }
}

WakeKey PrepareWake(std::atomic<uint32_t>& atomic) {
  return {Addr(atomic)};
}

static std::chrono::milliseconds ToMillis(std::chrono::nanoseconds ns) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(ns);
}

bool WaitTimed(std::atomic<uint32_t>& atomic, uint32_t old,
               std::chrono::milliseconds timeout, std::memory_order mo) {
  using Clock = std::chrono::steady_clock;

  const auto deadline = Clock::now() + timeout;

  while (atomic.load(mo) == old) {
    auto now = Clock::now();

    if (now >= deadline) {
      return false;
    }

    // deadline > now
    std::chrono::milliseconds left = ToMillis(deadline - now);
    SystemWaitTimed(Addr(atomic), old, /*millis=*/left.count() + 1);
  }

  return true;
}

void WakeOne(WakeKey key) {
  SystemWake(key.addr, 1);
}

void WakeAll(WakeKey key) {
  SystemWake(key.addr, INT32_MAX);
}



}
}