#include "server.h"

struct timeval tv;
int maxfd = 0;

int main(int argc, char * argv[]) {
  tv.tv_sec = 1;
  tv.tv_usec = 0; //controls length that select will block (not using NULL since server may need actions independent of client)

  int listen_socket = makeServer();

  fd_set master_sds;
  FD_ZERO( & master_sds);

  fd_set read_sds;
  fd_set write_sds;

  while (1) {
    FD_ZERO( & read_sds);
    read_sds = master_sds;
    FD_SET(listen_socket, & read_sds);

    FD_ZERO( & write_sds);
    write_sds = master_sds;

    listener(listen_socket, & read_sds, & write_sds, & master_sds);
  }
}

int new_maxfd(fd_set * master_sds, int old_max) {
  int max = 0;
  for (int fd = 0; fd <= old_max; fd++) {
    if (FD_ISSET(fd, master_sds)) {
      max = fd;
    }
  }
  return max;
}

void recv_respond(int client_socket, fd_set * write_sds, fd_set * master_sds) {
  char chat[SIZE];
  memset(chat, 0, SIZE);

  int bytes = recv(client_socket, chat, SIZE - 1, 0); //recieve

  if (bytes < 0) {
    close(client_socket);
    delete_client(client_socket);
    FD_CLR(client_socket, master_sds);
    return;
  }

  chat[bytes] = '\0';

  for (int fd = 0; fd <= maxfd; fd++) {
    if (FD_ISSET(fd, write_sds) && fd != client_socket) {
      if (send(fd, chat, bytes, 0) < 0) { //and respond
        close(fd);
        delete_client(fd);
        FD_CLR(fd, master_sds);
        if (fd == maxfd) {
          maxfd = new_maxfd(master_sds, maxfd);
        }
      }
    }
  }
}

void listener(int listen_socket, fd_set * read_sds, fd_set * write_sds, fd_set * master_sds) {
  int queue = select(maxfd + 1, read_sds, NULL, NULL, & tv);
  if (FD_ISSET(listen_socket, read_sds)) {
    struct sockaddr_storage client_addr;
    int client_socket = server_tcp_handshake(listen_socket, & client_addr);

    char client_ip[INET_ADDRSTRLEN];
    if (client_addr.ss_family == AF_INET) {
      struct sockaddr_in * s = (struct sockaddr_in * ) & client_addr;
      inet_ntop(AF_INET, & s -> sin_addr, client_ip, sizeof(client_ip));
    } else {
      //idk IPV6?
    }

    char name[50];
    int bytes = recv(client_socket, name, 49, 0);
    name[bytes] = '\0';
    add_client(client_socket, name, client_ip);

    FD_SET(client_socket, master_sds);
    queue--;
  }
  for (int fd = 0; fd <= maxfd && queue > 0; fd++) {
    if (FD_ISSET(fd, read_sds)) {
      recv_respond(fd, write_sds, master_sds);
      queue--;
    }
  }
}

int add_client(int fd, char * name, char * ip) {
  for (int i = 0; i < 100; i++) {
    if (!clients[i].active) {
      clients[i].fd = fd;
      strncpy(clients[i].name, name, 49);

      strncpy(clients[i].ip, ip, 16);
      clients[i].ip[INET_ADDRSTRLEN - 1] = '\0';
      clients[i].active = 1;
    }
    return i;
  }
  return -1;
}

char * get_cname(int fd) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].fd == fd) {
      return clients[i].name;
    }
  }
  return NULL;
}

char * get_cip(int fd) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].fd == fd) {
      return clients[i].ip;
    }
  }
  return NULL;
}

void delete_client(int fd) {
  for (int i = 0; i < 100; i++) {
    if (clients[i].active && clients[i].fd == fd) {
      clients[i].active = 0;
      return;
    }
  }
}

void initialize_c() {
  for (int i = 0; i < 100; i++) {
    clients[i].active = 0;
  }
}
