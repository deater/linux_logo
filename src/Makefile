# Generated automatically from Makefile.in by configure.
##############################################################
#  Makefile for Linux Logo src directory                     #
#                                                            #
#  Changed to use gnu atonconfigure 27 March 1999            #
#                                                            #
##############################################################

SHELL = /bin/sh
VPATH = ../././src

subdirs = @subdirs@
top_srcdir = .././.
srcdir = ../././src
prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
mandir = $(prefix)/man/man1

BUILD_OS = LINUX
BUILD_CPU = ix86

CC = gcc
CPPFLAGS = 
CFLAGS = -Wall -DBUILD_OS=$(BUILD_OS) -DBUILD_CPU=$(BUILD_CPU) $(CPPFLAGS) -g -O2
LDFLAGS = 
LIBS = 
INSTALL = /bin/install -c

#Your compiler.  If gcc doesn't work, try CC
#CC = gcc

#For Linux Systems
#PLATFORM = 'Linux'
#C_OPTS = -O2 -Wall -DLINUX_ANSI 

#For Sgi
#PLATFORM = 'SGI'
#C_OPTS = -O2 -DIRIX_ANSI -DSGI

#For AIX
#PLATFORM = 'AIX'
#C_OPTS = -O2 -DAIX

#For All Other Unixes
#PLATFORM = 'Default Unix'
#C_OPTS = -O2 

all:	linux_logo

clean:
	/bin/rm -f *.o
	/bin/rm -f linux_logo
	/bin/rm *~
	
distclean:	clean

install:	linux_logo
	$(INSTALL) linux_logo $(bindir)

uninstall:
	/bin/rm -f $(bindir)/linux_logo
	
linux_logo:	linux_logo.o sysinfo.o bogomips.o
	$(CC) $(CFLAGS) -o linux_logo linux_logo.o bogomips.o sysinfo.o $(LIBS)
	@strip linux_logo

linux_logo.o:	linux_logo.c ascii_penguin.h defaults.h
	@echo Compiling for $(PLATFORM)
	@echo Edit the Makefile to change Platform
	@echo Edit defaults.h to change Default Values
	$(CC) $(CFLAGS) -c linux_logo.c

sysinfo.o:	sysinfo.c sysinfo.c sysinfo_default.c sysinfo_common.h sysinfo_aix.c sysinfo_irix.c sysinfo_ix86.c sysinfo_m68k.c sysinfo_ppc.c sysinfo_sparc.c sysinfo_mips.c
	$(CC) $(CFLAGS) -c sysinfo.c 

bogomips.o:	bogomips.c
	$(CC) $(CFLAGS) -c bogomips.c
