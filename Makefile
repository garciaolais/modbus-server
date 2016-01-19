cc= gcc -Wall
LIBS= -lmodbus

all:
	$(cc) server.c -o server $(LIBS)
clean:
	rm -vf client server
