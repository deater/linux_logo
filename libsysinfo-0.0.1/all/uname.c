#include "sysinfo.h"

#include <string.h>      /* strncpy */
#include <sys/utsname.h> /* uname   */

int uname_get_os_info(os_info_t *os_info) {  
    
    struct utsname buf;

    uname( &buf);

    strncpy(os_info->os_name,buf.sysname,32);
    strncpy(os_info->os_version,buf.release,32);
    strncpy(os_info->os_revision,buf.version,64);
/*   os_info->host_name=strdup(buf.nodename); */
    return 0;
}

char *uname_get_host_name(char hostname[65],char domain[65]) {
    
    struct utsname buf;
   
    uname(&buf);
   
    strncpy(hostname,buf.nodename,64);
/*    strncpy(domain,buf.domainname,64);*/
    strncpy(domain,"Unknown",64);  /* Can't get there from here */
    return hostname;
}

