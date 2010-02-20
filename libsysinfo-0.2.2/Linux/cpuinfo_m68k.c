/* Re-written from scratch 3 March 2001      */
/* Handles m68k chips on Linux architecture  */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

    FILE *fff;
    char cpuinfo_file[BUFSIZ];
    char temp_string[BUFSIZ];
    char vendor_string[BUFSIZ],model_string[BUFSIZ];
    int cpu_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=0;
 
       /* In the old old days we got our info from /proc/hardware */
       /* modern versions of Linux report in in /proc/cpuinfo     */
       /* Like everyone else.  Try to auto-detect this...         */
      
    strncpy(cpuinfo_file,get_cpuinfo_file(),BUFSIZ);
    if ( !(strncmp(cpuinfo_file,"/proc/cpuinfo",13))) {
       
       fff=fopen(cpuinfo_file,"r");
       if (fff==NULL) {
	  strncpy(cpuinfo_file,"/proc/hardware",15);  
       }
       else {
	  fclose(fff);
       }
    }
      
    if ((fff=fopen(cpuinfo_file,"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"CPU",3))) {
	     strncpy(model_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(model_string,BUFSIZ);
	  }
	  
	  if (!(strncmp(temp_string,"Clocking",8))) {
	     megahertz=atof(parse_line(temp_string));  
	  }
	  
	     /* Ugh why must people play with capitalization */
	  if ( !(strncmp(temp_string,"bogomips",8)) ||
	       !(strncmp(temp_string,"BogoMips",8)) ||
	       !(strncmp(temp_string,"BogoMIPS",8))) {
	     bogomips+=atof(parse_line(temp_string));
	     cpu_count++;  /* Cheating way to detect number of intel CPU's */
	  }
       }
    }
  
    if (strstr(model_string,"COLDFIRE")) {
       strncpy(model_string,"COLDFIRE",9);
    }
   
	
   
    strncpy(cpu_info->chip_vendor,"Motorola",9);
    strncpy(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);
  
   
    if (get_pretty_printing()) {
       /* Fix MHz */
       if (megahertz>0.0)
	  cpu_info->megahertz=fix_megahertz(25,megahertz);
    }
    else {
       cpu_info->megahertz=megahertz;	
    }
   
    cpu_info->num_cpus=cpu_count; 
    cpu_info->bogomips=bogomips;

    return 0;
   
}

int get_hardware(char *hardware_string) {
    
    char temp_string[BUFSIZ];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"Model",15))) {
             strncpy(hardware_string,parse_line(temp_string),
		     SYSINFO_HARDWARE_STRING_SIZE);
	  }

       }
    }
    return 1;
}

    /* Some architectures might have better ways of detecting RAM size */
long long get_arch_specific_mem_size(void) {
       /* We have no special way of detecting RAM */
       return 0;
}
