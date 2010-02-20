include ../Makefile.default

ifneq (,$(findstring 86,$(ARCH)))
   ARCH := x86
endif
ifneq (,$(findstring arm,$(ARCH)))
   ARCH := arm
endif
ifneq (,$(findstring sh,$(ARCH)))
   ARCH := sh
endif


all:	cpuinfo.o sysinfo_linux.o

cpuinfo.o:	  $(ARCH)


alpha:	cpuinfo_alpha.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_alpha.c

arm:	cpuinfo_arm.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_arm.c

avr32:	cpuinfo_avr32.c
	$(CROSS)$(CC) $(CFLAGS) -o avr32.o -c cpuinfo_avr32.c

cris:	cpuinfo_cris.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_cris.c

frv:	cpuinfo_frv.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_frv.c

ia64:	cpuinfo_ia64.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_ia64.c

m32r:	cpuinfo_m32r.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_m32r.c

m68k:	cpuinfo_m68k.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_m68k.c

microblaze:	cpuinfo_microblaze.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_microblaze.c

mips:	cpuinfo_mips.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_mips.c
	
mips64:	cpuinfo_mips.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_mips.c	

parisc:	cpuinfo_parisc.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_parisc.c
	
parisc64:	cpuinfo_parisc.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_parisc.c	
	
ppc:	cpuinfo_ppc.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_ppc.c

ppc64:	cpuinfo_ppc.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_ppc.c

s390:	cpuinfo_s390.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_s390.c
	
s390x:	cpuinfo_s390x.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_s390x.c	

sh:	cpuinfo_sh3.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_sh3.c

sparc:	cpuinfo_sparc.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_sparc.c

sparc64:	cpuinfo_sparc.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_sparc.c

vax:	cpuinfo_vax.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_vax.c

x86:	cpuinfo_x86.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_x86.c
	
x86_64:	cpuinfo_x86_64.c
	$(CROSS)$(CC) $(CFLAGS) -o cpuinfo.o -c cpuinfo_x86_64.c
		
sysinfo_linux.o:	sysinfo_linux.c
	$(CROSS)$(CC) $(CFLAGS) -c sysinfo_linux.c

clean:	
	rm -f *~ *.o
 
