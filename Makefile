##############################################################
#  Makefile for Linux Logo 2.00 -- by Vince Weaver           #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
CC = gcc

#For Intel Linux Systems
PLATFORM = 'Linux Intel'
SYSINFO = sysinfo_ix86
C_OPTS = -O2 -Wall -DLINUX_ANSI 

#For m68k Linux Systems
#PLATFORM = 'Linux m68k'
#SYSINFO = sysinfo_m68k
#C_OPTS = -O2 -Wall -DLINUX_ANSI

#For Sgi
#PLATFORM = 'SGI'
#SYSINFO = sysinfo_sgi
#C_OPTS = -O2 -DIRIX_ANSI -DSGI

#For AIX
#PLATFORM = 'AIX'
#SYSINFO = sysinfo_aix
#C_OPTS = -O2 -DAIX

#For All Other Unixes
#PLATFORM = 'Default Unix'
#SYSINFO = sysinfo_default
#C_OPTS = -O2 

all:	linux_logo

clean:
	rm -f *.o
	rm -f linux_logo
	rm *~

install:	linux_logo
	cp linux_logo /usr/local/bin
	
linux_logo:	linux_logo.o $(SYSINFO).o bogomips.o
	$(CC) $(C_OPTS) -o linux_logo linux_logo.o bogomips.o $(SYSINFO).o $(L_OPTS)
	@strip linux_logo

linux_logo.o:	linux_logo.c ascii_penguin.h
	@echo Compiling for $(PLATFORM)
	@echo Edit the Makefile to change Platform
	$(CC) $(C_OPTS) -c linux_logo.c

$(SYSINFO).o:	$(SYSINFO).c
	$(CC) $(C_OPTS) -c $(SYSINFO).c

bogomips.o:	bogomips.c
	$(CC) $(C_OPTS) -c bogomips.c
