#ifndef LIBRARY_H
#define LIBRARY_H

#define PORT "19230"

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

int makeServer();



int server_tcp_handshake(int ls);



int client_tcp_handshake(char * server_ip);

#endif
