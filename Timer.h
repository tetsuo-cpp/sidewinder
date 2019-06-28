#ifndef SIDEWINDER_TIMER_H
#define SIDEWINDER_TIMER_H

#include "Interfaces.h"

namespace sidewinder {

class Timer {
public:
  template <typename T>
  Timer(T &&func, std::chrono::system_clock::duration interval, ICore &core)
      : func(std::forward<T>(func)), interval(interval), core(core),
        isSetVal(false) {
    alarm.func = [this]() { onTick(); };
  }
  virtual ~Timer();

  void set();
  void unset();

private:
  void onTick();

  const std::function<void()> func;
  const std::chrono::system_clock::duration interval;
  ICore &core;
  Alarm alarm;
  bool isSetVal;
};

} // namespace sidewinder

#endif // SIDEWINDER_TIMER_H
