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

../libsysinfo.a:	cpuinfo.o sysinfo_linux.o
	ar crus ../libsysinfo.a cpuinfo.o sysinfo_linux.o
	ranlib ../libsysinfo.a

include *.inc

#cpuinfo.o:	cpuinfo_ix86.c
#	$(CC) $(C_FLAGS) -o cpuinfo.o -c cpuinfo_ix86.c
		
sysinfo_linux.o:	sysinfo_linux.c
	$(CC) $(C_FLAGS) -c sysinfo_linux.c

clean:	
	rm -f *~ *.o ../libsysinfo.a libsysinfo_arch.a
 
