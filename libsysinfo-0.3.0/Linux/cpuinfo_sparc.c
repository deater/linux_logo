/* Re-written from scratch 4 March 2001      */
/* Handles sparc chips on Linux architecture */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */
#include <fnmatch.h>

#include "../sysinfo.h"
#include "../include/generic.h"



int get_cpu_info(struct cpu_info_type *cpu_info) {

    FILE *fff;
    char temp_string[BUFSIZ],temp[BUFSIZ];
    char vendor_string[BUFSIZ],model_string[BUFSIZ];
    int cpu_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	
	  if ( !(strncmp(temp_string,"cpu",3))) {
	     strncpy(model_string,parse_line(temp_string),BUFSIZ);
	     clip_lf(model_string,BUFSIZ);
	  }

	  if ( !(strncmp(temp_string,"ncpus active",12))) {
	     cpu_count=atoi(parse_line(temp_string));	     
	  }
	  
	     /* Suggested change by Ben Collins <bmc@visi.net> */
	  if ( !(strncasecmp(temp_string,"bogomips",8)) ||
	     !(fnmatch("Cpu[0-9]*Bogo*",temp_string,0))) {
	      
#if 0	  
	     /* Ugh why must people play with capitalization */
	  if ( !(strncmp(temp_string,"bogomips",8)) ||
	       !(strncmp(temp_string,"BogoMips",8)) ||
	       !(strncmp(temp_string,"BogoMIPS",8)) ||
	       !(strncmp(temp_string,"Cpu",3))) {
#endif	     
	     bogomips+=atof(parse_line(temp_string));
	  }
	  
       }
    }
  
    strncpy(cpu_info->chip_vendor,"Sparc",SYSINFO_CHIP_VENDOR_SIZE);
    strncpy(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);
  
       /* Fix up cpuinfo some */
   
    if (!strncmp(model_string,"Cypress",7)) {
       strncpy(cpu_info->chip_vendor,"Cypress",8);
       sscanf(model_string,"%*s %s",temp);
       strncpy(cpu_info->chip_type,temp,SYSINFO_CHIP_TYPE_SIZE);
    }
   
    if (!strncmp(model_string,"ROSS",4)) {
       strncpy(cpu_info->chip_vendor,"ROSS",5);
       sscanf(model_string,"%*s %s",temp);
       strncpy(cpu_info->chip_type,temp,SYSINFO_CHIP_TYPE_SIZE);
    }
   
    if (!strncmp(model_string,"Texas",5)) {
       strncpy(cpu_info->chip_vendor,"TI",3);
       sscanf(model_string,"%*s %*s %*s %*s %s",temp);
       strncpy(cpu_info->chip_type,temp,SYSINFO_CHIP_TYPE_SIZE);
       
       if (strstr(model_string,"UltraSparc II ")!=NULL) {
	  strncpy(cpu_info->chip_type,"UltraSparc II",14);  
       }
       
    }

    if (strstr(model_string,"SpitFire")!=NULL) {
       strncpy(cpu_info->chip_type,"SpitFire",9);
    } else
    if (strstr(model_string,"Power-UP")!=NULL) {
       strncpy(cpu_info->chip_type,"Power-UP",9);  
    } else
    if (strstr(model_string,"UltraSparc II ")!=NULL) {
       strncpy(cpu_info->chip_type,"UltraSparc II",14);  
    } else
    if (strstr(model_string,"UltraSparc III+")!=NULL) {
       strncpy(cpu_info->chip_type,"UltraSparc III+",16);  
    }   
       
   
       
    cpu_info->num_cpus=cpu_count;
    cpu_info->megahertz=megahertz;
    cpu_info->bogomips=bogomips;

    return 0;
   
}

int get_hardware(char *hardware_string) {
    
    char temp_string[BUFSIZ];
    FILE *fff;
   
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	  	  
	  if (!(strncmp(temp_string,"type",4))) {
             strncpy(hardware_string,parse_line(temp_string),
		     SYSINFO_HARDWARE_STRING_SIZE);
	  }
       }
    }
    return 1;
}

   
    /* I don't have a machine to test the below code on.  I have */
    /* had multiple reports that the PROM code DOESN'T work, so  */
    /* until someone sends me a patch that fixes it, I have turned */
    /* off the PROM code */
   
#define CROSS_DEBUGGING 1
   
/* Following routine provided by Ben Collins <bmc@visi.net>  */
/* Ripped from prtconf: Copyright (C) 1998 Jakub Jelinek (jj@ultra.linux.cz) */

   
#if (CROSS_DEBUGGING==1)
 
long long get_arch_specific_mem_size(void) {
    
       /* /proc/kcore does not reflect memsize on sparc */
   return MEM_USE_MEMINFO;
    
}
#else
   
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <asm/openpromio.h>
   
    static int promfd;
    static char buf[4096];
    static int prom_root_node, prom_current_node;
#define DECL_OP(size) struct openpromio *op = (struct openpromio *)buf; op->oprom_size = (size)

static int prom_getsibling(int node) {
	       
    DECL_OP(sizeof(int));
	
    if (node == -1) return 0;
       *(int *)op->oprom_array = node;
       if (ioctl (promfd, OPROMNEXT, op) < 0)
	  return 0;
       prom_current_node = *(int *)op->oprom_array;
       return *(int *)op->oprom_array;	
}
   
static int prom_getchild(int node) {

    DECL_OP(sizeof(int));
	
    if (!node || node == -1) return 0;
    *(int *)op->oprom_array = node;
    if (ioctl (promfd, OPROMCHILD, op) < 0)
       return 0;
    prom_current_node = *(int *)op->oprom_array;
    return *(int *)op->oprom_array;
}
   
static char *prom_getproperty(char *prop, int *lenp) {
     
    DECL_OP(4096-128-4);
	  
    strcpy (op->oprom_array, prop);
    if (ioctl (promfd, OPROMGETPROP, op) < 0)
       return 0;
    if (lenp) *lenp = op->oprom_size;
       return op->oprom_array;
}
       
static int prom_searchsiblings(char *name) {
    
    char *prop;
    int len;
	
    for (;;) {
       if (!(prop = prom_getproperty("name", &len)))
	  return 0;
       prop[len] = 0;
       if (!strcmp(prop, name))
	  return prom_current_node;
       if (!prom_getsibling(prom_current_node))
       return 0;	            
    }
}
   
static inline int is_sparc64(void) {
	       
    struct utsname uts_info;
	
    prom_getsibling(0);
    uname(&uts_info);
    if (!strcmp(uts_info.machine, "sparc64"))
       return 1;
	
    return 0;
	
}
   
/* On sparc, the best method of memory detection is the prom. We use
 * sparse memory, so /proc/kcore is almost never right, and we all know
 * that /proc/meminfo never reports physical ram accurately.  */
     
long long get_arch_specific_mem_size(void) {
      
    long long memory_size = 0;
    int len, i;
    unsigned int *prop;
	  
    promfd = open("/dev/openprom", O_RDONLY);
    if (promfd == -1) {
       memory_size=MEM_USE_MEMINFO;
       goto mem_done;
    }
	  
    prom_root_node = prom_getsibling(0);
    if (!prom_root_node) {
       memory_size=MEM_USE_MEMINFO;
       goto mem_done;
    }
	  
    prom_getchild(prom_getsibling(0));
    if (!prom_searchsiblings("memory")) {
       memory_size=MEM_USE_MEMINFO;
       goto mem_done;
    }

    prop = (unsigned int *)prom_getproperty("reg", &len);
	  
    if (!prop || (len % sizeof(int)))
       goto mem_done;
	  
    len /= sizeof(int);
    if (is_sparc64()) {
       if (len % 4) {
	  goto mem_done;
		               
       } else {
          for (i = 0; i < len; i+=4) {
	      memory_size += ((unsigned long long)prop[i + 2] << 32);
	      memory_size += prop[i + 3];
	  }               
       }
    } else {
       if (len % 3) {
	  goto mem_done;
       } else {
          for (i = 0; i < len; i+=3)
	      memory_size += prop[i + 2];
       }
    }
   
mem_done:
    if (promfd >= 0)
       close(promfd);

    /* Memory size is megabytes */
    return memory_size;
}
	  
#endif
