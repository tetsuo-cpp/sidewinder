#include "Client.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <zconf.h>

namespace sidewinder {

Client::Client(ICore &core, IClientHandler &handler)
    : core(core), handler(handler), offset(0) {
  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd < 0)
    throw std::runtime_error("failed socket call");

  sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(7980);

  if (connect(socketFd, reinterpret_cast<const sockaddr *>(&address),
              sizeof(address)) < 0)
    throw std::runtime_error("failed connect call");

  core.registerFd(socketFd, this);
}

Client::~Client() {
  core.deregisterFd(socketFd);
  close(socketFd);
}

void Client::onReadable(int fd) {
  assert(fd == socketFd);
  if (offset == buffer.size())
    throw std::runtime_error("buffer filled up");

  int bytesRead = read(fd, buffer.data() + offset, buffer.size() - offset);
  if (bytesRead < 0)
    throw std::runtime_error("read call failed");

  const bool handled = handler.handleData(buffer.data(), offset + bytesRead);

  if (handled)
    offset = 0;
  else
    offset += bytesRead;
}

void Client::sendData(const char *data, int len) {
  int bytesSent = send(socketFd, data, len, 0);
  if (bytesSent < 0)
    throw std::runtime_error("send call failed");
}

} // namespace sidewinder
