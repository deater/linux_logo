##############################################################
#  Makefile for Linux Logo 3.9b3  -- by Vince Weaver         #
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

#
# Installation location
#

DESTDIR = /usr/local
INSTALL_BINPATH = $(DESTDIR)/bin
INSTALL_MANPATH = $(DESTDIR)/man
INSTALL_DOCPATH = /usr/share/doc
PROGNAME = linux_logo

#
# You shouldn't have to modify 
# anything below this point.
#

all:	parse_logos linux_logo
	cd po && make

logos-all:
	find ./logos -type f > logo_config
	make all

logos-default:
	echo "./logos/banner.logo" > logo_config
	echo "./logos/classic.logo" >> logo_config
	make all

clean:
	rm -f *.o
	rm -f linux_logo linux_logo-dyn parse_logos
	rm -f *~
	cd libsysinfo && make clean
	cd po && make clean

	
linux_logo:	linux_logo.o vmw_string.o ./libsysinfo/libsysinfo.a
	$(CC) $(C_OPTS) -o linux_logo linux_logo.o vmw_string.o ./libsysinfo/libsysinfo.a $(L_OPTS)
	@strip linux_logo

linux_logo_shared:	linux_logo.o vmw_string.o ./libsysinfo/libsysinfo.a
	$(CC) $(C_OPTS) -o linux_logo-dyn linux_logo.o vmw_string.o -L./libsysinfo -lsysinfo
	@strip linux_logo-dyn

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

install:	linux_logo
	/usr/bin/install -s -m 755 $(PROGNAME) $(INSTALL_BINPATH)
	/usr/bin/install -m 644 $(PROGNAME).1.gz $(INSTALL_MANPATH)/man1
	cd po && make install

install-doc:
	/usr/bin/install -d -m 755 $(INSTALL_DOCPATH)/$(PROGNAME)
	/usr/bin/install -p -m 644 *[A-Z] $(INSTALL_DOCPATH)/$(PROGNAME)
	
# The old way of installing
install-by-copying:
	cp linux_logo /usr/local/bin
