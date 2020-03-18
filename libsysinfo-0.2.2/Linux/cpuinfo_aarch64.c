/* Handles arm chips on Linux architecture   */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

    FILE *fff;
    char temp_string[BUFSIZ];
    char vendor_string[BUFSIZ],model_string[BUFSIZ],hardware_string[BUFSIZ];
    int cpu_count=0;
    float bogomips=0.0;
   
    vendor_string[0]=model_string[0]=hardware_string[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"Processor",9))) {
	     strncpy(vendor_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(vendor_string,BUFSIZ);
	  }	  
	  if ( !(strncmp(temp_string,"Processor",9))) {
	     strncpy(model_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(model_string,BUFSIZ);
	  }	  
	  
	  if ( !(strncmp(temp_string,"cpu",3))) {
	     strncpy(model_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(model_string,BUFSIZ);
	  }
	  
	     /* Huge big ugly hack */
	  if (strstr(model_string,"sa11")!=NULL) {
	     strncpy(model_string,"StrongARM",16);
	  } else
	  if (strstr(model_string,"StrongARM")!=NULL) {
	     strncpy(model_string,"StrongARM",14);
	  } else
	  if (strstr(model_string,"XScale")!=NULL) {
	     strncpy(model_string,"XScale",14);
	  } else	    
	  if (strstr(model_string,"710")!=NULL) {
	     strncpy(model_string,"710",4);
	  } else
	  if (strstr(model_string,"940")!=NULL) {
	     strncpy(model_string,"940",4);
	  } else
	  if (strstr(model_string,"Feroceon")!=NULL) {
	     strncpy(model_string,"Feroceon",9);
	  } 	  
	  if (!strncmp(model_string,"ARMv",4)) {
	     sscanf(model_string,"%s",model_string);
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

    strncpy(cpu_info->chip_vendor,vendor_string,SYSINFO_CHIP_VENDOR_SIZE);
    strncpy(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);
   
       /* This done off of only 2 cpuinfos, so probably not the best */
    if (!strncmp(vendor_string,"ARM",3)) {
       strncpy(cpu_info->chip_vendor,"ARM",4);
    }
    else
    if (!strncmp(vendor_string,"Intel",5)){
       strncpy(cpu_info->chip_vendor,"Intel",6);
    }
    else strncpy(cpu_info->chip_vendor,"ARM",4);

    cpu_info->num_cpus=cpu_count;
    cpu_info->megahertz=0.0;
    cpu_info->bogomips=bogomips;

    return 0;
   
}

int get_hardware(char *hardware_string) {

    char temp_string[BUFSIZ];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"Hardware",8))) {
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
