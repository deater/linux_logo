##############################################################
#  Makefile for Linux Logo 3.9b2  -- by Vince Weaver         #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
CC = gcc

#For Linux Systems
PLATFORM = 'Linux'
C_OPTS = -O2 -Wall -I./libsysinfo
L_OPTS = -L./libsysinfo


#For All Other Unixes
#PLATFORM = 'Default Unix'
#C_OPTS = -O2 

all:	parse_logos linux_logo

clean:
	rm -f *.o
	rm -f linux_logo parse_logos
	rm -f *~
	cd libsysinfo && make clean

install:	linux_logo
	cp linux_logo /usr/local/bin
	
linux_logo:	linux_logo.o vmw_string.o ./libsysinfo/libsysinfo.a
	$(CC) $(C_OPTS) -o linux_logo linux_logo.o vmw_string.o ./libsysinfo/libsysinfo.a $(L_OPTS)
	@strip linux_logo

./libsysinfo/libsysinfo.a:
	cd libsysinfo && make

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

