#
#  linux_logo in assembler
#
#  by Vince Weaver <vince@deater.net>
#
#  assemble with     "as -o ll.o ll.s"
#  link with         "ld -s -o ll ll.o"

#  BUGS:  No Pretty Printing (rounding, cpu-model cleanup)
#      :  Only works with <1GB of RAM
#      :  Doesn't print vendor name

.equ U_SYSNAME,0
.equ U_NODENAME,65
.equ U_RELEASE,65*2
.equ U_VERSION,(65*3)
.equ U_MACHINE,(65*4)
.equ U_DOMAINNAME,65*5



	
	.globl _start	
_start:	

	eieio
	#=========================
	# PRINT LOGO
	#=========================
	
	li	0,4			# number of the "write" syscall
	li      3,1			# stdout
	lis     4,logo@ha		# our nice huge logo
	addi	4,4,logo@l
	bl	strlen
	sc	           		# do syscall

	lis     4,line_feed@ha		# print line feed
	addi    4,4,line_feed@l
        bl	put_char

	
	#==========================
	# PRINT VERSION
	#==========================
	
	li	0,122			# uname syscall
	lis	3,uname_info@ha		# uname struct
	addi    3,3,uname_info@l
	sc				# do syscall
	
	lis	25,temp_string@ha	# destination is temp_string */	
	addi	25,25,temp_string@l
	mr	28,25		   	# save for later
	
       	lis	27,uname_info+U_SYSNAME@ha	   	# os-name from uname "Linux"
	addi	27,27,uname_info+U_SYSNAME@l
	bl	strcat
	
	lis	27,ver_string@ha	# source is " Version "
	addi	27,27,ver_string@l
	bl 	strcat
	
	lis	27,uname_info+U_RELEASE@ha		# version from uname "2.4.1"
	addi	27,27,uname_info+U_RELEASE@l
	bl 	strcat
	
	lis	27,compiled_string@ha	# source is ", Compiled "
	addi	27,27,compiled_string@l
	bl 	strcat

	lis	27,uname_info+U_VERSION@ha		# compiled date
	addi	27,27,uname_info+U_VERSION@l
	bl 	strcat

	mr	4,25
	bl	strlen			# returns size in edx
	
	bl	center			# print some spaces

	mr	4,25
	li 	0,4			# write syscall
	li 	3,1			# stdout
        bl	strlen			
        sc
	
	lis     4,line_feed@ha		# print line feed
	addi    4,4,line_feed@l
        bl	put_char
	
	
      	#===============================
	# Middle-Line
	#===============================
	
	#=========
	# Load /proc/cpuinfo into buffer
	#=========

	li	0,5			# open()
	lis	3,cpuinfo@ha
	addi	3,3,cpuinfo@l		# '/proc/cpuinfo'
	li	4,0			# O_RDONLY <bits/fcntl.h>
	li	5,0
	sc				# syscall.  fd in r0.  
					# we should check that r0>=0
	mr	3,0			
	mr     13,0
	li	0,3			# read

	lis	4,disk_buffer@ha
	addi	4,4,disk_buffer@l
	li	5,4096		 	# 4096 is maximum size of proc file ;)
	sc

	mr	3,13
	li	0,6			# close
	sc			

	#=============
	# Number of CPU's
	#=============
	
	# Assume 1 for now
	# my iBook's /proc/cpuinfo does not have a "processor' line ???
	
	mr	28,25		   	# pointer to temp_string
	
       	lis	27,one@ha	       
	addi	27,27,one@l
	bl	strcat	
	
	#=========
	# MHz
	#=========
	
	li	6,'c'			# find 'clock : ' and grab up to .
	li	7,'l'
	li	8,'o'			
	li	9,'.'
   	bl	find_string
   
   	lis	27,megahertz@ha		# print 'MHz '
	addi	27,27,megahertz@l
	bl	strcat
   
/*   
   	;=========
	; Chip Name
	;=========
	
   	mov	bx,'na'			; find 'name\t: ' and grab up to \n
	mov	cx,'me'
	mov	dl,0xa
	call	find_string
	
	mov	esi,comma		; print ', '
	call	strcat
	
	;========
	; RAM
	;========
	
	mov	eax,106			; stat() syscall
	mov	ebx,kcore		; /proc/kcore
	mov	ecx,stat_buff
	int	0x80
	
	mov	eax,[s_size]		; size in bytes of RAM
	shr	eax,10			; divide to get K
	shr	eax,10			; divide to get M
	
	mov	ecx,100			; divide mem by 100 [works only on 3-digit
	div	cl			;                    values of RAM]
	cmp	al,0			; suppress leading zeros
	jz	tens
	add	al,0x30			; convert to ascii
	stosb				; print hundreds digit
tens:	
	shr	eax,8			; move ah into al
	mov	ecx,10			; divide reaminder by 10
	div	cl			; quotient in al, remainder in ah
	cmp	al,0			; suppress leading zeros
	jz	ones
       	add	al,0x30			; convert to ascii
	stosb				; print tens digit
ones:	
	shr	eax,8			; remainder is ones digit
	add	al,0x30			; convert to ascii
	stosb				; print ones digit
		
	mov	esi,ram_comma		; print 'M RAM, '
	call	strcat
	
	;========
	; Bogomips
	;========
	
	mov	bx,'mi'			; find 'mips\t: ' and grab up to \n
	mov	cx,'ps'
	mov	dl,0xa
	call	find_string
   
   	mov	esi,bogo_total
	call	strcat
*/   
#	mr	4,25
#	bl	strlen			# returns size in edx
	
#	bl	center			# print some spaces

#	mr	4,25
#	li 	0,4			# write syscall
#	li 	3,1			# stdout
 #       bl	strlen			
  #      sc

	lis     4,line_feed@ha		# print line feed
	addi    4,4,line_feed@l
        bl	put_char

	#=================================
	# Print Host Name
	#=================================

	lis	4,uname_info+U_NODENAME@ha	       
	addi	4,4,uname_info+U_NODENAME@l
	
	bl	strlen
	bl	center			# center it
	li 	0,4			# write syscall
	li 	3,1			# stdout
	lis	4,uname_info+U_NODENAME@ha		
	addi	4,4,uname_info+U_NODENAME@l
        bl	strlen
	sc

	lis     4,line_feed@ha		# print line feed
	addi    4,4,line_feed@l
        bl	put_char

	#================================
	# Exit
	#================================
	
        li	3,0		# 0 exit value
        li	0,1             # put the exit syscall number in r0
        sc              	# and exit

	#=================================
	# FIND_STRING 
	#=================================
	#   r9 is char to end at
	#   r6,r7,r8 are 3-char ascii string to look for
	#   r28 points at output buffer

find_string:

	mflr 31
	lis 	13,disk_buffer@ha
	addi	13,13,disk_buffer@l

	lis	17,temp_string@ha
	addi	17,17,temp_string@l
loopoop:
	lbz	14,0(13)
	addi	13,13,1
	stb	14,0(17)
	mr	4,17
	bl	put_char		
        cmpi	0,0,14,0
        bc	12,2,done
	b	loopoop


find_loop:
/*	lbz	14,0(13)	        # watch for first char
	addi	13,13,1
	cmpi	0,0,14,0
	bc	12,2,done
	
	cmp	0,0,14,6
	bc	4,10,find_loop
	
	lbz	14,0(13)	        # watch for second char
	addi	13,13,1
	cmp	0,0,14,7
	bc	4,10,find_loop
	
	lbz	14,0(13)		# watch for third char
	addi	13,13,1
	cmp	0,0,14,8
	bc	4,10,find_loop
	
					# if we get this far, we matched

find_colon:
	lbz	14,0(13)		# repeat till we find colon
	addi	13,13,1
	cmpi	0,0,14,0
	bc	12,2,done

	cmpi	0,0,14,':'
	bc	4,2,find_colon
 
	addi 13,13,1			# skip a char [should be space]
*/	
store_loop:	 
	 lbz	14,0(13)		# load value
#	 addi	13,13,1
#	 cmpi	0,0,14,0
#	 bc	12,2,done
	 
	 li	9,':'
	 
    	 cmp	0,0,14,9		# is it end string?
	 bc 	12,2,almost_done	# if so, finish
#	 stb	14,0(28)		# if not store and continue
#	 addi	28,28,1
	 b	store_loop
	 
almost_done:	 
	li	14,'V'                    # replace last value with null
	stb	14,0(28)
	addi	28,28,1

done:
     	mtlr 31
	blr


	#================================
	# put_char
	#================================
	# value to print pointed to by r4

put_char:
	li	0,4			# write char
	li	3,1			# stdout
	li 	5,1			# write 1 char
	sc
	blr
	

	#================================
	# strcat
	#================================
	# r13 = "temp"
	# r27 = "source"
	# r28 = "destination"

strcat:
	lbz     13,0(27)		# load a byte from [r27]
	stb	13,0(28)		# store a byte to [r28]
	addi	27,27,1
	addi	28,28,1
	cmpi	0,0,13,0		# is it zero?
	bc	0,2,strcat		# if not loop
	subi	28,28,1			# point to one less than null
	blr				# return

	#===============================
	# strlen
	#===============================
	# r4 points to string
	# r5 is returned with length

strlen:
        mr	18,4			# copy pointer
	li	5,0			# set count to 0
str_loop:
	addi	18,18,1			# increment pointer
	addi	5,5,1			# increment counter
	lbz	17,0(18)		# load byte
	cmpi    0,0,17,0		# is it zero?
	bc	0,2,str_loop		# if not, loop
	blr				# return from function
	
	#==============================
	# center
	#==============================
	# r5 has length of string
	# r13, r14 = temp
	
center:
       mflr	14
        li	13,80			# 80 column screen
	subf	13,5,13
	srawi	13,13,1			# then divide by 2
	lis     4,space@ha		# print line feed
	addi    4,4,space@l	
center_loop:
	bl 	put_char		# and print that many spaces
	subi	13,13,1
	cmpi	0,0,13,0	
	bc	0,2,center_loop
	mtlr	14
	blr


#===========================================================================
#	section .data
#===========================================================================
	
logo:	.ascii "[0;1;37;47m#################################################################[0;30;47m#####[1;37m#########[1;37;40m\n[0;1;37;47m################################################################[0;30;47m#######[1;37m########[1;37;40m\n[0;1;37;47m###################[31m#[37m############################################[0;30;47m##[1;37mO[0;30;47m#[1;37mO[0;30;47m##[1;37m########[1;37;40m\n[0;1;37;47m##[0;30;47m######[1;37m##########[31m##[0;30;47m#[1;37m###########################################[0;30;47m#[1;33m#####[0;30;47m#[1;37m########[1;37;40m\n[0;1;37;47m####[0;30;47m##[1;37m#############[0;30;47m#[1;37m##########################################[0;30;47m##[1;37m##[33m###[37m##[0;30;47m##[1;37m######[1;37;40m\n[0;1;37;47m####[0;30;47m##[1;37m#########[31m###[37m###[0;30;47m###[1;37m#[0;30;47m####[1;37m###[0;30;47m###[1;37m####[0;30;47m###[1;37m##[0;30;47m#####[1;37m#[0;30;47m######[1;37m#####[0;30;47m#[1;37m##########[0;30;47m##[1;37m#####[1;37;40m\n[0;1;37;47m####[0;30;47m##[1;37m########[31m#[37m##[31m#[0;30;47m#[1;37m###[0;30;47m###[1;37m####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m####[0;30;47m##[1;37m###[0;30;47m##[1;37m#######[0;30;47m#[1;37m############[0;30;47m##[1;37m####[1;37;40m\n[0;1;37;47m####[0;30;47m##[1;37m#######[31m#[37m###[31m#[0;30;47m#[1;37m###[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m######[0;30;47m###[1;37m#########[0;30;47m#[1;37m############[0;30;47m###[1;37m###[1;37;40m\n[0;1;37;47m####[0;30;47m##[1;37m##########[31m##[0;30;47m#[1;37m###[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m######[0;30;47m###[1;37m########[33m##[0;30;47m#[1;37m###########[0;30;47m##[1;33m#[37m###[1;37;40m\n[0;1;37;47m####[0;30;47m##[1;37m#######[0;30;47m#[1;37m#[31m##[0;30;47m#[1;37m####[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m#####[0;30;47m##[1;37m#[0;30;47m##[1;37m#####[33m######[0;30;47m#[1;37m#######[30m#[33m######[37m#[1;37;40m\n[0;1;37;47m####[0;30;47m##[1;37m######[0;30;47m##[1;37m#[31m##[0;30;47m#[1;37m#[0;30;47m#[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m###[1;37m###[0;30;47m###[1;37m####[0;30;47m##[1;37m###[0;30;47m##[1;37m####[33m#######[0;30;47m#[1;37m#####[0;30;47m#[1;33m#######[37m#[1;37;40m\n[0;1;37;47m##[0;30;47m############[1;37m##[0;30;47m###[1;37m##[0;30;47m####[1;37m###[0;30;47m####[1;37m###[0;30;47m####[1;37m#[0;30;47m###[1;37m#[0;30;47m#####[1;37m#[0;30;47m######[1;37m###[33m#####[30m#[0;30;47m#####[1m#[33m#####[37m###[1;37;40m\n\0"
line_feed:      .ascii "\n"
space:	        .ascii " "
ver_string:	.ascii " Version \0"
compiled_string: .ascii	", Compiled \0"
megahertz:	.ascii "MHz \0"
comma:		.ascii ", \0"
ram_comma:	.ascii "M RAM, \0"
bogo_total:	.ascii " Bogomips Total\0"

cpuinfo:	.ascii "/proc/cpuinfo\0"
kcore:		.ascii "/proc/kcore\0"


ordinal:
one:	.ascii	"One\0"
two:	.ascii	"Two\0"
three:	.ascii	"Three\0"
four:	.ascii	"Four\0"
/*
;============================================================================
	section .bss
;============================================================================
	
out_char:		resb	1	
*/
.lcomm version_string,255
.lcomm temp_string,255
/*
string_ptr:		resd	1

stat_buff:				; urgh get from /usr/src/linux/include/asm/stat.h
s_device:		resd	1	; not glibc
s_inode:		resd	1
s_mode:			resw	1
s_nlink:		resw	1
s_uid:			resw	1
s_gid:			resw	1
s_rdev:			resd	1
s_size:			resd	1
s_blksize:		resd	1
s_blocks:		resd	1
s_atime:		resd	2	
s_mtime:		resd	2
s_ctime:		resd	4
*/

.lcomm uname_info,(65*6)
.lcomm disk_buffer,4096
