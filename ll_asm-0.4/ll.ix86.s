#
#  linux_logo in assembler    0.4
#
#  by Vince Weaver <vince@deater.net>
#
#  assemble with     "as -o ll.o ll.s"
#  link with         "ld -o ll ll.o"

#  BUGS:  No Pretty Printing (rounding, cpu-model cleanup)
#      :  Only works with <1GB of RAM
#      :  Doesn't print vendor name

# offsets into the results returned by the uname syscall
.equ U_SYSNAME,0
.equ U_NODENAME,65
.equ U_RELEASE,65*2
.equ U_VERSION,(65*3)
.equ U_MACHINE,(65*4)
.equ U_DOMAINNAME,65*5

# offset into the results returned by the stat syscall
.equ S_SIZE,20

	.globl _start	
_start:	
	#=========================
	# PRINT LOGO
	#=========================
	
	mov 	$new_logo,%esi		# point input to new_logo
	mov 	$out_buffer,%edi	# point output to buffer
	xor 	%ecx,%ecx		# clear color register
	mov 	$(('['<<8)+27),%bx	# store ^[[ in bx to save time
	mov 	$10,%dh			# ten, used in division later
main_logo_loop:	
	lodsb				# load character
	cmp	$0,%al			# if zero, we are done
	je 	done_logo
	cmp	$27,%al			# if ^[, we are a color
	jne 	blit_repeat

	mov 	%bx,%ax			# load ^[[
	stosw				# out to buffer

	xor	%eax,%eax		# clear eax
        lodsb				# load color
	cmp	$10,%al			# is it less than 10?		
	jl	out_ones		# if so skip ahead
	
	div	%dh			# divide ax by 10
	add	$0x30,%al		# convert tens to ascii
	stosb				# out the tens digit
	mov	%ah,%al			# copy ones to al
	
out_ones:
	add	$0x30,%al		# convert ones digit to ascii
	stosb				# out to buffer
	
	mov	$'m',%al		# load 'm'
	stosb				# store to buffer

	jmp 	main_logo_loop		# done with color

blit_repeat:
	mov     %al,%dl			# save the character
	lodsb				# get times to repeat
	mov	%al,%cl			# move times to repeat in loop reg
	mov	%dl,%al			# restore the character to output
	rep stosb			# rle to buffer
	
	jmp main_logo_loop

done_logo:	
	mov     $4,%eax			# number of the "write" syscall
	mov     $1,%ebx			# stdout
	mov     $out_buffer,%ecx	# our nice huge logo
	call	strlen
	int     $0x80     		# do syscall

	movb	$0xa,out_char		# print line-feed
	call	put_char
	
	#==========================
	# PRINT VERSION
	#==========================
	
	mov	$122,%eax		# uname syscall
	mov	$uname_info,%ebx	# uname struct
	int	$0x80			# do syscall
	
	mov	$out_buffer,%edi		# destination is temp_string
	mov	$(uname_info+U_SYSNAME),%esi	# os-name from uname "Linux"
	call	strcat
	
	mov	$ver_string,%esi		# source is " Version "
	call 	strcat
	
	mov	$(uname_info+U_RELEASE),%esi    # version from uname "2.4.1"
	call 	strcat
	
	mov	$compiled_string,%esi		# source is ", Compiled "
	call 	strcat

	mov	$(uname_info+U_VERSION),%esi	# compiled date
	call 	strcat
	
	mov	$out_buffer,%ecx
	call	strlen			# returns size in edx
	
	call	center			# print some spaces
	
	mov 	$4,%eax			# write syscall
	mov 	$1,%ebx			# stdout
	mov 	$out_buffer,%ecx	
        call	strlen			
        int	$0x80
	
	movb	$0xa,out_char		# print line-feed
	call	put_char
	
  
	
	#===============================
	# Middle-Line
	#===============================
	
	#=========
	# Load /proc/cpuinfo into buffer
	#=========

	mov	$5,%eax			# open()
	mov	$cpuinfo,%ebx		# '/proc/cpuinfo'
	mov	$0,%ecx			# O_RDONLY <bits/fcntl.h>
	xor	%edx,%edx
	int	$0x80			# syscall.  fd in eax.  
					# we should check that eax>=0
	mov	%eax,%ebx
	
	mov	$3,%eax			# read
	mov	$disk_buffer,%ecx
	mov	$4096,%edx		# 4096 is maximum size of proc file #)
	int	$0x80

	mov	$6,%eax			# close
	int	$0x80			

	#=============
	# Number of CPU's
	#=============
	
	xor	%ebx,%ebx		# chip count
	
	mov	$disk_buffer,%esi
bogo_loop:	
	lodsb				# really inefficient way to search
	cmp 	$0,%al			# for 'bogo' string
	je 	done_bogo		# and then count them up
	cmp	$'b',%al		# the number found
	jne 	bogo_loop		# is number of cpus on intel Linux
	lodsb
	cmp	$'o',%al
	jne	bogo_loop
	lodsb
	cmp	$'g',%al
	jne	bogo_loop
	lodsb
	cmp	$'o',%al
	jne	bogo_loop
	inc	%ebx			# we have a bogo
	jmp	bogo_loop
done_bogo:
       
       	dec	%ebx			# correct for array index begin at 0
	shl	$2,%ebx			# *4  [ie, make it into dword]
	mov	ordinal,%esi		# load pointer array
	add	%ebx,%esi		# index into it
	mov	$out_buffer,%edi	# destination string
	call	strcat			# copy it

	mov	$space,%esi		# print a space
	call	strcat
	
	
	#=========
	# MHz
	#=========
	
	mov	$('M'<<8+' '),%bx	# find ' MHz\t: ' and grab up to .
	mov	$('z'<<8+'H'),%cx	# we are little endian
	mov	$'.',%dl			
   	call	find_string
   
   	mov	$megahertz,%esi		# print 'MHz '
	call	strcat
   
   
   	#=========
	# Chip Name
	#=========
	
   	mov	$('a'<<8+'n'),%bx     	# find 'name\t: ' and grab up to \n
	mov	$('e'<<8+'m'),%cx	# we are little endian
	mov	$0xa,%dl
	call	find_string
	
	mov	$comma,%esi		# print ', '
	call	strcat
	
	#========
	# RAM
	#========
	
	mov	$106,%eax	       	# stat() syscall
	mov	$kcore,%ebx		# /proc/kcore
	mov	$stat_buff,%ecx
	int	$0x80
	
	mov	(stat_buff+S_SIZE),%eax	# size in bytes of RAM
	shr	$10,%eax		# divide to get K
	shr	$10,%eax		# divide to get M
	
	mov	$100,%ecx		# divide mem by 100 
	div	%cl			# [works only on 3-digit values of RAM]
	cmp	$0,%al			# suppress leading zeros
	jz	tens
	add	$0x30,%al		# convert to ascii
	stosb				# print hundreds digit
tens:	
	shr	$8,%eax			# move ah into al
	mov	$10,%ecx		# divide reaminder by 10
	div	%cl			# quotient in al, remainder in ah
	cmp	$0,%al			# suppress leading zeros
	jz	ones
      	add	$0x30,%al		# convert to ascii
	stosb				# print tens digit
ones:	
	shr	$8,%eax			# remainder is ones digit
	add	$0x30,%al		# convert to ascii
	stosb				# print ones digit
		
	mov	$ram_comma,%esi		# print 'M RAM, '
	call	strcat
	
	#========
	# Bogomips
	#========
	
	mov	$('i'<<8+'m'),%bx      	# find 'mips\t: ' and grab up to \n
	mov	$('s'<<8+'p'),%cx
	mov	$0xa,%dl
	call	find_string
   
   	mov	$bogo_total,%esi
	call	strcat
   
	mov	$out_buffer,%ecx	# string done, lets print it
	call	strlen			# returns size in edx
	
	call	center			# print some spaces
		
	mov 	$4,%eax			# write syscall
	mov 	$1,%ebx			# stdout
	mov 	$out_buffer,%ecx		
        call	strlen			
        int 	$0x80
       		
	movb 	$0xa,out_char		# print line-feed
	call 	put_char
	
	#=================================
	# Print Host Name
	#=================================
	
	mov	$out_buffer,%edi		# output string
	mov	$(uname_info+U_NODENAME),%esi	# host name from uname()
	mov	$out_buffer,%ecx		# for strlen
	call	strcat
	call	strlen
	call	center			# center it
	mov 	$4,%eax			# write syscall
	mov 	$1,%ebx			# stdout
	mov 	$out_buffer,%ecx		
        call	strlen			
        int 	$0x80

	movb 	$0xA, out_char		# print line-feed
	call 	put_char

	#================================
	# Exit
	#================================
	
        xor     %ebx,%ebx
        mov     $1,%eax           	# put the exit syscall number in eax
        int     $0x80             	# and exit



	#=================================
	# FIND_STRING 
	#=================================
	#   dl is char to end at
	#   bx and cx are 4-char ascii string to look for
	#   edi points at output buffer

find_string:
					
	mov	$disk_buffer,%esi	# look in cpuinfo buffer
find_loop:
	lodsb				# watch for first char
	cmp	$0,%al
	je	done
	cmp	%bl,%al
	jne	find_loop
	lodsb				# watch for second char
	cmp	%bh,%al
	jne	find_loop
	lodsb				# watch for third char
	cmp	%cl,%al
	jne	find_loop
	lodsb				# watch for fourth char
	cmp	%ch,%al
	jne	find_loop
	
					# if we get this far, we matched

find_colon:
	lodsb				# repeat till we find colon
	cmp	$0,%al
	je	done
	cmp	$':',%al
	jne	find_colon

	lodsb				# skip a char [should be space]
	
store_loop:	 
	 lodsb				# load value
	 cmp	$0,%al
	 je	done
    	 cmp	%dl,%al			# is it end string?
	 je 	almost_done		# if so, finish
	 stosb				# if not store and continue
	 jmp	store_loop
	 
almost_done:	 
	xor	%eax,%eax		# replace last value with null
	stosb
	dec	%edi			# move pointer back 
done:
	ret


	#================================
	# put_char
	#================================
	# value to print in [out_char]

put_char:
	push 	%eax
	push 	%ebx
	push 	%ecx
	push 	%edx
	mov	$4,%eax			# write char
	mov	$1,%ebx			# stdout
	mov	$out_char,%ecx		# output character
	mov 	$1,%edx			# write 1 char
	int	$0x80
	pop	%edx
	pop	%ecx
	pop	%ebx
	pop	%eax
	ret

	#================================
	# strcat
	#================================

strcat:
	lodsb				# load a byte from [ds:esi]
	stosb				# store a byte to [es:edi]
	cmp	$0,%al			# is it zero?
	jne	strcat			# if not loop
	dec	%edi			# point to one less than null
	ret				# return

	#===============================
	# strlen
	#===============================
	# ecx points to string
	# edx is returned with length

strlen:
	push 	%ecx			# save pointer
	xor	%edx,%edx		# clear counter
str_loop:
	inc	%ecx
	inc	%edx
	cmpb 	$0,(%ecx)		# repeat till we find zero
	jne	str_loop
	
	pop %ecx
	ret
	
	#==============================
	# center
	#==============================
	# edx has length of string
	
center:
	cmp	$80,%edx		# see if we are >80
	jge	done_center		# if so, bail
	mov	%edx,%ecx		# 80-length
	neg	%ecx
	add	$80,%ecx
	shr	%ecx			# then divide by 2
	movb 	$0x20,out_char		# load in a space
center_loop:
	call 	put_char		# and print that many spaces
	loop	center_loop
done_center:	
	ret


#===========================================================================
#	section .data
#===========================================================================

.equ		NORMAL , 0
.equ		BOLD   , 1
.equ		F_BLACK,30
.equ		F_RED  ,31
.equ		F_YELLW,33
.equ		F_WHITE,37
.equ		B_BLACK,40
.equ		B_WHITE,47

		# logo is Run Length Encoded.  This saves ~1k
		# first character is char to output, second is run-length
		# if first char is ESC (27) then what follows is a color
		# to be print using "^[[xm" where x is the color
		# I could have compressed this more, but I left it generic
		# enough that _any_ logo, not just default, can be used
		# we could save a mov instruction by flipping order of fields
		# oh well

new_logo:	.byte	27,NORMAL,  27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',65
                .byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',5 
		.byte	27,BOLD,    27,F_WHITE, '#',9
		.byte	27,NORMAL,  10,1
line_2:		.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',64
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',7
		.byte	27,BOLD,    27,F_WHITE, '#',8
		.byte	27,NORMAL,  10,1
line_3:	        .byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',19 
		.byte	27,F_RED,   '#',1
		.byte	27,F_WHITE, '#',44
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, 'O',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, 'O',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',8
		.byte	27,NORMAL,  10,1
line_4:		.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',2
		.byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',6
		.byte      27,BOLD,    27,F_WHITE, '#',10
		.byte      27,F_RED,   '#',2 
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',43
		.byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte      27,BOLD,    27,F_YELLW, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',8
		.byte	27,NORMAL,  10,1
line_5:		.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',4
		.byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte      27,BOLD,    27,F_WHITE, '#',13
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',42
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,F_YELLW, '#',3
		.byte	27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',6
		.byte	27,NORMAL,  10,1
line_6:		.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',4
		.byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',9
		.byte	27,F_RED,   '#',3
		.byte	27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',4
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3 
		.byte	27,BOLD,    27,F_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',5
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',6
	        .byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',10
		.byte 	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  10,1
line_7:		.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',8
		.byte	27,F_RED,   '#',1
	        .byte	27,F_WHITE, '#',2
		.byte	27,F_RED,   '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',7
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',12
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',4
	        .byte	27,NORMAL,  10,1
line_8:		.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',4
	        .byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte      27,BOLD,    27,F_WHITE, '#',7
		.byte	27,F_RED,   '#',1
		.byte	27,F_WHITE, '#',3
		.byte	27,F_RED,   '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte      27,BOLD,    27,F_WHITE, '#',6
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',9
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',12
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  10,1
line_9:		.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',10
		.byte	27,F_RED,   '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte      27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',6
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',8
		.byte	27,F_YELLW, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',11
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_YELLW, '#',1
		.byte	27,F_WHITE, '#',3
		.byte	27,NORMAL,  10,1
line_10:	.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',7
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,F_RED,   '#',2 
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,F_YELLW, '#',6
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',7
		.byte	27,F_BLACK, '#',1
		.byte	27,F_YELLW, '#',6
		.byte      27,F_WHITE, '#',1
		.byte	27,NORMAL,  10,1
line_11:	.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',6
	        .byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,F_RED,   '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
	        .byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',4
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',2
		.byte	27,BOLD,    27,F_WHITE, '#',4
		.byte	27,F_YELLW, '#',7
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE,  '#',1
		.byte	27,BOLD,    27,F_WHITE, '#',5
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',1
		.byte	27,BOLD,    27,F_YELLW, '#',7
		.byte	27,F_WHITE, '#',1
		.byte	27,NORMAL,  10,1
line_12:	.byte	27,BOLD,    27,F_WHITE, 27,B_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',12
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',2
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',4
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',4
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',4
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',3
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',5
		.byte	27,BOLD,    27,F_WHITE, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',6
		.byte	27,BOLD,    27,F_WHITE, '#',3
		.byte	27,F_YELLW, '#',5
		.byte	27,F_BLACK, '#',1
		.byte	27,NORMAL,  27,F_BLACK, 27,B_WHITE, '#',5
		.byte	27,BOLD,    '#',1
		.byte	27,F_YELLW, '#',5
		.byte	27,F_WHITE, '#',3
		.byte	27,NORMAL,  27,BOLD,	10,1
		.byte	0,0	

ver_string:	.ascii	" Version \0"
compiled_string:	.ascii	", Compiled \0"
space:		.ascii	" \0"
megahertz:	.ascii	"MHz \0"
comma:		.ascii	", \0"
ram_comma:	.ascii	"M RAM, \0"
bogo_total:	.ascii	" Bogomips Total\0"

cpuinfo:	.ascii	"/proc/cpuinfo\0"
kcore:		.ascii	"/proc/kcore\0"

ordinal:	.long	one,two,three,four	

one:	.ascii	"One\0"
two:	.ascii	"Two\0"
three:	.ascii	"Three\0"
four:	.ascii	"Four\0"

#============================================================================
#	section .bss
#============================================================================
	
.lcomm out_char,1	
.lcomm version_string,(255*2)

.lcomm string_ptr,(1*4)

.lcomm stat_buff,(4*2+2*4+4*12)
	# urgh get above from /usr/src/linux/include/asm/stat.h
	# not glibc

.lcomm uname_info,(65*6)

.lcomm	disk_buffer,4096	# we cheat!!!!
.lcomm	out_buffer,8192



