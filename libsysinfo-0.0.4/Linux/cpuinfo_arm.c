/* Re-written from scratch 3 March 2001      */
/* Handles arm chips on Linux architecture   */
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
    float bogomips=0.0;
   
    vendor_string[0]=model_string[0]=hardware_string[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,255,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"Processor",9))) {
	     strncpy(vendor_string,parse_line(temp_string),256);
	     clip_lf(vendor_string,255);
	  }	  
	  if ( !(strncmp(temp_string,"Hardware",8))) {
	     strncpy(model_string,parse_line(temp_string),256);
	     clip_lf(model_string,255);
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
       sscanf(vendor_string,"%s %s %s",throw_away,throw_away,cpu_info->chip_type);
    }
   
    if (!strncmp(vendor_string,"Intel",5)){
       strncpy(cpu_info->chip_vendor,"Intel",6);
    }

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
	  	  
	  if (!(strncmp(temp_string,"Processor",9))) {
             strncpy(hardware_string,parse_line(temp_string),64);
	  }

       }
    }
    return 1;
}

#if 0
   
    if ((fff=fopen(logo_info->cpuinfo_file,"r") )!=NULL) {
       while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	 
	  if ( !(strcmp(temp_string2,"Processor")) ) {
	     fscanf(fff,"%s",(char *)&temp_string);
	     read_string_from_disk(fff,(char *)&vendor);
	     
	        /* Huge big ugly hack */
	     if (strstr(vendor,"sa1100")!=NULL) {
		model=strdup("sa1100");
	     }
	     if (strstr(vendor,"710")!=NULL) {
		model=strdup("ARM 710");
	     }
	     sscanf(vendor,"%s",(char *)&vendor);
	     strcat(vendor," ");
	  }
	  
 	  if ( !(my_string_comp(temp_string2,"BogoMIPS")) ) {
	    fscanf(fff,"%s%f",(char *)&bogomips_total,&bogomips);
	    total_bogo+=bogomips;			       
	  }
	  if ( !(my_string_comp(temp_string2,"BogoMips")) ) {
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
      
      hw_info->num_cpus=cpus;
     
      hw_info->cpu_vendor=strdup(vendor);

      if (megahertz>1) {
	 sprintf(temp_string,"%.0fMHz ",(megahertz/1000000));
	 hw_info->megahertz=strdup(temp_string);
      }
   
     printf("%s\n",model); fflush(stdout);
   
     if (model==NULL)
        hw_info->cpu_type=strdup("Unknown");
     else 
        hw_info->cpu_type=strdup(model);
#endif  
 

    /* Some architectures might have better ways of detecting RAM size */
long int get_arch_specific_mem_size(void) {
       /* We have no special way of detecting RAM */
       return -1;
}
