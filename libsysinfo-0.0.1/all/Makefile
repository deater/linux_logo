#Your compiler.  If gcc doesn't work, try CC
CC = gcc

C_FLAGS = -O2 -Wall -I..

all:	bogomips.o fix_mhz.o parsing.o sysinfo_common.o uname.o

bogomips.o:	bogomips.c
	$(CC) $(C_FLAGS) -c bogomips.c

fix_mhz.o:	fix_mhz.c
	$(CC) $(C_FLAGS) -c fix_mhz.c

parsing.o:	parsing.c
	$(CC) $(C_FLAGS) -c parsing.c

sysinfo_common.o:	sysinfo_common.c
	$(CC) $(C_FLAGS) -c sysinfo_common.c

uname.o:	uname.c
	$(CC) $(C_FLAGS) -c uname.c 

clean:
	rm -f *.o
	rm -f *~
