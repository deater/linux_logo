
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>
#include <string.h>

#include "sysinfo.h"
#include "include/generic.h"

#define SIZE(x) sizeof(x)/sizeof(x[0])

extern float external_bogomips(void);

int get_cpu_info(struct cpu_info_type *cpu_info) {

   size_t val_int=0;
   size_t val_len=0;

   char val_str[BUFSIZ];

   int ctl_cpu[] = { CTL_HW, HW_MODEL };
   int ctl_ncpu[] = { CTL_HW, HW_NCPU };
   
   val_len = SIZE(val_str);
   if (sysctl(ctl_cpu, SIZE(ctl_cpu), val_str, &val_len,0,0))
     perror("sysctl");
   else
     strncpy(cpu_info->chip_type,val_str,SYSINFO_CHIP_TYPE_SIZE);
   
   val_len = sizeof(val_int);   
   if (sysctl(ctl_ncpu, SIZE(ctl_ncpu), &val_int, &val_len,0,0))
     perror("sysctl");
   else
     cpu_info->num_cpus = val_int;

   
   cpu_info->bogomips=external_bogomips();
   cpu_info->megahertz=0.0;
   
   return 0;
   
}
