#include <stdio.h>
#include <stdlib.h>  /* atol   */
#include <string.h>  /* strstr */
#include <ctype.h>   /* isdigit */
#include <sys/stat.h> /* stat */
#include <unistd.h>  /* stat */

#include "sysinfo.h"
#include "uname.h"

    /* Not supported under solaris yet */
int get_uptime (void) {            
   
    FILE *fff;
    float uptime_seconds;
   
    return 0;
}

    /* Based on code contributed by Anders Rundegren <anders@rundegren.com> */
void get_load_average(float *load_1,float *load_5,float *load_15) {
   
    FILE *fff;
    char throw_away[255];
   
    fff=popen("w | head -n 1","r");
    if (fff!=NULL) {
       fscanf(fff,"%s %s %s %s %s %s %s %f %f %f", 
	      throw_away,throw_away,throw_away,
	      throw_away,throw_away,throw_away,throw_away,
	      load_1, load_5, load_15);
       fclose (fff);
    }
}

int get_os_info(os_info_t *os_info) {
    return uname_get_os_info(os_info);
}

char *get_host_name(char hostname[65],char domain[65]) {
    return uname_get_host_name(hostname,domain);
}


    /* Unimplemented on Solaris */
long int get_mem_size(void) {

    return 0;
 
}

extern float external_bogomips(void);

    /* Unimplemented on Solaris */
int get_cpu_info(cpu_info_t *cpu_info) {
   
    cpu_info->bogomips=external_bogomips();
  
    return 0;
}


