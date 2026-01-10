#ifndef CLIENT_H
#define CLIENT_H

#include "helper.h"

char NAME[50];

fd_set master_sds;
fd_set read_sds;
fd_set write_sds;

WINDOW * chat_win;
WINDOW * input_win;

void client_get(int ss);

int get_input(int ss);
#endif
