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

void get_os_info(struct os_info_type *os_info)
{  
   struct utsname buf;
   
   clear_os_pointers(os_info);
   
   uname( &buf);

   os_info->os_name=strdup(buf.sysname);
   os_info->os_version=strdup(buf.release);
   os_info->os_revision=strdup(buf.version);
   os_info->host_name=strdup(buf.nodename);
   os_info->uptime=strdup(utmp_get_uptime());
   os_info->load_average=strdup(get_loadavg_noproc());
}
    

void get_hw_info(struct hw_info_type *hw_info,int skip_bogomips,
		                        char *cpuinfo_file)
{
   FILE *fff;
   char chip[BUFSIZ]="Unknown";
   char temp_string[BUFSIZ],bogomips_total[BUFSIZ]="???";
   char bogo_total[BUFSIZ];
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */

  clear_hw_pointers(hw_info);
   
  fff=popen("hinv -t cpu","r");
  fscanf(fff,"%s",(char *)&temp_string);
  read_string_from_disk(fff,(char *)&chip);
   pclose(fff);
  fff=popen("hinv -t memory","r");
  fscanf(fff,"%s%s%s",(char *)&temp_string,(char *)&temp_string,
	 (char *)&temp_string);
   read_string_from_disk(fff,(char *)&temp_string);
   pclose(fff);
   	    
      if (!skip_bogomips)
         if ( (external_bogomips( (char *)&bogomips_total))==-1 )
         sprintf(bogo_total," ");
         else sprintf(bogo_total,"%s",bogomips_total);
      else sprintf(bogo_total," ");

      /* Yes.. artificial.  Does anyone ever use this? */
      /* I do have a machine to test on if there is demand --vmw */
   
      hw_info->mem_size=strdup(temp_string);
   
      sprintf(temp_string,"%s",bogo_total);
      hw_info->bogo_total=strdup(temp_string);
   
      hw_info->cpu_vendor=strdup("MIPS ");
  
      hw_info->num_cpus=1;
      
      hw_info->cpu_type=strdup(chip);
}
      
