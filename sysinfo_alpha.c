/* sysinfo_alpha.c                                                *\
\* I was trying to make this easier to add other platforms/       */
/* architectures.  Feel free to add yours, and send me the patch. *\
\*----------------------------------------------------------------*/
/* Added Linux alpha support   -- JrB                             */

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
    os_info->uptime=strdup(linux_get_proc_uptime());
    os_info->load_average=strdup(linux_get_proc_loadavg());
 }
    

void get_hw_info(struct hw_info_type *hw_info,
		 struct linux_logo_info_type *logo_info)

{
   FILE *fff;
   int cpus=0;
   struct stat buff;
   long long int mem;
   float bogomips=0.0;
   char temp_string2[BUFSIZ],model[BUFSIZ]="Unknown";
   char temp_string[BUFSIZ],bogomips_total[BUFSIZ];
   float total_bogo=0.0;
   float megahertz=0.0;
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */

    clear_hw_pointers(hw_info);
   
   if ((fff=fopen(logo_info->cpuinfo_file,"r") )!=NULL) {
           while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	 if (cpus==0) {
	    if ( !(strcmp(temp_string2,"model")) ) {
		 fscanf(fff,"%s",(char *)&temp_string);
		 read_string_from_disk(fff,(char *)&model);
	         sscanf(model,"%s",(char *)&temp_string);
	    }
	    if ( !(strcmp(temp_string2,"cycle")) ) {
	       fscanf(fff,"%s%s%s%f",(char *)&temp_string,(char *)&temp_string,
		      (char *)&temp_string,&megahertz);
	    }
	 }
	 if ( !(my_string_comp(temp_string2,"bogomips")) ) {
	    cpus++;
	    fscanf(fff,"%s%f",(char *)&bogomips_total,&bogomips);
	    total_bogo+=bogomips;			       
	 }
	 
      }
   }

      stat("/proc/kcore",&buff);
      mem=buff.st_size;
      mem/=1024; mem/=1024;
      sprintf(temp_string,"%ldM",(long int)mem);
      hw_info->mem_size=strdup(temp_string);
   
      sprintf(temp_string,"%.2f",total_bogo);
      hw_info->bogo_total=strdup(temp_string);
   
      hw_info->cpu_vendor=strdup("Alpha ");
   
      hw_info->num_cpus=cpus;

      if (megahertz>1) {
	 sprintf(temp_string,"%.0fMHz ",(megahertz/1000000));
	 hw_info->megahertz=strdup(temp_string);
      }
   
      hw_info->cpu_type=strdup(model);
   
   
}
