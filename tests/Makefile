CC = gcc
CFLAGS = -Wall -O2 
LFLAGS =

LIBSYSINFO = libsysinfo-0.2.2

all:	memory_print memory_method

memory_print:	memory_print.o
	$(CC) $(LFLAGS) -o memory_print memory_print.o

memory_print.o:	memory_print.c
	$(CC) $(CFLAGS) -c memory_print.c
	
memory_method:	memory_method.o ../$(LIBSYSINFO)/libsysinfo.a
	$(CC) $(LFLAGS) -o memory_method memory_method.o ../$(LIBSYSINFO)/libsysinfo.a

memory_method.o:	memory_method.c
	$(CC) $(CFLAGS) -I../$(LIBSYSINFO) -c memory_method.c	

clean:	
	rm -f *~ *.o memory_print memory_method
