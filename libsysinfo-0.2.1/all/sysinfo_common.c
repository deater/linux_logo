#include "version.h"
#include "sysinfo.h"

#include <string.h>
#include <stdio.h>

struct our_state {
    int pretty_printing;
    int custom_cpuinfo;
    char cpuinfo_file[BUFSIZ];
};

struct our_state internal_state={1,0,"/proc/cpuinfo"};

char *get_sysinfo_version(char *version) {
    snprintf(version,BUFSIZ,"%i.%i.%i",VERSION_MAJOR,VERSION_MINOR,VERSION_SUBMINOR);
    return version;
}

int get_numeric_sysinfo_version(void) {
    return (VERSION_MAJOR*10000)+(VERSION_MINOR*100)+VERSION_SUBMINOR;  
}

int set_cpuinfo_file(char *filename) {
    internal_state.custom_cpuinfo=1;
    strncpy(internal_state.cpuinfo_file,filename,BUFSIZ);
    return 0;
}

char *get_cpuinfo_file(void) {
    return internal_state.cpuinfo_file;  
}

void set_pretty_printing(int value) {
   internal_state.pretty_printing=value;
}

int get_pretty_printing(void) {
   return internal_state.pretty_printing;
}
