/* Re-written from scratch 3 March 2001      */
/* Handles m68k chips on Linux architecture  */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "sysinfo.h"
#include "generic.h"

int get_cpu_info(cpu_info_t *cpu_info) {

    FILE *fff;
    char cpuinfo_file[256];
    char temp_string[256];
    char vendor_string[256],model_string[256],hardware_string[256];
    int cpu_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=hardware_string[0]=0;
 
       /* We get all of our info here from /proc/hardware on m68k */
   
    strncpy(cpuinfo_file,get_cpuinfo_file(),100);
    if ( !(strncmp(cpuinfo_file,"/proc/cpuinfo",13))) {
       strncpy(cpuinfo_file,"/proc/hardware",15);  
    }
      
    if ((fff=fopen(cpuinfo_file,"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"CPU",3))) {
	     strncpy(model_string,parse_line(temp_string),256);
	     clip_lf(model_string,255);
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
    strncpy(cpu_info->chip_type,model_string,63);
  
   
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

int get_hardware(char hardware_string[65]) {
    
    char temp_string[256];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"Model",15))) {
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
