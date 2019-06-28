#ifndef SIDEWINDER_CLIENT_H
#define SIDEWINDER_CLIENT_H

#include "Address.h"
#include "Interfaces.h"
#include "Timer.h"

#include <vector>

namespace sidewinder {

struct ClientConfig {
  int maxRetries = 0;
  int reconnectInterval = 1;
  size_t bufSize = 1024;
};

class Client : public IFdHandler {
public:
  Client(ICore &core, IClientHandler &handler, Address addr,
         const ClientConfig &config);
  virtual ~Client();

  // IFdHandler impl.
  void onReadable(int fd) override;

  void start();
  void stop();
  void sendData(const char *data, int len);

private:
  void attemptConnect();

  ICore &core;
  IClientHandler &handler;
  const Address addr;
  const ClientConfig config;
  int socketFd;
  std::vector<char> buffer;
  int offset;
  std::unique_ptr<Timer> connectTimer;
  int numRetries;
};

} // namespace sidewinder

#endif // SIDEWINDER_CLIENT_H
