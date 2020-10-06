CC=gcc
all: main
main: server.out client.out

client.out: client.c
	$(CC) client.c -o client.out
server.out: server.c
	$(CC) server.c -o server.out

test:
	./server.out &
	./client.out 6 3 > out.log
	cat out.log
clean:
	rm *.out
	rm *.log