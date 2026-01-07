.PHONY: client server clean compile
compile: client.o server.o helper.o
	@gcc -o cliont client.o helper.o
	@gcc -o sorver server.o helper.o
client: cliont
	@./cliont
server: sorver
	@./sorver
client.o: client.c helper.h
	@gcc -c client.c
server.o: server.c helper.h
	@gcc -c server.c
helper.o: helper.c helper.h
	@gcc -c helper.c
clean:
	@rm *.o
	@rm *~
