#pragma once

#include "types.hpp"

namespace stp {
namespace timer {

// Millisecond resolution.

session_t Timeout(uint64 mescs);
void Sleep(uint64 msecs);

void Timeout(process_t pid, session_t session, uint64 msecs);

// zero-based monotonic time from startup.
uint64 Time();

// realtime at startup.
uint64 StartTime();

// monotonic realtime, same as StartTime() + Time().
uint64 RealTime();

// update monotonic time, return updated Time().
uint64 UpdateTime();

}
}
