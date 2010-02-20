/* Re-written from scratch 3 March 2001      */
/* Handles ppc chips on Linux architecture   */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

    FILE *fff;
    char temp_string[BUFSIZ],temp[BUFSIZ];
    char vendor_string[BUFSIZ],model_string[BUFSIZ];
    int cpu_count=0,processor_count=0,bogomips_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"cpu",3))) {
	     strncpy(model_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(model_string,BUFSIZ);
	  }
	  
	  if (!(strncmp(temp_string,"clock",5))) {
	     megahertz=atof(parse_line(temp_string));  
	  }

	  if ( !(strncmp(temp_string,"processor",9))) {
	     processor_count++;
	  }
	  
	     /* Ugh why must people play with capitalization */
	  if ( !(strncmp(temp_string,"bogomips",8)) ||
	       !(strncmp(temp_string,"BogoMips",8)) ||
	       !(strncmp(temp_string,"BogoMIPS",8))) {
	     bogomips+=atof(parse_line(temp_string));
	     bogomips_count++;  /* Cheating way to detect number of intel CPU's */
	  }
       }
    }
   
       /* on cell, there is no bogomips count! */
    if (bogomips_count==0) {
       cpu_count=processor_count;
    }
    else {
	cpu_count=bogomips_count;
    }
   
    strncpy(cpu_info->chip_vendor,"PPC",4);
    strncpy(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);

    if (strstr(model_string,"POWER3")!=NULL) {
       strncpy(cpu_info->chip_type,"POWER3",7);
    }
    if (strstr(model_string,"POWER4")!=NULL) {
       strncpy(cpu_info->chip_type,"POWER4",7);
    }
    if (strstr(model_string,"POWER5")!=NULL) {
       strncpy(cpu_info->chip_type,"POWER5",7);
    }
    if (strstr(model_string,"POWER6")!=NULL) {
       strncpy(cpu_info->chip_type,"POWER6",7);
    }
   
       /* I Have an iBook now ;) */
    if (strstr(model_string,"745/755")!=NULL) {
       strncpy(cpu_info->chip_type,"G3",3);
    }
       
       /* But that's not the only iBook around! ;) */
    if (strstr(model_string,"750CX")!=NULL) {
       strncpy(cpu_info->chip_type,"G3",3);	    
    }
   
    if (strstr(model_string,"750FX")!=NULL) {
       strncpy(cpu_info->chip_type,"G3",3);	    
    }
   
    if (strstr(model_string,"7400")!=NULL) {
       strncpy(cpu_info->chip_type,"7400",5);
    }
 
       /* There should be a way to figure out */
       /* G4/G3 more efficiently than this    */
    if (strstr(model_string,"7410,")!=NULL) {
       strncpy(cpu_info->chip_type,"G4",3);
    }
   
    if (strstr(model_string,"7455")!=NULL) {
       strncpy(cpu_info->chip_type,"G4",3);
    }
   
       /* I have a powerbook now too ;) */
    if (strstr(model_string,"7457")!=NULL) {
       strncpy(cpu_info->chip_type,"G4",3);
    }

    if (strstr(model_string,"Cell")!=NULL) {
       strncpy(cpu_info->chip_type,"Cell",5);
    }
  	
    if (!strncmp(model_string,"PowerPC",7)) {
       sscanf(model_string,"%*s %s",temp);
       strncpy(cpu_info->chip_type,temp,SYSINFO_CHIP_TYPE_SIZE);
    }
  
    cpu_info->num_cpus=cpu_count;
    cpu_info->megahertz=megahertz;
    cpu_info->bogomips=bogomips;

    return 0;
   
}

int get_hardware(char *hardware_string) {
    
    char temp_string[BUFSIZ];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"machine",7))) {
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
   
       /* sysinfo is the only way that works on cell? */
       /* can we detect that somehow and force it? */
   
       /* For mac the meminfo way is definitely better */     
       return MEM_USE_MEMINFO;
}
