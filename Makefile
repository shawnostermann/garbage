CC=gcc
CPPFLAGS=-Wall -Werror -O2 -g
LDLIBS_SOLARIS=-lnsl -lsocket
LDLIBS=-lm 

default: garbage
