include ../Makefile.default

all:	cpuinfo.o sysinfo_bsd.o

cpuinfo.o:	cpuinfo_bsd.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -o cpuinfo.o -c cpuinfo_bsd.c
		
sysinfo_bsd.o:	sysinfo_bsd.c
	$(CROSS)$(CC) $(CFLAGS) $(LIBSYSINFO_INCLUDE) -c sysinfo_bsd.c

clean:	
	rm -f *~ *.o
 
