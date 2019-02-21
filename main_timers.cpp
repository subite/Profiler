#include <time.h>
#include <stdint.h>

#include <iostream>
#include <iomanip>

namespace {

  inline uint64_t toNs(struct timespec &ts) {
    return ((uint64_t)ts.tv_sec) * ((uint64_t)1000'000'000ull) + (uint64_t)ts.tv_nsec;
  }

  inline uint64_t timeNs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return toNs(ts);
  }

  void testClock(const char *id, clockid_t clk_id)
  {
    struct timespec ts;
    clock_gettime(clk_id, &ts);
    uint64_t start = timeNs();
    uint64_t count = 1'000'000;
    for (size_t i = 0; i < count; ++i) {
      clock_gettime(clk_id, &ts);
    }
    uint64_t end = timeNs();
    clock_getres(clk_id, &ts);
    uint64_t resolution = toNs(ts);
    std::cout << std::setw(25) << id
              << " resolution: " << std::setw(8) << resolution << "ns overhead: "
              << std::setw(6) << (end -start)/count << "ns" << std::endl;
  }
}
int main() {
  testClock("CLOCK_PROCESS_CPUTIME_ID", CLOCK_PROCESS_CPUTIME_ID);
  testClock("CLOCK_THREAD_CPUTIME_ID", CLOCK_THREAD_CPUTIME_ID);
  testClock("CLOCK_MONOTONIC_RAW", CLOCK_MONOTONIC_RAW);
  testClock("CLOCK_BOOTTIME", CLOCK_BOOTTIME);
  testClock("CLOCK_REALTIME", CLOCK_REALTIME);
  testClock("CLOCK_MONOTONIC", CLOCK_MONOTONIC);
  testClock("CLOCK_MONOTONIC_COARSE", CLOCK_MONOTONIC_COARSE);
}
