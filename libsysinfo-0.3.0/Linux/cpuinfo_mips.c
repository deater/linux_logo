/* Re-written from scratch 3 March 2001      */
/* Handles mips chips on Linux architecture  */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

    FILE *fff;
    char temp_string[BUFSIZ];
    char vendor_string[BUFSIZ],model_string[BUFSIZ],temp[BUFSIZ];
    int cpu_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=0;

    strncpy(vendor_string,"MIPS",5);
   
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"cpu model",9))) {
	     strncpy(model_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(model_string,BUFSIZ);
	  }
	  else if ( !(strncmp(temp_string,"cpu  ",5)) ||
		    !(strncmp(temp_string,"cpu\t",4))) {
	     strncpy(vendor_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(vendor_string,BUFSIZ);
	  }
	  
	     /* Ugh why must people play with capitalization */
	  if ( !(strncmp(temp_string,"bogomips",8)) ||
	       !(strncmp(temp_string,"BogoMips",8)) ||
	       !(strncmp(temp_string,"BogoMIPS",8))) {
	     bogomips+=atof(parse_line(temp_string));
	     cpu_count++;  /* Cheating way to detect number of CPUs */
	  }
       }
    }

    strncpy(cpu_info->chip_vendor,vendor_string,SYSINFO_CHIP_VENDOR_SIZE);
    strncpy(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);
   
    if (!strncmp(model_string,"NEC",3)) {
       sscanf(model_string,"%*s %s",temp);
       strncpy(cpu_info->chip_type,temp,SYSINFO_CHIP_TYPE_SIZE);
    }
    else if (!strncmp(model_string,"MIPS",4)) {
       sscanf(model_string,"%*s %s",temp);
       strncpy(cpu_info->chip_type,temp,SYSINFO_CHIP_TYPE_SIZE);	
    }
    else {
       sscanf(model_string,"%s",temp);
       strncpy(cpu_info->chip_type,temp,SYSINFO_CHIP_TYPE_SIZE);  
    }
   
    cpu_info->num_cpus=cpu_count;
    cpu_info->megahertz=megahertz/1000000.0;
    cpu_info->bogomips=bogomips;

    return 0;
   
}

int get_hardware(char *hardware_string) {
    
    char temp_string[BUFSIZ];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"system type",11))) {
             strncpy(hardware_string,parse_line(temp_string),
		     SYSINFO_HARDWARE_STRING_SIZE);
	  }
       }
    }
    return 1;
}

    /* Some architectures might have better ways of detecting RAM size */
long long get_arch_specific_mem_size(void) {
       /* We use sysinfo */
       return MEM_USE_SYSINFO;
}
