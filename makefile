.PHONY: client server clean compile
compile: client.o server.o helper.o server_helper.o
	@gcc -Wall -o cliont client.o helper.o -lncurses
	@gcc -Wall -o sorver server.o helper.o server_helper.o -lncurses
client: cliont
	@./cliont
server: sorver
	@./sorver
client.o: client.c helper.h client.h
	@gcc -Wall -c client.c
server.o: server.c helper.h server_helper.h server.h
	@gcc -Wall -c server.c
server_helper.o: server_helper.c server_helper.h
		@gcc -Wall -c server_helper.c
helper.o: helper.c helper.h
	@gcc -Wall -c helper.c
clean:
	@rm *.o
	@rm *~
