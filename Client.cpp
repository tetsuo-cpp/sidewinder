#include "Client.h"

#include <sys/socket.h>
#include <zconf.h>

namespace sidewinder {

Client::Client(ICore &core, IClientHandler &handler, Address addr,
               const ClientConfig &config)
    : core(core), handler(handler), addr(std::move(addr)), config(config),
      socketFd(-1), offset(0), numRetries(0) {
  buffer.resize(config.bufSize, 0);
}

Client::~Client() { stop(); }

void Client::start() {
  connectTimer = std::make_unique<Timer>(
      [this]() { attemptConnect(); },
      std::chrono::seconds(config.reconnectInterval), core);
  connectTimer->set();
}

void Client::stop() {
  core.deregisterFd(socketFd);
  if (socketFd > 0)
    close(socketFd);
  if (connectTimer)
    connectTimer->unset();
}

void Client::attemptConnect() {
  if (config.maxRetries != 0 && numRetries++ > config.maxRetries) {
    handler.onError(ClientError::RetriesExhausted, "retries exhausted");
    connectTimer->unset();
    return;
  }

  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd < 0)
    return;

  const auto sockAddr = addr.getSockAddrIn();
  if (connect(socketFd, reinterpret_cast<const sockaddr *>(&sockAddr),
              sizeof(sockAddr)) < 0)
    return;

  core.registerFd(socketFd, this);
  assert(connectTimer);
  connectTimer->unset();
  handler.onConnection();
}

void Client::onReadable(int fd) {
  assert(fd == socketFd);
  if (offset == buffer.size()) {
    handler.onError(ClientError::BufferReset, "buffer filled up");
    offset = 0;
  }

  const int bytesRead =
      recv(fd, buffer.data() + offset, buffer.size() - offset, 0);

  if (bytesRead < 0) {
    handler.onError(ClientError::ReadFailed, "failed read call");
    return;
  } else if (bytesRead == 0) {
    core.deregisterFd(fd);
    handler.onDisconnection();
    return;
  }

  const bool handled = handler.handleData(buffer.data(), offset + bytesRead);
  if (handled)
    offset = 0;
  else
    offset += bytesRead;
}

void Client::sendData(const char *data, int len) {
  int totalBytesSent = 0;
  int numAttempts = 0;
  // Sends can get interrupted so we need to try repeatedly.
  while (true) {
    int bytesSent = send(socketFd, data, len, 0);
    if (bytesSent < 0)
      throw std::runtime_error("failed send call");
    totalBytesSent += bytesSent;
    if (totalBytesSent >= len)
      break;
    if (++numAttempts > config.sendRetries)
      throw std::runtime_error("repeatedly failed to send");
  }
}

} // namespace sidewinder
