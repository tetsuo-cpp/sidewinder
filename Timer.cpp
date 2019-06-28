#include "Timer.h"

namespace sidewinder {

Timer::~Timer() { unset(); }

void Timer::onTick() {
  func();
  isSetVal = false;
  set();
}

void Timer::set() {
  if (isSetVal)
    return;
  isSetVal = true;

  // Set the alarm to go off at the next interval.
  alarm.time = std::chrono::system_clock::now() + interval;
  core.registerAlarm(&alarm);
}

void Timer::unset() {
  isSetVal = false;
  core.deregisterAlarm(&alarm);
}

} // namespace sidewinder