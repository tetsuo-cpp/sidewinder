#ifndef SIDEWINDER_SERVER_H
#define SIDEWINDER_SERVER_H

#include "Address.h"
#include "Interfaces.h"

#include <unordered_map>
#include <vector>

namespace sidewinder {

struct ServerConfig {
  size_t bufSize = 1024;
};

class Server : public IFdHandler {
public:
  Server(ICore &core, IServerHandler &handler, Address addr,
         const ServerConfig &config);
  virtual ~Server();

  // IFdHandler impl.
  void onReadable(int fd) override;

  void start();
  void stop();
  void deregisterConnection(IConnection *conn);

private:
  void acceptConnection();
  void readData(int fd);

  struct ConnectionInfo {
    ConnectionInfo(IConnection *conn, size_t bufSize) : offset(0), conn(conn) {
      buffer.resize(bufSize, 0);
    }

    int offset;
    std::vector<char> buffer;
    IConnection *conn;
  };

  ICore &core;
  IServerHandler &handler;
  const Address addr;
  const ServerConfig config;
  int socketFd;
  std::unordered_map<int, ConnectionInfo> conns;
};

} // namespace sidewinder

#endif // SIDEWINDER_SERVER_H
