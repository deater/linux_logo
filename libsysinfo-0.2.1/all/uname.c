#include "sysinfo.h"

#include <string.h>      /* strncpy */
#include <sys/utsname.h> /* uname   */

int uname_get_os_info(struct os_info_type *os_info) {  
    
    struct utsname buf;

    uname(&buf);

    strncpy(os_info->os_name,buf.sysname,SYSINFO_OS_NAME_SIZE);
    strncpy(os_info->os_version,buf.release,SYSINFO_OS_VERSION_SIZE);
    strncpy(os_info->os_revision,buf.version,SYSINFO_OS_REVISION_SIZE);
/*   os_info->host_name=strdup(buf.nodename); */
    return 0;
}

char *uname_get_host_name(char *hostname,char *domain) {
    
    struct utsname buf;
   
    uname(&buf);
   
    strncpy(hostname,buf.nodename,SYSINFO_HOSTNAME_SIZE);
/*    strncpy(domain,buf.domainname,64);*/
    strncpy(domain,"Unknown",SYSINFO_DOMAIN_SIZE);  /* Can't get there from here */
    return hostname;
}
