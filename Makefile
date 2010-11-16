##############################################################
#  Makefile for Linux Logo -- by Vince Weaver                #
##############################################################

-include Makefile.default

PROGNAME = linux_logo

#ifeq ($(OS),IRIX64) 
#   LDFLAGS += -lintl
#endif

#
# Installation location
#
INSTALL_BINPATH = $(PREFIX)/bin
INSTALL_MANPATH = $(PREFIX)/share/man
INSTALL_DOCPATH = $(PREFIX)/share/doc

#
# Libsysinfo location
#
LIBSYSINFO_INCLUDE = -I$(LIBSYSINFO)
LIBSYSINFO_LIBRARY = -L$(LIBSYSINFO)

ifneq ($(XGETTEXT),)
    TRANSLATIONS = translations
endif

all:	Makefile.default parse_logos linux_logo $(TRANSLATIONS)

Makefile.default:	 
	ifneq($(CONFIGURE_RAN),1)
	$(error Please run configure first)
	endif

translations:
	cd po && $(MAKE)

logos-all:
	find ./logos -type f > logo_config
	$(MAKE) all

logo_config:
	echo "./logos/banner.logo" > logo_config
	echo "./logos/classic.logo" >> logo_config

clean:	Makefile.default
	rm -f *.o
	rm -f linux_logo linux_logo-dyn parse_logos load_logos.h
	rm -f *~
	cd $(LIBSYSINFO) && $(MAKE) clean
	cd po && $(MAKE) clean

distclean: clean
	cd $(LIBSYSINFO) && $(MAKE) distclean
	rm -f Makefile.default logo_config

linux_logo:	linux_logo.o load_logo.o ./$(LIBSYSINFO)/libsysinfo.a
	$(CROSS)$(CC) $(LDFLAGS) -o linux_logo linux_logo.o load_logo.o $(LIBSYSINFO_LIBRARY) ./$(LIBSYSINFO)/libsysinfo.a 

linux_logo_shared:	linux_logo.o load_logo.o ./$(LIBSYSINFO)/libsysinfo.a
	$(CROSS)$(CC) $(CFLAGS) -o linux_logo-dyn linux_logo.o load_logo.o $(LIBSYSINFO_LIBRARY) -lsysinfo


./$(LIBSYSINFO)/libsysinfo.a:
	cd $(LIBSYSINFO) && $(MAKE)

parse_logos:	parse_logos.o load_logo_native.o
	$(CC) $(LDFLAGS) -o parse_logos parse_logos.o load_logo_native.o

parse_logos.o:	parse_logos.c logo_config
	$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c parse_logos.c

load_logos.h:	logo_config parse_logos
	./parse_logos

load_logo.o:	load_logo.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c load_logo.c

load_logo_native.o:	load_logo.c
	$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -o load_logo_native.o -c load_logo.c	

linux_logo.o:	linux_logo.c defaults.h load_logos.h
	@echo Compiling for $(OS)
	@echo Edit defaults.h to change Default Values
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) \
		      -DLOCALE_DIR=\"$(PREFIX)/share/locale\" -DUSE_I18N=$(USE_I18N) \
		      -c linux_logo.c

install:	linux_logo
	$(INSTALL) -c -m755 $(PROGNAME) -D $(INSTALL_BINPATH)/$(PROGNAME)
	$(INSTALL) -c -D -m644 $(PROGNAME).1.gz $(INSTALL_MANPATH)/man1/$(PROGNAME).1.gz
	cd po && $(MAKE) install

install-doc:
	$(INSTALL) -c -d -m 755 $(INSTALL_DOCPATH)/$(PROGNAME)
	$(INSTALL) -c -p -m 644 *[A-Z] $(INSTALL_DOCPATH)/$(PROGNAME)

# The old way of installing
install-by-copying:
	cp linux_logo $(PREFIX)/bin
