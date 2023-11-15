#pragma once

#include <gopp/thread/sys/futex/system/wait.hpp>

#include <atomic>
#include <chrono>
#include <cstdint>

namespace gopp::thread{
namespace futex{

struct WakeKey {
  uint32_t* addr;
};

void Wait(std::atomic<uint32_t>& atomic, uint32_t old,
          std::memory_order mo = std::memory_order::seq_cst);

bool WaitTimed(std::atomic<uint32_t>& atomic, uint32_t old,
               std::chrono::milliseconds timeout,
               std::memory_order mo = std::memory_order::seq_cst);



WakeKey PrepareWake(std::atomic<uint32_t>& atomic);

void WakeOne(WakeKey key);
void WakeAll(WakeKey key);


} // namespace futex
} // namespace gopp::thread