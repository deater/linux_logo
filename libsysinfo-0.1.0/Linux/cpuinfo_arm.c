/* Re-written from scratch 3 March 2001      */
/* Handles arm chips on Linux architecture   */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(cpu_info_t *cpu_info) {

    FILE *fff;
    char temp_string[256]; 
    char vendor_string[256],model_string[256],hardware_string[256];
    int cpu_count=0;
    float bogomips=0.0;
   
    vendor_string[0]=model_string[0]=hardware_string[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"Processor",9))) {
	     strncpy(vendor_string,parse_line(temp_string),256);
	     clip_lf(vendor_string,255);
	  }	  
	  if ( !(strncmp(temp_string,"Processor",9))) {
	     strncpy(model_string,parse_line(temp_string),256);
	     clip_lf(model_string,255);
	  }	  
	  
	  if ( !(strncmp(temp_string,"cpu",3))) {
	     strncpy(model_string,parse_line(temp_string),256);
	     clip_lf(model_string,255);
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

    strncpy(cpu_info->chip_vendor,vendor_string,32);
    strncpy(cpu_info->chip_type,model_string,63);
   
       /* This done off of only 2 cpuinfos, so probably not the best */
    if (!strncmp(vendor_string,"ARM",3)) {
       strncpy(cpu_info->chip_vendor,"ARM",4);
//       sscanf(vendor_string,"%s %s %s",throw_away,throw_away,cpu_info->chip_type);
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

int get_hardware(char hardware_string[65]) {
    
    char temp_string[256];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"Hardware",8))) {
             strncpy(hardware_string,parse_line(temp_string),64);
	  }

       }
    }
    return 1;
}

    /* Some architectures might have better ways of detecting RAM size */
long int get_arch_specific_mem_size(void) {
       /* We have no special way of detecting RAM */
       return -1;
}
