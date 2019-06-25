#include "Server.h"
#include "Connection.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace sidewinder {

Server::Server(ICore &core, IServerHandler &handler, Address addr)
    : core(core), handler(handler), addr(std::move(addr)), socketFd(-1) {}

Server::~Server() {
  for (const auto &c : conns) {
    core.deregisterFd(c.first);
    close(c.first);
  }

  core.deregisterFd(socketFd);
  if (socketFd > 0)
    close(socketFd);
}

void Server::init() {
  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd < 0)
    throw std::runtime_error("failed socket call");

  const auto sockAddr = addr.getSockAddrIn();
  if (bind(socketFd, reinterpret_cast<const sockaddr *>(&sockAddr),
           sizeof(sockAddr)) < 0)
    throw std::runtime_error("failed bind call");

  if (listen(socketFd, 10) < 0)
    throw std::runtime_error("failed listen call");

  // Set the socket as non-blocking.
  const int fdFlags = fcntl(socketFd, F_GETFL);
  fcntl(socketFd, F_SETFL, fdFlags | O_NONBLOCK);

  core.registerFd(socketFd, this);
}

void Server::onReadable(int fd) {
  if (fd == socketFd)
    acceptConnection();
  else
    readData(fd);
}

void Server::readData(int fd) {
  auto iter = conns.find(fd);
  assert(iter != conns.end());

  auto &connInfo = iter->second;

  if (connInfo.buffer.size() == connInfo.offset)
    throw std::runtime_error("buffer filled up");

  const int bytesRead = read(fd, connInfo.buffer.data() + connInfo.offset,
                             connInfo.buffer.size() - connInfo.offset);

  if (bytesRead < 0)
    throw std::runtime_error("read call failed");

  const bool handled = handler.handleData(
      connInfo.buffer.data(), connInfo.offset + bytesRead, connInfo.conn);

  if (handled)
    connInfo.offset = 0;
  else
    connInfo.offset += bytesRead;
}

void Server::acceptConnection() {
  const int newFd = accept(socketFd, nullptr, nullptr);
  if (newFd <= 0)
    return;

  core.registerFd(newFd, this);

  auto newConn = std::make_unique<Connection>(newFd, *this);
  conns.emplace(newFd, ConnectionInfo(newConn.get()));
  handler.onConnection(std::move(newConn));
}

void Server::deregisterConnection(IConnection *conn) {
  auto iter = std::find_if(conns.begin(), conns.end(),
                           [conn](const std::pair<int, ConnectionInfo> &c) {
                             return c.second.conn == conn;
                           });

  assert(iter != conns.end());
  core.deregisterFd(iter->first);
  conns.erase(iter);
}

} // namespace sidewinder
