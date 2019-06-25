#include "Client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <zconf.h>

namespace sidewinder {

Client::Client(ICore &core, IClientHandler &handler, Address addr)
    : core(core), handler(handler), addr(std::move(addr)), socketFd(-1),
      offset(0) {}

Client::~Client() {
  core.deregisterFd(socketFd);
  if (socketFd > 0)
    close(socketFd);
}

void Client::init() {
  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd < 0)
    throw std::runtime_error("failed socket call");

  connectTimer = std::make_unique<Timer>([this]() { attemptConnect(); },
                                         std::chrono::seconds(5), core);
}

void Client::attemptConnect() {
  const auto sockAddr = addr.getSockAddrIn();
  if (connect(socketFd, reinterpret_cast<const sockaddr *>(&sockAddr),
              sizeof(sockAddr)) < 0)
    return;

  core.registerFd(socketFd, this);
  assert(connectTimer);
  connectTimer->stop();
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
