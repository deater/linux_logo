/* sysinfo_sparc.c                                                *\
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
		 char *host_name, char *uptime)
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
 }
    

void get_hardware_info(char *cpuinfo,char *bogo_total,int skip_bogomips)
{
   FILE *fff;
   int cpus=0;
   struct stat buff;
   long long int mem;
   float bogomips=0.0;
   char temp_string2[BUFSIZ],model[BUFSIZ],vendor[BUFSIZ];
   char temp_string[BUFSIZ],bogomips_total[BUFSIZ];
   float total_bogo=0.0;
   /*Anyone have more than 9 cpu's yet?*/	
	char ordinal[10][10]={"Zero","One","Two","Three","Four","Five","Six",
	                      "Seven","Eight","Nine"};
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */
   
   if ((fff=fopen("/proc/cpuinfo","r") )!=NULL) {
      while ( (fscanf(fff,"%s",(char *)&temp_string2)!=EOF) ) {
	 if (!(strcmp(temp_string2,"ncpus")) && (cpus==0)) {
	    fscanf(fff,"%s%s",(char *)&temp_string,(char *)&temp_string);
	    fscanf(fff,"%d",&cpus);
	    rewind(fff);
	 }
	 if ( !(strcmp(temp_string2,"cpu")) ) {
	    fscanf(fff,"%s",(char *)&temp_string);
	    read_string_from_disk(fff,(char *)&model);
	    sprintf(temp_string,"%s",model);
	       
	    /* Fix Ugly Look Proc info with custom */
	    if (strstr(temp_string,"Texas")!=NULL){
	       sprintf(vendor,"%s","TI");	
	    }
            if (strstr(temp_string,"SuperSparc")!=NULL) {
	       sprintf(model,"%s","SuperSparc");
	    }
	 }
	  
	 if ( strstr(temp_string2,"Bogo")!=NULL ) {
	    if ( strstr(temp_string2,"Cpu")!=NULL ) {
	       fscanf(fff,"%s%f",(char *)&bogomips_total,&bogomips);
	       total_bogo+=bogomips;
	    }
	    if ( strstr(temp_string2,"Mips")!=NULL ) {
	       fscanf(fff,"%s%f",(char *)&bogomips_total,&bogomips);
	       total_bogo+=bogomips;
	    }
	 }
      }
   }
   
	 

   stat("/proc/kcore",&buff);
   mem=buff.st_size;
   mem/=1024; mem/=1024;
	    
      sprintf(cpuinfo,"%s %s %s Processor%s %ldM RAM",ordinal[cpus],
	      vendor,model,(cpus>1)?"s,":",",(long int)mem);
      sprintf(bogo_total,"%.2f Bogomips Total",total_bogo);      

}
      
