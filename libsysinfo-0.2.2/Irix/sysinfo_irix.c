/* sysinfo_irix.c                                                 */


#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <string.h>

#include "../sysinfo.h"
#include "../include/uname.h"
#include "../include/generic.h"


int get_os_info(struct os_info_type *os_info) {
   return uname_get_os_info(os_info);
}


   /* read from "uptime" maybe? */
int get_uptime (void) {

   return 0;
}

   /* read from "uptime" maybe? */
void get_load_average(float *load_1,float *load_5,float *load_15) {

   *load_1=0.0;
   *load_5=0.0;
   *load_15=0.0;
   
}


char *get_host_name(char *hostname,char *domain) {
       return uname_get_host_name(hostname,domain);
}

long long get_mem_size(void) {
   
   FILE *fff;
   long long size;
   
   fff=popen("hinv -t memory","r");
   fscanf(fff,"%*s%*s%*s %lld",&size);
   pclose(fff);
   return size;
}

int get_cpu_info(struct cpu_info_type *cpu_info) {

   FILE *fff;
   
   /* 1 300 MHZ IP30 Processor */
   fff=popen("hinv","r");
   fscanf(fff,"%d %f",&cpu_info->num_cpus,&cpu_info->megahertz);
   pclose(fff);
   
   /* Cpu: MIPS R12000 */
   fff=popen("hinv -t cpu","r");
   fscanf(fff,"%*s %32s %64s",cpu_info->chip_vendor,cpu_info->chip_type);
   pclose(fff);

   cpu_info->bogomips=external_bogomips();
   
   return 0;
}
