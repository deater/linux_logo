/* Re-written from scratch 4 March 2001      */
/* Handles sparc chips on Linux architecture */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "sysinfo.h"
#include "generic.h"

int get_cpu_info(cpu_info_t *cpu_info) {

    FILE *fff;
    char temp_string[256],throw_away[256];
    char vendor_string[256],model_string[256],hardware_string[256];
    int cpu_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=hardware_string[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"cpu",3))) {
	     strncpy(model_string,parse_line(temp_string),256);  
	     clip_lf(model_string,255);
	  }

	  if ( !(strncmp(temp_string,"ncpus active",12))) {
	     cpu_count=atoi(parse_line(temp_string));
	     
	  }
	  
	     /* Ugh why must people play with capitalization */
	  if ( !(strncmp(temp_string,"bogomips",8)) ||
	       !(strncmp(temp_string,"BogoMips",8)) ||
	       !(strncmp(temp_string,"BogoMIPS",8)) ||
	       !(strncmp(temp_string,"Cpu",3))) {
	     bogomips+=atof(parse_line(temp_string));
	  }
       }
    }
  
    strncpy(cpu_info->chip_vendor,"Sparc",32);
    strncpy(cpu_info->chip_type,model_string,63);
  
       /* Fix up cpuinfo some */
   
    if (!strncmp(model_string,"Cypress",7)) {
       strncpy(cpu_info->chip_vendor,"Cypress",8);
       sscanf(model_string,"%s %s",throw_away,cpu_info->chip_type);
    }
   
    if (!strncmp(model_string,"ROSS",4)) {
       strncpy(cpu_info->chip_vendor,"ROSS",5);
       sscanf(model_string,"%s %s",throw_away,cpu_info->chip_type);
    }
   
    if (!strncmp(model_string,"Texas",5)) {
       strncpy(cpu_info->chip_vendor,"TI",3);
       sscanf(model_string,"%s %s %s %s %s",throw_away,throw_away,
	      throw_away,throw_away,cpu_info->chip_type);
    }
       
   
    cpu_info->num_cpus=cpu_count;
    cpu_info->megahertz=megahertz;
    cpu_info->bogomips=bogomips;

    return 0;
   
}

int get_hardware(char hardware_string[65]) {
    
    char temp_string[256];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"type",4))) {
             strncpy(hardware_string,parse_line(temp_string),64);
	  }

       }
    }
    return 1;
}
