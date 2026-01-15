// 149.89.40.112 is this machine's address if it is to function as a server
#include "client.h"

struct timeval tv;
int main(int argc, char * argv[]) {
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  char * IP = "127.0.0.1";
  if (argc > 1) {
    IP = argv[1];
  }

  memset(NAME, 0, 50);


  printf("Make a name (no spaces, use '_'): \n");
  fgets(NAME, 49, stdin);
  NAME[strcspn(NAME, "\n")] = '\0';

  memset(room_code, 0, 50);
  strcpy(room_code, "lobby");
  room_code[49] = '\0';

  int server_socket = client_tcp_handshake(IP);


  if(send(server_socket, NAME, strlen(NAME) + 1, 0) <= 0) { //sends name
    perror("cant send name");
    exit(1);
  }



  FD_ZERO( & master_sds);
  FD_SET(server_socket, & master_sds);
  FD_SET(STDIN_FILENO, & master_sds);

  ncurses( & chat_win, & input_win, & status_win);
  mvwprintw(status_win, 1, 1, "ROOM [%s]",room_code);
  wrefresh(status_win);
  while (1) {
    FD_ZERO( & read_sds);
    read_sds = master_sds;
    select(server_socket + 1, & read_sds, NULL, NULL, & tv);
    if (FD_ISSET(server_socket, & read_sds)) {
      client_get(server_socket);
    }

    if (FD_ISSET(STDIN_FILENO, &read_sds)) {
      if (get_input(server_socket)) break;
    }
  }
  endwin();
}

void client_get(int ss) {

  char otherchat[SIZE + 60];
  memset(otherchat, 0, SIZE + 60);

  int bytes = recv(ss, otherchat, SIZE + 60 - 1, 0);
  if (bytes <= 0) {
    endwin();
    exit(1);
  }
  otherchat[bytes] = '\0';

  check_whisper(otherchat);

  wprintw(chat_win, "%s\n", otherchat);
  attroff(A_ITALIC);
  wrefresh(chat_win);
}

int check_whisper(char* otherchat){
  char copy[SIZE + 60];
  strncpy(copy, otherchat, SIZE + 60);  //to not modify otherchat
  char* tag = copy;

  strsep(&tag, " ");
  if(!strncmp(tag, "whispers:",  9)){
     attron(A_ITALIC);
     return 1;
  }
  return 0;
}

int get_input(int ss){
  char input[SIZE];
  memset(input, 0, SIZE);

  echo();

  mvwprintw(input_win, 1, 1, "%s > ", NAME);
  wrefresh(input_win);

  wgetnstr(input_win, input, SIZE - 1);

  noecho();

  if (strcmp(input, "q") == 0) return 1; //quit


  input[strcspn(input, "\n")] = '\0';
  if (!strncmp(input, "/join ", 6)){
      char copy[SIZE];
      strncpy(copy, input, SIZE - 1);
      char* temp = copy;
      strsep(&temp, " ");
      strcpy(room_code, temp);
      room_code[49] = '\0';

      werase(status_win);
      box(status_win, 0, 0);
      mvwprintw(status_win, 1, 1, "ROOM [%s]",room_code);
      wrefresh(status_win);
  }

  wprintw(chat_win, "%s: %s\n", NAME, input);
  wrefresh(chat_win);

//  wprintw(chat_win, "asdasdads\n");
//  wrefresh(chat_win);
  if (send(ss, input, strlen(input) + 1, 0) <= 0) {
    endwin();
    exit(1);
  }

  werase(input_win); //clear input buff, including border
  box(input_win, 0, 0);
  wrefresh(input_win);

  return 0;
}
