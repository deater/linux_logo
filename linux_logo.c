/*-----------------------------------------------------------------------------
LINUX LOGO 1.07 - Creates a Nifty Logo With some System Info - 12 February 1998
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
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/utsname.h>

#define ESCAPE '\033'
#define VERSION "1.07"
#define MAX_YSIZE 50

#include "ascii_penguin.h"

#ifdef IRIX_ANSI     /* It's easy to create a custom Ansi */
#include "irix.h"    /* Just make it with an ansi-making tool (i.e. TheDraw) */
#elif LINUX_ANSI     /* it MUST be 16 lines long.  Any width.  */
#include "penguin.h" /* The first 6 lines can be longer */
#elif CUSTOM_ANSI    /* The rest of the lines should be padded to the same */
#include "custom.h"  /* Length so the output is lined up*/
#else                /* Just stick quotation marks around the ansi (as in */
#include "default.h" /*    the included .h files) and it should work */
#endif

#include "getsysinfo.h"

/* Prints an ansi string, changing "^[" to the ESCAPE charachter '\033' *\
\*    And also enables easier parsing of the output                     */
/* The name doesn't indicate any sort of ANSI standard; it is left over *\
\*    from years gone by when I used to make "ansi art" for a WWIV BBS  */
int ansi_print(const char *string,int no_periods,int offset)
{
   int i;
   for(i=0;i<offset;i++) putchar(' ');
   i=0;
   while(1) {
      switch(string[i]){
       case '\0': return 0;
       case '^' : if (string[i+1]=='[') {putchar('\033'); i++;} break;
       case '\\': if (string[i+1]=='n') {printf("\n"); i++;} 
       case '.' : if (no_periods) putchar(' '); else putchar(string[i]); break;
       default  : putchar(string[i]);
      }
      i++;
   } 
   return 0;
}

void help_message(char *binname, char full)
{
   printf("\nLinux Logo Version %s",VERSION);
   printf(" -- by Vince Weaver (weave@eng.umd.edu)\n");
   printf("   Newest Versions at:\n");
   printf("      http://www.glue.umd.edu/~weave/vmwprod\n");
   printf("      http://sunsite.unc.edu/pub/Linux/logos/penguins\n\n");
   if (!full) exit(0);
   printf("Usage:   %s [-a] [-n] [-p] [-o Num] [-s] [-v] [-h]\n",binname);
   printf("         [-a] -- ascii only monochrome penguin\n");
   printf("         [-n] -- toggle periods off [may make output look better/cleaner]\n");
   printf("         [-p] -- preserve cursor location\n");
   printf("         [-o Num] offset output by Num spaces to the right\n");
   printf("         [-s] -- skip Bogomips [speeds up display on non-Linux platforms]\n");
   printf("         [-v] -- version information\n");
   printf("         [-h] -- this help screen\n\n");
   exit(0); 
 }



int main(int argc,char **argv)
{
   char os_name[65],host_name[65],os_version[65],os_revision[65];
   char cpu_info[65],*endptr;
   char ch,temp_string[100];
   int i,no_periods=0,preserve_xy=0,skip_bogomips=0,offset=0,plain_ascii=0;
   char bogo_total[65];
   char **logo;

   
/* Parse Command Line Arguments */
   for (i=1;i<argc;i++) {
       /* they all start with a '-' */
       if (argv[i][0]=='-') { 
  	  if (argv[i][1]=='-') ch=argv[i][2];
	  else ch=argv[i][1];
      
	  switch(ch){
	   case 'v': case 'V': help_message(argv[0], 0);
	   case 'h': case 'H': case '?': help_message(argv[0], 1);
	   case 'a': case 'A': plain_ascii=1; break;
	   case 'n': case 'N': no_periods=1; break;
	   case 'p': case 'P': preserve_xy=1; break;
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
	    default: break;
	  }
     }
   }

   /* Start Printing the Design */

   if (plain_ascii) 
      logo=(char**)ascii_logo;
   else
      logo=(char**)color_logo;
     
   if (preserve_xy) ansi_print("^[7",no_periods,0);
   ansi_print("^[[40m^[[40m\n",no_periods,0);

   for(i=0;i<7;i++) {
      ansi_print(logo[i],no_periods,offset); 
      printf("\n"); 
   }
   
    ansi_print(logo[7],no_periods,offset);

      /* Get some OS info using some external functions in getsysinfo.c */
    get_os_info((char *)&os_name,(char *)&os_version,(char *)&os_revision,(char *)&host_name);
    
    sprintf(temp_string,"^[[2C^[[1;37;40m%s Version %s^[[0m\n",os_name,os_version);
    ansi_print(temp_string,0,0);
    ansi_print(logo[8],no_periods,offset);
    sprintf(temp_string,"^[[2C^[[1;37;40m%s^[[0m\n",os_revision);
    ansi_print(temp_string,0,0);
   
      /* Get some hardware info using getsysinfo.c */
     get_hardware_info((char *)&cpu_info,(char *)&bogo_total,skip_bogomips);
     
     ansi_print(logo[9],no_periods,offset);
     sprintf(temp_string,"^[[2C^[[1;37;40m%s^[[0m\n",cpu_info);
     ansi_print(temp_string,0,0);
   
     ansi_print(logo[10],no_periods,offset);
     sprintf(temp_string,"^[[2C^[[1;37;40m%s^[[0m\n",bogo_total);
     ansi_print(temp_string,0,0);
       
     /* Print Host Name */
   ansi_print(logo[11],no_periods,offset);
   sprintf(temp_string,"^[[2C^[[1;37;40m%s^[[0m\n",host_name);
   ansi_print(temp_string,0,0);
	    
   for(i=12;i<16;i++) { ansi_print(logo[i],no_periods,offset); printf("\n"); 
   }
  
   ansi_print("^[[0m^[[255D\n",no_periods,0);
   if(preserve_xy) ansi_print("^[8",no_periods,0);
    
    return 0;
}
