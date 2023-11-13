#pragma once

#include <cstdint>
#include <cstddef>
#include <climits>

namespace gopp::thread {

// Wrappers for platform-specific system calls (futex, ulock, WaitOnAddress)

int SystemWait(uint32_t* addr, uint32_t old);
int SystemWaitTimed(uint32_t* addr, uint32_t expected, uint32_t millis);

int SystemWake(uint32_t* addr, size_t count);

}  // namespace gopp::thread
