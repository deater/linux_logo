/* sysinfo_irix.c                                                 *\
\* I was trying to make this easier to add other platforms/       */
/* architectures.  Feel free to add yours, and send me the patch. *\
\*----------------------------------------------------------------*/
/* Initial gernic Linux and Irix -- Vince Weaver                  */

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <string.h>

#include "sysinfo_common.h"

int external_bogomips(char *bogomips_total);

void get_os_info(char *os_name, char *os_version, char *os_revision,
		 char *host_name, char *uptime)
{  
   struct utsname buf;
   uname( &buf);

   strcpy(os_name,buf.sysname);
   strcpy(os_version,buf.release);   
   strcpy(os_revision,buf.version);
   strcpy(os_revision,"Revision ");
   strcat(os_revision,buf.version);
   strcpy(host_name,buf.nodename);
   
   strcpy(uptime,utmp_get_uptime(uptime));
 }
    

void get_hardware_info(char *cpuinfo,char *bogo_total,int skip_bogomips)
{
   FILE *fff;
   char chip[BUFSIZ]="Unknown";
   char temp_string[BUFSIZ],bogomips_total[BUFSIZ]="???";
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */

  fff=popen("hinv -t cpu","r");
  fscanf(fff,"%s",(char *)&temp_string);
  read_string_from_disk(fff,(char *)&chip);
   pclose(fff);
  fff=popen("hinv -t memory","r");
  fscanf(fff,"%s%s%s",(char *)&temp_string,(char *)&temp_string,
	 (char *)&temp_string);
   read_string_from_disk(fff,(char *)&temp_string);
   pclose(fff);
   
   sprintf(cpuinfo,"%s, %s RAM",chip,temp_string);
	    
      if (!skip_bogomips)
         if ( (external_bogomips( (char *)&bogomips_total))==-1 )
         sprintf(bogo_total," ");
         else sprintf(bogo_total,"%s Bogomips Total",bogomips_total);
      else sprintf(bogo_total," ");
	 
}
      
