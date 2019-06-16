#ifndef SIDEWINDER_CLIENT_H
#define SIDEWINDER_CLIENT_H

#include "Interfaces.h"

#include <array>

namespace sidewinder {

class Client : public IFdHandler {
public:
  Client(ICore &core, IClientHandler &handler);
  virtual ~Client();

  // IFdHandler impl.
  void onReadable(int fd) override;

  void sendData(const char *data, int len);

private:
  ICore &core;
  IClientHandler &handler;
  int socketFd;
  std::array<char, 1024> buffer;
  int offset;
};

} // namespace sidewinder

#endif // SIDEWINDER_CLIENT_H
