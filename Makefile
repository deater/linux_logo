##############################################################
#  Makefile for Linux Logo 0.1  -- by Vince Weaver           #
#                                                            #
#  To modify for your configuration, add or remove the #     #
#                                                            #
##############################################################

#Your compiler.  If gcc doesn't work, try CC
CC = gcc

#For Linux
C_OPTS = -O2 

all:	linux_logo

clean:
	rm -f *.o
	rm -f linux_logo

linux_logo:	linux_logo.o
	$(CC) $(C_OPTS) -o linux_logo linux_logo.o $(L_OPTS)
	@strip linux_logo

linux_logo.o:	linux_logo.c
	$(CC) $(C_OPTS) -c linux_logo.c
