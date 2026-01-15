#ifndef HELPER_H
#define HELPER_H

#define PORT "19230"
#define SIZE 200

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>

#include <ncurses.h>

int makeServer();

int server_tcp_handshake(int ls, struct sockaddr_storage * client_addr);

int client_tcp_handshake(char * server_ip);


void ncurses(WINDOW** chat_win, WINDOW** input_win, WINDOW** status_win);

int chatterbox(WINDOW ** chat_win, WINDOW ** input_win, int ss);  //not practicable. TEST
#endif
