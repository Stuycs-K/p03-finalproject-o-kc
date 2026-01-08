#include "helper.h"

void subserver(int client_socket) {

}
WINDOW *chat_win, *input_win;
int main(int argc, char * argv[]) {

  chatterbox(&chat_win, &input_win);
  return 0;
}


  /*
  int listen_socket = makeServer();

  char* msgs[100];
  int i = 0; //present history position

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
  }*/
