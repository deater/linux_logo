##############################################################
#  Makefile for Linux Logo 3.9b1  -- by Vince Weaver         #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
CC = gcc

#For Linux Systems
PLATFORM = 'Linux'
C_OPTS = -O2 -Wall -DLINUX_ANSI

#For All Other Unixes
#PLATFORM = 'Default Unix'
#C_OPTS = -O2 

all:	parse_logos linux_logo

clean:
	rm -f *.o
	rm -f linux_logo parse_logos
	rm *~

install:	linux_logo
	cp linux_logo /usr/local/bin
	
linux_logo:	linux_logo.o sysinfo.o bogomips.o vmw_string.o
	$(CC) $(C_OPTS) -o linux_logo linux_logo.o bogomips.o sysinfo.o vmw_string.o $(L_OPTS)
	@strip linux_logo

vmw_string.o:	vmw_string.c
	$(CC) $(C_OPTS) -c vmw_string.c
	
parse_logos:	parse_logos.o vmw_string.o
	$(CC) $(C_OPTS) -o parse_logos parse_logos.o vmw_string.o $(L_OPTS)
	
parse_logos.o:	parse_logos.c logo_config
	$(CC) $(C_OPTS) -c parse_logos.c

load_logos.h:	logo_config parse_logos
	./parse_logos

linux_logo.o:	linux_logo.c defaults.h load_logos.h
	@echo Compiling for $(PLATFORM)
	@echo Edit the Makefile to change Platform
	@echo Edit defaults.h to change Default Values
	$(CC) $(C_OPTS) -c linux_logo.c

sysinfo.o:	sysinfo.c sysinfo.c sysinfo_default.c sysinfo_common.h sysinfo_aix.c sysinfo_irix.c sysinfo_ix86.c sysinfo_m68k.c sysinfo_ppc.c sysinfo_sparc.c sysinfo_mips.c
	$(CC) $(C_OPTS) -c sysinfo.c 

bogomips.o:	bogomips.c
	$(CC) $(C_OPTS) -c bogomips.c
