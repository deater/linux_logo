/******************************************************************
 *     sysinfo_bsd.c  - FreeBSD patch for linux_logo              * 
 * August 31, 2000 by Shawn Rapp                                  *
 * - Made cute little devil pic from daemon screen saver for the  *
 *   linux_logo to use.  Bye Bye fat little penguin  =)           *
 * - CPU information                                              *
 * - OS information                                               *
 * - Somewhat gets the build date (needs parsing)                 *
 * - BogoMips                                                     *
 * - Physical Memory total (not averaged to convention though)    *
 *                                                                *
 * Left to be done                                                *
 * - CPU mgz                                                      *
 * - Patch telnetd to display /etc/issue when specified in        * 
 *   /etc/gettytab.                                               *
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h> /* getloadavg() */
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>
#include <string.h>

#include "sysinfo.h"
#include "include/generic.h"
#include "include/uname.h"

#define SIZE(x) sizeof(x)/sizeof(x[0])


int get_os_info(struct os_info_type *os_info) {
    return uname_get_os_info(os_info);
}


/* Based on /usr/src/usr.bin/w/w.c */
int get_uptime (void) {

     struct timeval boottime;
     time_t uptime=0, now;
     size_t size;
     int mib[2];

     now=time(NULL);

        /*
         * Print how long system has been up.
         * (Found by looking getting "boottime" from the kernel)
         */
      mib[0] = CTL_KERN;
      mib[1] = KERN_BOOTTIME;
      size = sizeof(boottime);
      if (sysctl(mib, 2, &boottime, &size, NULL, 0) != -1 &&
         boottime.tv_sec != 0) {

         uptime = now - boottime.tv_sec;

      }
      return uptime;
}

/* Based on /usr/src/usr.bin/w/w.c */
void get_load_average(float *load_1,float *load_5,float *load_15) {
   
   double avenrun[3];
   
   getloadavg(avenrun, sizeof(avenrun) / sizeof(avenrun[0]));
   
   *load_1=avenrun[0];
   *load_5=avenrun[1];
   *load_15=avenrun[2];
      
   return;
}



char *get_host_name(char *hostname,char *domain) {
    return uname_get_host_name(hostname,domain);
}


long long get_mem_size(void) {
   
    int ctl_ram[] = { CTL_HW, HW_PHYSMEM };
    long long mem_size=0;

    size_t val_int=0;
    size_t val_len=0;
   
    val_len = sizeof(val_int);
    if (sysctl(ctl_ram, SIZE(ctl_ram), &val_int, &val_len,0,0))
       perror("sysctl");
    else {
       mem_size=(val_int/1000000);
    }
      
    return mem_size;     
}

double get_cpu_mhz(void) {
   
      struct cpu_info_type cpu_info;
   
      get_cpu_info(&cpu_info);
   
      return (double)cpu_info.megahertz;
   
}
