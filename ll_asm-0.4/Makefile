all:	ll configure

ll:	ll.o
	ld -o ll ll.o
	@strip ll
	
ll.o:	ll.s
	as -o ll.o ll.s

ll.s:	configure
	./configure

configure:	configure.c
	gcc -O2 -Wall -o configure configure.c

clean:
	rm -f ll *.o *~ configure ll.s
