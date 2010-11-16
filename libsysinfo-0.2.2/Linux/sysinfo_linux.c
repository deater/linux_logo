#include <stdio.h>
#include <stdlib.h>  /* atol, strtol   */
#include <string.h>  /* strncmp, strstr,strchr */
#include <ctype.h>   /* isdigit */
#include <sys/stat.h> /* stat */
#include <unistd.h>  /* stat */

#include <sys/sysinfo.h> /* sysinfo() */

#include "../sysinfo.h"
#include "../include/uname.h"


    /* This code modeled on the linux sh-utils 1.16 uptime.c code */
int get_uptime (void) {            
   
    FILE *fff;
    float uptime_seconds;
    int result;
   
    fff=fopen("/proc/uptime","r");
    if (fff!=NULL) {
       result=fscanf(fff,"%f",&uptime_seconds);
       fclose (fff);
    }
   
    return uptime_seconds;
}

    /* Based on code contributed by Anders Rundegren <anders@rundegren.com> */
void get_load_average(float *load_1,float *load_5,float *load_15) {
   
    FILE *fff;
    int result;
   
    fff=fopen("/proc/loadavg","r");
    if (fff!=NULL) {
       result=fscanf(fff,"%f" "%f" "%f", load_1, load_5, load_15);
       fclose (fff);
    }
}

int get_os_info(struct os_info_type *os_info) {
    return uname_get_os_info(os_info);
}

char *get_host_name(char *hostname,char *domain) {
    return uname_get_host_name(hostname,domain);
}


    /* This is a newer method to obtain memory info from /proc/iomem memory
     * map under 2.4.x. It should work with ACPI systems too. */
    /* Originally Contributed by Sitsofe Wheeler <sits@sucs.swan.ac.uk> */
    /* Improved a bit (in theory) to handle 64-bit machines -vmw */
long long get_mem_size_iomem(void) {
    
    unsigned long long mem_size=0,begin,end;
    char *pos;
    char temp_string[BUFSIZ];
    FILE *info;
     
    if ((info=fopen("/proc/iomem", "r"))) {
       while (fgets(temp_string, BUFSIZ, info)) {
          pos=strchr(temp_string, ':');
	  /* It's safe to +1 to pos because in the worst case we will be on the
	   * end of string marker. I just hope strncmp does bounds checking */
	  if (pos && (strncmp(pos+1, " System RAM", 10)==0 ||
	     strncmp(pos+1, " ACPI", 5)==0)) {
	     /* Extract the upper memory position */
	     /*printf("Adding %s region ",pos);*/
	     pos = strchr(temp_string, '-');
	     if (pos) {
		*pos=0;
		end=strtoull(pos+1, NULL, 16);
		begin=strtoull(temp_string,NULL,16);
		mem_size+=(end-begin);
		/*printf("%lld bytes\n",(end-begin));*/
	     }
	  }
       }
       fclose(info);
    }
       
    if (mem_size > 0) {

          /* if over, round up */
       if (mem_size%(1024*1024)) {
          mem_size = (mem_size/(1024*1024))+1;
       }
       else {
	  mem_size = (mem_size/(1024*1024));
       }

       return mem_size;
    }
    else return 0;
}




    /* mem_size in megabytes by statting /proc/kcore */
    /* This won't work on 32bit machines with > 2Gig of RAM */
long long get_mem_size_stat(void) {

    long long mem_size=0;
    struct stat buff;
   
    stat("/proc/kcore",&buff);
    mem_size=buff.st_size;
    mem_size/=(1024*1024);

    return mem_size;
}

long long get_mem_size_sysinfo(void) {
 
   long long mem_size=0;
   struct sysinfo system_info;
   
   sysinfo(&system_info);
  
   mem_size=(system_info.totalram*system_info.mem_unit)/(1024*1024);
   
   return mem_size;
   
}

    /* get mem size from /proc/meminfo */
long long get_mem_size_meminfo(void) {
    
    long long mem_size=0;
    char temp_string[BUFSIZ];
    FILE *info;
    int position=0;
   
    info=fopen("/proc/meminfo", "r");
   
    if (info==NULL) {
       printf("Error opening /proc/meminfo!\n");
       return 0;
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
	  mem_size = atoll(temp_string+position);
    }
    fclose(info);
   
    return mem_size/1024ULL;
 
}
      
   /* Try the new 2.4 method of looking in /proc/iomem for memory and if it
    * fails fall back to the default method of stating /proc/kmem or
    * filtering /proc/meminfo */
long long get_mem_size(void) {

    long long mem_size=0;
   
       /* First try any arch-specific memsize functions */
    mem_size=get_arch_specific_mem_size();

    if (mem_size == MEM_USE_MEMINFO) {
       mem_size = 0;
       goto use_meminfo;
    }

    if (mem_size == MEM_USE_SYSINFO) {
       mem_size = 0;
       goto use_sysinfo;
    }
   
       /* Next try the 2.4.x+ method of iomem */
    if (mem_size == 0) mem_size = get_mem_size_iomem();

       /* Try stat-ing /proc/kcore */
    if (mem_size == 0) mem_size = get_mem_size_stat();   

use_sysinfo:

       /* sysinfo should return same as /proc/meminfo */
       /* which, sadly, is often from 1MB-20MB off    */
    if (mem_size == 0) mem_size = get_mem_size_sysinfo();
   
use_meminfo:
       /* If all else fails, try using /proc/meminfo */
    if (mem_size == 0) mem_size = get_mem_size_meminfo();
   
    return mem_size;     
}
