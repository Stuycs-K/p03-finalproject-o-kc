#ifndef SERVER_H
#define SERVER_H

#include "helper.h"
#include <arpa/inet.h>
//EXTRA VARIABLES
struct clientinfo{
  int fd;
  char name[50];
  char ip[INET_ADDRSTRLEN];
  int active;          //this makes adding and removing easier since theres no need to shift
}; //1 active 0 inactive

struct clientinfo clients[100];
int client_count = 0;             //tdlr: possible values to extract: fd, name, ip, count

fd_set master_sds;
fd_set read_sds;
fd_set write_sds;

WINDOW * chat_win;
WINDOW * input_win;
int special_status;

struct banned{
  char ip[INET_ADDRSTRLEN];
  int active;
};
struct banned blacklist[100];

int new_maxfd(int old_max);

void recv_respond(int client_socket);
void header(char* name_chat, int cs, char* chat, char*addon);
void sender(int fd, int cs, char* name_chat);

void listener(int listen_socket);

int add_client(int fd, char*name, char* ip);

char* get_cname(int fd);

int get_cfd(char* name);

char* get_cip(int fd);

void delete_client(int fd);

void delete_client_name(char* key, int ban);

int add_banned(char * ip);
int is_banned(char * ip);

void initialize_c();
void initialize_b();

void user_interface(int * special_status);
void parse_helper(int*special_status, int* pos, char*special_store);
#endif
