#include "Profiler.h"

#include <stdint.h>
#include <iostream>

namespace
{
inline uint64_t toNs(struct timespec &ts)
{
  return ((uint64_t) ts.tv_sec) * ((uint64_t) 1000'000'000ull) + (uint64_t)ts.tv_nsec;
}

inline uint64_t timeNs()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  return toNs(ts);
}

struct RealTimeConfig {
  typedef typename std::chrono::system_clock clock;
  typedef typename std::chrono::duration<uint32_t, std::micro> duration;
};

template <size_t N>
void perform() {
  uint64_t counter = 100'000;
  uint64_t start = timeNs();
  for (size_t i = 0; i < counter; ++i) {
    Profiler::PTimeline<RealTimeConfig, N> timeline("MainStart");
    for (size_t j = 1; j < 1000; ++j) {
      Profiler::PTimePoint all("Run");
    }
  }
  uint64_t end = timeNs();
  std::cout << N << " samples with an average overhead of " << (end -start)/counter << "ns" << std::endl;
}
}

int main() {
  perform<10>();
  perform<100>();
  perform<500>();
  perform<1000>();
}
