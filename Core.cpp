#include "Core.h"

namespace sidewinder {

Core::Core() : stopping(false), maxFd(-1) { FD_ZERO(&readers); }

void Core::run() {
  while (!stopping) {
    timeval tv{0, 0};
    fd_set readSet = readers;

    int numFds = select(maxFd + 1, &readSet, nullptr, nullptr, &tv);
    if (numFds < 0)
      throw std::runtime_error("failed select call");
    if (numFds == 0) {
      serviceAlarms();
      continue;
    }

    for (int i = 0; i < FD_SETSIZE; ++i) {
      auto iter = handlerMap.find(i);
      if (iter == handlerMap.end())
        continue;
      if (FD_ISSET(i, &readSet))
        iter->second->onReadable(i);
    }
  }
}

void Core::registerFd(int fd, IFdHandler *handler) {
  FD_SET(fd, &readers);
  handlerMap.emplace(fd, handler);
  maxFd = std::max(fd, maxFd);
}

void Core::deregisterFd(int fd) {
  FD_CLR(fd, &readers);
  handlerMap.erase(fd);
}

void Core::registerAlarm(const Alarm *alarm) { alarms.push_back(alarm); }

void Core::deregisterAlarm(const Alarm *alarm) {
  alarms.erase(std::remove(alarms.begin(), alarms.end(), alarm), alarms.end());
}

void Core::stop() { stopping = true; }

void Core::serviceAlarms() {
  const auto now = std::chrono::system_clock::now();
  for (const auto &alarm : alarms) {
    if (now >= alarm->time)
      alarm->func();
  }

  // Get rid of anything that triggered.
  alarms.erase(
      std::remove_if(alarms.begin(), alarms.end(),
                     [now](const Alarm *alarm) { return now >= alarm->time; }),
      alarms.end());
}

} // namespace sidewinder
