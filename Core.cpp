//
// Created by Alex Cameron on 2019-06-10.
//

#include "Core.h"

namespace sidewinder {

Core::Core() : stopping(false) {
  FD_ZERO(&readers);
  FD_ZERO(&writers);
}

void Core::run() {
  while (!stopping) {
    timeval tv{0, 0};
    int numFds = select(0, &readers, &writers, nullptr, &tv);
    if (numFds < 0)
      throw std::runtime_error("error in select call");

    for (int i = 0; i < FD_SETSIZE; ++i) {
      auto iter = handlerMap.find(i);
      if (iter == handlerMap.end())
        continue;

      if (FD_ISSET(i, &readers))
        iter->second->onReadable(i);
      if (FD_ISSET(i, &writers))
        iter->second->onWriteable(i);
    }
  }
}

void Core::registerFd(int fd, IFdHandler *handler, FdMode mode) {
  switch (mode) {
  case FdMode::FM_Read:
    FD_SET(fd, &readers);
    break;
  case FdMode::FM_Write:
    FD_SET(fd, &writers);
    break;
  case FdMode::FM_ReadWrite:
    FD_SET(fd, &readers);
    FD_SET(fd, &writers);
    break;
  }

  handlerMap.emplace(fd, handler);
}

void Core::deregisterFd(int fd) {
  FD_CLR(fd, &readers);
  FD_CLR(fd, &writers);
  handlerMap.erase(fd);
}

void Core::stop() { stopping = true; }

} // namespace sidewinder