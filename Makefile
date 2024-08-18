# Makefile for k5prog

CC=gcc
COPTS=-g -O2 -Wall
OSXOPTS=${COPTS}

default: k5prog

clean:
	rm k5prog


k5prog: k5prog.c uvk5.h
	$(CC) $(COPTS) k5prog.c -o k5prog

# ei2081 osx build testing
osx: k5prog.c uvk5.h 
	$(CC) $(OSXOPTS) k5prog.c -o k5prog

# osx debug build, just set the flag=1 to enable the debug print statements
osxd: k5prog.c uvk5.h osxdebug.h
	$(CC) $(OSXOPTS) -D DEBUG=1 k5prog.c -o k5prog
	