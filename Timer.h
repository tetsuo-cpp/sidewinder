#ifndef SIDEWINDER_TIMER_H
#define SIDEWINDER_TIMER_H

#include "Interfaces.h"

namespace sidewinder {

class Timer {
public:
  template <typename T>
  Timer(T &&func, std::chrono::system_clock::duration interval, ICore &core)
      : func(std::forward<T>(func)), interval(interval), core(core) {
    // Set the alarm to go off at the next interval.
    alarm.func = [this]() { onTimer(); };
    alarm.time = std::chrono::system_clock::now() + interval;
    core.registerAlarm(&alarm);
  }

  virtual ~Timer();

  void onTimer();
  void stop();

private:
  const std::function<void()> func;
  const std::chrono::system_clock::duration interval;
  ICore &core;
  Alarm alarm;
};

} // namespace sidewinder

#endif // SIDEWINDER_TIMER_H
