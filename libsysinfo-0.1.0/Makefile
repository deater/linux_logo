include Makefile.defaults

OS =   $(shell uname)

all:   libsysinfo.a

clean:
	rm -f *.o *~
	rm -f libsysinfo.a libsysinfo.so config.h
	cd Linux && $(MAKE) clean
	cd FreeBSD && $(MAKE) clean
	cd Irix && $(MAKE) clean
	cd all && $(MAKE) clean
	rm -f os

install:	


libsysinfo.a:	
	cd all && $(MAKE)
	cd $(OS) && $(MAKE)
	ar crus libsysinfo.a ./$(OS)/*.o ./all/*.o

shared:	libsysinfo.a
	ld -shared -o libsysinfo.so ./$(OS)/*.o ./all/*.o
