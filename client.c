// 149.89.40.112 is this machine's address if it is to function as a server
#include "helper.h"
void client_chat(int ss){
  char chat[200];
  memset(chat, 0, 200);
  if(fgets(chat, 200, stdin)==NULL){  //recieve client input!!!
    exit(0);
  }
  chat[strcspn(chat, "\n")] = '\0';

  send(ss, chat, strlen(chat)+1, 0);
}

void client_get(int ss){
  char otherchat[200];
  memset(otherchat, 0, 200);

  int bytes = recv(ss, otherchat, 199, 0);

  otherchat[bytes] = '\0';

  printf("%s\n", otherchat);
}

void client_history(int ss){



}

int main(int argc, char * argv[]) {
  char* IP = "149.89.40.102";
  if(argc>1){
     IP=argv[1];
  }

  int server_socket = client_tcp_handshake(IP);

  while(1){
     client_chat(server_socket);
  }
}
