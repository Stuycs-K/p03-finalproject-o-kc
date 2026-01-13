#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H

#include "helper.h"
#include <arpa/inet.h>
#include <ctype.h> 

struct timeval tv;
//EXTRA VARIABLES
struct clientinfo{
  int fd;
  char name[50];
  char ip[INET_ADDRSTRLEN];
  int active;          //this makes adding and removing easier since theres no need to shift
}; //1 active 0 inactive

struct clientinfo clients[100];
int client_count;             //tdlr: possible values to extract: fd, name, ip, count

fd_set master_sds;
fd_set read_sds;
fd_set write_sds;
int maxfd;

WINDOW * chat_win;
WINDOW * input_win;
int special_status;

struct banned{
  char ip[INET_ADDRSTRLEN];
  int active;
};
struct banned blacklist[100];



void recv_respond(int client_socket);   //responds to an active client socket
void header(char* name_chat, int cs, char* chat, char*addon);  //formats a chat message to send based on the sent input
void sender(int fd, int cs, char* name_chat);   //sends a chat message to a fd, with checks

void listener(int listen_socket);        //handles incoming fds
int ip_convert_check(char* ip, struct sockaddr_storage client_addr);
int recvname(int fd, char * name, char * ip);

int add_client(int fd, char*name, char* ip); //add a client struct to list

char* get_cname(int fd);  //get name from fd input
int get_cfd(char* name);  //get fd from name input
char* get_cip(int fd);    //get ip from fd input

void delete_client(int fd);               //removes a client struct from list
void delete_client_name(char* key, int ban);  //removes based on name and add to b_list
int new_maxfd(int old_max);  //updates the max_fd


int add_banned(char * ip);  //add a banned ip to list
int is_banned(char * ip);   //checks if ip is banned

void initialize_c();
void initialize_b();

void user_interface();
void new_status(int mode, char * modname, int* pos, char*special_store);
void status(char * modname, char*special_store);
void parse_helper(int * pos, char * special_store, char * modname);
#endif
