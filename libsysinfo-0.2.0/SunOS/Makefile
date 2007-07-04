include ../Makefile.default

all:	cpuinfo.o sysinfo_solris.o

sysinfo_solaris.o:	sysinfo_solaris.c
	$(CROSS)$(CC) $(CFLAGS) -c sysinfo_solaris.c

clean:	
	rm -f *~ *.o 
 
