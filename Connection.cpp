#include "Connection.h"

#include <sys/socket.h>

namespace sidewinder {

Connection::Connection(int fd, Server &server, int sendRetries)
    : fd(fd), server(server), sendRetries(sendRetries) {}

Connection::~Connection() { server.deregisterConnection(this); }

void Connection::sendData(const char *data, int len) {
  int totalBytesSent = 0;
  int numAttempts = 0;
  // Sends can get interrupted so we need to try repeatedly.
  while (totalBytesSent < len) {
    int bytesSent = send(fd, data, len, 0);
    if (bytesSent < 0)
      throw std::runtime_error("failed send call");
    totalBytesSent += bytesSent;
    if (++numAttempts > sendRetries)
      throw std::runtime_error("repeatedly failed to send");
  }
}

} // namespace sidewinder