#ifndef SIDEWINDER_CLIENT_H
#define SIDEWINDER_CLIENT_H

#include "Address.h"
#include "Interfaces.h"

#include <array>

namespace sidewinder {

class Client : public IFdHandler {
public:
  Client(ICore &core, IClientHandler &handler, Address addr);
  virtual ~Client();

  // IFdHandler impl.
  void onReadable(int fd) override;

  void init();
  void sendData(const char *data, int len);

private:
  ICore &core;
  IClientHandler &handler;
  const Address addr;
  int socketFd;
  std::array<char, 1024> buffer;
  int offset;
};

} // namespace sidewinder

#endif // SIDEWINDER_CLIENT_H
