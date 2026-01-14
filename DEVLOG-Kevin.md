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

### 2024-01-09 - Client & Server (MVP)
In 5 hours, I have created a functional client sender and receiver. Client fork was replaced with select
- checks to read from stdin and server_socket per loop
- fixed formatting bugs; limit ncurses updates based on select activity
In 6 hours, I have created a function server sender, receiver, and user interface.
- The server can kick clients using a convoluted stdin input (select registers stdin by character)
   - can expand into a blacklist using another struct + activity array
- Due to incorporating ncurses, server must have endwin() safety checks to prevent corruption
- Some redundant code has been abstracted into methods (delete_client_name)

### 2024-01-10 - Server: Blacklisting & Whisper
Created a blacklist to ban any bad ips
Rudimental whisper code, /whisper “” allows private communication
Code is A MESS, will fix + create personalized chat + history tmrw
- (lots of copypaste to do whisper, will have to see where its optimal to abstract)
- Personal chat could apply recv flags
- History would be simple if I had clean code
The personalized chat will be an extension of whisper, using ncurses to set up a new window

Later refocusing on client.c, will format whispers and name tags to be different  

### 2024-01-11 - Cleanup
Created server helper methods, whisper is functional & tested
Experimenting with approaches to do private chats

### 2024-01-12 - Cleanup II
Made server helper bigger, and fixed ban which was broken

### 2024-01-13 - Cleanup III & Rooms
- Fixed bugs from italicize formatting (only works if supported by txt edit)
- Fixed undefined reference errors from how .h file was formatted, deleted server.h
- Organized server_helper.h file
