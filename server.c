#include "helper.h"

void recieve_respond(int client_socket, int* sds) {
   char chat[SIZE];
   memset(chat, 0, SIZE);

   int bytes = recv(client_socket, chat, SIZE-1, 0);
   otherchat[bytes] = '\0';

   for(int i = 0; i < 100; i++){
     send
   }
}

int main(int argc, char * argv[]) {

  int listen_socket = makeServer();

  int sds[100];   //resets every cycle but different from select, actually both can be combined!!!

  fd_set sd;
  FD_ZERO( &sd );

  while (1) {
    FD_ZERO( &sd );
    FD_SET(listen_socket,&sd);


    if (FD_ISSET(listen_socket, &sd)) {   //client is sending info

      int client_socket = server_tcp_handshake(listen_socket);
      recieve_respond(client_socket);


    }

  }
}
