/* sysinfo_default.c                                              *\
\* I was trying to make this easier to add other platforms/       */
/* architectures.  Feel free to add yours, and send me the patch. *\
\*----------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <string.h>

#include "sysinfo_common.h"

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
   os_info->uptime=strdup(utmp_get_uptime());
   os_info->load_average=strdup(get_loadavg_noproc());
}
    
void get_hw_info(struct hw_info_type *hw_info,
		 struct linux_logo_info_type *logo_info)

{
   char bogomips_total[BUFSIZ]="???";
   char bogo_total[BUFSIZ];
   
         if (!logo_info->skip_bogomips)
              if ( (external_bogomips( (char *)&bogomips_total))==-1 )
                sprintf(bogo_total," ");
            else sprintf(bogo_total,"%s Bogomips Total",bogomips_total);
         else sprintf(bogo_total," ");
   
         hw_info->bogo_total=strdup(bogo_total);
   
   
   clear_hw_pointers(hw_info);
   
   hw_info->cpu_type=strdup("Unkown Architecture");
   
}
      
