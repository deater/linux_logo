#include <stdio.h>
#include <stdlib.h>  /* atol   */
#include <string.h>  /* strstr */
#include <ctype.h>   /* isdigit */
#include <sys/stat.h> /* stat */
#include <unistd.h>  /* stat */

#include "sysinfo.h"
#include "uname.h"

#define MEM_FROM_STAT 1      /* use stat() on /proc/kcore for memsize */

    /* This code modeled on the linux sh-utils 1.16 uptime.c code */
int get_uptime (void) {            
   
    FILE *fff;
    float uptime_seconds;
   
    fff=fopen("/proc/uptime","r");
    if (fff!=NULL) {
       fscanf(fff,"%f",&uptime_seconds);
       fclose (fff);
    }
   
    return uptime_seconds;
}

    /* Based on code contributed by Anders Rundegren <anders@rundegren.com> */
void get_load_average(float *load_1,float *load_5,float *load_15) {
   
    FILE *fff;
   
    fff=fopen("/proc/loadavg","r");
    if (fff!=NULL) {
       fscanf(fff,"%f" "%f" "%f", load_1, load_5, load_15);
       fclose (fff);
    }
}

int get_os_info(os_info_t *os_info) {
    return uname_get_os_info(os_info);
}

char *get_host_name(char hostname[65],char domain[65]) {
    return uname_get_host_name(hostname,domain);
}


    /* mem_size in Megabytes (will this work on > 4Gig systems?) */
long int get_mem_size(void) {

    long int mem_size=0;
   
#ifdef MEM_FROM_STAT
    struct stat buff;
   
    stat("/proc/kcore",&buff);
    mem_size=buff.st_size;
    mem_size/=1024; mem_size/=1024;
   
#else
    
    FILE *info;
    char temp_string[256];
    int position=0;
   
       /* Get values out of /proc/meminfo instead*/
    info=fopen("/proc/meminfo", "r");
   
    if (info==NULL) {
       printf("Error opening /proc/meminfo!\n");
       return -1;
    }
   
    while (fgets(temp_string, BUFSIZ, info)) {
       position=0;
	  /* Skip label */
       while (temp_string[position]) {
          if (isdigit(temp_string[position])) break;
	     position++;
       }
	  
          /* Grab values (in kb) */
       if (strstr(temp_string, "MemTotal"))
	  mem_size = atol(temp_string+position);
    }
    mem_size/=1024;
#endif
   
 
   
#if 0   
      /* Fix off by one ACPI memory error */
   if (logo_info->pretty_output) {
      if ( (fff=fopen("/proc/iomem","r"))!=NULL) {
	 while(!feof(fff)) {
	    read_string_from_disk(fff,(char *)&temp_string2);
            if (strstr(temp_string2,"ACPI")!=NULL) {
	       acpi_found=1;
	    }
	 }
	 fclose(fff);
	 if (acpi_found) mem++;
      }
   }
#endif     
    return mem_size;
 
}
      

