all:	ll

ll:	ll.o
	ld -o ll ll.o
	
ll.o:	ll.asm
	nasm -g -f elf ll.asm

clean:
	rm -f ll *.o *~
