//
// Created by Alex Cameron on 2019-06-10.
//

#include "Server.h"

#include <netinet/in.h>
#include <sys/socket.h>

namespace sidewinder {

Server::Server(ICore &core) : core(core), buffer() {
  socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd == 0)
    throw std::runtime_error("failed socket call");

  int opt;
  if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) != 0)
    throw std::runtime_error("failed setsockopt call");

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);
  if (bind(socketFd, reinterpret_cast<const sockaddr *>(&address),
           sizeof(address)) < 0)
    throw std::runtime_error("failed bind call");

  core.registerFd(socketFd, this, FdMode::FM_Read);
}

void Server::onWriteable(int fd) {}

void Server::onReadable(int fd) {}

} // namespace sidewinder
