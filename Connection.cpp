#include "Connection.h"
#include <sys/socket.h>

namespace sidewinder {

Connection::Connection(int fd, Server &server) : fd(fd), server(server) {}

Connection::~Connection() { server.deregisterConnection(this); }

void Connection::sendData(const char *data, int len) {
  int bytesSent = send(fd, data, len, 0);
  if (bytesSent < 0)
    throw std::runtime_error("send call failed");
}

} // namespace sidewinder