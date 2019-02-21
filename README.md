# Profiler

This is the base for a C++14 low overhead always-on profiler for linux.

This code is accompanied by a presentation given to my colleagues to show them that profiling and low latency software can go hand in hand and that profiling can be enabled in production code with a very small overhead.

# Overview
- Profiler.h/cpp
  The main classes with timeline, ...
- ProfilerUtils.h/cpp
  Helper functionc to convert the timeline to the chrome tracing format.
  This will generate a file that can be vizualised with "chrome://tracing" in chrome.
- main.cpp
  A demo application show casing the Profiler
- main_profiler_perf.cpp
  A demo application showing the overhead of profiling.
- main_timers.cpp
  A demo application measuring the overhead of different clocks sources on linux.


# License
![](https://upload.wikimedia.org/wikipedia/commons/thumb/0/0a/WTFPL_badge.svg/1200px-WTFPL_badge.svg.png)

This code is released under the **WTFPL** (Do What The Fuck You Want To Public License). So you are free to do with it what you want.

