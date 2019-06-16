#include "Core.h"
#include "Server.h"

#include <string>
#include <zconf.h>

struct StubHandler : public sidewinder::IServerHandler {
  void onConnection(std::unique_ptr<sidewinder::IConnection> conn) override {
    conns.push_back(std::move(conn));
  }

  bool handleData(const char *data, int len,
                  sidewinder::IConnection *conn) override {
    printf("Received data: %.*s\n", len, data);

    const std::string msg("PONG!");
    conn->sendData(msg.data(), msg.size());

    sleep(1);
    return true;
  }

  std::vector<std::unique_ptr<sidewinder::IConnection>> conns;
};

int main(int argc, char **argv) {
  sidewinder::Core core;
  StubHandler handler;
  sidewinder::Server server(core, handler);
  core.run();
}