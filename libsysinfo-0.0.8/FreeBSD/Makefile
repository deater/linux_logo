##############################################################
#  Makefile for libsysinfo-0.0.1  -- by Vince Weaver         #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
CC = gcc

C_FLAGS = -O2 -Wall -I.. -I. -I../include

all:	../libsysinfo.a

../libsysinfo.a:	cpuinfo.o sysinfo_bsd.o
	ar crus ../libsysinfo.a cpuinfo.o sysinfo_bsd.o
	ranlib ../libsysinfo.a

#include *.inc

cpuinfo.o:	cpuinfo_bsd.c
	$(CC) $(C_FLAGS) -o cpuinfo.o -c cpuinfo_bsd.c
		
sysinfo_bsd.o:	sysinfo_bsd.c
	$(CC) $(C_FLAGS) -c sysinfo_bsd.c

clean:	
	rm -f *~ *.o ../libsysinfo.a libsysinfo_arch.a
 
