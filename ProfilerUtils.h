#ifndef PROFILERUTILS_H
#define PROFILERUTILS_H

#include "Profiler.h"
#include <ostream>

namespace Profiler {

template<class config, std::size_t N>
void toChromeTracingFormat(std::ostream &ss, const PTimeline<config, N> &timeline) {
  ss << "[";
  typename PTimeline<config, N>::TimePoint timelineStart = timeline.getStart();
  int x = 0;
  for (typename PTimeline<config, N>::const_iterator it = timeline.begin(); it != timeline.end(); ++it) {
    ss << (x == 0 ? "\n" : ",\n");
    typename PTimeline<config, N>::const_iterator::reference value = *it;
    auto begin = std::chrono::duration_cast<std::chrono::microseconds>(value.begin - timelineStart);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(value.duration);
    ss << "{\"name\": \"" << value.id << "\", \"cat\": \"PERF\", \"ph\": \"X\", \"ts\": " << begin.count() << ", \"dur\": " << duration.count() << ", \"pid\": " << 1 << ", \"tid\": " << 1 << "}";
    x++;
  }
  ss << "\n]\n";
}

}
#endif
