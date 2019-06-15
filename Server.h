#ifndef SIDEWINDER_SERVER_H
#define SIDEWINDER_SERVER_H

#include "Interfaces.h"

#include <array>
#include <unordered_map>
#include <vector>

namespace sidewinder {

class Server : public IFdHandler {
public:
  Server(ICore &core, IServerHandler &handler);
  virtual ~Server();

  // IFdHandler impl.
  void onReadable(int fd) override;

  void deregisterConnection(IConnection *conn);

private:
  void acceptConnection();
  void readData(int fd);

  struct ConnectionInfo {
    explicit ConnectionInfo(IConnection *conn) : offset(0), conn(conn) {}

    int offset;
    std::array<char, 1024> buffer;
    IConnection *conn;
  };

  ICore &core;
  IServerHandler &handler;
  int socketFd;
  std::unordered_map<int, ConnectionInfo> conns;
};

} // namespace sidewinder

#endif // SIDEWINDER_SERVER_H
