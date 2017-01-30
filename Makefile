CC=gcc
CPPFLAGS=-Wall -Werror -O2 -g
LDLIBS=

default: garbage

clean: 
	rm -rf *.o garbage garbage.dSYM core
