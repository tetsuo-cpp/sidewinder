#ifndef SIDEWINDER_ADDRESS_H
#define SIDEWINDER_ADDRESS_H

#include <netinet/in.h>
#include <string>

namespace sidewinder {

struct Address {
  Address(std::string ip, int port);
  explicit Address(int port);

  sockaddr_in getSockAddrIn() const;

  const std::string ip;
  const int port;
};

} // namespace sidewinder

#endif // SIDEWINDER_ADDRESS_H
