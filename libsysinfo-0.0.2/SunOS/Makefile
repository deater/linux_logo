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

../libsysinfo.a:	sysinfo_solaris.o
	ar crus ../libsysinfo.a sysinfo_solaris.o
	ranlib ../libsysinfo.a

sysinfo_solaris.o:	sysinfo_solaris.c
	$(CC) $(C_FLAGS) -c sysinfo_solaris.c

clean:	
	rm -f *~ *.o ../libsysinfo.a libsysinfo_arch.a
 
