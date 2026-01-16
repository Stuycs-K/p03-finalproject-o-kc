[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WnpErSEz)
# Chatprogram

### 09F-ChenK

### Project Description:

Server-client feedback with aesthetic display using NCURSES
The server will have some management abilities
The client will have some control over how their messages are sent

### Instructions:

INSTALL - SSH git@github.com:Stuycs-K/systemslab16-networking-o-kc.git
COMPILE - make compile on terminal
RUN - SERVER - make s
    - CLIENT - make c

CLIENT INTERACTIONS
 - #1: input name (no spaces) & press ENTER
 - #2: type input
        - /whisper [name] lets you send a message only seen by [name]
        - /join [roomcode] restricts your incoming and outgoing chat to members of the room
 - #3: input 'q' & ENTER to exit
SERVER INTERACTIONS
 - #1: input 'q' to exit
 - #2: input 'b' to enter kick mode
        - valid names lose connection to the server
 - #3: input 'B' to enter ban mode
        - blacklisted ips get kicked and cannot connect again (if client & server same, may break program)
 - #4: input 't' to enter type mode
        - ENTER short, universally sent server msg (no more than 50 bytes)

### Resources/ References:
- ncurses.h     (display window input)
- <arpa/inet.h> (to get client ip from sockaddr_storage)
- <ctype.h>     (italics *if supported by terminal)
