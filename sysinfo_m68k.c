/* sysinfo_m68k.c                                                 *\
\* I was trying to make this easier to add other platforms/       */
/* architectures.  Feel free to add yours, and send me the patch. *\
\*----------------------------------------------------------------*/
/* Added Linux mc6800 support    -- Christian Marillat            */
/* the arch is called m68k, the processors 680x0                  */
/* the gcc predefine is mc68000, NOT mc6800 neither m6800         */
/* sidenote: linux-m68k is running on amiga, atari, mac and vme   */
/*                                                            CTS */


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
   int cpus=0,model_seen=0;
   struct stat buff;
   long long int mem;
   float bogomips=0.0;
   char temp_string2[BUFSIZ],model[BUFSIZ]="Unknown";
   char chip[BUFSIZ]="Unknown";
   char model2[BUFSIZ]="Unknown",megahertz[BUFSIZ]=" ";
   char temp_string[BUFSIZ];
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */
   
   clear_hw_pointers(hw_info);
   
   if (!strncmp(logo_info->cpuinfo_file,"/proc/cpuinfo",20)) {
      logo_info->cpuinfo_file=strdup("/proc/hardware");
   }
   
   if ((fff=fopen(logo_info->cpuinfo_file,"r") )!=NULL) {
      while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	 if (cpus==0) {
	    if ( !(strcmp(temp_string2,"Model:")) ) {
		 fscanf(fff,"%s",(char *)&temp_string);
		 fscanf(fff,"%s",(char *)&model2);
	         sprintf(model,"%s ",temp_string);
	         model_seen=1;
	    }
            if ( !(strcmp(temp_string2,"CPU:")) ){
		 fscanf(fff,"%s",(char *)&chip);
	    }
			if ( !(strcmp(temp_string2,"Clocking:")) ) {
			fscanf(fff,"%s",(char *)&megahertz);
		}
	 }
	 if ( !(my_string_comp(temp_string2,"BogoMips:")) ) {
	    fscanf(fff,"%f",&bogomips);
	 }
      }
   }
/*
      if (model_seen)     
         sprintf(cpuinfo,"%s @ %s %s %s %ldM RAM",chip,clock,
		 model,model2,(long int)mem);
      else
         sprintf(cpuinfo,"One %s %s, %ldM RAM",clock,chip,(long int)mem);
      sprintf(bogo_total,"%.2f Bogomips",bogomips);      
  */ 
      stat("/proc/kcore",&buff);
      mem=buff.st_size;
      mem/=1024; mem/=1024;
      sprintf(temp_string,"%ldM",(long int)mem);
      hw_info->mem_size=strdup(temp_string);
   
      /* Things are a bit confused.  I've lost the atari m68k */
      /* /proc/hardware file most of this was written from... */
   
      sprintf(temp_string,"%.2f",bogomips);
      hw_info->bogo_total=strdup(temp_string);
   
      if (model_seen) {
         hw_info->cpu_vendor=strdup(model);
         hw_info->cpu_type=strdup(chip);
         /* model2???*/
      }
      else hw_info->cpu_type=strdup(chip);
   
      hw_info->num_cpus=1;
   
	 sprintf(temp_string,"%s ",megahertz);
	 hw_info->megahertz=strdup(temp_string);
      
}
      
