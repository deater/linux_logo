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
 * - System uptime                                                *
 * - Load average                                                 *
 ******************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>
#include <string.h>

#include "sysinfo_common.h"
#include "vmw_string.h"

#define SIZE(x) sizeof(x)/sizeof(x[0])

int external_bogomips(char *bogomips_total);

void get_os_info(struct os_info_type *os_info)
{  
   struct utsname buf;
      
   clear_os_pointers(os_info);
   
   uname( &buf);
   
   os_info->os_name=strdup(buf.sysname);
   os_info->os_version=strdup(buf.release);
   os_info->os_revision=strdup(buf.version);
   os_info->host_name=strdup(buf.nodename);
//   os_info->uptime=strdup(utmp_get_uptime());
//   os_info->load_average=strdup(get_loadavg_noproc());
  
}

    
void get_hw_info(struct hw_info_type *hw_info,
		 struct linux_logo_info_type *logo_info)

{
   char bogomips_total[BUFSIZ]="???";
   char bogo_total[BUFSIZ];
   
   struct utsname buf;
   char val_str[100];
   int val_int;
   int val_len;
   int ctl_cpu[] = { CTL_HW, HW_MODEL };
   int ctl_ncpu[] = { CTL_HW, HW_NCPU };
   int ctl_ram[] = { CTL_HW, HW_PHYSMEM };

   uname( &buf);      

         if (!logo_info->skip_bogomips)
            if ( (external_bogomips( bogomips_total))==-1 )
                sprintf(bogo_total," ");
            else sprintf(bogo_total,"%s Bogomips Total",bogomips_total);
         else sprintf(bogo_total," ");
   
   clear_hw_pointers(hw_info);
   hw_info->bogo_total=strdup(bogomips_total);
   
   val_len = SIZE(val_str);
 
   if (sysctl(ctl_cpu, SIZE(ctl_cpu), val_str, &val_len,0,0))
     perror("sysctl");
   else
     hw_info->cpu_type=strdup(val_str);
   
   val_len = sizeof(val_int);   
   if (sysctl(ctl_ncpu, SIZE(ctl_ncpu), &val_int, &val_len,0,0))
     perror("sysctl");
   else
     hw_info->num_cpus = val_int;

   val_len = sizeof(val_int);
   if (sysctl(ctl_ram, SIZE(ctl_ram), &val_int, &val_len,0,0))
     perror("sysctl");
   else {
     sprintf(val_str,"%imb",(val_int/1000000));
     hw_info->mem_size = strdup(val_str);
   }
}
      
