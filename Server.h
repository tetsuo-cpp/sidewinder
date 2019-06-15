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

  ICore &core;
  IServerHandler &handler;
  std::array<char, 1024> buffer;
  int socketFd;
  std::vector<int> fds;
  int offset;
  std::unordered_map<int, IConnection *> conns;
};

} // namespace sidewinder

#endif // SIDEWINDER_SERVER_H
