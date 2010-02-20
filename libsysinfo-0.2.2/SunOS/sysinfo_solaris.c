#include <stdio.h>
#include <stdlib.h>  /* atol   */
#include <string.h>  /* strstr */
#include <ctype.h>   /* isdigit */
#include <sys/stat.h> /* stat */
#include <unistd.h>  /* stat */

#include "sysinfo.h"
#include "include/uname.h"

    /* Not supported under solaris yet */
int get_uptime (void) {            
   
//    FILE *fff;
//    float uptime_seconds;
   
    return 0;
}

    /* Based on code contributed by Anders Rundegren <anders@rundegren.com> */
void get_load_average(float *load_1,float *load_5,float *load_15) {
   
    FILE *fff;
    char throw_away[BUFSIZ];
   
    fff=popen("w | head -n 1","r");
    if (fff!=NULL) {
       fscanf(fff,"%64s %64s %64s %64s %64s %64s %64s %f %f %f", 
	      throw_away,throw_away,throw_away,
	      throw_away,throw_away,throw_away,throw_away,
	      load_1, load_5, load_15);
       fclose (fff);
    }
}

int get_os_info(struct os_info_type *os_info) {
    int result,i,after_point=0,j=0;
    char temp_version[33];
   
    result=uname_get_os_info(os_info);
   

       /* This won't work with pre-Solaris versions */
    strncpy(os_info->os_name,"Solaris",8);
       
    i=0;
    while(i<strlen(os_info->os_version)) {
       if (after_point) {
	  temp_version[j]=os_info->os_version[i];
	  j++;
       }

       if (os_info->os_version[i]=='.') {
	  after_point=1;
       }
       i++;
    }
    temp_version[j]='\0';
    strncpy(os_info->os_version,temp_version,SYSINFO_OS_VERSION_SIZE);
    
    return result;
}

char *get_host_name(char *hostname,char *domain) {
    return uname_get_host_name(hostname,domain);
}


    
long long get_mem_size(void) {

    FILE *pipe;
    char string[BUFSIZ];
    long long memsize;
   
    pipe=popen("/usr/sbin/prtconf","r");
    fgets(string,BUFSIZ,pipe); /* skip line */
    fscanf(pipe,"%*s %*s %lld",&memsize);
   
    return memsize;
 
}

extern float external_bogomips(void);

    
int get_cpu_info(struct cpu_info_type *cpu_info) {
   
    FILE *pipe;
    char string[BUFSIZ];
    int cpus=0;
    float version;
   
    cpu_info->bogomips=external_bogomips();

    pipe=popen("/usr/sbin/psrinfo","r");
    while(fgets(string,BUFSIZ,pipe)!=NULL) {
      cpus++;
    }
    pclose(pipe);
   
    cpu_info->num_cpus=cpus;
    strncpy(cpu_info->chip_vendor,"SPARC",33);
    
    pipe=popen("uname -r","r");
    fscanf(pipe,"%f",&version);
    pclose(pipe);
   
    /* Solaris 9 psrinfo doesn't support -p option */
    if (version>5.2) {
       /* don't support -p option, do something else */
       pipe=popen("/usr/sbin/psrinfo -v","r");
       fgets(string,BUFSIZ,pipe);  /* ignore line */
       fgets(string,BUFSIZ,pipe);  /* ignore line */
       fscanf(pipe,"%*s %64s %*s %*s %*s %f",cpu_info->chip_type,
	      &cpu_info->megahertz);
       pclose(pipe);
    }
    else {
       /* we support -p option... */
       pipe=popen("/usr/sbin/psrinfo -p -v","r");
       fgets(string,BUFSIZ,pipe);  /* ignore line */
       fscanf(pipe,"%64s %*s %*s %*s %f",cpu_info->chip_type,
	      &cpu_info->megahertz);
       pclose(pipe);
    }

    return 0;
}


