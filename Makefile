CC=g++
CPPFLAGS=-Wall -I./include
LDFLAGS=-L./lib

all: test

clean:
	rm -rf test.exe
	rm -rf *.o
