#include "server_helper.h"
struct timeval tv;
int maxfd = 0;
struct room room_codes[100];
struct clientinfo clients[100];
int client_count = 0;
fd_set master_sds;
fd_set read_sds;
fd_set write_sds;
WINDOW * chat_win;
WINDOW * input_win;
WINDOW * status_win;
int special_status = 0;
struct banned blacklist[100];
static void sighandler(int signo) {
  if (signo == SIGINT || signo == SIGTERM) {
    clean_all();
  }
}
int main(int argc, char * argv[]) {
  signal(SIGINT, sighandler);
  tv.tv_sec = 1;
  tv.tv_usec = 0; //controls length that select will block (not using NULL since server may need actions independent of client)

  init_client();
  init_blacklist();

  int listen_socket = makeServer();
  maxfd = listen_socket;

  FD_ZERO( & master_sds);

  ncurses( & chat_win, & input_win, & status_win);
  cbreak();
  noecho();
  while (1) {
    if (!special_status){
      mvwprintw(input_win, 1, 1, "> ");
    }
    wrefresh(input_win);

    FD_ZERO( & read_sds);
    read_sds = master_sds;
    FD_SET(listen_socket, & read_sds);
    FD_SET(STDIN_FILENO, & read_sds);

    FD_ZERO( & write_sds);
    write_sds = master_sds;

    listener(listen_socket); //recv, if there are any messages it also sends
  }
}
