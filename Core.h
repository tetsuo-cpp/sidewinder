//
// Created by Alex Cameron on 2019-06-10.
//

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
  void registerFd(int fd, IFdHandler *handler, FdMode mode) override;
  void deregisterFd(int fd) override;
  void stop() override;

private:
  bool stopping;
  fd_set readers;
  fd_set writers;
  std::unordered_map<int, IFdHandler *> handlerMap;
};

} // namespace sidewinder

#endif // SIDEWINDER_CORE_H
