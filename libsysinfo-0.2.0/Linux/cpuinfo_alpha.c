/* Re-written from scratch 3 March 2001      */
/* Handles alpha chips on Linux architecture */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

    FILE *fff;
    char temp_string[256],throw_away[256];
    char vendor_string[256],model_string[256],hardware_string[256];
    int cpu_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=hardware_string[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"cpu model",9))) {
	     strncpy(model_string,parse_line(temp_string),256);
	     clip_lf(model_string,255);
	  }
	  else if ( !(strncmp(temp_string,"cpu  ",5)) ||
		    !(strncmp(temp_string,"cpu\t",4))) {
	     strncpy(vendor_string,parse_line(temp_string),256);  
	     clip_lf(vendor_string,255);
	  }
	  
	  if (!(strncmp(temp_string,"cpus detected",13))) {
	     cpu_count=atoi(parse_line(temp_string));
	  }
	     /* Older cpuinfo */
	  if (!(strncmp(temp_string,"CPUs probed",11))) {
	     sscanf(temp_string,"%s %s %i",throw_away,throw_away,&cpu_count);  
	  }
	  
	  if (!(strncmp(temp_string,"cycle frequency",15))) {
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
  
    strncpy(cpu_info->chip_vendor,vendor_string,32);
    strncpy(cpu_info->chip_type,model_string,63);
  
       /* Sanity check.  You can't run Linux w/o a cpu can you? */
       /* an ev5 cpuinfo I have says "cpus detected: 0"         */
    if (cpu_count==0) cpu_count=1;
   
    cpu_info->num_cpus=cpu_count;
    cpu_info->megahertz=megahertz/1000000.0;
    cpu_info->bogomips=bogomips;

    return 0;
   
}

int get_hardware(char hardware_string[65]) {
    
    char temp_string[256];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"platform string",15))) {
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
