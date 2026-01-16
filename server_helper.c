#include "server_helper.h"

//NETWORK-----------------------------
void recv_respond(int client_socket) {
  char chat[SIZE];
  memset(chat, 0, SIZE);

  int bytes = recv(client_socket, chat, SIZE - 1, 0); //recieve

  if (bytes <= 0) {
    delete_client(client_socket);
    return;
  }

  chat[bytes] = '\0';

  char name_chat[SIZE + 60];

  if (!strncmp(chat, "/whisper ", 9) && bytes >= 12) {
    whisper(name_chat, client_socket, chat);
    return;
  } else if (!strncmp(chat, "/join ", 5) && bytes >= 8) {
      char old_room[50];
      strncpy(old_room, get_croomcode(client_socket), 49);
      old_room[49] = '\0';

      char *temp = chat;
      strsep(&temp, " ");
      char new_room[50];
      strncpy(new_room, temp, 49);
      new_room[49] = '\0';
      join_room(client_socket, chat);

      header(name_chat, client_socket, new_room, "has joined the room --");
      loop_join(name_chat, client_socket, old_room);
    }
    else if  (!strncmp(chat, "/pjoin ",6) && bytes >= 9){
      char old_room[50];
      strncpy(old_room, get_croomcode(client_socket), 49);
      old_room[49] = '\0';

      char *temp = chat;
      strsep(&temp, " ");
      char new_room[50];
      strncpy(new_room, temp, 49);
      new_room[49] = '\0';

      join_room(client_socket, chat);

      header(name_chat, client_socket, "!", "has left this room");
      loop_join(name_chat, client_socket, old_room);
    }
    else{
      header(name_chat, client_socket, chat, ":");
    }

    loop_all(name_chat, client_socket);
  }


void whisper(char * name_chat, int cs, char * chat) {

  char * pos = chat + 9; //go to named portion
  char * name = strsep( & pos, " ");
  header(name_chat, cs, pos, "whispers");
  int wfd = get_cfd(name);
  sender(wfd, cs, name_chat);
}

void header(char * name_chat, int cs, char * chat, char * addon) {
  snprintf(name_chat, SIZE + 60, "%s %s %s", get_cname(cs), addon, chat);
}

void sender(int fd, int cs, char * name_chat) {
    if (send(fd, name_chat, strlen(name_chat) + 1, 0) <= 0) { //and respond
      delete_client(fd);
    }

}

void loop_all(char* name_chat, int cs){
  for (int fd = 0; fd <= maxfd; fd++) {
    if (FD_ISSET(fd, & write_sds) && fd != cs && same_room(fd, cs)) {
      sender(fd, cs, name_chat);
    }
  }
}

void loop_join(char* name_chat, int cs, char* old_room){
  for (int fd = 0; fd <= maxfd; fd++) {
    if (FD_ISSET(fd, & write_sds) && fd != cs && !strcmp(get_croomcode(fd), old_room)) {
      sender(fd, cs, name_chat);
    }
  }
}

//----------------------------------
void listener(int listen_socket) {
  int queue = select(maxfd + 1, & read_sds, NULL, NULL, & tv);
  if (FD_ISSET(listen_socket, & read_sds)) {
    struct sockaddr_storage client_addr;
    int client_socket = server_tcp_handshake(listen_socket, & client_addr);

    if (client_socket > maxfd) {
      maxfd = client_socket;
    }

    char client_ip[INET_ADDRSTRLEN];
    if (ip_convert_check(client_ip, client_addr)) {
      close(client_socket);
    } else {
      char name[50];
      if (recv_name(client_socket, name, client_ip)) {
        add_client(client_socket, name, client_ip); //add
      } else {
        close(client_socket); //if name recv fails, then socket is freed before it even gets stored
      }
      queue--;
    }
  }

  if (FD_ISSET(STDIN_FILENO, & read_sds)) {
    user_interface();
    wrefresh(input_win);
  }

  for (int fd = 0; fd <= maxfd && queue > 0; fd++) {
    if (FD_ISSET(fd, & read_sds) && fd != STDIN_FILENO && fd != listen_socket) {
      recv_respond(fd); //indicates server must recieve these client messages
      queue--;
    }
  }
}

int ip_convert_check(char * ip, struct sockaddr_storage client_addr) {
  if (client_addr.ss_family == AF_INET) {
    struct sockaddr_in * s = (struct sockaddr_in * ) & client_addr;
    inet_ntop(AF_INET, & s -> sin_addr, ip, INET_ADDRSTRLEN); //get IP
  } else {
    //idk IPV6?
  }
  return is_banned(ip);
}

int recv_name(int fd, char * name, char * ip) {
  int bytes = recv(fd, name, 49, 0); //get name
  if (bytes > 0) {
    name[bytes] = '\0';
  }
  return bytes;
}

//TEAM MANAGING---------------------
//default is lobby

int same_room(int fd1, int fd2) {
  if (!strcmp(get_croomcode(fd1), get_croomcode(fd2))) {
    return 1;
  }
  return 0;
}

void join_room(int cs, char * chat) {
  char * room_name = chat;
  strsep( & room_name, " "); //changes chat position too

  if (room_name != NULL) {
    set_croomcode(cs, room_name);
  }
}

//CLIENT MANAGING---------------------
int add_client(int fd, char * name, char * ip) {
  FD_SET(fd, & master_sds);
  for (int i = 0; i < 100; i++) {
    if (!clients[i].active) {
      clients[i].fd = fd;
      strncpy(clients[i].name, name, 49);    //copy name into name

      strncpy(clients[i].ip, ip, 16);        //copy ip into ip
      clients[i].ip[INET_ADDRSTRLEN - 1] = '\0';
      clients[i].active = 1;
      client_count++;
      return i;
    }
  }
  return -1;
}

char * get_cname(int fd) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && clients[i].fd == fd) {
      return clients[i].name;
    }
  }
  return NULL;
}

int get_cfd(char * name) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && !strcmp(name, clients[i].name)) {
      return clients[i].fd;
    }
  }
  return -1;
}

char * get_cip(int fd) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && clients[i].fd == fd) {
      return clients[i].ip;
    }
  }
  return NULL;
}

char * get_croomcode(int fd) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && clients[i].fd == fd) {
      return clients[i].room_code;
    }
  }
  return NULL;
}

int set_croomcode(int fd, char * code) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && clients[i].fd == fd) {
      strncpy(clients[i].room_code, code, 49);  //copy roomcode into roomcode
      clients[i].room_code[49] = '\0';
      return i;
    }
  }
  return -1;
}

void delete_client(int fd) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && clients[i].fd == fd) {
      close(fd);
      FD_CLR(fd, & master_sds);
      clients[i].active = 0;
      client_count--;

      if (fd == maxfd) {
        maxfd = new_maxfd(maxfd); //updates the max_fd
      }
      return;
    }
  }
}

void delete_client_name(char * key, int ban) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && !strcmp(key, clients[i].name)) {
      close(clients[i].fd);
      FD_CLR(clients[i].fd, & master_sds);
      if (ban) {
        add_banned(clients[i].ip);
      }
      clients[i].active = 0;
      client_count--;
      return;
    }
  }
}

int new_maxfd(int old_max) {
  int max = 0;
  for (int fd = 0; fd <= old_max; fd++) {
    if (FD_ISSET(fd, & master_sds)) {
      max = fd;
    }
  }
  return max;
}

//BAN MANAGING-------------------------
int add_banned(char * ip) {
  for (int i = 0; i < 100; i++) {
    if (!blacklist[i].active) {
      strncpy(blacklist[i].ip, ip, 16);           //copy blacklisted ip into ip
      blacklist[i].ip[INET_ADDRSTRLEN - 1] = '\0';
      blacklist[i].active = 1;
      return i;
    }
  }
  return -1;
}

void remove_banned(char * ip) {
  for (int i = 0; i < 100; i++) {
    if (blacklist[i].active && !strcmp(ip, blacklist[i].ip)) {
      blacklist[i].active = 0;
    }
  }
}

int is_banned(char * ip) {
  for (int i = 0; i < 100; i++) {
    if (blacklist[i].active && !strcmp(ip, blacklist[i].ip)) {
      return 1;
    }
  }
  return 0;
}

void init_client() {
  for (int i = 0; i < 100; i++) {
    clients[i].active = 0;
    strncpy(clients[i].room_code, "lobby", 6);   //copies lobby into roomcode
    clients[i].room_code[5] = '\0';
  }
}

void init_blacklist() {
  for (int i = 0; i < 100; i++) {
    blacklist[i].active = 0;
  }
}

//UI-----------------------------------
void user_interface() {
  static char special_store[50]; //when special status is activated, this emulates reading a string
  static int pos = 0;

  if (!special_status) {
    int c = wgetch(input_win);

    if (c == 98) { //b for kick - ENTER kick mode
      new_status(1, "kick", & pos, special_store);
    } else if (c == 66) { //B for Ban
      new_status(2, "ban", & pos, special_store);
    } else if (c == 113) { //Q for exit
      endwin();
      exit(1);
    }
  } else if (special_status == 1) { // since select is valid EVEN WHEN theres only one character in stdin
    parse_helper( & pos, special_store, "kick");
  } else if (special_status == 2) {
    parse_helper( & pos, special_store, "ban");
  }
}

void new_status(int mode, char * modname, int * pos, char * special_store) {
  special_status = mode;
  * pos = 0;
  memset(special_store, 0, 50);
  werase(input_win);
  box(input_win, 0, 0);
  mvwprintw(input_win, 1, 1, "%s : ", modname);
  wrefresh(input_win);
}

void status(char * modname, char * special_store) {
  werase(input_win);
  box(input_win, 0, 0);
  mvwprintw(input_win, 1, 1, "%s : %s", modname, special_store);
  wrefresh(input_win);
}

void parse_helper(int * pos, char * special_store, char * modname) {
  int c = wgetch(input_win); // cannot use wgetnstr!

  if (c == '\n') { //enter; 2 cases since mac doesnt register \n
    special_store[ * pos] = '\0';
    if (!strcmp(special_store, "return")) {
      special_status = 0;
      * pos = 0; //reset "string reading" operation
    } else {
      delete_client_name(special_store, special_status - 1);
      * pos = 0;
    }
    werase(input_win);
    box(input_win, 0, 0);
    wrefresh(input_win);
  } else if (c == KEY_BACKSPACE || c == 127 || c == 8) { // Backspace may be differennt
    if ( * pos > 0) {
      ( * pos) --;
      special_store[ * pos] = '\0';
      status(modname, special_store);
    }
  } else if ( * pos < 49 && isprint(c)) { //fill buff with name (should contain all valid characters)
    special_store[ * pos] = c;
    ( * pos) ++;
    special_store[ * pos] = '\0';
    status(modname, special_store);
  }
}

int is_command() {
  return 0;
}
