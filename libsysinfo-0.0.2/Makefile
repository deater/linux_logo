##############################################################
#  Makefile for libsysinfo-0.0.1  -- by Vince Weaver         #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
CC = gcc

C_FLAGS = -O2 -Wall -I.. -I.

all:	configure ./os/Makefile.inc libsysinfo.a

clean:
	rm -f *.o *~
	rm -f libsysinfo.a libsysinfo.so config.h
	rm -f ./os/Makefile.inc configure
	cd os && make clean
	cd all && make clean

install:	

./os/Makefile.inc:	configure
	./configure

configure:	configure.o
	$(CC) $(L_OPTS) -o configure configure.o
	
configure.o:	configure.c
	$(CC) $(C_OPTS) -c configure.c

libsysinfo.a:	configure
	cd all && make
	cd os && make
	ar crus libsysinfo.a ./os/*.o ./all/*.o

shared:	libsysinfo.a
	ld -shared -o libsysinfo.so ./os/*.o ./all/*.o
