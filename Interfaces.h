#ifndef SIDEWINDER_INTERFACES_H
#define SIDEWINDER_INTERFACES_H

#include <chrono>
#include <functional>
#include <memory>

namespace sidewinder {

struct Alarm {
  std::function<void()> func;
  std::chrono::system_clock::time_point time;
};

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
  virtual void registerAlarm(const Alarm *alarm) = 0;
  virtual void deregisterAlarm(const Alarm *alarm) = 0;
  virtual void stop() = 0;
};

class IConnection {
public:
  virtual ~IConnection() = default;
  virtual void sendData(const char *data, int len) = 0;
};

enum class ServerError { ReadFailed, BufferReset };

class IServerHandler {
public:
  virtual ~IServerHandler() = default;
  virtual void onConnection(std::unique_ptr<IConnection> conn) = 0;
  virtual void onDisconnection(IConnection *conn) = 0;
  virtual bool handleData(const char *data, int len, IConnection *conn) = 0;
  virtual void onError(ServerError, const std::string &) {}
};

enum class ClientError { ReadFailed, RetriesExhausted, BufferReset };

class IClientHandler {
public:
  virtual ~IClientHandler() = default;
  virtual void onConnection() = 0;
  virtual void onDisconnection() = 0;
  virtual bool handleData(const char *data, int len) = 0;
  virtual void onError(ClientError, const std::string &) {}
};

} // namespace sidewinder

#endif // SIDEWINDER_INTERFACES_H
