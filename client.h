#ifndef CLIENT_H
#define CLIENT_H

#include "helper.h"

char NAME[50];

char room_code[50];

fd_set master_sds;
fd_set read_sds;
fd_set write_sds;

WINDOW * chat_win;
WINDOW * input_win;
WINDOW * status_win;

void client_get(int ss);

int check_whisper(char* otherchat);

int get_input(int ss);

void clean_all();
#endif
