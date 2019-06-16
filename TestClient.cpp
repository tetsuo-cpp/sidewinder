#include "Client.h"
#include "Core.h"
#include "Interfaces.h"

#include <string>
#include <zconf.h>

struct StubHandler : sidewinder::IClientHandler {
  bool handleData(const char *data, int len) override {
    printf("Received data: %.*s", len, data);

    const std::string msg("PING!");
    client->sendData(msg.data(), msg.size());

    sleep(1);
    return true;
  }

  sidewinder::Client *client;
};

int main(int argc, char **argv) {
  sidewinder::Core core;
  StubHandler handler;
  sidewinder::Client client(core, handler);
  handler.client = &client;

  // Begin the ping-pong.
  const std::string msg("INIT!");
  client.sendData(msg.data(), msg.size());
  core.run();
}
