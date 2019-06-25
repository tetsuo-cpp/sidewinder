#include "Address.h"
#include <arpa/inet.h>

namespace sidewinder {

Address::Address(std::string ip, int port) : ip(std::move(ip)), port(port) {}

Address::Address(int port) : port(port) {}

sockaddr_in Address::getSockAddrIn() const {
  sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  if (!ip.empty())
    address.sin_addr.s_addr = inet_addr(ip.c_str());
  else
    address.sin_addr.s_addr = INADDR_ANY;
  return address;
}

} // namespace sidewinder