#ifndef SIDEWINDER_CORE_H
#define SIDEWINDER_CORE_H

#include "Interfaces.h"

#include <sys/select.h>
#include <unordered_map>
#include <vector>

namespace sidewinder {

class Core : public ICore {
public:
  Core();

  // ICore impl.
  void run() override;
  void registerFd(int fd, IFdHandler *handler) override;
  void deregisterFd(int fd) override;
  void setAlarm(const Alarm &alarm) override;
  void stop() override;

private:
  void serviceAlarms();

  bool stopping;
  fd_set readers;
  std::unordered_map<int, IFdHandler *> handlerMap;
  int maxFd;
  std::vector<Alarm> alarms;
};

} // namespace sidewinder

#endif // SIDEWINDER_CORE_H
