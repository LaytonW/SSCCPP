#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "XNRW/include/ThreadPool.h"

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: server <number of connections> <message size in bytes>"
      << std::endl;
    return -1;
  }
  const auto messageSize = std::stoull(argv[1], nullptr, 10);
  const auto numConn = std::stoull(argv[2], nullptr, 10);
  const auto reply = "ACK";

  XNRW::ThreadPool threadPool(numConn);

  int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in serverAddress, clientAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(1234);
  socklen_t clientLen = sizeof(clientAddress);
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  listen(serverSocket, numConn);

  for (auto i = 0; i < numConn; i++) {
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress,
                              &clientLen);
    threadPool.addTask([clientSocket, messageSize, reply]() {
      std::vector<unsigned char> buffer;
      read(clientSocket, buffer.data(), messageSize);
      write(clientSocket, reply, sizeof(reply));
      close(clientSocket);
    });
  }

  threadPool.wait();

  return 0;
}
