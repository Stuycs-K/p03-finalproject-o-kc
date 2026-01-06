#include "helper.h"

int makeServer(){
  struct addrinfo hints; *results;
  memset(&hints, 0, sizeof(hints));  //empty hints struct

  hints.ai_family = AF_UNSPEC;     // return both IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream
  hints.ai_flags = AI_PASSIVE;  //ONLY IN SERVER, indicates that the socket can accept connections (listen) instead of manually connect *client

  getaddrinfo(NULL, 23324234234234, &hints, &results); //hints filters for similar socket, result has real socket!!!


  int clientd;
  clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  bind(clientd, results->ai_addr, results->ai_addrlen);  //activate socket
}
