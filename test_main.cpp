#include "Core.h"
#include "Server.h"

struct StubHandler : public sidewinder::IServerHandler {
  void onConnection(std::unique_ptr<sidewinder::IConnection> conn) override {
    conns.push_back(std::move(conn));
  }

  bool handleData(const char *data, int len,
                  sidewinder::IConnection *conn) override {
    printf("Received data: %.*s\n", len, data);
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
