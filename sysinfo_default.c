/* sysinfo_default.c                                              *\
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

int external_bogomips(char *bogomips_total);

void get_os_info(char *os_name, char *os_version, char *os_revision,
		 char *host_name, char *uptime,char *load_avg)
{  
   struct utsname buf;
   uname( &buf);

   strcpy(os_name,buf.sysname);
   strcpy(os_version,buf.release);   
   strcpy(os_revision,buf.version);
   strcpy(host_name,buf.nodename);
   
   strcpy(uptime,utmp_get_uptime(uptime));
   strcpy(load_avg,get_loadavg_noproc(load_avg));
 
 }
    

void get_hardware_info(char *cpuinfo,char *bogo_total,int skip_bogomips)
{
   char bogomips_total[BUFSIZ]="???";
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */
   
      sprintf(cpuinfo,"Unknown CPU");	    

   
      if (!skip_bogomips)
         if ( (external_bogomips( (char *)&bogomips_total))==-1 )
         sprintf(bogo_total," ");
         else sprintf(bogo_total,"%s Bogomips Total",bogomips_total);
      else sprintf(bogo_total," ");
	 
}
      
