#include "helper.h"

void subserver(int client_socket) {
  
}

int main(int argc, char * argv[]) {
  int listen_socket = makeServer();

  while (1) {
    int client_socket = server_tcp_handshake(listen_socket);

    int pid = fork();
    if (!pid) {
      subserver(client_socket);

      close(client_socket);
      exit(0);
    } else {
      close(client_socket); //parent accepts since if it was concurrent you would get blockage
    }
  }
}
