/* sysinfo_arm.c                                                *\
\*--------------------------------------------------------------*/
/* lousy arm support -- vmw                                     */

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
   int cpus=1;
   struct stat buff;
   long long int mem;
   float bogomips=0.0;
   char temp_string2[BUFSIZ],vendor[BUFSIZ]="Unknown";
   char temp_string[BUFSIZ],bogomips_total[BUFSIZ];
   char *model=NULL;
   float total_bogo=0.0;
   float megahertz=0.0;
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */

    clear_hw_pointers(hw_info);
   
    if ((fff=fopen(logo_info->cpuinfo_file,"r") )!=NULL) {
       while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	 
	  if ( !(strcmp(temp_string2,"Processor")) ) {
	     fscanf(fff,"%s",(char *)&temp_string);
	     read_string_from_disk(fff,(char *)&vendor);
	     
	        /* Huge big ugly hack */
	     if (strstr(vendor,"sa1100")!=NULL) {
		model=strdup("sa1100");
	     }
	     if (strstr(vendor,"710")!=NULL) {
		model=strdup("ARM 710");
	     }
	     sscanf(vendor,"%s",(char *)&vendor);
	     strcat(vendor," ");
	  }
	  
 	  if ( !(my_string_comp(temp_string2,"BogoMIPS")) ) {
	    fscanf(fff,"%s%f",(char *)&bogomips_total,&bogomips);
	    total_bogo+=bogomips;			       
	  }
	  if ( !(my_string_comp(temp_string2,"BogoMips")) ) {
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
      
      hw_info->num_cpus=cpus;
     
      hw_info->cpu_vendor=strdup(vendor);

      if (megahertz>1) {
	 sprintf(temp_string,"%.0fMHz ",(megahertz/1000000));
	 hw_info->megahertz=strdup(temp_string);
      }
   
     printf("%s\n",model); fflush(stdout);
   
     if (model==NULL)
        hw_info->cpu_type=strdup("Unknown");
     else 
        hw_info->cpu_type=strdup(model);
   
   
}
