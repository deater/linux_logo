/*-------------------------------------------------------------------------*\
  LINUX LOGO 4.06 - Creates Nifty Logo With System Info - 13 November 2002
 
    by Vince Weaver (vince@deater.net, http://www.deater.net/weave )
		     
  A program to display text/ansi logos with system information.

  Thanks to many, many people who sent patches in.  See the CHANGES file for
     a list of all those who helped out.
 
\*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <locale.h>

#include "i18n.h"

#define ESCAPE '\033'
#define VERSION "4.06"

#include "sysinfo.h"
#include "linux_logo.h"
#include "logo_types.h"
#include "vmw_string.h"

/* Change the values in the below file to alter default behavior */
#include "defaults.h"

    /* See defaults.h when setting this */
#if (USE_I8N==0)
#undef _(string)
#define _(string) string
#endif


#define SAVE_GETOPT  {old_optind=optind; old_opterr=opterr; old_optopt=optopt;}
#define RESET_GETOPT {optind=old_optind; opterr=old_opterr; optopt=old_optopt;}

#define FILE_OPTIONS_PASS          0
#define COMMAND_OPTIONS_PASS       1
#define WANT_DEFAULT_OPTIONS_PASS  2

char *ordinal(int value) {
   static char text[20];
   
   switch(value) {
    case 0: strncpy(text,_("Zero"),19); break;
    case 1: strncpy(text,_("One"),19); break;
    case 2: strncpy(text,_("Two"),19); break;
    case 3: strncpy(text,_("Three"),19); break;
    case 4: strncpy(text,_("Four"),19); break;
    case 5: strncpy(text,_("Five"),19); break;
    case 6: strncpy(text,_("Six"),19); break;
    case 7: strncpy(text,_("Seven"),19); break;
    case 8: strncpy(text,_("Eight"),19); break;
    case 9: strncpy(text,_("Nine"),19); break;
    default: strncpy(text,_("Many"),19);break;
      
   }
   return text;
}


/* Some global variables.  Possibly bad in practice, but it saves a lot *\
\* of paramater passing, which has caused bugs to develop before.       */

struct logo_info    *logo_info_head = NULL;
struct logo_info    *logo_info_tail = NULL;

void draw_banner_logo(struct logo_info *the_logo, struct linux_logo_info_type *settings);

struct logo_info *load_logo_from_disk(char *filename) {

   struct logo_info *new_logo;
   int logo_start=0,ascii_logo_start=0;
   int ascii_size=0,size=0;
   char temp_st[BUFSIZ];
   FILE *fff;
      
   new_logo=calloc(1,sizeof(struct logo_info));

   if (new_logo==NULL) {
      printf("Error allocating memory!\n");
      return NULL;
   }
	
   fff=fopen(filename,"r");
   if (fff==NULL) {
      printf("Error loading logo: %s\n",filename);
      return NULL;
   }
   
   new_logo->logo=NULL;
   new_logo->ascii_logo=NULL;
   
   while (!feof(fff)) {
      read_string_from_disk_exact(fff,temp_st);
      if (!strncmp(temp_st,"END_LOGO",8)) logo_start=0;
      if (!strncmp(temp_st,"END_ASCII_LOGO",14)) ascii_logo_start=0;
      if (logo_start) {
	 size+=strlen(temp_st);
	 if (new_logo->logo==NULL) {
	    (char *)new_logo->logo=strdup(temp_st);
	 }
	 else {
	    new_logo->logo=realloc(new_logo->logo,size+1);
	    strncat( (char *)new_logo->logo,temp_st,strlen(temp_st));
	 }
	 new_logo->ysize++;
      }
      if (ascii_logo_start) {
         ascii_size+=strlen(temp_st);
	 if (new_logo->ascii_logo==NULL) {
	    (char *)new_logo->ascii_logo=strdup(temp_st);
	 }
	 else {
	    new_logo->ascii_logo=realloc(new_logo->ascii_logo,ascii_size+1);
	    strncat( (char *)new_logo->ascii_logo,temp_st,strlen(temp_st));
	 }
	 new_logo->ascii_ysize++;
      }
      if (!strncmp(temp_st,"BEGIN_ASCII_LOGO",16)) ascii_logo_start=1;
      if (!strncmp(temp_st,"BEGIN_LOGO",10)) logo_start=1;
      if ( (!ascii_logo_start) && (!logo_start) ) {
	 if (!strncmp(temp_st,"SYSINFO_POSITION",16)) {
	    if (!strncmp(temp_st+17,"bottom",6)) {
	       new_logo->sysinfo_position=SYSINFO_BOTTOM;
	    }
	    if (!strncmp(temp_st+17,"right",5)) {
	       new_logo->sysinfo_position=SYSINFO_RIGHT;
	    }
	 }
	 if (!strncmp(temp_st,"DESCRIPTION_STRING",18)) {
	    new_logo->description=strdup(temp_st+19);	   
	 }
      }
   }
   
   new_logo->next_logo=NULL;
   
   logo_info_tail=new_logo;
 
   fclose(fff);
   
   return new_logo;   
}

void free_logos() {
    struct logo_info *logo;
    struct logo_info *prev_logo;
   
    logo=logo_info_head;
   
    while(logo!=NULL) {
       if (logo->description) free(logo->description);
       if (logo->logo) free(logo->logo);
       if (logo->ascii_logo) free(logo->ascii_logo);
       prev_logo=logo;
       logo=logo->next_logo;
       free(prev_logo);
    }
    logo_info_head=NULL;
    logo_info_tail=NULL;
}


void setup_logos() {
 
   struct logo_info *new_logo;
   
#include "load_logos.h"
   
//   logo_info_head=load_logo_from_disk("./logos/banner.logo");   
//   logo_info_tail->next_logo=load_logo_from_disk("./logos/classic.logo");
   
}
    /* Setup the "settings" structure to some defaults */
    /* Edit the "defaults.h" file to change these      */
void setup_info(struct linux_logo_info_type *settings) {
    
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
    settings->user_text = NULL;  /* Change this and display_usertext to *\
                                 \*        have a default message       */
    settings->format=NULL;
}

    /* Use ansi codes or spaces to shift the cursor to the right */
void shift_right(int shift,int plain_ascii)
{
    int i;
   
    if (shift<=0) return; 
   
    if (plain_ascii) for(i=0;i<shift;i++) putchar(' ');
    else printf("%c[%iC",ESCAPE,shift);
}

/* Prints an ansi string, changing "^[" to the ESCAPE charachter '\033' *\
\*    And also enables easier parsing of the output                     */
/* The name doesn't indicate any sort of ANSI standard; it is left over *\
\*    from years gone by when I used to make "ansi art" for a WWIV BBS  */
int ansi_print(const char *string)
{
    int i;
   
    i=0;
    while(1) {
       switch(string[i]){
          case '\0': return 0;
	  case '^' : if (string[i+1]=='[') {putchar(ESCAPE); i++;} break;
          case '\\': if (string[i+1]=='n') {printf("\n"); i++;} 
          default  : putchar(string[i]);
       }
       i++;
    } 
    return 0;
}



char *print_line(char *string)
{

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


    /* print some centered text */
void center_and_print(char *string,int size,int width,
		      struct linux_logo_info_type *settings)
{
   int i;

   if (settings->center_sysinfo) {
      i=((width-size)/2);
      shift_right(i,settings->plain_ascii);
   }
   
      /* Why was I setting the colors here? */
   if (!settings->plain_ascii) ansi_print("^[[1;37;40m");
   printf("%s",string);
   if (!settings->plain_ascii) ansi_print("^[[0m");
   printf("\n");
}

    /* Uses escape codes to clear the screen */
void clear_screen(struct linux_logo_info_type *settings)
{
   if (!settings->plain_ascii) ansi_print("^[[2J^[[0;0H\n");  
}

    /* Change uptime seconds into time string */
char *uptime_string(int seconds) {
   
    int up_days,up_hrs,up_mins;
    char temp_string[BUFSIZ];
   
    up_days=seconds/86400;
    up_hrs=(seconds-(up_days*86400))/3600;
    up_mins=(seconds-(up_days*86400)-(up_hrs*3600))/60;
  
    if (up_days<=0) 
       sprintf(temp_string,"Uptime %d %s %d %s",
	       up_hrs,(up_hrs==1 ? "hour":"hours"),
	       up_mins,(up_mins==1 ? "minute":"minutes"));
    else 
       sprintf(temp_string,"Uptime %d %s %d %s %d %s",
	       up_days,(up_days==1 ? "day":"days"),
	       up_hrs,(up_hrs==1 ? "hour":"hours"),
	       up_mins,(up_mins==1 ? "minute":"minutes"));
    return strdup(temp_string);
}

    /* Prints the help */
void help_message(char *binname, char full)
{
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
	   "debugging\n");
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
    printf("         [-L ...] -- multiple Logo options.  See README\n\n");
    printf(" B=Banner mode only, C=Classic Mode Only  *=Works Only in Linux"
	   "\n\n");
}

    /* The nifty customizable sysinfo parser */
int print_sysinfo(int line, char *string,
		  struct linux_logo_info_type *settings) {
   
    static os_info_t os_info;
    static cpu_info_t cpu_info;
   
    static int sysinfo_already_run=0; 
   
    int x=0,y=0,len;
    int string_ptr;
    char temp_string[255],hostname[65],domain[65];
    float load_1,load_5,load_15;
   
       /* We only need to fetch the system info once */
    if (!sysinfo_already_run) {
       get_os_info(&os_info);
       get_cpu_info(&cpu_info);
       sysinfo_already_run=1;
    }
       
    len=strlen(settings->format);
   
    string_ptr=0;
    string[string_ptr]='\000';

    while (y<line) {
        if (settings->format[x]=='\n') y++;
        x++;
        if (x>len) return 1;
    }
    
    while (settings->format[x]!='\n') {
       if (settings->format[x]!='#') {
	  string[string_ptr]=settings->format[x];
	  string_ptr++;
       }
       else {
	  string[string_ptr]='\000';
	  x++;
	  if (x>len) break;	  
	  switch(settings->format[x]) {
	   case '#': string_ptr+=vmw_strcat(string,"#"); break;
	   case 'B': sprintf(temp_string,"%.2f",cpu_info.bogomips);
	             string_ptr+=vmw_strcat(string,temp_string); break;
	   case 'C': string_ptr+=vmw_strcat(string,os_info.os_revision); break;
	   case 'E': string_ptr+=vmw_strcat(string,settings->user_text); break;
	   case 'H': string_ptr+=vmw_strcat(string,get_host_name(hostname,domain)); break;
	   case 'L': get_load_average(&load_1,&load_5,&load_15);
	             sprintf(temp_string,"Load Average %.2f, %.2f, %.2f",load_1,load_5,load_15);
	             string_ptr+=vmw_strcat(string,temp_string); 
	             break;
	   case 'M': if (cpu_info.megahertz>0.0) {
	                if (cpu_info.megahertz>999.0) {
			   sprintf(temp_string,"%.3gGHz",cpu_info.megahertz/1000.0);
	                }
	                else {
			    sprintf(temp_string,"%.3gMHz",cpu_info.megahertz);
			}
	                string_ptr+=vmw_strcat(string,temp_string); 
	             }
	             else {
			/* No megahertz.  Back up the pointer.  This is a hack */
			if (string_ptr>0) string_ptr--;
		     }
	             break;
	   case 'N': if (cpu_info.num_cpus<=9) 
	                string_ptr+=vmw_strcat(string,ordinal(cpu_info.num_cpus)); 
	             else if (cpu_info.num_cpus<=99999) {
			sprintf(temp_string,"%d",cpu_info.num_cpus);
			string_ptr+=vmw_strcat(string,temp_string);
		     }
	             else string_ptr+=vmw_strcat(string,ordinal(10));
	             break;
	   case 'O': string_ptr+=vmw_strcat(string,os_info.os_name); break;
	   case 'P': if (cpu_info.num_cpus!=1) 
	                string_ptr+=vmw_strcat(string,_(PROCESSOR_PLURAL));
	             else string_ptr+=vmw_strcat(string,_(PROCESSOR_SINGULAR));
	             break;
	   case 'R': if (get_mem_size()>1000) {
	                sprintf(temp_string,"%.2gGB",
				((float)get_mem_size())/1024.0);
	             } else sprintf(temp_string,"%ldM",get_mem_size());
	             string_ptr+=vmw_strcat(string,temp_string); 
                     break;
	   case 'S': if (cpu_info.num_cpus!=1) 
	                string_ptr+=vmw_strcat(string,"s"); break;
	   case 'T': string_ptr+=vmw_strcat(string,cpu_info.chip_type); break; 
	   case 'U': string_ptr+=vmw_strcat(string,uptime_string(get_uptime())); 
	             break;
	   case 'V': string_ptr+=vmw_strcat(string,os_info.os_version); break;
	   case 'X': if (!strncmp(cpu_info.chip_vendor,"Unknown",7)) { 
	                if (string_ptr>0) string_ptr--;
	             }
	             else {
	                string_ptr+=vmw_strcat(string,cpu_info.chip_vendor); 
		     }
		     break;
	   default: printf("\nInvalid format '#%c'\n",settings->format[x]);
	  }
       }
       x++;
       if (x>len) break;
    }
   
    string[string_ptr]='\000';
    return 0;
}

    /* The Main Drawing Routine */
void draw_logo(struct logo_info *logo_override,
	       struct linux_logo_info_type *settings)
{
    struct logo_info *our_logo_info;
    char temp_string[BUFSIZ];
    char *string_point;
    int i,ysize;
   
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

    if (settings->wipe_screen) clear_screen(settings);
  
       /* Select the proper logo */
    if (settings->plain_ascii) {
       ysize=our_logo_info->ascii_ysize;
       if (our_logo_info->ascii_logo==NULL) {
	  printf("\nSpecified logo has no ascii version!\n\n");
	  return;
       }
       else string_point=(char *)our_logo_info->ascii_logo;
    }
    else {
       ysize=our_logo_info->ysize;
       if (our_logo_info->logo==NULL) {
	  printf("\nSpecified logo has no non-ascii version!\n\n");
	  return;
       }
       else string_point=(char *)our_logo_info->logo;
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
	  
	  if (!settings->plain_ascii) ansi_print("^[[40m^[[40m\n");
  
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
             if (print_sysinfo(i-7,temp_string,settings)!=1) {
	        if (!settings->plain_ascii) ansi_print("^[[1;37;40m");
	        printf("%s",temp_string);
	        if (!settings->plain_ascii) ansi_print("^[[0m");
	     }
             printf("\n");
	  }
       }
    }
      
       /* Print the sysinfo if we haven't already */
    if (!settings->display_logo_only) {
       if (settings->banner_mode) {
          i=0;
          while (print_sysinfo(i,temp_string,settings)!=1) {
	     center_and_print(temp_string,strlen(temp_string),settings->width,
			   settings);
	     i++;
	  }
       }
    }
     
       /* Restore cursor color to normal */
    if (!settings->plain_ascii) 
       ansi_print("^[[0m^[[255D");
}


int main(int argc,char **argv)
{
    char *endptr,*temp_st;
    int c,i,x,logo_num=1,random_logo=0,do_listing=0,logo_override=0;
    int logo_found;
    char temp_string[BUFSIZ],random_type='e',random_type2='e';
    struct linux_logo_info_type settings;
    struct logo_info *temp_logo,*custom_logo=NULL;
    struct timeval time_time;
    FILE *config_file=NULL;
    char *fake_data;
    char **fake_argv=NULL;
    int file_size=0,ch=0,in_quote=0,fake_data_offset=0,fake_argc=0,counter=0; 
    int oldch=0,arguments_pass=0,read_from_file=0;
    int *custom_argc=NULL;
    char **custom_argv=NULL;
    int ignore_config_file=0;
    int old_optind,old_opterr,old_optopt,string_size;
    char *tempst;
   
       /* i18n */
    setlocale(LC_ALL,"");
    textdomain("linux_logo");
    bindtextdomain("linux_logo", "/usr/lib/locale");
   
       /* Set some defaults */
    setup_info(&settings); 
    
       /* ****************************************************** */
       /* Look for ~/.linux_logo or /etc/linux_logo config files */
       /* ****************************************************** */
   
       /* Check for the files */
   
       /* look for ~/.linux_logo */
    if (getenv("HOME")) {
       string_size=strlen(getenv("HOME"));
       tempst=calloc(strlen("/.linux_logo")+string_size,sizeof(char));
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
    if (config_file==NULL) {
       read_from_file=0;
    } 
    else {
          /* Create "fake" argc and argv */
       read_from_file=1;   
          /* calculate size */
       while( (ch=fgetc(config_file))!=EOF) {
          if (ch=='\n') break;
          file_size++;
       }
          /* create room for the fake command-line */
       fake_data=calloc(file_size+12,sizeof(char));
          /* stick "linux_logo" as argv[0] */
       strncpy(fake_data,"linux_logo ",11);
       fake_data_offset=11;
          /* actually read the fake command-line in */
       rewind(config_file);

       ch=' ';
   
       for(i=0;i<file_size;i++) {
          oldch=ch;
          ch=fgetc(config_file);

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
    }
   
       /* Hack! Hack! Hack!  Oh, why do I abuse getopt() so */
    SAVE_GETOPT;
   
    while (arguments_pass<3) {
          /* Pass 0 read from file, if there */
       if (arguments_pass==0) {
	  if (!read_from_file) {
	     arguments_pass++;
	  }
	  else {
             custom_argc=&fake_argc;
             custom_argv=fake_argv;
	  }
       }
          /* Pass 1 is command-line and overrides file */
       if (arguments_pass==1) {
             /* reset getopt()  *HACK HACK HACK* */
	  RESET_GETOPT;
	  custom_argc=&argc;
	  custom_argv=argv;
	  optarg=NULL;
       }
          /* -i passed, so reset everything and just go through command */
          /* line again */
       if (arguments_pass==2) {
	  setup_info(&settings);
	  custom_logo=NULL;
	  RESET_GETOPT;
	  custom_argc=&argc;
	  custom_argv=argv;
	  optarg=NULL;
       }
       
             /*******************************************************/
             /* Parse options (from file or command line arguments) */
             /*******************************************************/
       
       while ((c = getopt (*custom_argc, custom_argv,"D:F:L:"
			           "a::b::c::de:fghiklno:pst:uvw:y"))!=-1) {
          switch (c) {
	     case 'a': settings.plain_ascii=1; break;
	     case 'b': settings.banner_mode=1; break;
	     case 'c': settings.banner_mode=0; break;
	     case 'd': settings.pretty_output=0; 
	               set_pretty_printing(0);
	               break; 
	     case 'D': custom_logo=load_logo_from_disk(optarg);
	               break;
	     case 'e': set_cpuinfo_file(optarg); break;
	     case 'f': settings.wipe_screen=1; break;
	     case 'F': string_size=strlen(optarg);
	               if (string_size==0) break;
	     
	               settings.custom_format=1;
	               if (settings.format) free(settings.format);
	            
	               settings.format=calloc(string_size,sizeof(char));
	               strncpy(settings.format,optarg,string_size);
	              	  
	               /* Decode the \n's. */
	               /* Should I decode other \ codes? */
	               /* also, \ codes are always smaller, hence why */
	               /* we can overwrite our own buffer */
	     
	               i=0; x=0;
	               while(i<strlen(settings.format)) {
		          if (settings.format[i]=='\\') {  
			     switch(settings.format[i+1]) {
			        case 'n': settings.format[x]='\n'; i++; break;
			        default:  settings.format[x]='\\'; break; 
			     }
		          }
		          else settings.format[x]=settings.format[i];
		          i++; x++;
		       }
	               settings.format[x]='\0';
	               break;
	     case 'g': settings.display_sysinfo_only=1; break;
	     case 'h': help_message(argv[0], 1); 
	               exit(0);
	               break;
	     case 'i': ignore_config_file=1; 
	               break;
	     case 'k': settings.center_sysinfo=0;
	               break;
	     case 'l': settings.display_logo_only=1; break;
	     case 'L': 
	               logo_num=strtol(optarg,&endptr,10);
	               if ( endptr == optarg ) {
		          temp_st=strdup(optarg);
		          if (!strncmp(temp_st,"list",4)) {
			     do_listing=1;
		          }
		          else 
		          if (!strncmp(temp_st,"random",6)) {
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
	               break;
	     case 'n': settings.no_periods=1; break;
	     case 'o': 
	               settings.offset=strtol(optarg,&endptr,10);
	               if ( endptr == optarg ) help_message(argv[0], 1);
	               break;
	     case 'p': settings.preserve_xy=1; break;
	     case 's': settings.skip_bogomips=1; break;
	     case 't': 
	               settings.display_usertext=1;
	               settings.user_text=strdup(optarg);
	               break;
	     case 'u': settings.show_uptime=1; break;
	     case 'v': help_message(argv[0], 0);
	               exit(0);
	               break;
	     case 'w':
	               settings.width=strtol(optarg,&endptr,10);
	               if ( endptr == optarg ) help_message(argv[0], 1);
	               break;
	     case 'y': settings.show_load=1; break;
	     case '?': printf("Try \"%s -h\" to show help\n\n",argv[0]);
	               exit(0); 
	               break;
          }
       }
   
          /* If unkown arguments, print the help */
       if ( custom_argv[optind] != NULL ) {
          printf("Unkown option %i: \"%s\"\n",optind,custom_argv[optind]);
	  exit(1);
	  
       }
       arguments_pass++;   
       if ((!ignore_config_file) && (arguments_pass==2)) arguments_pass++;
    }
    
       /*******************************************************/
       /* DONE WITH ALL THE STUPID OPTION PARSING             */
       /* now actually do things                              */
       /*******************************************************/
       
    setup_logos(&settings);
   
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
	      printf("\nError!  Logo %d is invalid!  \"linux_logo -L list\""
		     " lists valid logo numbers.\n\n",logo_num);
	      return 1;
	   }
	   custom_logo=custom_logo->next_logo;
	   i++;
       }
    }
   
       /* We have to keep these consistent or funny things happen */
    if (custom_logo!=NULL) {
       settings.banner_mode=custom_logo->sysinfo_position;
    }

       /* Prepare the sysinfo stuff if not done for us   */
       /* Handle "normal" output by basically faking the */
       /* appropriate "custom" output string             */
    if (!settings.custom_format) {
       if (settings.banner_mode) 
	  settings.format=strdup(_(DEFAULT_BANNER_FORMAT));
       else settings.format=strdup(DEFAULT_CLASSIC_FORMAT);
    
       if (settings.display_usertext) {
          sprintf(temp_string,"#E\n%s",settings.format);
          settings.format=strdup(temp_string);
       }
       
       if (settings.show_load) {
	  settings.format[strlen(settings.format)-3]='\000';
	  sprintf(temp_string,"%s#L\n#H\n",settings.format);
	  settings.format=strdup(temp_string);
       }
       
       if (settings.show_uptime) {
	  settings.format[strlen(settings.format)-3]='\000';
	  sprintf(temp_string,"%s#U\n#H\n",settings.format);
	  settings.format=strdup(temp_string);
       }
    }
   
       /* Preserve xy if so desired */
    if ((settings.preserve_xy) && (!settings.plain_ascii)) 
       ansi_print("^[7");
   
       /* Draw the logo */
    
    draw_logo(custom_logo,&settings);
         
       /* Restore xy if we saved it */
    if ((settings.preserve_xy) && (!settings.plain_ascii)) ansi_print("^[8");
   
    return 0;
}
