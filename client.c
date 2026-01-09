// 149.89.40.112 is this machine's address if it is to function as a server
#include "helper.h"

WINDOW ** chat_win;
WINDOW ** input_win;

void client_get(int ss) {
  char otherchat[SIZE];
  memset(otherchat, 0, SIZE);

  int bytes = recv(ss, otherchat, SIZE - 1, 0);

  otherchat[bytes] = '\0';

  printf("%s\n", otherchat);
}

int main(int argc, char * argv[]) {
  char * IP = "149.89.40.102";
  if (argc > 1) {
    IP = argv[1];
  }

  char NAME[50];
  memset(NAME, 0, 50);

  printf("Make a name: \n");
  fgets(NAME, 49, stdin);

  int server_socket = client_tcp_handshake(IP);

  send(server_socket, NAME, strlen(NAME)+1, 0);

  int pid = fork();
  if (!pid) {



    ncurses(chat_win, input_win);
    while (1) {
      char input[SIZE];
      memset(input, 0, SIZE);

      echo();

      mvwprintw( * input_win, 1, 1, "> ");
      wrefresh( * input_win);

      wgetnstr( * input_win, input, SIZE - 1);

      noecho();

      if (strcmp(input, "q") == 0) break; //quit

      input[strcspn(input, "\n")] = '\0';

      wprintw( * chat_win, "NAME: %s\n", input);
      wrefresh( * chat_win);

      send(server_socket, input, strlen(input)+1, 0);

      werase( * input_win); //clear input buff, including border
      box( * input_win, 0, 0);

      wrefresh( * input_win);
    }
  }

}
