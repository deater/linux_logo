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
long int get_mem_size_iomem(void) {
    
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
    else return -1;
}




    /* mem_size in megabytes by statting /proc/kcore */
    /* This won't work on 32bit machines with > 2Gig of RAM */
long int get_mem_size_stat(void) {

    long int mem_size=-1;
    struct stat buff;
   
    stat("/proc/kcore",&buff);
    mem_size=buff.st_size;
    mem_size/=(1024*1024);

    return mem_size;
}

long int get_mem_size_sysinfo(void) {
 
   long int mem_size=-1;
   struct sysinfo system_info;
   
   sysinfo(&system_info);
  
   mem_size=system_info.totalram/(1024*1024);
   
   if (mem_size>0) return mem_size;
   else return -1;
   
}

    /* get mem size from /proc/meminfo */
long int get_mem_size_meminfo(void) {
    
    long int mem_size=-1;
    char temp_string[BUFSIZ];
    FILE *info;
    int position=0;
   
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
    fclose(info);
   
    return mem_size/1024;
 
}
      
   /* Try the new 2.4 method of looking in /proc/iomem for memory and if it
    * fails fall back to the default method of stating /proc/kmem or
    * filtering /proc/meminfo */
long int get_mem_size(void) {

    long int mem_size=-1;
   
       /* First try any arch-specific memsize functions */
    mem_size=get_arch_specific_mem_size();

    if (mem_size == -2) {
       mem_size = -1;
       goto meminfo_jump;  /* hack! */
    }
   
       /* Next try the 2.4.x+ method of iomem */
    if (mem_size == -1) mem_size = get_mem_size_iomem();

       /* Try stat-ing /proc/kcore */
    if (mem_size == -1) mem_size = get_mem_size_stat();   

meminfo_jump:

       /* sysinfo should return same as /proc/meminfo */
       /* which, sadly, is often from 1MB-20MB off    */
    if (mem_size == -1) mem_size = get_mem_size_sysinfo();
   
       /* If all else fails, try using /proc/meminfo */
    if (mem_size == -1) mem_size = get_mem_size_meminfo();
   
    return mem_size;     
}
