//
// Created by Alex Cameron on 2019-06-10.
//

#ifndef SIDEWINDER_SERVER_H
#define SIDEWINDER_SERVER_H

#include "Interfaces.h"

#include <array>

namespace sidewinder {

class Server : public IFdHandler {
public:
  explicit Server(ICore &core);

  // IFdHandler impl.
  void onWriteable(int fd) override;
  void onReadable(int fd) override;

private:
  ICore &core;
  const std::array<char, 1024> buffer;
  int socketFd;
};

} // namespace sidewinder

#endif // SIDEWINDER_SERVER_H
