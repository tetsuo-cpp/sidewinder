//
// Created by Alex Cameron on 2019-06-10.
//

#ifndef SIDEWINDER_INTERFACES_H
#define SIDEWINDER_INTERFACES_H

namespace sidewinder {

class IFdHandler {
public:
  virtual void onWriteable(int fd) = 0;
  virtual void onReadable(int fd) = 0;
};

enum class FdMode { FM_Read, FM_Write, FM_ReadWrite };

class ICore {
public:
  virtual void run() = 0;
  virtual void registerFd(int fd, IFdHandler *handler, FdMode mode) = 0;
  virtual void deregisterFd(int fd) = 0;
  virtual void stop() = 0;
};

} // namespace sidewinder

#endif // SIDEWINDER_INTERFACES_H
