# Dev Log:

This document must be updated daily every time you finish a work session.

## Kevin Chen

### 2024-01-06 - Helper Files
I have, in, at least 6 hours, finished the basic framework of my helper file.
- The server setup creates a listening socket bound to the home address.
- The server (parent process) can accept client connections
- When the server and client accept and connect respectively, a handshake is completed
(beginning to develop server logic)

### 2024-01-07 - Chatterbox
In 5 hours, I have created a functional client prompter, tested in server.c
- ncurses creates 2 windows: chat and input. The input window is boxed. The chat window is scrollable
- starting on the client functions (send, recv, history)

### 2024-01-08 - Select & Server
I have, in, at least 6 hours, applied select and also necessary server functions for greater control over clients  
- The IP address is accessible by a struct array
- The server will print name + chat
- Client progress (forking strategy)

### 2024-01-09 - Client
In 5 hours, I have created a functional client sender and reciever, using a client subprocess for recieving chat messages. In theory, select could be used with stdin but with only one child process its easier to manage forks

### 2024-01-10 -

### 2024-01-11 -

### 2024-01-12 -
