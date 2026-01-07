#include "helper.h"

int makeServer(){
  struct addrinfo hints, *results;
  memset(&hints, 0, sizeof(hints));  //empty hints struct

  hints.ai_family = AF_UNSPEC;     // return both IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream
  hints.ai_flags = AI_PASSIVE;  //ONLY IN SERVER, indicates that the socket can accept connections (listen) instead of manually connect *client
  //NULL indicates socket is own; necessary for PASSIVE to work
  getaddrinfo(NULL, PORT, &hints, &results); //hints filters for similar socket, result has real socket!!!


  int clientd;
  clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  bind(clientd, results->ai_addr, results->ai_addrlen);  //activate socket

  listen(clientd,100);

  freeaddrinfo(results);

  return clientd;  //listen_socket
}



int server_tcp_handshake(int ls){
    int client_socket;

    struct sockaddr_storage client_addr;
    socklen_t sock_size = sizeof(client_addr);

    client_socket = accept(ls, (struct sockaddr *) &client_addr, &sock_size);
    //client ip addr isn't needed here***

    return client_socket;
    //linked to, but is NOT listen_socket
}



int client_tcp_handshake(char * server_ip) {
  struct addrinfo hints, *results;
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo(server_ip, PORT, &hints, &results);

  int serverd;
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  connect(serverd, results->ai_addr, results->ai_addrlen);
  //instead of binding **local vs remote socket

  freeaddrinfo(results);

  return serverd;
}
