##############################################################
#  Makefile for libsysinfo-0.0.8  -- by Vince Weaver         #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
CC = gcc

OS = $(shell uname)

C_FLAGS = -g -O2 -Wall -I.. -I.

all:	configure ./$(OS)/Makefile.inc libsysinfo.a

clean:
	rm -f *.o *~
	rm -f libsysinfo.a libsysinfo.so config.h
	rm -f ./$(OS)/Makefile.inc configure
	cd $(OS) && $(MAKE) clean
	cd all && $(MAKE) clean
	rm -f os

install:	

./$(OS)/Makefile.inc:	configure
	./configure

configure:	configure.o
	$(CC) $(L_OPTS) -o configure configure.o

configure.o:	configure.c
	$(CC) $(C_OPTS) -c configure.c

libsysinfo.a:	configure
	cd all && $(MAKE)
	cd $(OS) && $(MAKE)
	ar crus libsysinfo.a ./$(OS)/*.o ./all/*.o

shared:	libsysinfo.a
	ld -shared -o libsysinfo.so ./$(OS)/*.o ./all/*.o
