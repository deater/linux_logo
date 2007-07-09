include ../Makefile.default

all:	sysinfo_solaris.o

sysinfo_solaris.o:	sysinfo_solaris.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c sysinfo_solaris.c

clean:	
	rm -f *~ *.o 
 
