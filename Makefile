cc= gcc -Wall
LIBS= -lmodbus

all:
	$(CC) client.c -o client $(LIBS)
	$(cc) server.c -o server2 $(LIBS)
clean:
	rm -vf client server
