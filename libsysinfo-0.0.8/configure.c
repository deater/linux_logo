#include <stdio.h>
#include "sysinfo.h"
#include "arch.h"

#include <string.h>      /* strncpy */
#include <sys/utsname.h> /* uname   */

int linux_detect_arch(void) {
 
    /* Yes this is a bit messy, but it cleans up the makefile a bit *\
    \* The C-Preproccessor can be out friend ;)                     */
   
/*   return ARCH_HPPA; */
      
#if defined(__alpha__)
   return ARCH_ALPHA;   
#elif defined(__arm__)
    return ARCH_ARM;
#elif defined(__cris__)
    return ARCH_CRIS;
#elif defined(__ia64__)
    return ARCH_IA64;
#elif defined(i386) 
    return ARCH_IX86;
#elif defined(__x86_64__)
    return ARCH_X86_64;
#elif (defined(mc68000) || #cpu(m68k))
    return ARCH_M68K;
#elif defined(__mips__)
    return ARCH_MIPS;
#elif defined(__hppa__)
    return ARCH_HPPA;
#elif defined(__PPC__)
    return ARCH_PPC;
#elif defined(__s390__)
    return ARCH_S390;
#elif defined(__sh__)
    return ARCH_SH3;
#elif defined(__sparc__)
    return ARCH_SPARC;
#elif defined(__vax__)
    return ARCH_VAX;
#else
    return ARCH_UNKNOWN;
#endif 

}

int main(int argc, char **argv) {
 
    FILE *fff;
    struct utsname buf;
    char temp_string[256],arch[65];
    int command_override=0;

    if (argc>1) {
       command_override=1;
    }
   
    uname(&buf);
    
    system("rm -f os");
   
    sprintf(temp_string,"ln -s %s os",buf.sysname);
    system(temp_string);

    fff=fopen("./os/Makefile.inc","w");
    if (fff==NULL) {
       printf("Error!  Could not create config.h\n");
       return -1;
    }
   
    printf("\n");
    printf("Configuring for %s Operating System\n",buf.sysname);
    if (!strncmp(buf.sysname,"Linux",5)) {
       if (command_override) {
	  strncpy(arch,argv[1],63);
       }
       else {
	  strncpy(arch,arch_names[linux_detect_arch()],63);
       }
       
       printf("+ compiling for %s architecture\n",arch);
  
       fprintf(fff,"cpuinfo.o:\tcpuinfo_%s.c\n",arch);
       fprintf(fff,"\t$(CC) $(C_FLAGS) -o cpuinfo.o -c cpuinfo_%s.c\n",arch);

    }
    if (!strncmp(buf.sysname,"SunOS",5)) {   
       fprintf(fff,"# comment\n");
    }
   
    printf("\n");
    fclose(fff);
    return 0;
}
