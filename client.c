// 149.89.40.112 is this machine's address if it is to function as a server
#include "client.h"

WINDOW ** chat_win;
WINDOW ** input_win;

int main(int argc, char * argv[]) {
  char * IP = "149.89.40.102";
  if (argc > 1) {
    IP = argv[1];
  }

  char NAME[50];
  memset(NAME, 0, 50);

  printf("Make a name: \n");
  fgets(NAME, 49, stdin);
  input[strcspn(NAME, "\n")] = '\0';

  int server_socket = client_tcp_handshake(IP);

  send(server_socket, NAME, strlen(NAME)+1, 0);   //sends name

  int pid = fork();
  if (!pid) {                         //parent is prompter
    ncurses(chat_win, input_win);
    while (1) {
      char input[SIZE]; * chat_win
      memset(input, 0, SIZE);

      echo();

      mvwprintw( * input_win, 1, 1, "> ");
      wrefresh( * input_win);

      wgetnstr( * input_win, input, SIZE - 1);

      noecho();

      if (strcmp(input, "q") == 0) break; //quit

      input[strcspn(input, "\n")] = '\0';

      wprintw( * chat_win, "%s: %s\n", NAME, input);
      wrefresh( * chat_win);

      if (send(server_socket, input, strlen(input)+1, 0)<=0){
        exit(1);
      }

      werase( * input_win); //clear input buff, including border
      box( * input_win, 0, 0);

      wrefresh( * input_win);
    }
    else{
      client_get(server_socket);
    }
  }
}

void client_get(int ss) {
  char otherchat[SIZE+60];
  memset(otherchat, 0, SIZE+60);

  int bytes = recv(ss, otherchat, SIZE+60 - 1, 0);
  if (bytes <= 0){
    exit(1);
  }
  otherchat[bytes] = '\0';

  wprintw( * chat_win, "%s\n", otherchat);
  wrefresh( * chat_win);
}
