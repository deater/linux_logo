/* sysinfo_ix86.c                                                 *\
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
    

void get_hw_info(struct hw_info_type *hw_info,int skip_bogomips,
		       char *cpuinfo_file)
{
   FILE *fff;
   int cpus=0;
   struct stat buff;
   long long int mem;
   float bogomips=0.0;
   char temp_string2[BUFSIZ],model[BUFSIZ]="Unknown";
   char vendor[BUFSIZ]="Unknown",chip[BUFSIZ]="Unknown";
   char temp_string[BUFSIZ],bogomips_total[BUFSIZ]="???";
   float total_bogo=0.0;
   float megahertz=0.0;
      
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */

   clear_hw_pointers(hw_info);
   
   if ((fff=fopen(cpuinfo_file,"r") )!=NULL) {
      while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	 if (cpus==0) {
	    if ( !(strcmp(temp_string2,"cpu")) ){
		 fscanf(fff,"%s%s",(char *)&temp_string,(char *)&chip);

	         if ( !(strcmp(temp_string,"MHz"))) {
		    fscanf(fff,"%f",&megahertz);
		 }
	       /* Work around 2.0.x <-> 2.1.x incompatibilities? */
	         if (chip[0]==':') {
		    fscanf(fff,"%s",(char *)&temp_string);  
	            sprintf(chip,"%s86",temp_string);
		 }
	         
	    }
	    if ( !(strcmp(temp_string2,"model")) ) {
		 fscanf(fff,"%s",(char *)&temp_string);
	         if (!(strcmp(temp_string,"name")) ) { /* Fix 2.1.120!UGH!*/
		    fscanf(fff,"%s",(char *)&temp_string);
		 }
		 read_string_from_disk(fff,(char *)&model);
	         sscanf(model,"%s",(char *)&temp_string);
	       
	       /* Fix Ugly Look Proc info with custom */
	         	         
	         if ( !(strcmp(temp_string,"K6")))
		    sprintf(model,"%s","K6");	       
	         if ( !(strcmp(temp_string,"AMD-K6tm")))
		    sprintf(model,"%s","K6");
	         if ( !(strcmp(temp_string,"K6-2")))
		    sprintf(model,"%s","K6-2");
	       	 if ( !(strncmp(temp_string,"6x86L",5)))
		    sprintf(model,"%s","6x86");
	         if ( !(strncmp(temp_string,"6x86M",5)))
		    sprintf(model,"%s","6x86MMX");
	         if ( !(strncmp(temp_string,"K5",2)))
		    sprintf(model,"%s","K5");
	         if ( !(strcmp(temp_string,"unknown")))
		    sprintf(model,"%s",chip);
	    }
	    if (!(strcmp(temp_string2,"vendor_id"))
	       || !(strcmp(temp_string2, "vid"))) { /* Fix 1.2.13 */
	       fscanf(fff,"%s",(char *)&temp_string);
	       read_string_from_disk(fff,(char *)&vendor);
	       sscanf(vendor,"%s",(char *)&temp_string);

	       if ( !(strcmp(temp_string,"AuthenticAMD"))) {
	          sprintf(vendor,"%s","AMD ");
		  
		  /* Does the following line fix things on K6 systems with *\
                  \* 2.0.3x? It is very hard to add support for older      */
	          /* kernels without breaking a different chip type.       *\
                  \* 2.1.x fixes this. Hopefully 2.2 is out soon.          */
                  /* If it breaks K6-3D, someone with one send me a        *\
	          \*    /proc/cpuinfo                                      */
		  if (model[0]=='6') sprintf(model,"%s","K6");
	       }
	       if ( !(strcmp(temp_string,"GenuineIntel"))) {
	          sprintf(vendor,"%s","Intel ");
		  /* Report Pentium MMX's right on Intel? */
		  if ( !strcmp(chip,"586") ) {
		     sprintf(model,"%s","Pentium");
		  }
		  /* This Attempted to report Pentium II's correctly on 2.0.x*\
                  \* but didn't work.  Come on, Linus, release 2.2 ;)       */
		  
		  if ( !strcmp(chip,"686") ) {
		     if (model[0]=='3') sprintf(model,"%s","Pentium II");
		  }
		  
	       }
	       if ( !(strcmp(temp_string,"CyrixInstead"))) {
	          sprintf(vendor,"%s","Cyrix ");            
	       }
	       if ( !(strcmp(temp_string,"CentaurHauls"))) {
		  /* I've heard that all the cpuinfo stuff my centaur *\
                  \* Is fully user customizable and it can masquerade */
		  /* As any chip type.  However this should catch the *\
                  \* default type.                                    */
		  sprintf(vendor,"%s","Centaur ");  
	       }
	       if ( !(strcmp(temp_string,"TransmetaNow"))) {
		  sprintf(vendor,"%s","Transmeta ");  
		  /* Hehe this is a joke.  No I have no clue what *\
                  \* Transmeta does.  ;)                          */
	       }
	       if ( !(strcmp(temp_string,"unknown"))) {
		  vendor[0]='\0';
	       }
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
   
   hw_info->cpu_vendor=strdup(vendor);
   
   hw_info->num_cpus=cpus;
   
   if (megahertz>1) {
      sprintf(temp_string,"%.0fMHz ",megahertz);
      hw_info->megahertz=strdup(temp_string);
   }
   
   hw_info->cpu_type=strdup(model);
   
}
      
