/*-----------------------------------------------------------------------------
  LINUX LOGO 3.04 -Creates a Nifty Logo With some System Info- 13 January 2000
    by Vince Weaver (weave@eng.umd.edu, http://www.glue.umd.edu/~weave )
		  
  perfect if you want a Penguin on Boot Up, but not in the kernel.
  Just stick this file in the rc files somewhere.
 
  The file is ANSI escape sequences, so it should work OK with most terminals.
 
 The image was created using the logo.xpm from the Linux distribution,
     ppm2ansi by Carsten Haitzler -- http://www.rasterman.com
     and was hand edited using THEDRAW under dosemu 0.66.7

 Thanks to many, many people who sent patches in.  See the Changes file for
     a list of all those who helped out.
 
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>

#define ESCAPE '\033'
#define VERSION "3.04"
#define MAX_YSIZE 50

#include "getsysinfo.h"

/* Change the values in the below file to alter default behavior */
#include "defaults.h"

/* Some global variables.  Possibly bad in practice, but it saves a lot *\
\* of paramater passing, which has caused bugs to develop before        */
/* To change these defaults, edit the 'defaults.h' file                 */ 

struct os_info_type os_info;
struct hw_info_type hw_info;

void setup_info(struct linux_logo_info_type *logo_info) {
    
    logo_info->width=DEFAULT_WIDTH;                  /* Defaults to 80   */
    logo_info->no_periods=DEFAULT_NO_PERIODS;        /* Defaults to None */ 
    logo_info->preserve_xy=DEFAULT_PRESERVE_XY;      /* Defaults to Off  */
    logo_info->skip_bogomips=DEFAULT_SKIP_BOGOMIPS;  /* Defaults to No   */
    logo_info->offset=DEFAULT_OFFSET;                /* Defaults to 0    */
    logo_info->plain_ascii=DEFAULT_PLAIN_ASCII;      /* Defaults to No   */
    logo_info->banner_mode=DEFAULT_BANNER_MODE;      /* Defualts to Yes  */
    logo_info->wipe_screen=DEFAULT_WIPE_SCREEN;      /* Defaults to No   */
    logo_info->show_uptime=DEFAULT_SHOW_UPTIME;      /* Defaults to No   */
    logo_info->show_load=DEFAULT_SHOW_LOAD;          /* Defaults to No   */
    logo_info->narrow_logo=DEFAULT_NARROW_LOGO;      /* Defaults to No   */
    logo_info->pretty_output=DEFAULT_PRETTY_OUTPUT;  /* Defaults to Yes  */
     
    logo_info->display_logo_only=0;
    logo_info->display_sysinfo_only=0;
    logo_info->display_usertext=0;
    logo_info->custom_format=0;
    logo_info->symbol=DEFAULT_SYMBOL;                /* Defaults to '#'  */
    logo_info->symbol_bgnd=DEFAULT_SYMBOL_BGND;      /* Defaults to '#'  */
    logo_info->user_text = NULL; /* Change this and display_usertext to *\
                                 \*        have a default message       */
    logo_info->cpuinfo_file=NULL;
    logo_info->format=NULL;
}


void do_spacing(int spaces,int plain_ascii)
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
	       int alt_char_bgnd,struct linux_logo_info_type *logo_info)
{
    int i,next_is_escape=0;
   
    do_spacing(offset,logo_info->plain_ascii);
    i=0;
    while(1) {
       if (next_is_escape) {
	  if (logo_info->plain_ascii) {
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
	case '%' : if (!logo_info->narrow_logo) {
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

void center_and_print(char *string,int size,int width,
		      struct linux_logo_info_type *logo_info)
{
   int i;
   char temp_string[BUFSIZ];

   i=((width-size)/2);
   do_spacing(i,logo_info->plain_ascii);
   sprintf(temp_string,"^[[1;37;40m%s^[[0m\n",string);
   ansi_print(temp_string,0,0,0,0,logo_info);   
}

void clear_screen(struct linux_logo_info_type *logo_info)
{
   ansi_print("^[[2J^[[0;0H\n",0,0,0,0,logo_info);  
}

void help_message(char *binname, char full)
{
    printf("\nLinux Logo Version %s",VERSION);
    printf(" -- by Vince Weaver (weave@eng.umd.edu)\n");
    printf("   Newest Versions at:\n");
    printf("      http://www.glue.umd.edu/~weave/vmwprod\n");
    printf("      http://metalab.unc.edu/pub/Linux/logos/penguins\n\n");
    if (!full) exit(0);
    printf("Usage:   %s [-a] [-b] [-c] [-d] [-e file] [-f] [-g] [-h] [-kX] "
	   "[-l]\n"
	   "                    [-n] [-o num] [-p] [-rX] [-s] [-t str] [-u] "
	   "[-v] [-w Num]\n"
           "                    [-x] [-y] [-F format]\n",binname);
    printf("         [-a]     -- Display logo as ascii only monochrome\n");
    printf("         [-b]     -- New default Banner Logo!\n");
    printf("         [-c]     -- The Old [classic] linux_logo look\n");
    printf("         [-d]     -- disable \"prettying\" of output\n");
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
    printf("      *  [-y]     -- show load average\n");
    printf("         [-F format] Format output.  See README.\n\n");
    printf(" B=Banner mode only, C=Classic Mode Only  *=Works Only in Linux"
	   "\n\n");
    exit(0); 
}

int my_strcat(char *dest,char *src) {
   
    if (src!=NULL) {
       strcat(dest,src);
       return strlen(src);
    }
    else return 0;
}

int print_sysinfo(int line, char *string,
		  struct linux_logo_info_type *logo_info) {
    int x=0,y=0,len;
    int string_ptr;
   
    len=strlen(logo_info->format);
   
    string_ptr=0;
    string[string_ptr]='\000';

    while (y<line) {
        if (logo_info->format[x]=='\n') y++;
        x++;
        if (x>len) return 1;
    }
    
    while (logo_info->format[x]!='\n') {
       if (logo_info->format[x]!='#') {
	  string[string_ptr]=logo_info->format[x];
	  string_ptr++;
       }
       else {
	  string[string_ptr]='\000';
	  x++;
	  if (x>len) break;	  
	  switch(logo_info->format[x]) {
	   case '#': string_ptr+=my_strcat(string,"#"); break;
	   case 'B': string_ptr+=my_strcat(string,hw_info.bogo_total); break;
	   case 'C': string_ptr+=my_strcat(string,os_info.os_revision); break;
	   case 'E': string_ptr+=my_strcat(string,logo_info->user_text); break;
	   case 'H': string_ptr+=my_strcat(string,os_info.host_name); break;
	   case 'L': string_ptr+=my_strcat(string,os_info.load_average); break;
	   case 'M': string_ptr+=my_strcat(string,hw_info.megahertz); break;
	   case 'N': string_ptr+=my_strcat(string,ordinal[hw_info.num_cpus]); 
	             break;
	   case 'O': string_ptr+=my_strcat(string,os_info.os_name); break;
	   case 'R': string_ptr+=my_strcat(string,hw_info.mem_size); break;
	   case 'S': if (hw_info.num_cpus!=1) 
	                string_ptr+=my_strcat(string,"s"); break;
	   case 'T': string_ptr+=my_strcat(string,hw_info.cpu_type); break; 
	   case 'U': string_ptr+=my_strcat(string,os_info.uptime); break;
	   case 'V': string_ptr+=my_strcat(string,os_info.os_version); break;
	   case 'X': string_ptr+=my_strcat(string,hw_info.cpu_vendor); break;
	   default: printf("\nInvalid format '#%c'\n",logo_info->format[x]);
	  }
       }
       x++;
       if (x>len) break;
    }
    string[string_ptr]='\000';
    return 0;
}


void draw_classic_logo(char **logo, struct linux_logo_info_type *logo_info)
{
    char temp_string[BUFSIZ];
    int i;

    get_os_info(&os_info);
    get_hw_info(&hw_info,logo_info);
    
   /* OK we've got the info ready, let's print it all */
    if (logo_info->wipe_screen) clear_screen(logo_info);
    for(i=0;i<7;i++) {
       ansi_print(logo[i],logo_info->no_periods,logo_info->offset,0,0,
		  logo_info); 
       printf("\n"); 
    }
    
    for(i=7;i<16;i++) {
       ansi_print(logo[i],logo_info->no_periods,logo_info->offset,0,0,
		  logo_info);
       do_spacing(2,logo_info->plain_ascii);
       if (print_sysinfo(i-7,temp_string,logo_info)!=1) {
	  ansi_print("^[[1;37;40m",0,0,0,0,logo_info);
	  ansi_print(temp_string,0,0,0,0,logo_info);
	  ansi_print("^[[0m\n",0,0,0,0,logo_info);
       }
       else printf("\n");
    }
  
    ansi_print("^[[0m^[[255D\n",logo_info->no_periods,0,0,0,logo_info);
    if (logo_info->preserve_xy) 
       ansi_print("^[8",logo_info->no_periods,0,0,0,logo_info);
}



void draw_banner_logo(struct linux_logo_info_type *logo_info)
{
    char temp_string[BUFSIZ];
    int i;
      
    if (logo_info->width<80) logo_info->width=80;

    if (logo_info->wipe_screen) clear_screen(logo_info);
    if (!logo_info->display_sysinfo_only) {
       for(i=0;i<12;i++) {
          do_spacing((logo_info->width-80)/2,logo_info->plain_ascii);
          if (logo_info->plain_ascii) 
	     ansi_print(ascii_banner[i],0,0,logo_info->symbol,' ',logo_info);
          else ansi_print(banner[i],0,0,logo_info->symbol,
			  logo_info->symbol_bgnd,logo_info);
          printf("\n");
       }
       if (!logo_info->display_logo_only) printf("\n");
    }
   
    if (!logo_info->display_logo_only) {
       get_os_info(&os_info);
       get_hw_info(&hw_info,logo_info);
       
       i=0;
       while (print_sysinfo(i,temp_string,logo_info)!=1) {
	  center_and_print(temp_string,strlen(temp_string),logo_info->width,
			   logo_info);
	  i++;
       }
    }
    ansi_print("^[[0m^[[255D",logo_info->no_periods,0,0,0,logo_info);
}


int main(int argc,char **argv)
{
    char *endptr;
    int c,i,x;
    char temp_string[BUFSIZ];
   
    struct linux_logo_info_type logo_info;
   
    setup_info(&logo_info); 
       
    logo_info.cpuinfo_file=strdup("/proc/cpuinfo");
   
    while ((c = getopt (argc, argv,"F:"
			           "a::b::c::de:fghk:lno:pr:st:uvw:xy"))!=-1)
       switch (c) {
	  case 'a': logo_info.plain_ascii=1; break;
	  case 'b': logo_info.banner_mode=1; break;
	  case 'c': logo_info.banner_mode=0; break;
	  case 'd': logo_info.pretty_output=0; break; 
	  case 'e': logo_info.cpuinfo_file=strdup(optarg); break;
	  case 'f': logo_info.wipe_screen=1; break;
	  case 'F': 
	            logo_info.custom_format=1;
	            logo_info.format=strdup(optarg);
	              /* Decode the \n's.  Why do I have to do this? */
	            i=0; x=0;
	            while(i<strlen(logo_info.format)) {
		      if (logo_info.format[i]=='\\') {  
			 switch(logo_info.format[i+1]) {
			  case 'n': temp_string[x]='\n'; i++; break;
			  default: temp_string[x]='\\'; i++; break; 
			 }
		      }
		      else temp_string[x]=logo_info.format[i];
		      i++; x++;
		    }
	            sprintf(logo_info.format,"%s",temp_string);
	            break;
	  case 'g': logo_info.display_sysinfo_only=1; break;
	  case 'h': help_message(argv[0], 1);
	  case 'k': logo_info.symbol_bgnd=optarg[0]; break;
	  case 'l': logo_info.display_logo_only=1; break;
	  case 'n': logo_info.no_periods=1; break;
	  case 'o': 
	            logo_info.offset=strtol(optarg,&endptr,10);
	            if ( endptr == optarg ) help_message(argv[0], 1);
	            break;
	  case 'p': logo_info.preserve_xy=1; break;
	  case 'r': logo_info.symbol=optarg[0]; break;
	  case 's': logo_info.skip_bogomips=1; break;
	  case 't': 
	            logo_info.display_usertext=1;
	            logo_info.user_text=strdup(optarg);
	            break;
	  case 'u': logo_info.show_uptime=1; break;
	  case 'v': help_message(argv[0], 0);
	  case 'w':
	            logo_info.width=strtol(optarg,&endptr,10);
	            if ( endptr == optarg ) help_message(argv[0], 1);
	            break;
	  case 'x': logo_info.narrow_logo=1; break;
	  case 'y': logo_info.show_load=1; break;
	  case '?': help_message(argv[0], 1);
       }
    if ( argv[optind] != NULL ) help_message(argv[0], 1);

    if (!logo_info.custom_format) {
       if (logo_info.banner_mode) 
	  logo_info.format=strdup(DEFAULT_BANNER_FORMAT);
       else logo_info.format=strdup(DEFAULT_CLASSIC_FORMAT);
    
          /* Maybe not the best way to do this, but I wanted it to *\
          \* be backwards comaptible with old linux_logos          */
       if (logo_info.display_usertext) {
          sprintf(temp_string,"#E\n%s",logo_info.format);
          logo_info.format=strdup(temp_string);
       }
       if (logo_info.show_load) {
	  logo_info.format[strlen(logo_info.format)-3]='\000';
	  sprintf(temp_string,"%s#L\n#H\n",logo_info.format);
	  logo_info.format=strdup(temp_string);
       }
       if (logo_info.show_uptime) {
	  logo_info.format[strlen(logo_info.format)-3]='\000';
	  sprintf(temp_string,"%s#U\n#H\n",logo_info.format);
	  logo_info.format=strdup(temp_string);
       }
    }
   
       /* Start Printing the Design */
    if (logo_info.preserve_xy) ansi_print("^[7",logo_info.no_periods,0,0,0,
					  &logo_info);
   
    if (logo_info.banner_mode) draw_banner_logo(&logo_info);
    else if (logo_info.plain_ascii) draw_classic_logo( (char**)ascii_logo,
						       &logo_info);
    else {
       ansi_print("^[[40m^[[40m\n",0,0,0,0,&logo_info);
       draw_classic_logo( (char**)color_logo,&logo_info);
    }
    if (logo_info.preserve_xy) ansi_print("^[8",0,0,0,0,&logo_info);
   
    return 0;
}
