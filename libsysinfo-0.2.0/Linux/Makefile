include ../Makefile.default


ifneq (,$(findstring 86,$(ARCH)))
   ARCH := x86
endif
ifneq (,$(findstring arm,$(ARCH)))
   ARCH := arm
endif



all:	cpuinfo.o sysinfo_linux.o

cpuinfo.o:	  $(ARCH)


alpha:	cpuinfo_alpha.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_alpha.c

arm:	cpuinfo_arm.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_arm.c

cris:	cpuinfo_cris.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_cris.c
	
ia64:	cpuinfo_ia64.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_ia64.c

m68k:	cpuinfo_m68k.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_m68k.c

mips:	cpuinfo_mips.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_mips.c

parisc:	cpuinfo_parisc.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_parisc.c
	
parisc64:	cpuinfo_parisc.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_parisc.c	
	
ppc:	cpuinfo_ppc.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_ppc.c

ppc64:	cpuinfo_ppc.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_ppc.c

s390:	cpuinfo_s390.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_s390.c

sh3:	cpuinfo_sh3.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_sh3.c

sparc:	cpuinfo_sparc.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_sparc.c

sparc64:	cpuinfo_sparc.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_sparc.c

vax:	cpuinfo_vax.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_vax.c

x86:	cpuinfo_x86.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_x86.c
	
x86_64:	cpuinfo_x86_64.c
	$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_x86_64.c
		
sysinfo_linux.o:	sysinfo_linux.c
	$(CC) $(CFLAGS) -c sysinfo_linux.c

clean:	
	rm -f *~ *.o
 
