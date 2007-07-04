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
	rm -f Makefile.default
	

install:	


libsysinfo.a:	
	cd all && $(MAKE)
	cd $(OS) && $(MAKE)
	$(CROSS)$(AR) crus libsysinfo.a ./$(OS)/*.o ./all/*.o

shared:	libsysinfo.a
	ld -shared -o libsysinfo.so ./$(OS)/*.o ./all/*.o
