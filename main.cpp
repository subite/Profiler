#include <iostream>

#include "Profiler.h"
#include "ProfilerUtils.h"
#include <chrono>
#include <thread>
#include <random>
#include <fstream>

using namespace std::chrono_literals;
using namespace Profiler;

namespace {
std::random_device rd_fun;
std::mt19937 gen_fun(rd_fun());
std::uniform_int_distribution<> dis_fun(1, 10);

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 100);

void func_1();
void func_2();
void func_3();
void func_4();
void func_5();
void func_6();
void func_7();
void func_8();
void func_9();

void func_1() {
  PTimePoint all("func_1");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 1)
    func_9();
}

void func_2() {
  PTimePoint all("func_2");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 2)
    func_1();
}

void func_3() {
  PTimePoint all("func_3");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 3)
    func_2();
}

void func_4() {
  PTimePoint all("func_4");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 4)
    func_3();
}

void func_5() {
  PTimePoint all("func_5");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 5)
    func_4();
}

void func_6() {
  PTimePoint all("func_6");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 6)
    func_5();
}

void func_7() {
  PTimePoint all("func_7");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 7)
    func_6();
}

void func_8() {
  PTimePoint all("func_8");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 8)
    func_7();
}

void func_9() {
  PTimePoint all("func_9");
  std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
  int r = dis_fun(gen_fun);
  if (r <= 9)
    func_8();
}
}


struct RealTimeConfig {
  typedef typename std::chrono::system_clock clock;
  typedef typename std::chrono::duration<uint32_t, std::micro> duration;
};

int main() {
  Profiler::PTimeline<RealTimeConfig, 1000> timeline("MainStart");
  {
    PTimePoint all("Global scope");
    for (int i = 1; i < 100; ++i) {
      int r = dis_fun(gen_fun);
      switch(r) {
        case 1: func_1(); break;
        case 2: func_2(); break;
        case 3: func_3(); break;
        case 4: func_4(); break;
        case 5: func_5(); break;
        case 6: func_6(); break;
        case 7: func_7(); break;
        case 8: func_8(); break;
        default: func_9(); break;
      }
      //PTimePoint loop("Inner scope");
      //std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
  }
  timeline.stop();
  std::ofstream file;
  file.open ("timeline.json");
  toChromeTracingFormat(file, timeline);
  file.flush();
  file.close();
  return 0;
}
