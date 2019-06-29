#ifndef SIDEWINDER_CONNECTION_H
#define SIDEWINDER_CONNECTION_H

#include "Interfaces.h"
#include "Server.h"

namespace sidewinder {

class Connection : public IConnection {
public:
  Connection(int fd, Server &server, int sendRetries);
  virtual ~Connection();

  void sendData(const char *data, int len) override;

private:
  int fd;
  Server &server;
  int sendRetries;
};

} // namespace sidewinder

#endif // SIDEWINDER_CONNECTION_H
