include Makefile.default

all:   libsysinfo.a

clean:
	rm -f *.o *~
	rm -f libsysinfo.a libsysinfo.so config.h
	cd AIX && $(MAKE) clean
	cd Linux && $(MAKE) clean
	cd FreeBSD && $(MAKE) clean
	cd Irix && $(MAKE) clean
	cd SunOS && $(MAKE) clean
	cd w32 && $(MAKE) clean
	cd all && $(MAKE) clean
	
distclean: clean
	rm -f Makefile.default	
	

install:	


libsysinfo.a:	Linux/cpuinfo_alpha.c \
		Linux/cpuinfo_ia64.c \
		Linux/cpuinfo_ppc.c \
		Linux/cpuinfo_vax.c \
		Linux/cpuinfo_arm.c \
		Linux/cpuinfo_m68k.c \
		Linux/cpuinfo_s390.c \
		Linux/cpuinfo_x86_64.c \
		Linux/cpuinfo_avr32.c \
		Linux/cpuinfo_mips.c \
		Linux/cpuinfo_sh3.c \
		Linux/cpuinfo_x86.c \
		Linux/cpuinfo_cris.c \
		Linux/cpuinfo_parisc.c \
	       	Linux/cpuinfo_sparc.c \
		Linux/sysinfo_linux.c 
	cd all && $(MAKE)
	cd $(OS) && $(MAKE)
	$(CROSS)$(AR) crus libsysinfo.a ./$(OS)/*.o ./all/*.o

shared:	libsysinfo.a
	ld -shared -o libsysinfo.so ./$(OS)/*.o ./all/*.o
