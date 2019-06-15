#ifndef SIDEWINDER_CORE_H
#define SIDEWINDER_CORE_H

#include "Interfaces.h"

#include <sys/select.h>
#include <unordered_map>

namespace sidewinder {

class Core : public ICore {
public:
  Core();

  // ICore impl.
  void run() override;
  void registerFd(int fd, IFdHandler *handler) override;
  void deregisterFd(int fd) override;
  void stop() override;

private:
  bool stopping;
  fd_set readers;
  std::unordered_map<int, IFdHandler *> handlerMap;
  int maxFd;
};

} // namespace sidewinder

#endif // SIDEWINDER_CORE_H
