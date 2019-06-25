#include "Client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <zconf.h>

namespace sidewinder {

Client::Client(ICore &core, IClientHandler &handler, Address addr)
    : core(core), handler(handler), addr(std::move(addr)), offset(0) {}

Client::~Client() {
  core.deregisterFd(socketFd);
  close(socketFd);
}

void Client::init() {
  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd < 0)
    throw std::runtime_error("failed socket call");

  auto sockAddr = addr.getSockAddrIn();
  if (connect(socketFd, reinterpret_cast<const sockaddr *>(&sockAddr),
              sizeof(sockAddr)) < 0)
    throw std::runtime_error("failed connect call");

  core.registerFd(socketFd, this);
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
