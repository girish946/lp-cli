CC=gcc
LDFLAGS=-lcurl -lm
INCLUDE=/usr/include/curl

all: clean json paste main

json: json.c
	$(CC) -I.. -c $@.c

paste: paste.c
	$(CC)  -c $@.c 

main: json paste main.c
	$(CC) -o lpcli main.c paste.o json.o $(LDFLAGS)

clean:
	rm -rf *.o
	rm -rf lpcli
