##############################################################
#  Makefile for Linux Logo -- by Vince Weaver                #
##############################################################

include Makefile.default

CPPFLAGS = -I./$(LIBSYSINFO) 
LDFLAGS = -L./$(LIBSYSINFO)

ifeq ($(OS),IRIX64) 
   LDFLAGS += -lintl
endif

#
# Installation location
#
INSTALL_BINPATH = $(PREFIX)/bin
INSTALL_MANPATH = $(PREFIX)/share/man
INSTALL_DOCPATH = $(PREFIX)/share/doc


all:	parse_logos linux_logo translations

translations:
	cd po && $(MAKE)

logos-all:
	find ./logos -type f > logo_config
	$(MAKE) all

logos-default:
	echo "./logos/banner.logo" > logo_config
	echo "./logos/classic.logo" >> logo_config
	$(MAKE) all

clean:
	rm -f *.o
	rm -f linux_logo linux_logo-dyn parse_logos load_logos.h
	rm -f *~
	cd $(LIBSYSINFO) && $(MAKE) clean
	cd po && $(MAKE) clean

linux_logo:	linux_logo.o load_logo.o ./$(LIBSYSINFO)/libsysinfo.a
	$(CC) $(CFLAGS) $(CPPFLAGS) -o linux_logo linux_logo.o load_logo.o  ./$(LIBSYSINFO)/libsysinfo.a $(LDFLAGS)

linux_logo_shared:	linux_logo.o load_logo.o ./$(LIBSYSINFO)/libsysinfo.a
	$(CC) $(CFLAGS) $(CPPFLAGS) -o linux_logo-dyn linux_logo.o load_logo.o  -L./$(LIBSYSINFO) -lsysinfo


./$(LIBSYSINFO)/libsysinfo.a:
	cd $(LIBSYSINFO) && $(MAKE)

parse_logos:	parse_logos.o load_logo.o
	$(CC) -o parse_logos parse_logos.o load_logo.o $(LDFLAGS)

parse_logos.o:	parse_logos.c logo_config
	$(CC) $(CFLAGS) $(CPPFLAGS) -c parse_logos.c

load_logos.h:	logo_config parse_logos
	./parse_logos

load_logo.o:	load_logo.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c load_logo.c

linux_logo.o:	linux_logo.c defaults.h load_logos.h
	@echo Compiling for $(OS)
	@echo Edit defaults.h to change Default Values
	$(CC) $(CFLAGS) $(CPPFLAGS) -c linux_logo.c

install:	linux_logo
	/usr/bin/install -m755 $(PROGNAME) -D $(INSTALL_BINPATH)/$(PROGNAME)
	/usr/bin/install -m644 $(PROGNAME).1 $(INSTALL_MANPATH)/man1/$(PROGNAME).1
	cd po && $(MAKE) install

install-doc:
	/usr/bin/install -d -m 755 $(INSTALL_DOCPATH)/$(PROGNAME)
	/usr/bin/install -p -m 644 *[A-Z] $(INSTALL_DOCPATH)/$(PROGNAME)

# The old way of installing
install-by-copying:
	cp linux_logo /usr/local/bin
