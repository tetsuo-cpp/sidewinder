#include "Core.h"
#include "Server.h"
#include "Timer.h"

#include <string>
#include <zconf.h>

struct StubHandler : public sidewinder::IServerHandler {
  void onConnection(std::unique_ptr<sidewinder::IConnection> conn) override {
    const std::string msg("INIT_FROM_SERVER!");
    conn->sendData(msg.data(), msg.size());
    conns.push_back(std::move(conn));
  }

  void onDisconnection(sidewinder::IConnection *conn) override {
    conns.erase(std::remove_if(
                    conns.begin(), conns.end(),
                    [conn](const std::unique_ptr<sidewinder::IConnection> &c) {
                      return c.get() == conn;
                    }),
                conns.end());
  }

  bool handleData(const char *data, int len,
                  sidewinder::IConnection *conn) override {
    printf("Received data: %.*s\n", len, data);

    const std::string msg("PONG!");
    conn->sendData(msg.data(), msg.size());

    return true;
  }

  std::vector<std::unique_ptr<sidewinder::IConnection>> conns;
};

int main(int argc, char **argv) {
  sidewinder::Core core;
  StubHandler handler;
  sidewinder::Address address(7980);
  sidewinder::ServerConfig config;
  sidewinder::Server server(core, handler, address, config);
  sidewinder::Timer timer([]() { printf("testing\n"); },
                          std::chrono::seconds(5), core);
  server.start();
  core.run();
}
