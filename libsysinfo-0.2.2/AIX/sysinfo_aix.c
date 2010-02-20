/* getsysinfo_aix.c                                               *\
\* I was trying to make this easier to add other platforms/       */
/* architectures.  Feel free to add yours, and send me the patch. *\
\*----------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <string.h>

#include "sysinfo_common.h"
#include "vmw_string.h"

int external_bogomips(char *bogomips_total);

void get_os_info(struct os_info_type *os_info)
{  
   struct utsname buf;
   
   clear_os_pointers(os_info);
   
   uname( &buf);

    os_info->os_name=strdup(buf.sysname);
    os_info->os_version=strdup(buf.version);  /* switched around, JSt */
    /*os_info->os_revision=strdup(buf.version);*/
    os_info->host_name=strdup(buf.nodename);
    os_info->uptime=strdup(utmp_get_uptime());/* Neither of below implemented*/
    os_info->load_average=strdup(get_loadavg_noproc());
 }
    
void get_hw_info(struct hw_info_type *hw_info,
		 struct linux_logo_info_type *logo_info)

{
   FILE *fff;
   int cpus=0;
   long long mem;
   char temp_string2[BUFSIZ];
   char chip[BUFSIZ]="Unknown";
   char temp_string[BUFSIZ],bogomips_total[BUFSIZ]="???";
   char bogo_total[BUFSIZ];
   float megahertz=0.0;
      
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */

    clear_hw_pointers(hw_info);
   
       /*      sprintf(cpuinfo,"Unknown CPU");*/
    if ((fff=popen("lsattr -El proc0","r") )!=NULL) {
       while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	  if ( !(strcmp(temp_string2,"type")) ) { 
/* moved &chip, JSt
   output of lsattr -El proc0 gives
state enable Processor state False
type  POWER2 Processor type  False
   on AIX 4.1.5 systems
 */
	     fscanf(fff,"%64s%64s%64s%64s%64s",(char *)&chip,(char *)&temp_string,
		   (char *)&temp_string,(char *)&temp_string,(char *)&temp_string);
	  }
       }
    }
    pclose(fff);

/* count cpus ... ugly using wc, JSt */
    if ((fff=popen("lsdev -Cc processor -SA|wc -l","r") )!=NULL) {
	     fscanf(fff,"%d",&cpus);
	  }
    pclose(fff);
/* check mem, JSt */
    if ((fff=popen("lsattr -E -l sys0 -a realmem -F value","r") )!=NULL) {
	     fscanf(fff,"%lld",&mem);
	  }
    pclose(fff);
  	    
      if (!logo_info->skip_bogomips)
         if ( (external_bogomips( (char *)&bogomips_total))==-1 )
         snprintf(bogo_total,BUFSIZ," ");
         else snprintf(bogo_total,BUFSIZ,"%s Bogomips Total",bogomips_total);
      else snprintf(bogo_total,BUFSIZ," ");

/* Added for 3.0 best I could.. have no AIX box to test on --vmw */   
      snprintf(temp_string,BUFSIZ,"%ldM",(long int)mem/1024);
      hw_info->mem_size=strdup(temp_string);
   
      hw_info->bogo_total=strdup(bogo_total);
      
      hw_info->num_cpus=cpus;
   
      if (megahertz>1) {
	       snprintf(temp_string,BUFSIZ,"%.0fMHz ",megahertz);
	       hw_info->megahertz=strdup(temp_string);
      }
   
      hw_info->cpu_type=strdup(chip);
}
