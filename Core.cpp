#include "Core.h"

namespace sidewinder {

Core::Core() : stopping(false), maxFd(-1) { FD_ZERO(&readers); }

void Core::run() {
  while (!stopping) {
    timeval tv{1, 0};
    fd_set readSet = readers;

    int numFds = select(maxFd + 1, &readSet, nullptr, nullptr, &tv);
    if (numFds < 0)
      throw std::runtime_error("failed select call");

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

void Core::stop() { stopping = true; }

} // namespace sidewinder
