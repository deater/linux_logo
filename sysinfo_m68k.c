/* sysinfo_m68k.c                                                 *\
\* I was trying to make this easier to add other platforms/       */
/* architectures.  Feel free to add yours, and send me the patch. *\
\*----------------------------------------------------------------*/
/* Added Linux mc6800 support    -- Christian Marillat            */

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
   strcpy(os_revision,"Compiled ");
   strcat(os_revision,buf.version);
   strcpy(host_name,buf.nodename);

   strcpy(uptime,linux_get_proc_uptime(uptime));
   strcpy(load_avg,get_loadavg_noproc(load_avg));
 }
    

void get_hardware_info(char *cpuinfo,char *bogo_total,int skip_bogomips,
		       char *cpuinfo_file)
{
   FILE *fff;
   int cpus=0,model_seen=0;
   struct stat buff;
   long long int mem;
   float bogomips=0.0;
   char temp_string2[BUFSIZ],model[BUFSIZ]="Unknown";
   char chip[BUFSIZ]="Unknown";
   char model2[BUFSIZ]="Unknown",clock[BUFSIZ]="Unknown";
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */
   
   if (!strncmp(cpuinfo_file,"/proc/cpuinfo",20)) {
      strcpy("/proc/hardware",cpuinfo_file);
   }
   
   if ((fff=fopen(cpuinfo_file,"r") )!=NULL) {
      while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	 if (cpus==0) {
	    if ( !(strcmp(temp_string2,"Model:")) ) {
		 fscanf(fff,"%s",(char *)&model);
		 fscanf(fff,"%s",(char *)&model2);
	         model_seen=1;
	    }
            if ( !(strcmp(temp_string2,"CPU:")) ){
		 fscanf(fff,"%s",(char *)&chip);
	    }
			if ( !(strcmp(temp_string2,"Clocking:")) ) {
			fscanf(fff,"%s",(char *)&clock);
		}
	 }
	 if ( !(my_string_comp(temp_string2,"BogoMips:")) ) {
	    fscanf(fff,"%f",&bogomips);
	 }
      }
   }

   stat("/proc/kcore",&buff);
   mem=buff.st_size;
   mem/=1024; mem/=1024;
      if (model_seen)     
         sprintf(cpuinfo,"%s @ %s %s %s %ldM RAM",chip,clock,
		 model,model2,(long int)mem);
      else
         sprintf(cpuinfo,"One %s %s, %ldM RAM",clock,chip,(long int)mem);
      sprintf(bogo_total,"%.2f Bogomips",bogomips);      
    
      /* End Linux */
 	 
}
      
