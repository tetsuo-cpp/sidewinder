#include "Server.h"
#include "Connection.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace sidewinder {

Server::Server(ICore &core, IServerHandler &handler)
    : core(core), handler(handler), offset(0) {
  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd < 0)
    throw std::runtime_error("failed socket call");

  sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(7980);
  if (bind(socketFd, reinterpret_cast<const sockaddr *>(&address),
           sizeof(address)) < 0)
    throw std::runtime_error("failed bind call");

  if (listen(socketFd, 10) < 0)
    throw std::runtime_error("failed listen call");

  // Set the socket as non-blocking.
  const int fdFlags = fcntl(socketFd, F_GETFL);
  fcntl(socketFd, F_SETFL, fdFlags | O_NONBLOCK);

  core.registerFd(socketFd, this);
}

Server::~Server() {
  for (const auto fd : fds)
    close(fd);

  close(socketFd);
}

void Server::onReadable(int fd) {
  if (fd == socketFd)
    acceptConnection();
  else
    readData(fd);
}

void Server::readData(int fd) {
  if (buffer.size() == offset)
    throw std::runtime_error("buffer filled up");

  auto iter = conns.find(fd);
  assert(iter != conns.end());

  const int bytesRead =
      read(fd, buffer.data() + offset, buffer.size() - offset);
  if (bytesRead < 0)
    throw std::runtime_error("read call failed");

  // Buffers need to be maintained per FD. Let's ignore that for now.
  const bool handled =
      handler.handleData(buffer.data(), offset + bytesRead, iter->second);
  if (handled)
    offset = 0;
  else
    offset += bytesRead;
}

void Server::acceptConnection() {
  const int newFd = accept(socketFd, nullptr, nullptr);
  if (newFd <= 0)
    return;

  fds.push_back(newFd);
  core.registerFd(newFd, this);

  auto newConn = std::make_unique<Connection>(newFd, *this);
  conns.emplace(newFd, newConn.get());
  handler.onConnection(std::move(newConn));
}

void Server::deregisterConnection(IConnection *conn) {
  auto iter = std::find_if(conns.begin(), conns.end(),
                           [conn](const std::pair<int, IConnection *> &p) {
                             return p.second == conn;
                           });

  conns.erase(iter);
}

} // namespace sidewinder
