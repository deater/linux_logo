/*-----------------------------------------------------------------------------
 LINUX LOGO 2.07 - Creates a Nifty Logo With some System Info - 14 July 1998
     by Vince Weaver (weave@eng.umd.edu, http://www.glue.umd.edu/~weave )
		  
  perfect if you want a Penguin on Boot Up, but not in the kernel.
  Just stick this file in the rc files somewhere.
 
  The file is ANSI escape sequences, so it should work OK with most terminals.
 
 The image was created using the logo.xpm from the Linux distribution,
     ppm2ansi by Carsten Haitzler -- http://www.cse.unsw.edu.au/~s2154962/
     and was hand edited using THEDRAW under dosemu 0.66.7

 Thanks to: Ornulf Staff -- help on SMP and -o option
            Filip M Gieszczykiewicz -- idea for -n option
            Christian Marillat -- Atari m68k 
            Nicolai Langfeldt -- help on SMP
            Adam Spiers -- some ANSI help
            Adam Lackorzynski -- improved '-o' handling, and modular
                                 help screen.
            Thomas Grewe -- AIX port
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/utsname.h>

#define ESCAPE '\033'
#define VERSION "2.07"
#define MAX_YSIZE 50

#include "getsysinfo.h"

/* Change the values in the below file to alter default behavior */
#include "defaults.h"

/* Some global variables.  Possibly bad in practice, but it saves a lot *\
\* of paramater passing, which has caused bugs to develop before        */
/* To change these defaults, edit the 'defaults.h' file                 */ 

int  width=DEFAULT_WIDTH,                  /* Defaults to 80   */
     no_periods=DEFAULT_NO_PERIODS,        /* Defaults to None */ 
     preserve_xy=DEFAULT_PRESERVE_XY,      /* Defaults to Off  */            
     skip_bogomips=DEFAULT_SKIP_BOGOMIPS,  /* Defaults to No   */
     offset=DEFAULT_OFFSET,                /* Defaults to 0    */
     plain_ascii=DEFAULT_PLAIN_ASCII,      /* Defaults to No   */
     banner_mode=DEFAULT_BANNER_MODE,      /* Defualts to Yes  */
     wipe_screen=DEFAULT_WIPE_SCREEN,      /* Defaults to No   */
     show_uptime=DEFAULT_SHOW_UPTIME,      /* Defaults to No   */
     display_logo_only=0,
     display_sysinfo_only=0;
char symbol=DEFAULT_SYMBOL,                /* Defaults to '#'  */
     symbol_bgnd=DEFAULT_SYMBOL_BGND;      /* Defaults to '#'  */

void do_spacing(int spaces)
{
    int i;
   
    if (spaces>0) {
       if (plain_ascii) for(i=0;i<spaces;i++) putchar(' ');
       else printf("%c[%iC",ESCAPE,spaces);
    }
}

/* Prints an ansi string, changing "^[" to the ESCAPE charachter '\033' *\
\*    And also enables easier parsing of the output                     */
/* The name doesn't indicate any sort of ANSI standard; it is left over *\
\*    from years gone by when I used to make "ansi art" for a WWIV BBS  */
int ansi_print(const char *string,int no_periods,int offset,int alt_char,
	       int alt_char_bgnd)
{
    int i,next_is_escape=0;
   
    do_spacing(offset);
    i=0;
    while(1) {
       if (next_is_escape) {
	  if (plain_ascii) {
	     while( (!isalpha(string[i])) && (string[i]!='\0')) i++;
	     i++;
	     next_is_escape=0;
	  }
	  else {
	     putchar('\033');
	     next_is_escape=0;
	  }
       }
       switch(string[i]){
        case '\0': return 0;
	case ESCAPE: {next_is_escape=1;} break;
        case '^' : if (string[i+1]=='[') {next_is_escape=1; i++;} break;
        case '\\': if (string[i+1]=='n') {printf("\n"); i++;} 
        case '.' : if (no_periods) putchar(' '); 
	           else putchar(string[i]); break;
        case '#' : if (alt_char) putchar(alt_char); 
	           else putchar('#'); break;
	case ',' : if (alt_char_bgnd) putchar(alt_char_bgnd);
	           else putchar(','); break;	  
        default  : putchar(string[i]);
       }
       i++;
    } 
    return 0;
}

void clear_screen()
{
   ansi_print("^[[2J^[[0;0H\n",0,0,0,0);  
}

void help_message(char *binname, char full)
{
    printf("\nLinux Logo Version %s",VERSION);
    printf(" -- by Vince Weaver (weave@eng.umd.edu)\n");
    printf("   Newest Versions at:\n");
    printf("      http://www.glue.umd.edu/~weave/vmwprod\n");
    printf("      http://sunsite.unc.edu/pub/Linux/logos/penguins\n\n");
    if (!full) exit(0);
    printf("Usage:   %s [-ascii] [-banner] [-classic] [-f] [-g] [-h] [-kX] "
	   "[-l]\n"
	   "                     [-n] [-o Num] [-p] [-rX] [-s] [-u] [-v] "
	   "[-w Num]\n",binname);
    printf("         [-ascii] -- Display logo as ascii only monochrome\n");
    printf("         [-banner] - New default Banner Logo!\n");
    printf("         [-classic]- The Old [original] linux_logo look\n");
    printf("         [-f]     -- force the screen clear before drawing\n");
    printf("         [-g]     -- give system info only\n");
    printf("         [-h]     -- this help screen\n");   
    printf("         [-kX]    -- X is a character to replace the "
	   "background one.\n");
    printf("         [-l]     -- display logo only\n");
    printf("         [-n]     -- toggle periods off [may make cleaner "
           "output\n");
    printf("         [-o Num] -- offset output Num spaces to the right\n");
    printf("         [-p]     -- preserve cursor location\n");
    printf("         [-rX]    -- X is a character to replace '#' with "
	   "in banner mode\n");  
    printf("         [-s]     -- skip Bogomips [speeds up on non-Linux "
           "platforms]\n");
    printf("         [-u]     -- show uptime\n");
    printf("         [-v]     -- version information\n");
    printf("         [-w Num] -- set width of screen to Num [default 80]\n\n");
    exit(0); 
}

void draw_classic_logo(char **logo)
{
    char os_name[BUFSIZ],host_name[BUFSIZ],os_version[BUFSIZ],
         os_revision[BUFSIZ],cpu_info[BUFSIZ],temp_string[BUFSIZ],
         bogo_total[BUFSIZ],uptime[BUFSIZ];
    int i,j;

    if (wipe_screen) clear_screen();
   
    for(i=0;i<7;i++) {
       ansi_print(logo[i],no_periods,offset,0,0); 
       printf("\n"); 
    }
   
    ansi_print(logo[7],no_periods,offset,0,0);

       /* Get some OS info using some external functions in getsysinfo.c */
    get_os_info((char *)&os_name,(char *)&os_version,
		(char *)&os_revision,(char *)&host_name,
		(char *)&uptime);
    
    do_spacing(2);
    sprintf(temp_string,"^[[1;37;40m%s Version %s^[[0m\n",
	    os_name,os_version);
    ansi_print(temp_string,0,0,0,0);
    ansi_print(logo[8],no_periods,offset,0,0);
    
    do_spacing(2);
    sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",os_revision);
    ansi_print(temp_string,0,0,0,0);
   
       /* Get some hardware info using getsysinfo.c */
    get_hardware_info((char *)&cpu_info,(char *)&bogo_total,skip_bogomips);
     
    ansi_print(logo[9],no_periods,offset,0,0);
    
    do_spacing(2);
    sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",cpu_info);
    ansi_print(temp_string,0,0,0,0);
   
    ansi_print(logo[10],no_periods,offset,0,0);
    do_spacing(2);
    sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",bogo_total);
    ansi_print(temp_string,0,0,0,0);

    if (show_uptime) {
       ansi_print(logo[11],no_periods,offset,0,0);
       do_spacing(2);
       sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",uptime);
       ansi_print(temp_string,0,0,0,0);
       j=12;
    }
    else j=11;
   
       /* Print Host Name */
    ansi_print(logo[j],no_periods,offset,0,0);
    do_spacing(2);
    sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",host_name);
    ansi_print(temp_string,0,0,0,0);
	    
    for(i=j+1;i<16;i++) {
       ansi_print(logo[i],no_periods,offset,0,0); 
       printf("\n"); 
    }
  
    ansi_print("^[[0m^[[255D\n",no_periods,0,0,0);
    if (preserve_xy) ansi_print("^[8",no_periods,0,0,0);
}

void draw_banner_logo()
{
    char os_name[BUFSIZ],host_name[BUFSIZ],os_version[BUFSIZ],
         os_revision[BUFSIZ],cpu_info[BUFSIZ],temp_string[BUFSIZ],
         bogo_total[BUFSIZ],uptime[BUFSIZ];
    int i;
      
    if (width<80) width=80;
   
    if (!display_sysinfo_only) {
       if (wipe_screen) clear_screen();
       for(i=0;i<12;i++) {
          do_spacing((width-80)/2);
          if (plain_ascii) ansi_print(ascii_banner[i],0,0,symbol,' ');
          else ansi_print(banner[i],0,0,symbol,symbol_bgnd);
          printf("\n");
       }
       if (!display_logo_only) printf("\n");
    }
   
    if (!display_logo_only) {
          /* Get some OS info using some external functions in getsysinfo.c */
       get_os_info((char *)&os_name,(char *)&os_version,
   		   (char *)&os_revision,(char *)&host_name,
                   (char *)&uptime);

       i=((width-(strlen(os_name)+11+strlen(os_version)+
		 strlen(os_revision)))/2);
       do_spacing(i);
       sprintf(temp_string,"^[[1;37;40m%s Version %s, %s^[[0m\n",
	       os_name,os_version,os_revision);
       ansi_print(temp_string,0,0,0,0);
    
          /* Get some hardware info using getsysinfo.c */
       get_hardware_info((char *)&cpu_info,(char *)&bogo_total,skip_bogomips);
      
       i=((width-(strlen(cpu_info)+4+strlen(bogo_total)))/2);
       do_spacing(i);
       sprintf(temp_string,"^[[1;37;40m%s, %s^[[0m\n",
 	       cpu_info,bogo_total);
       ansi_print(temp_string,0,0,0,0);
       
          /* Print Uptime */
       if (show_uptime) {
          i=((width-(strlen(uptime)))/2);
	  do_spacing(i);
          sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",uptime);
          ansi_print(temp_string,0,0,0,0);
       }
       
          /* Print Host Name */
       i=((width-(strlen(host_name)))/2);   
       do_spacing(i);
       sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",host_name);
       ansi_print(temp_string,0,0,0,0);
    }
    ansi_print("^[[0m^[[255D\n",no_periods,0,0,0);
}



int main(int argc,char **argv)
{
    char *endptr;
   char ch;
    
    int i;

       /* Parse Command Line Arguments */
    for(i=1;i<argc;i++) {
          /* they all start with a '-' */
       if (argv[i][0]=='-') { 
  	  if (argv[i][1]=='-') ch=argv[i][2];
	  else ch=argv[i][1];
      
	  switch(ch){
	   case 'v': case 'V': help_message(argv[0], 0);
	   case 'h': case 'H': case '?': help_message(argv[0], 1);
	   case 'l': case 'L': display_logo_only=1; break;
	   case 'g': case 'G': display_sysinfo_only=1; break;
	   case 'a': case 'A': plain_ascii=1; break;
	   case 'b': case 'B': banner_mode=1; break;
	   case 'c': case 'C': banner_mode=0; break;
	   case 'u': case 'U': show_uptime=1; break;
	   case 'r': case 'R': symbol=argv[i][2]; break;
	   case 'k': case 'K': symbol_bgnd=argv[i][2]; break;
	   case 'n': case 'N': no_periods=1; break;
	   case 'p': case 'P': preserve_xy=1; break;
	   case 'f': case 'F': wipe_screen=1; break;
           case 's': case 'S': skip_bogomips=1; break;
	   case 'o': case 'O':
	      if ((argc > i+1) && (*argv[i+1])) {
	         /* there's a following param with something to do */
		 offset=strtol(argv[i+1],&endptr,10);
		 if (*endptr) {
		     help_message(argv[0], 1);         
                 }
	       } else { /* -o is last param */
	         help_message(argv[0], 1);        
	       }
	   case 'w': case 'W':
	      if ((argc > i+1) && (*argv[i+1])) {
	         /* there's a following param with something to do */
		 width=strtol(argv[i+1],&endptr,10);
		 if (*endptr) {
		     help_message(argv[0], 1);         
                 }
	       } else { /* -w is last param */
	         help_message(argv[0], 1);        
	       }
	   default: break;
	  }
       }
    }

       /* Start Printing the Design */
    if (preserve_xy) ansi_print("^[7",no_periods,0,0,0);
   
    if (banner_mode) draw_banner_logo();
    else if (plain_ascii) draw_classic_logo( (char**)ascii_logo);
    else {
       ansi_print("^[[40m^[[40m\n",0,0,0,0);
       draw_classic_logo( (char**)color_logo);
    }
    if (preserve_xy) ansi_print("^[8",0,0,0,0);
   
    return 0;
}
