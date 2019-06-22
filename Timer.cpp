#include "Timer.h"

namespace sidewinder {

Timer::~Timer() { stop(); }

void Timer::onTimer() {
  func();
  alarm.time += interval;
  core.registerAlarm(&alarm);
}

void Timer::stop() { core.deregisterAlarm(&alarm); }

} // namespace sidewinder