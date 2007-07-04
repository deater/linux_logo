/*-------------------------------------------------------------------------*\ 
  LINUX LOGO 5.0-beta3 - Creates Nifty Logo With System Info - 4 July 2007 
  
    by Vince Weaver (vince@deater.net, http://www.deater.net/weave ) 
		      
  A program to display text/ansi logos with system information. 
 
  Thanks to many, many people who sent patches in.   
  See the CHANGES file for a list of all those who helped out. 
\*-------------------------------------------------------------------------*/  
  
  
#define VERSION "5.0-beta3"
  
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <locale.h>

#define ESCAPE "\033"

#include "sysinfo.h"
#include "linux_logo.h"
#include "logo_types.h"
#include "load_logo.h"

#if (USE_I18N==1)
#undef __OPTIMIZE__
#include <libintl.h>
#define _(String) gettext((String))
#else
#define _(String) (String)
#endif
  
/* Change the values in the below file to alter default behavior */
#include "defaults.h"

/* Some global variables.  Possibly bad in practice, but it saves a lot *\  
\* of paramater passing, which has caused bugs to develop before.       */  
  
struct logo_info    *logo_info_head = NULL;  
struct logo_info    *logo_info_tail = NULL;  
  
   /* Convert a number into an ordinal string */  
static void strcat_ordinal(int value,char *string) {  
  
     switch(value) {  
       case 0:  strcat(string,_("Zero"));  break;  
       case 1:  strcat(string,_("One"));   break;  
       case 2:  strcat(string,_("Two"));   break;  
       case 3:  strcat(string,_("Three")); break;  
       case 4:  strcat(string,_("Four"));  break;  
       case 5:  strcat(string,_("Five"));  break;  
       case 6:  strcat(string,_("Six"));   break;  
       case 7:  strcat(string,_("Seven")); break;  
       case 8:  strcat(string,_("Eight")); break;  
       case 9:  strcat(string,_("Nine"));  break;  
       default: strcat(string,_("Many"));  break;  
   }  
}  
  
  
  /* setup the default logos, which are always present */  
static void setup_default_logos() {  
   
   struct logo_info *new_logo;  
     
#include "load_logos.h"  
     
}  
  
  
    /* Setup the "settings" structure to some defaults */  
    /* Edit the "defaults.h" file to change these      */  
static void setup_info(struct linux_logo_info_type *settings) {  
      
    settings->width=DEFAULT_WIDTH;                  /* Defaults to 80   */  
    settings->no_periods=DEFAULT_NO_PERIODS;        /* Defaults to None */   
    settings->preserve_xy=DEFAULT_PRESERVE_XY;      /* Defaults to Off  */  
    settings->skip_bogomips=DEFAULT_SKIP_BOGOMIPS;  /* Defaults to No   */  
    settings->offset=DEFAULT_OFFSET;                /* Defaults to 0    */  
    settings->plain_ascii=DEFAULT_PLAIN_ASCII;      /* Defaults to No   */  
    settings->banner_mode=DEFAULT_BANNER_MODE;      /* Defualts to Yes  */  
    settings->wipe_screen=DEFAULT_WIPE_SCREEN;      /* Defaults to No   */  
    settings->show_uptime=DEFAULT_SHOW_UPTIME;      /* Defaults to No   */  
    settings->show_load=DEFAULT_SHOW_LOAD;          /* Defaults to No   */  
    settings->narrow_logo=DEFAULT_NARROW_LOGO;      /* Defaults to No   */  
    settings->pretty_output=DEFAULT_PRETTY_OUTPUT;  /* Defaults to Yes  */  
    set_pretty_printing(DEFAULT_PRETTY_OUTPUT);       
    settings->display_logo_only=0;  
    settings->display_sysinfo_only=0;  
    settings->display_usertext=0;  
    settings->custom_format=0;  
    settings->center_sysinfo=1;  
    strncpy(settings->user_text,"",BUFSIZ);  
    strncpy(settings->format,"",BUFSIZ);     
}  
  
    /* Use ansi codes or spaces to shift the cursor to the right */  
static void shift_right(int shift,int plain_ascii) {  
     
    int i;  
     
    if (shift<=0) return;   
     
    if (plain_ascii) {  
       for(i=0;i<shift;i++) {  
	  putchar(' ');  
       }  
    }  
    else {  
       printf("\033[%iC",shift);  
    }  
}  
  
   /* Prints a string, but stops at a newline or end of string */  
static char *print_line(char *string) {  
  
    char *temp_st=string;  
  
    while ((*temp_st!='\n') && (*temp_st!='\0')) {  
       putchar(*temp_st);  
       temp_st++;  
    }  
    if ((*temp_st)=='\n') {  
       return temp_st+1;  
    }  
    else return NULL;  
}  
  
  
    /* print enough chars to center text of given size */  
static void center(int size,int width,int plain_ascii) {  
     
   int i;  
  
   i=((width-size)/2);  
   shift_right(i,plain_ascii);  
  
}  
  
  
    /* Uses escape codes to clear the screen */  
static void clear_screen(int plain_ascii) {  
     
   if (!plain_ascii) {  
      printf("\033[2J\033[0;0H\n");  
   }  
   else {  
      /* clear the screen some other way? */    
   }  
}  
  
    /* Change uptime seconds into time string */  
static void strcat_uptime(int seconds,char *string) {  
     
    int up_days,up_hrs,up_mins;  
    char temp_string[BUFSIZ];  
     
    up_days=seconds/86400;  
    up_hrs=(seconds-(up_days*86400))/3600;  
    up_mins=(seconds-(up_days*86400)-(up_hrs*3600))/60;  
    
    strcat(string,_("Uptime "));  
    
    if (up_days>0) {  
       sprintf(temp_string,"%d %s ",  
	       up_days,(up_days==1 ? _("day"):_("days")));  
       strcat(string,temp_string);  
    }  
     
    if (up_hrs>0) {  
       sprintf(temp_string,"%d %s ",  
	       up_hrs,(up_hrs==1 ? _("hour"):_("hours")));  
       strcat(string,temp_string);  
    }  
    if (up_mins>0) {  
       sprintf(temp_string,"%d %s",  
	       up_mins,(up_mins==1 ? _("minute"):_("minutes")));  
       strcat(string,temp_string);         
    }      
}  
  
    /* Prints the help */  
static void help_message(char *binname, char full) {  
     
    char temp_version[256];  
    printf("\nLinux Logo Version %s using libsysinfo %s\n",VERSION,  
	   get_sysinfo_version(temp_version));  
    printf("\tby Vince Weaver <vince@deater.net>\n");  
    printf("   Newest Versions at:\n");  
    printf("      http://www.deater.net/weave/vmwprod/linux_logo\n");  
    printf("      http://metalab.unc.edu/pub/Linux/logos/penguins\n\n");  
    if (!full) exit(0);  
    printf("Usage:   %s [-a] [-b] [-c] [-d] [-D file] [-e file] [-f] "  
	   "[-g]\n"  
	   "                    [-h] [-i] [-k] [-l] [-n] [-o num] [-p] [-s] [-t str] "  
	   "[-u] [-v]\n"  
           "                    [-w Num] [-x] [-y] [-F format] "  
	   "[-L num | list | random_xy]\n",binname);  
    printf("         [-a]     -- Display an ascii-only Logo\n");  
    printf("         [-b]     -- Display a Banner Logo!\n");  
    printf("         [-c]     -- Display a \"Classic\" type logo\n");  
    printf("         [-d]     -- disable \"prettying\" of output\n");  
    printf("         [-D file]-- use custom logo from \"file\"\n");   
    printf("         [-e file]-- Use \"file\" instead of /proc/cpuinfo [for "  
	   "debugging]\n");  
    printf("         [-f]     -- force the screen clear before drawing\n");  
    printf("         [-F format] Format output.  See README.\n");  
    printf("      B  [-g]     -- give system info only\n");  
    printf("         [-h]     -- this help screen\n");     
    printf("         [-i]     -- ignore ~/.linux_logo and /etc/linux_logo.conf\n");  
    printf("         [-k]     -- keep sysinfo flushed-left (non-centered)\n");  
    printf("      B  [-l]     -- display logo only\n");  
    printf("      C  [-o Num] -- offset output Num spaces to the right\n");  
    printf("         [-p]     -- preserve cursor location\n");  
    printf("         [-s]     -- skip Bogomips [speeds up on non-Linux "  
           "platforms]\n");  
    printf("         [-t str] -- display user-supplied string\n");  
    printf("      *  [-u]     -- show uptime\n");  
    printf("         [-v]     -- version information\n");  
    printf("         [-w Num] -- set width of screen to Num [default 80]\n");  
    printf("      *  [-y]     -- show load average\n");  
    printf("         [-L num | list | random_xy] -- multiple Logo options.  See README\n\n");  
    printf(" B=Banner mode only, C=Classic Mode Only  *=Works Only in Linux"  
	   "\n\n");  
}  
  
  
    /* The nifty customizable sysinfo parser */  
static int generate_sysinfo(  
		  char ***string,  
		  struct linux_logo_info_type *settings) {  
     
    struct os_info_type os_info;  
    struct cpu_info_type cpu_info;  
     
    int x=0,len,line=0;  
    char temp_string[BUFSIZ],hostname[BUFSIZ],domain[BUFSIZ];  
    char temp_line[BUFSIZ];  
    float load_1,load_5,load_15;  
    char **temp_sysinfo;  
    int allocated_lines=10;  
     
       /* Get the system info */  
    get_os_info(&os_info);  
    get_cpu_info(&cpu_info);  
         
    len=strlen(settings->format);  
        
       /* allocate 10 lines to start with */  
    temp_sysinfo=calloc(allocated_lines,sizeof(char *));  
  
    while (x<len) {  
     
          /* clear out our line */  
       strcpy(temp_line,"");  
         
          /* handle a line */  
       while (settings->format[x]!='\n') {  
         
             /* If not a #, then just print the character */  
          if (settings->format[x]!='#') {  
	     strncat(temp_line,(settings->format)+x,1);  
          }  
          else {	    
	     x++;  
	     if (x>len) break;	    
	     switch(settings->format[x]) {  
	           /* double # just prints a # */  
	        case '#': strcat(temp_line,"#"); break;  
	           /* #B prints the bogomips */  
	           /* If really high, don't print fractional part */  
	        case 'B': if (cpu_info.bogomips<10000.0) {  
	                     sprintf(temp_string,"%.2f",cpu_info.bogomips);  
	                  }  
	                  else {  
			     sprintf(temp_string,"%.0f",cpu_info.bogomips);  
		          }  
	                  strcat(temp_line,temp_string);   
	                  break;  
	           /* #C prints OS revision */  
	        case 'C': strcat(temp_line,os_info.os_revision); break;  
	           /* #E prints user text */  
	        case 'E': strcat(temp_line,settings->user_text); break;  
	           /* #H prints hostname */  
	        case 'H': strcat(temp_line,get_host_name(hostname,domain)); break;  
	           /* #L prints load average */  
	        case 'L': get_load_average(&load_1,&load_5,&load_15);  
	                  sprintf(temp_string,"Load Average %.2f, %.2f, %.2f",load_1,load_5,load_15);  
	                  strcat(temp_line,temp_string);   
	                  break;  
	           /* #M prints Megahertz */  
	        case 'M': if (cpu_info.megahertz>0.0) {  
	                     if (cpu_info.megahertz>999.0) {  
			        sprintf(temp_string,"%.3gGHz",cpu_info.megahertz/1000.0);  
			     }  
	                     else {  
			        sprintf(temp_string,"%.3gMHz",cpu_info.megahertz);  
			     }  
	                     strcat(temp_line,temp_string);   
		          }  
	                  else {  
			     /* No megahertz.  Back up the pointer.  This is a hack */  
			     /* it avoids having extra spaces in the output         */  
			     if (strlen(temp_line)>0) {  
				temp_line[strlen(temp_line)-1]=0;  
			     }  
			  }  
	                  break;  
	           /* #N prints number of CPUs                */  
	           /* Spelled out if it is less than 10       */  
	           /*  The actual number if less than 100,000 */  
	           /*  The word "Many" if more than 100,000   */  
	        case 'N': if (cpu_info.num_cpus<=9) {  
	                     strcat_ordinal(cpu_info.num_cpus,temp_line);  
	                  }  
	                  else if (cpu_info.num_cpus<=99999) {  
			     sprintf(temp_string,"%d",cpu_info.num_cpus);  
			     strcat(temp_line,temp_string);  
		          }  
	                  else {  
			     strcat_ordinal(10,temp_line);  
		          }  
	                  break;  
	           /* #O prints the OS name */  
	        case 'O': strcat(temp_line,os_info.os_name); break;  
	           /* #P prints "Processor" or its plural.  Sort of a hack */  
	           /*   This probably doesn't work with all languages      */  
	        case 'P': if (cpu_info.num_cpus!=1) {  
	                     strcat(temp_line,_(PROCESSOR_PLURAL));  
	                  }  
	                  else {  
			     strcat(temp_line,_(PROCESSOR_SINGULAR));  
			  }  
	                  break;  
	           /* #R prints the amount of memory */  
	      case 'R': if (get_mem_size()>1023*1024) {  
	                   sprintf(temp_string,"%.2gTB",  
				   ((double)get_mem_size())/(1024.0*1024.0));  
	                }  
	                else if (get_mem_size()>1023) {  
	                   sprintf(temp_string,"%.2gGB",  
				 ((double)get_mem_size()/1024.0));  
	                } else {  
			   sprintf(temp_string,"%ldM",get_mem_size());  
		        }  
	                strcat(temp_line,temp_string);   
                        break;  
	           /* #S prints "s" if CPU's are more than 1.  DEPRECATED */  
	        case 'S': if (cpu_info.num_cpus!=1) {  
	                     strcat(temp_line,"s");   
		          }  
	                  break;  
	           /* #T prints the CPU type */  
	        case 'T': strcat(temp_line,cpu_info.chip_type); break;   
	           /* #U prints the uptime */  
	        case 'U': strcat_uptime(get_uptime(),temp_line); break;  
	           /* #V prints the OS version */  
	        case 'V': strcat(temp_line,os_info.os_version); break;  
	           /* #X Prints the CPU vendor */  
	        case 'X': if (!strncmp(cpu_info.chip_vendor,"Unknown",7)) {   
		   	     if (strlen(temp_line)>0) {  
				temp_line[strlen(temp_line)-1]=0;  
			     }  
	                  }  
	                  else {  
	                     strcat(temp_line,cpu_info.chip_vendor);   
		          }  
		          break;  
	        default: fprintf(stderr,"\nInvalid format '#%c'\n",  
				 settings->format[x]);  
	     }  
	       
	  }  
       	  x++;  
          if (x>=len) break;  
       }  
  
       temp_sysinfo[line]=strdup(temp_line);  
       line++;  
         
          /* If we are using more than the pre-allocated amount */  
          /* allocate more lines                                */  
       if (line>=allocated_lines) {  
	  temp_sysinfo=realloc(temp_sysinfo,sizeof(char *)*(line+1));  
	  allocated_lines=line;  
       }  
         
       /* skip the newline */  
       x++;  
    }  
    *string=temp_sysinfo;  
    return line;  
}  
  
  
    /* The Main Drawing Routine */  
static void draw_logo(struct logo_info *logo_override,  
	       struct linux_logo_info_type *settings) {  
     
    struct logo_info *our_logo_info;  
    char *string_point;  
    int i,ysize,sysinfo_length=0;  
    char **sysinfo_string;  
     
    sysinfo_length=generate_sysinfo(&sysinfo_string,settings);  
     
    if (logo_info_head==NULL) {      
       printf("\n\nNo logos available!!\n");  
       return;  
    }  
     
    if (logo_override!=NULL) {  
       our_logo_info=logo_override;  
    }  
     
    else { /* Search for a proper logo! */  
     
       our_logo_info=logo_info_head;  
       while (1) {  
          if (our_logo_info==NULL) {  
	     printf("\nAppropriate logo not found!\n\n");  
	     return;  
          }  
          if ((settings->banner_mode) && (our_logo_info->sysinfo_position==SYSINFO_BOTTOM)) {  
	     if (settings->plain_ascii) {   
	        if (our_logo_info->ascii_logo!=NULL) {  
	           break;  
		}  
	     }  
	     else { /* Not ascii */  
		if (our_logo_info->logo!=NULL) {  
		   break;  
		}  
	     }  
          }  
          if ((!settings->banner_mode) && (our_logo_info->sysinfo_position==SYSINFO_RIGHT)) { /* Classic mode */  
             if (settings->plain_ascii) {  
		if (our_logo_info->ascii_logo!=NULL) {  
	           break;  
		}  
	     } else { /* Not ascii */  
	       if (our_logo_info->logo!=NULL) {  
		  break;  
	       }  
	     }  
	  }  
	  our_logo_info=our_logo_info->next_logo;  
       }  
    }  
     
    if (settings->width<80) settings->width=80;  /* Right now we don't */  
                                                 /* handle width < 80  */  
  
    if (settings->wipe_screen) clear_screen(settings->plain_ascii);  
    
       /* Select the proper logo */  
    if (settings->plain_ascii) {  
       ysize=our_logo_info->ascii_ysize;  
       if (our_logo_info->ascii_logo==NULL) {  
	  printf("\nSpecified logo has no ascii version!\n\n");  
	  return;  
       }  
       else string_point=our_logo_info->ascii_logo;  
    }  
    else {  
       ysize=our_logo_info->ysize;  
       if (our_logo_info->logo==NULL) {  
	  printf("\nSpecified logo has no non-ascii version!\n\n");  
	  return;  
       }  
       else string_point=our_logo_info->logo;  
    }  
     
       /* Draw the logos */  
    if (!settings->display_sysinfo_only) {  
       if (settings->banner_mode) { /* Banner mode */  
          for(i=0;i<ysize;i++) {  
             shift_right((settings->width-80)/2,settings->plain_ascii);  
	     string_point=print_line(string_point);  
             printf("\n");  
	  }  
          if (!settings->display_logo_only) printf("\n");  
       }  
       else {  /* Classic mode logo */  
         
             /* Print the first 7 lines of logo */  
            
	  if (ysize<7) {  
	     printf("\nError! Classic logos must be at least 7 lines long!\n");  
	  }  
	    
	  if (!settings->plain_ascii) printf("\033[40m\033[40m\n");  
    
	  for(i=0;i<7;i++) {  
             shift_right(settings->offset,settings->plain_ascii);  
             string_point=print_line(string_point);  
	     printf("\n");   
	  }  
  
             /* The next lines can have cpuinfo after them */  
          for(i=7;i<ysize;i++) {  
             shift_right(settings->offset,settings->plain_ascii);  
             string_point=print_line(string_point);  
             shift_right(2,settings->plain_ascii);  
             if (i-7<sysinfo_length) {  
	        if (!settings->plain_ascii) printf("\033[1;37;40m");  
	        printf("%s",sysinfo_string[i-7]);  
	        if (!settings->plain_ascii) printf("\033[0m");  
	     }  
             printf("\n");  
	  }  
       }  
    }  
        
       /* Print the sysinfo if we haven't already */  
    if (!settings->display_logo_only) {  
       if (settings->banner_mode) {  
	  for(i=0;i<sysinfo_length;i++) {  
	       
	     if (settings->center_sysinfo) {  
		center(strlen(sysinfo_string[i]),settings->width,  
		       settings->plain_ascii);  
	     }  
	     	       
             if (!settings->plain_ascii) printf("\033[1;37;40m");  
             printf("%s",sysinfo_string[i]);  
             if (!settings->plain_ascii) printf("\033[0m");  
             printf("\n");  
	  }  
	  printf("\n");  
       }  
    }  
       
       /* Restore cursor color to normal */  
    if (!settings->plain_ascii)   
       printf("\033[0m\033[255D");  
}  
  
  
static char *get_arg(int *index,int argc,char **argv) {  
  
   char *argument;  
     
   if (argv[*index][2]=='\0') {  
      (*index)++;  
      if (*index>=argc) {  
	 argument=NULL;  
	 printf("Missing parameter to option -%c\n\n",argv[(*index)-1][1]);  
	 exit(1);  
      }  
      else {  
         argument=argv[*index];  
      }  
   }  
   else {  
      argument=&(argv[*index][2]);  
   }  
     
   return argument;  
     
}  
  
  
    /*******************************************************/  
    /* Parse options (from file or command line arguments) */  
    /*******************************************************/  
static void parse_command_line(struct linux_logo_info_type *settings,  
			int argc, char **argv) {  
  
    int i,x,string_size;  
    char *endptr,*argument;  
    int index=1,option;  
  
    if (argc<2) return;  
     
    while (1) {  
         
       if (argv[index][0]!='-') {  
	  printf("Unknown parameter %s\n",argv[index]);  
	  exit(1);  
       }  
       option=argv[index][1];  
  
         
       switch (option) {  
	  case 'a': settings->plain_ascii=1;   
	            break;  
	  case 'b': settings->banner_mode=1;   
	            break;  
	  case 'c': settings->banner_mode=0;   
	            break;  
	  case 'd': settings->pretty_output=0;   
	            set_pretty_printing(0);  
	            break;   
	            /* FIXME */  
	  case 'D': argument=get_arg(&index,argc,argv);  
	            logo_info_tail=load_logo_from_disk(argument);  
	            break;  
	  case 'e': argument=get_arg(&index,argc,argv);  
	            set_cpuinfo_file(argument);   
	            break;  
	  case 'f': settings->wipe_screen=1;   
	            break;  
	  case 'F': argument=get_arg(&index,argc,argv);  
	            string_size=strlen(argument);  
	            if (string_size==0) break;  
	       
	            settings->custom_format=1;  
	            strncpy(settings->format,argument,BUFSIZ);  
	              	    
	            /* Decode the \n's. */  
	            /* Should I decode other \ codes? */  
	            /* also, \ codes are always smaller, hence why */  
	            /* we can overwrite our own buffer */  
	       
	            i=0; x=0;  
	            while(i<strlen(settings->format)) {  
		       if (settings->format[i]=='\\') {    
			  switch(settings->format[i+1]) {  
			     case 'n': settings->format[x]='\n'; i++; break;  
			     default:  settings->format[x]='\\'; break;   
			  }  
		       }  
		       else {  
			  settings->format[x]=settings->format[i];  
		       }  
		       i++; x++;  
		    }  
	            settings->format[x]='\0';  
	            break;  
	  case 'g': settings->display_sysinfo_only=1;   
	            break;  
	  case 'h':   
	  case '?': help_message(argv[0], 1);   
	            exit(0);  
	            break;  
	            /* FIXME */  
	  case 'i': /* is it even possible to do this right? */  
	            /* we'd have to do two passes, possibly  */  
	            /* resetting to defaults in between      */  
	            /*ignore_config_file=1; */  
	            break;  
	  case 'k': settings->center_sysinfo=0;  
	            break;  
	  case 'l': settings->display_logo_only=1;   
	            break;  
  
	            /* FIXME */  
  
	  case 'L': argument=get_arg(&index,argc,argv);  
	              
#if 0  
	     	       /* Reset values in case we get this after  
			* reading the file */  
		    logo_num = 1; logo_override = 0; random_logo = 0;  
	            logo_num=strtol(argument,&endptr,10);  
	            if ( endptr == argument ) {  
		       temp_st=strdup(argument);  
		       if (!strncmp(temp_st,"list",4)) {  
			  do_listing=1;  
		       }  
		       else if (!strncmp(temp_st,"random",6)) {  
			  random_logo=1;  
			  random_type=temp_st[7];  
			  random_type2=temp_st[8];  
		       }  
		       else {  
			  printf("\nUnknown -L directive!\n\n");  
			  return 4;  
		       }  
		    }  
	            else { /* It's a number */  
		       logo_override=1;  
		    }  
#endif	  	    
	            break;  
  
	  case 'n': settings->no_periods=1;   
	            break;  
	  case 'o': argument=get_arg(&index,argc,argv);  
	            settings->offset=strtol(argument,&endptr,10);  
	            if ( endptr == argument ) {  
		       printf("Invalid offset length %s\n\n",argument);  
		       exit(1);  
		    }  
	            break;  
	  case 'p': settings->preserve_xy=1;   
	            break;  
	  case 's': settings->skip_bogomips=1;   
	            break;  
	  case 't': argument=get_arg(&index,argc,argv);  
	            settings->display_usertext=1;  
	            strncpy(settings->user_text,argument,BUFSIZ);  
	            break;  
	  case 'u': settings->show_uptime=1;   
	            break;  
	  case 'v': help_message(argv[0], 0);  
	            exit(0);  
	            break;  
	  case 'w': argument=get_arg(&index,argc,argv);  
	            settings->width=strtol(argument,&endptr,10);  
	            if ( endptr == argument ) {  
		       printf("Invalid width %s\n\n",argument);  
		       exit(1);  
		    }  
	            break;  
	  case 'y': settings->show_load=1;   
	            break;  
	  default:  printf("Unknown option -%c\n",option);  
	            exit(1);  
       }  
  
       index++;  
       if (index>=argc) break;  
    }  
}  
  
  
       /* Look for ~/.linux_logo or /etc/linux_logo config files */  
void read_config_file(struct linux_logo_info_type *settings) {     
  
    int string_size,valid_string=0,i,size=0,counter=0;  
    int fake_data_offset=0,ch,oldch,in_quote=0,fake_argc=0;  
    char *tempst,*fake_data,**fake_argv;  
    FILE *config_file=NULL;  
    char config_string[BUFSIZ];  
     
       /* Check for the files */  
     
       /* look for ~/.linux_logo */  
    if (getenv("HOME")) {  
       string_size=strlen(getenv("HOME"));  
       tempst=calloc(strlen("/.linux_logo")+string_size+1,sizeof(char));  
       strncpy(tempst,getenv("HOME"),string_size);  
       strncat(tempst,"/.linux_logo",strlen("/.linux_logo"));  
       config_file=fopen(tempst,"r");  
       free(tempst);  /* free the calloc's! */  
    }  
  
       /* if no ~/.linux_logo, check for /etc/linux_logo.conf */  
    if (config_file==NULL) {  
       config_file=fopen("/etc/linux_logo.conf","r");  
    }  
     
       /* If no config files, just make do with command-line arguments */  
       /* Note to Vince of 2000.. this is the most horrible     */  
       /* atrocity of code ever.  -- Vince of 2006              */  
     
    if (config_file!=NULL) {  
  
       /* skip over whitespace and comments */         
       while(!valid_string) {  
          fgets(config_string,BUFSIZ,config_file);  
	  for(i=0;i<strlen(config_string);i++) {  
	     if (config_string[i]=='#') break;  /* a comment */  
	     else if (config_string[i]=='\n') break; /* empty line */  
	     else if (config_string[i]==' ' || config_string[i]=='\t'); /* whitespace */  
	     else {  
		valid_string=1;  
		break;  
	     }  
	  }  
	  if (feof(config_file)) break;  
       }  
         
         
       if (valid_string) {  
	    
          size=strlen(config_string);  
  
	  config_string[size-1]='\0';  /* get rid of trailing \n */  
  
             /* create room for the fake command-line */  
          fake_data=calloc(size+12,sizeof(char));  
             /* stick "linux_logo" as argv[0] */  
          strncpy(fake_data,"linux_logo ",11);  
	  fake_data_offset=11;  
          strncat(fake_data,config_string,size);  
	  	    
	  ch=' ';  
	  for(i=0;i<size;i++) {  
	       
	     oldch=ch;  
	     ch=config_string[i]; /* start after linux_logo */  
	       
                /* if after a space, and not in a quote, begin a new token */  
             if ((oldch==' ') && (!in_quote) && (ch!=' ')) {  
	        if (fake_data_offset>0) fake_data[fake_data_offset]='\0';  
	        fake_data_offset++;  
	        fake_argc++;  
             }  
            
                /* The shell strips quotes and excess whitespace */  
                /* thought for now we just strip spaces outside of quotations */  
             if ((ch!='\"') && ((ch!=' ') || ((ch==' ') && (in_quote)))) {  
                fake_data[fake_data_offset]=ch;  
                fake_data_offset++;  
             }  
     
                /* start/end a quotation */  
             if (ch=='\"') in_quote= !in_quote;  
  
          }  
         
          fclose(config_file);  
     
          fake_argc+=1; /*  plus the last one */  
     
          /* Allocate room for the fake argv[] list of pointers */  
          /* +1 cause NULL at the end */  
          fake_argv=calloc(fake_argc+1,sizeof(char *));  
     
          fake_argv[0]=fake_data;  
          counter=1;  
          /* Actually find the tokens based on the NULLs put before */  
          for(i=0;i<fake_data_offset;i++) {  
             if (fake_data[i]=='\0') {  
	        if (counter<fake_argc) {  
	           fake_argv[counter]=fake_data+i+1;  
	        }  
	        counter++;  
             }  
          }  
  
	  parse_command_line(settings,fake_argc,fake_argv);  
       }  
  
    }  
}  
  
  
  
  
  
int main(int argc,char **argv) {  
     
  
    int i,logo_num=1,random_logo=0,do_listing=0,logo_override=0;  
    int logo_found;  
    char temp_string[BUFSIZ],*temp_pointer,random_type='e',random_type2='e';  
    struct linux_logo_info_type settings;  
    struct logo_info *temp_logo,*custom_logo=NULL;  
    struct timeval time_time;  
     
#if (USE_I18N==1)  
       /* i18n */  
    setlocale(LC_ALL, "");  
    bindtextdomain("linux_logo", LOCALE_DIR);  
    textdomain("linux_logo");  
#endif     
       /* Set some defaults */  
    setup_info(&settings);   
  
    read_config_file(&settings);  
      
    parse_command_line(&settings,argc,argv);  
      
       /*******************************************************/  
       /* DONE WITH ALL THE STUPID OPTION PARSING             */  
       /* now actually do things                              */  
       /*******************************************************/  
         
    setup_default_logos();  
     
    if (do_listing) {  
       printf("\nAvailable Built-in Logos:\n");  
       printf("\tNum\tType\tAscii\tDescription\n");  
  
       temp_logo=logo_info_head;  
       i=1;  
       while (temp_logo!=NULL) {  
	   printf("\t%d",i);  
	   if (temp_logo->sysinfo_position) printf("\tBanner");  
	   else printf("\tClassic");  
	   if (temp_logo->ascii_logo!=NULL) printf("\tYes");  
	   else printf("\tNo");  
	   printf("\t%s\n",temp_logo->description);  
	   temp_logo=temp_logo->next_logo;  
	   i++;  
       }  
       printf("\nDo \"linux_logo -L num\" where num is from "  
	      "above to get the appropriate logo.\n");  
       printf("Remember to also use -a to get ascii version.\n\n");  
       return 0;    
    }  
     
    if (random_logo) {  
       gettimeofday(&time_time,NULL);  
       srand(time_time.tv_usec);   /* Not really random, but... */  
       i=rand()%1024;  /* Hopefully we have less than 1024 logos */  
       custom_logo=logo_info_head;  
       while(i) {  
	  if (custom_logo->next_logo==NULL) custom_logo=logo_info_head;  
	  else custom_logo=custom_logo->next_logo;  
	  i--;    
       }  
       i=0;  
       while (i<2) {  
	  logo_found=1;  
	  if (random_type=='b') { /* Want banner mode */  
	     if (!custom_logo->sysinfo_position) logo_found=0;  
	     else settings.banner_mode=1;  
	  }    
	  if (random_type=='c') { /* Want classic mode */  
	     if (custom_logo->sysinfo_position) logo_found=0;  
	     else settings.banner_mode=0;  
	  }  
	  if (random_type=='e') { /* Want either */  
	     /* we should be OK */  
	  }    
  
	  if (random_type2=='e') { /* Any logo at all */  
	     settings.plain_ascii=rand()%2;  
	     if (custom_logo->ascii_logo==NULL)   
	        settings.plain_ascii=!settings.plain_ascii;  
	  }    
	  if (random_type2=='a') { /* Want Ascii */  
	     if (custom_logo->ascii_logo==NULL) logo_found=0;  
	     else settings.plain_ascii=1;  
	  }  
	    
	  if (random_type2=='n') { /* Want non-ascii */  
	     if (custom_logo->logo==NULL) logo_found=0;  
	     else settings.plain_ascii=0;  
	     break;  
	  }  
	    
	  if (logo_found) break;  
	    
	  if (custom_logo->next_logo==NULL) {  
	     custom_logo=logo_info_head;  
	     i++;  
	  }  
	  else custom_logo=custom_logo->next_logo;  
       }  
       if (i>1) {  
	  printf("\nSorry, can't generate random logo of that type.\n\n");  
	  return 3;  
       }  
    }  
     
    if (logo_override) {  
       custom_logo=logo_info_head;  
       i=1;  
       while (i<logo_num) {  
	   if (custom_logo->next_logo==NULL) {  
	      fprintf(stderr,"\nError!  Logo %d is invalid!  \"linux_logo -L list\""  
		     " lists valid logo numbers.\n\n",logo_num);  
	      custom_logo=logo_info_head;  
	      break;  
	   }  
	   custom_logo=custom_logo->next_logo;  
	   i++;  
       }  
    }  
     
       /* We have to keep these consistent or funny things happen */  
    if (custom_logo!=NULL) {  
       settings.banner_mode=custom_logo->sysinfo_position;  
    }  
     
/* VMW - done audit below */     
  
       /* Prepare the sysinfo stuff if not done for us   */  
       /* Handle "normal" output by basically faking the */  
       /* appropriate "custom" output string             */  
    if (!settings.custom_format) {  
         
          /* Set the format for banner or classic mode */  
       if (settings.banner_mode) {  
	  strncpy(settings.format,_(DEFAULT_BANNER_FORMAT),BUFSIZ);  
       }  
       else {  
	  strncpy(settings.format,DEFAULT_CLASSIC_FORMAT,BUFSIZ);  
       }  
         
          /* If usertext specified, add it at beginning */  
       if (settings.display_usertext) {  
	  strncpy(temp_string,settings.format,BUFSIZ);  
          strncpy(settings.format,"#E\n",BUFSIZ);  
	  strncat(settings.format,temp_string,BUFSIZ);  
       }  
         
          /* If want system load, add it second-to-last */  
       if (settings.show_load) {  
	    
	  /* put it before the hostname */  
	  temp_pointer=strstr(settings.format,"#H");  
	  /* If for some reason not found, put it at end */  
	  if (temp_pointer==NULL) {	      
	     strncat(settings.format,"#L\n",BUFSIZ);  
	  }  
	  else {  
	     strncpy(temp_pointer,"#L\n#H\n",BUFSIZ);  
	  }  
       }  
         
          /* if want uptime, add it second_to_last */  
       if (settings.show_uptime) {  
  
	  /* put it before the hostname */  
	  temp_pointer=strstr(settings.format,"#H");  
	  /* If for some reason not found, put it at end */  
	  if (temp_pointer==NULL) {	      
	     strncat(settings.format,"#U\n",BUFSIZ);  
	  }  
	  else {  
	     strncpy(temp_pointer,"#U\n#H\n",BUFSIZ);  
	  }	    
       }  
    }  
     
       /* Preserve xy if so desired */  
    if ( (settings.preserve_xy) && (!settings.plain_ascii) ) {  
       printf(ESCAPE"7");  
    }  
     
       /* Draw the logo */  
    draw_logo(custom_logo,&settings);  
           
       /* Restore xy if we saved it */  
    if ( (settings.preserve_xy) && (!settings.plain_ascii) ) {  
       printf(ESCAPE"8");  
    }  
     
    return 0;  
}  
