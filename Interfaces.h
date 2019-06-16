#ifndef SIDEWINDER_INTERFACES_H
#define SIDEWINDER_INTERFACES_H

#include <memory>

namespace sidewinder {

class IFdHandler {
public:
  virtual ~IFdHandler() = default;
  virtual void onReadable(int fd) = 0;
};

class ICore {
public:
  virtual ~ICore() = default;
  virtual void run() = 0;
  virtual void registerFd(int fd, IFdHandler *handler) = 0;
  virtual void deregisterFd(int fd) = 0;
  virtual void stop() = 0;
};

class IConnection {
public:
  virtual ~IConnection() = default;
  virtual void sendData(const char *data, int len) = 0;
};

class IServerHandler {
public:
  virtual ~IServerHandler() = default;
  virtual void onConnection(std::unique_ptr<IConnection> conn) = 0;
  virtual bool handleData(const char *data, int len, IConnection *conn) = 0;
};

class IClientHandler {
public:
  virtual ~IClientHandler() = default;
  virtual bool handleData(const char *data, int len) = 0;
};

} // namespace sidewinder

#endif // SIDEWINDER_INTERFACES_H
