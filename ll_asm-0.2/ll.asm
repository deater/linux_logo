;
;  linux_logo in assembler
;
;  by Vince Weaver <vince@deater.net>
;
;  assemble with     "nasm -o ll.o -f elf ll.asm"
;  link with         "ld -o ll ll.o"

;  BUGS:  No Pretty Printing (rounding, cpu-model cleanup)
;      :  Only works with <1GB of RAM
;      :  Doesn't print vendor name


	section .txt
	
	global _start	
_start:	

	;=========================
	; PRINT LOGO
	;=========================
	
	mov     eax,4			; number of the "write" syscall
	mov     ebx,1			; stdout
	mov     ecx,logo		; our nice huge logo
	call	strlen
	int     0x80     		; do syscall

	mov	byte [out_char],0xa	; print line-feed
	call	put_char
	
	;==========================
	; PRINT VERSION
	;==========================
	
	mov	eax,122			; uname syscall
	mov	ebx,uname_info		; uname struct
	int	0x80			; do syscall
	
	mov	edi,	temp_string	; destination is temp_string	
	mov	esi,	u_sysname	; os-name from uname "Linux"
	call	strcat
	
	mov	esi,	ver_string	; source is " Version "
	call 	strcat
	
	mov	esi,	u_release	; version from uname "2.4.1"
	call 	strcat
	
	mov	esi,	compiled_string	; source is ", Compiled "
	call 	strcat

	mov	esi,	u_version	; compiled date
	call 	strcat
	
	mov	ecx,temp_string
	call	strlen			; returns size in edx
	
	call	center			; print some spaces
	
	mov 	eax,4			; write syscall
	mov 	ebx,1			; stdout
	mov 	ecx,temp_string		
        call	strlen			
        int	0x80
	
	mov	byte [out_char],0xa	; print line-feed
	call	put_char
	
  
	
	;===============================
	; Middle-Line
	;===============================
	
	;=========
	; Load /proc/cpuinfo into buffer
	;=========

	mov	eax,5			; open()
	mov	ebx,cpuinfo		; '/proc/cpuinfo'
	mov	ecx,0			; O_RDONLY <bits/fcntl.h>
	xor	edx,edx
	int	0x80			; syscall.  fd in eax.  
					; we should check that eax>=0
	mov	ebx,eax
	
	mov	eax,3			; read
	mov	ecx,disk_buffer
	mov	edx,4096		; 4096 is maximum size of proc file ;)
	int	0x80

	mov	eax,6			; close
	int	0x80			

	;=============
	; Number of CPU's
	;=============
	
	xor	ebx,ebx			; chip count
	
	mov	esi,disk_buffer
bogo_loop:	
	lodsb				; really inefficient way to search
	cmp 	al,0			; for 'bogo' string
	je 	done_bogo		; and then count them up
	cmp	al,'b'			; the number found
	jne 	bogo_loop		; is number of cpus on intel Linux
	lodsb
	cmp	al,'o'
	jne	bogo_loop
	lodsb
	cmp	al,'g'
	jne	bogo_loop
	lodsb
	cmp	al,'o'
	jne	bogo_loop
	inc	ebx			; we have a bogo
	jmp	bogo_loop
done_bogo:
       
       	dec	ebx			; correct for array index begin at 0
	shl	ebx,2			; *4  [ie, make it into dword]
	mov	esi,[ordinal]		; load pointer array
	add	esi,ebx			; index into it
	mov	edi,temp_string		; destination string
	call	strcat			; copy it

	mov	esi,space		; print a space
	call	strcat
	
	
	;=========
	; MHz
	;=========
	
	mov	bx,' M'			; find ' MHz\t: ' and grab up to .
	mov	cx,'Hz'
	mov	dl,'.'			
   	call	find_string
   
   	mov	esi,megahertz		; print 'MHz '
	call	strcat
   
   
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
   
	mov	ecx,temp_string		; string done, lets print it
	call	strlen			; returns size in edx
	
	call	center			; print some spaces
		
	mov 	eax,4			; write syscall
	mov 	ebx,1			; stdout
	mov 	ecx,temp_string		
        call	strlen			
        int 	0x80
       		
	mov 	byte [out_char],0xA	; print line-feed
	call 	put_char
	
	;=================================
	; Print Host Name
	;=================================
	
	mov	edi,	temp_string	; output string
	mov	esi,	u_nodename	; host name from uname()
	mov	ecx,	temp_string	; for strlen
	call	strcat
	call	strlen
	call	center			; center it
	mov 	eax,4			; write syscall
	mov 	ebx,1			; stdout
	mov 	ecx,temp_string		
        call	strlen			
        int 	0x80

	mov 	byte [out_char],0xA	; print line-feed
	call 	put_char

	;================================
	; Exit
	;================================
	
        xor     ebx,ebx
        mov     eax,1           	; put the exit syscall number in eax
        int     0x80             	; and exit



	;=================================
	; FIND_STRING 
	;=================================
	;   dl is char to end at
	;   bx and cx are 4-char ascii string to look for
	;   edi points at output buffer

find_string:
					
	mov	esi,disk_buffer		; look in cpuinfo buffer
find_loop:
	lodsb				; watch for first char
	cmp	al,0
	je	done
	cmp	al,bl
	jne	find_loop
	lodsb				; watch for second char
	cmp	al,bh
	jne	find_loop
	lodsb				; watch for third char
	cmp	al,cl
	jne	find_loop
	lodsb				; watch for fourth char
	cmp	al,ch
	jne	find_loop
	
					; if we get this far, we matched

find_colon:
	lodsb				; repeat till we find colon
	cmp	al,0
	je	done
	cmp	al,':'
	jne	find_colon
 
	lodsb				; skip a char [should be space]
	
store_loop:	 
	 lodsb				; load value
	 cmp	al,0
	 je	done
    	 cmp	al,dl			; is it end string?
	 je 	almost_done		; if so, finish
	 stosb				; if not store and continue
	 jmp	store_loop
	 
almost_done:	 
	xor	eax,eax			; replace last value with null
	stosb
	dec	edi			; move pointer back 
done:
	ret


	;================================
	; put_char
	;================================
	; value to print in [out_char]

put_char:
	push 	eax
	push 	ebx
	push 	ecx
	push 	edx
	mov	eax,4			; write char
	mov	ebx,1			; stdout
	mov	ecx,out_char		; output character
	mov 	edx,1			; write 1 char
	int	0x80
	pop	edx
	pop	ecx
	pop	ebx
	pop	eax
	ret

	;================================
	; strcat
	;================================

strcat:
	lodsb				; load a byte from [ds:esi]
	stosb				; store a byte to [es:edi]
	cmp	al,0			; is it zero?
	jne	strcat			; if not loop
	dec	edi			; point to one less than null
	ret				; return

	;===============================
	; strlen
	;===============================
	; ecx points to string
	; edx is returned with length

strlen:
	push 	ecx			; save pointer
	xor	edx,edx			; clear counter
str_loop:
	inc	ecx
	inc	edx
	cmp 	byte [ecx],0		; repeat till we find zero
	jne	str_loop
	
	pop ecx
	ret
	
	;==============================
	; center
	;==============================
	; edx has length of string
	
center:
	mov	ecx,edx			; 80-length
	neg	ecx
	add	ecx,80
	shr	ecx,1			; then divide by 2
	mov 	byte [out_char],0x20	; load in a space
center_loop:
	call 	put_char		; and print that many spaces
	loop	center_loop
	ret


;===========================================================================
	section .data
;===========================================================================
	
logo:	db	'[0;1;37;47m#################################################################[0;30;47m#####[1;37m#########[1;37;40m',10,'[0;1;37;47m################################################################[0;30;47m#######[1;37m########[1;37;40m',10,'[0;1;37;47m###################[31m#[37m############################################[0;30;47m##[1;37mO[0;30;47m#[1;37mO[0;30;47m##[1;37m########[1;37;40m',10,'[0;1;37;47m##[0;30;47m######[1;37m##########[31m##[0;30;47m#[1;37m###########################################[0;30;47m#[1;33m#####[0;30;47m#[1;37m########[1;37;40m',10,'[0;1;37;47m####[0;30;47m##[1;37m#############[0;30;47m#[1;37m##########################################[0;30;47m##[1;37m##[33m###[37m##[0;30;47m##[1;37m######[1;37;40m',10,'[0;1;37;47m####[0;30;47m##[1;37m#########[31m###[37m###[0;30;47m###[1;37m#[0;30;47m####[1;37m###[0;30;47m###[1;37m####[0;30;47m###[1;37m##[0;30;47m#####[1;37m#[0;30;47m######[1;37m#####[0;30;47m#[1;37m##########[0;30;47m##[1;37m#####[1;37;40m',10,'[0;1;37;47m####[0;30;47m##[1;37m########[31m#[37m##[31m#[0;30;47m#[1;37m###[0;30;47m###[1;37m####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m####[0;30;47m##[1;37m###[0;30;47m##[1;37m#######[0;30;47m#[1;37m############[0;30;47m##[1;37m####[1;37;40m',10,'[0;1;37;47m####[0;30;47m##[1;37m#######[31m#[37m###[31m#[0;30;47m#[1;37m###[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m######[0;30;47m###[1;37m#########[0;30;47m#[1;37m############[0;30;47m###[1;37m###[1;37;40m',10,'[0;1;37;47m####[0;30;47m##[1;37m##########[31m##[0;30;47m#[1;37m###[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m######[0;30;47m###[1;37m########[33m##[0;30;47m#[1;37m###########[0;30;47m##[1;33m#[37m###[1;37;40m',10,'[0;1;37;47m####[0;30;47m##[1;37m#######[0;30;47m#[1;37m#[31m##[0;30;47m#[1;37m####[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m#####[0;30;47m##[1;37m#[0;30;47m##[1;37m#####[33m######[0;30;47m#[1;37m#######[30m#[33m######[37m#[1;37;40m',10,'[0;1;37;47m####[0;30;47m##[1;37m######[0;30;47m##[1;37m#[31m##[0;30;47m#[1;37m#[0;30;47m#[1;37m##[0;30;47m##[1;37m#####[0;30;47m##[1;37m##[0;30;47m###[1;37m###[0;30;47m###[1;37m####[0;30;47m##[1;37m###[0;30;47m##[1;37m####[33m#######[0;30;47m#[1;37m#####[0;30;47m#[1;33m#######[37m#[1;37;40m',10,'[0;1;37;47m##[0;30;47m############[1;37m##[0;30;47m###[1;37m##[0;30;47m####[1;37m###[0;30;47m####[1;37m###[0;30;47m####[1;37m#[0;30;47m###[1;37m#[0;30;47m#####[1;37m#[0;30;47m######[1;37m###[33m#####[30m#[0;30;47m#####[1m#[33m#####[37m###[1;37;40m',10,0
ver_string:	db	' Version ',0
compiled_string	db	', Compiled ',0
space:		db	' ',0
megahertz:	db	'MHz ',0
comma:		db	', ',0
ram_comma:	db	'M RAM, ',0
bogo_total:	db	' Bogomips Total',0

cpuinfo:	db	'/proc/cpuinfo',0
kcore:		db	'/proc/kcore',0

ordinal:	dd	one,two,three,four	

one:	db	'One',0
two:	db	'Two',0
three:	db	'Three',0
four:	db	'Four',0

;============================================================================
	section .bss
;============================================================================
	
out_char:		resb	1	
version_string:		resw	255

temp_string:		resb	255
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

uname_info:
u_sysname:		resb	65
u_nodename:		resb	65
u_release:		resb	65
u_version:		resb	65
u_machine:		resb	65
u_domainname:		resb	65

disk_buffer:		resb	4096	; we cheat!!!!




