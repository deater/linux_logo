include ../Makefile.default

all:	bogomips.o fix_mhz.o parsing.o string_truncate.o sysinfo_common.o uname.o

bogomips.o:	bogomips.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c bogomips.c

fix_mhz.o:	fix_mhz.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c fix_mhz.c

parsing.o:	parsing.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c parsing.c

sysinfo_common.o:	sysinfo_common.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c sysinfo_common.c

string_truncate.o:	string_truncate.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c string_truncate.c

uname.o:	uname.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c uname.c

clean:
	rm -f *.o
	rm -f *~
