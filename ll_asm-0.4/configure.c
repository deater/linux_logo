#include <stdio.h>

#include <string.h>      /* strncpy */
#include <sys/utsname.h> /* uname   */
#include <stdlib.h>      /* system  */

#define ARCH_UNKNOWN 0
#define ARCH_MIPS    1
#define ARCH_PPC     2
#define ARCH_M68K    3
#define ARCH_ALPHA   4
#define ARCH_SPARC   5
#define ARCH_ARM     6
#define ARCH_IX86    7
#define ARCH_IA64    8

char *arch_names[]={
    "Unknown",
    "mips",
    "ppc",
    "m68k",
    "alpha",
    "sparc",
    "arm",
    "ix86",
    "ia64"
};



int linux_detect_arch(void) {
 
    /* Yes this is a bit messy, but it cleans up the makefile a bit *\
    \* The C-Preproccessor can be out friend ;)                     */

#if defined(__mips__)
    return ARCH_MIPS;
#elif defined(__PPC__)
    return ARCH_PPC;
#elif (defined(mc68000) || #cpu(m68k))
    return ARCH_M68K;
#elif  defined(__alpha__)
    return ARCH_ALPHA;
#elif defined(__sparc__)
    return ARCH_SPARC;
#elif defined(__arm__)
    return ARCH_ARM;
#elif defined(i386)
    return ARCH_IX86;
#elif defined(__ia64__)
    return ARCH_IA64;
#else
    return ARCH_UNKNOWN;
#endif 

}

int main(int argc, char **argv) {
 
    struct utsname buf;
    char temp_string[256],arch[65];
    int command_override=0;

    if (argc>1) {
       command_override=1;
    }
   
    uname(&buf);
   
    sprintf(temp_string,"rm -f ll.s");
    system(temp_string);


    strncpy(arch,arch_names[linux_detect_arch()],63);   
    printf("+ compiling for %s architecture\n",arch);
  
    sprintf(temp_string,"ln -n -s ll.%s.s ll.s",arch);
    system(temp_string);    
   
    return 0;
}
