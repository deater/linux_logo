/*-----------------------------------------------------------------------------
LINUX LOGO 2.12 -Creates a Nifty Logo With some System Info- 30 October 1998
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
#include <string.h>
#include <sys/utsname.h>

#define ESCAPE '\033'
#define VERSION "2.12"
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
     show_load=DEFAULT_SHOW_LOAD,          /* Defaults to No   */
     narrow_logo=DEFAULT_NARROW_LOGO,      /* Defaults to No   */
     display_logo_only=0,
     display_sysinfo_only=0,
     display_usertext=0;
char symbol=DEFAULT_SYMBOL,                /* Defaults to '#'  */
     symbol_bgnd=DEFAULT_SYMBOL_BGND;      /* Defaults to '#'  */
char *user_text = NULL; /* Change this and display_usertext to *\
                         \*        have a default message       */
char *cpuinfo_file=NULL;

void do_spacing(int spaces)
{
    int i;
   
    if ((spaces>0)&&(spaces<1000000)) { /* A hack.  Ever >1M wide displays?*/  
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
	case '%' : if (!narrow_logo) {
	              if (alt_char_bgnd) putchar(alt_char_bgnd);
	              else putchar(','); 
	           }  break;
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

void center_and_print(char *string,int size,int width)
{
   int i;
   char temp_string[BUFSIZ];

   i=((width-size)/2);
   do_spacing(i);
   sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",string);
   ansi_print(temp_string,0,0,0,0);   
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
    printf("Usage:   %s [-a] [-b] [-c] [-f] [-g] [-h] [-kX] "
	   "[-n] [-o Num]\n"
	   "                    [-p] [-rX] [-s] [-u] [-v] [-w Num]\n",
	   binname);
    printf("         [-a]     -- Display logo as ascii only monochrome\n");
    printf("         [-b]     -- New default Banner Logo!\n");
    printf("         [-c]     -- The Old [original] linux_logo look\n");
    printf("         [-e file]-- Use \"file\" instead of /proc/cpuinfo [for "
	   "debugging\n");
    printf("         [-f]     -- force the screen clear before drawing\n");
    printf("      B  [-g]     -- give system info only\n");
    printf("         [-h]     -- this help screen\n");   
    printf("      B  [-kX]    -- X is a character to replace the "
	   "background one.\n");
    printf("      B  [-l]     -- display logo only\n");
    printf("      C  [-n]     -- toggle periods off [may make cleaner "
           "output]\n");
    printf("      C  [-o Num] -- offset output Num spaces to the right\n");
    printf("         [-p]     -- preserve cursor location\n");
    printf("      B  [-rX]    -- X is a character to replace '#' with "
	   "in banner mode\n");  
    printf("         [-s]     -- skip Bogomips [speeds up on non-Linux "
           "platforms]\n");
    printf("         [-t str] -- display user-supplied string\n");
    printf("      *  [-u]     -- show uptime\n");
    printf("         [-v]     -- version information\n");
    printf("         [-w Num] -- set width of screen to Num [default 80]\n");
    printf("      B  [-x]     -- narrow logo [useful if issue displays wrong]"
	   "\n");
    printf("      *  [-y]     -- show load average\n\n");
    printf(" B=Banner mode only, C=Classic Mode Only  *=Works Only in Linux"
	   "\n\n");
    exit(0); 
}

void draw_classic_logo(char **logo)
{
    char os_name[BUFSIZ],host_name[BUFSIZ],os_version[BUFSIZ],
         os_revision[BUFSIZ],cpu_info[BUFSIZ],
         bogo_total[BUFSIZ],uptime[BUFSIZ],load_avg[BUFSIZ];
    int i,num_info=0;

    char info_lines[8][BUFSIZ]={"\000","\000","\000","\000",
                                "\000","\000","\000","\000"}; 
                                /* yes ugly.  should malloc it */
                                /* But I am lazy right now ;)  */
    
    if (display_usertext) {
       sprintf(info_lines[num_info++],"^[[1;37;40m%s^[[0m\n",user_text);
    }
   
       /* Get some OS info using some external functions in getsysinfo.c */
    get_os_info( (char *)&os_name,(char *)&os_version,
		 (char *)&os_revision,(char *)&host_name,
		 (char *)&uptime,(char *)&load_avg);
    sprintf(info_lines[num_info++],"^[[1;37;40m%s Version %s^[[0m\n",
	    os_name,os_version);
    sprintf(info_lines[num_info++],"^[[1;37;40m%s^[[0m\n",os_revision);
 
       /* Get some hardware info using getsysinfo.c */
    get_hardware_info((char *)&cpu_info,(char *)&bogo_total,skip_bogomips,
		      cpuinfo_file);
    sprintf(info_lines[num_info++],"^[[1;37;40m%s^[[0m\n",cpu_info);
    sprintf(info_lines[num_info++],"^[[1;37;40m%s^[[0m\n",bogo_total);
     
    if (show_uptime) 
       sprintf(info_lines[num_info++],"^[[1;37;40m%s^[[0m\n",uptime);
    
    if (show_load)
       sprintf(info_lines[num_info++],"^[[1;37;40m%s^[[0m\n",load_avg);
	        
    sprintf(info_lines[num_info++],"^[[1;37;40m%s^[[0m\n",host_name);
 
       /* OK we've got the info ready, let's print it all */
    if (wipe_screen) clear_screen();
    for(i=0;i<7;i++) {
       ansi_print(logo[i],no_periods,offset,0,0); 
       printf("\n"); 
    }
    for(i=7;i<15;i++) {
       ansi_print(logo[i],no_periods,offset,0,0);
       do_spacing(2);
       if (info_lines[i-7][0]!='\000') ansi_print(info_lines[i-7],0,0,0,0);
       else printf("\n");
    }
	    
    for(i=15;i<16;i++) {
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
         bogo_total[BUFSIZ],uptime[BUFSIZ],load_avg[BUFSIZ];
    int i;
      
    if (width<80) width=80;

    if (wipe_screen) clear_screen();
    if (!display_sysinfo_only) {
       for(i=0;i<12;i++) {
          do_spacing((width-80)/2);
          if (plain_ascii) ansi_print(ascii_banner[i],0,0,symbol,' ');
          else ansi_print(banner[i],0,0,symbol,symbol_bgnd);
          printf("\n");
       }
       if (!display_logo_only) printf("\n");
    }
   
    if (!display_logo_only) {
      
       if (display_usertext) 
	  center_and_print(user_text,strlen(user_text),width);

          /* Get some OS info using some external functions in getsysinfo.c */
       get_os_info((char *)&os_name,(char *)&os_version,
   		   (char *)&os_revision,(char *)&host_name,
                   (char *)&uptime,(char *)&load_avg);

       sprintf(temp_string,"%s Version %s, %s",os_name,os_version,os_revision);
       center_and_print(temp_string,strlen(temp_string),width);

          /* Get some hardware info using getsysinfo.c */
       get_hardware_info((char *)&cpu_info,(char *)&bogo_total,skip_bogomips,
			 cpuinfo_file);

       sprintf(temp_string,"%s, %s",cpu_info,bogo_total);
       center_and_print(temp_string,strlen(temp_string),width);
       
          /* Print Uptime */
       if (show_uptime) 
	  center_and_print(uptime,strlen(uptime),width);
	  
          /* Print Load Average */
       if (show_load) 
	  center_and_print(load_avg,strlen(load_avg),width);
       
          /* Print Host Name */
       center_and_print(host_name,strlen(host_name),width);
       
    }
    ansi_print("^[[0m^[[255D\n",no_periods,0,0,0);
}



int main(int argc,char **argv)
{
    char *endptr;
    int c;
   
    cpuinfo_file=strdup("/proc/cpuinfo");
   
    while ((c = getopt (argc, argv,"A::B::C::E:FGHK:LNO:PR:ST:UVW:XY"
			           "a::b::c::e:fghk:lno:pr:st:uvw:xy"))!=-1)
       switch (c) {
	  case 'a': case 'A': plain_ascii=1; break;
	  case 'b': case 'B': banner_mode=1; break;
	  case 'c': case 'C': banner_mode=0; break;
	  case 'e': case 'E': cpuinfo_file=strdup(optarg); break;
	  case 'f': case 'F': wipe_screen=1; break;
	  case 'g': case 'G': display_sysinfo_only=1; break;
	  case 'h': case 'H': help_message(argv[0], 1);
	  case 'k': case 'K': symbol_bgnd=optarg[0]; break;
	  case 'l': case 'L': display_logo_only=1; break;
	  case 'n': case 'N': no_periods=1; break;
	  case 'o': case 'O':
	            offset=strtol(optarg,&endptr,10);
	            if ( endptr == optarg ) help_message(argv[0], 1);
	            break;
	  case 'p': case 'P': preserve_xy=1; break;
	  case 'r': case 'R': symbol=optarg[0]; break;
	  case 's': case 'S': skip_bogomips=1; break;
	  case 't': case 'T':
	            display_usertext=1;
	            user_text=strdup(optarg);
	            break;
	  case 'u': case 'U': show_uptime=1; break;
	  case 'v': case 'V': help_message(argv[0], 0);
	  case 'w': case 'W':
	            width=strtol(optarg,&endptr,10);
	            if ( endptr == optarg ) help_message(argv[0], 1);
	            break;
	  case 'x': case 'X': narrow_logo=1; break;
	  case 'y': case 'Y': show_load=1; break;
	  case '?': help_message(argv[0], 1);
       }
    if ( argv[optind] != NULL ) help_message(argv[0], 1);
   
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
