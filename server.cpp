#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: server <message size in bytes>" << std::endl;
    return -1;
  }
  unsigned long long messageSize = std::stoull(argv[1], nullptr, 10);
  int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in serverAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddress.sin_port = htons(1234);
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  while (true) {
    //
  }
  return 0;
}
