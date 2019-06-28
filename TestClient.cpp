#include "Client.h"
#include "Core.h"
#include "Interfaces.h"

#include <string>
#include <zconf.h>

struct StubHandler : public sidewinder::IClientHandler {
  bool handleData(const char *data, int len) override {
    printf("Received data: %.*s\n", len, data);

    const std::string msg("PING!");
    client->sendData(msg.data(), msg.size());

    sleep(1);
    return true;
  }

  sidewinder::Client *client = nullptr;
};

int main(int argc, char **argv) {
  sidewinder::Core core;
  StubHandler handler;
  sidewinder::Address address("127.0.0.1", 7980);
  sidewinder::ClientConfig config;
  sidewinder::Client client(core, handler, address, config);
  handler.client = &client;
  client.start();
  core.run();
}
