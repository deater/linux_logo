include ../Makefile.default

all:	bogomips.o fix_mhz.o parsing.o sysinfo_common.o uname.o

bogomips.o:	bogomips.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c bogomips.c

fix_mhz.o:	fix_mhz.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c fix_mhz.c

parsing.o:	parsing.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c parsing.c

sysinfo_common.o:	sysinfo_common.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c sysinfo_common.c

uname.o:	uname.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c uname.c 

clean:
	rm -f *.o
	rm -f *~
