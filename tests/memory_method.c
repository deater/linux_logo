#include <stdio.h>
#include "sysinfo.h"

/* takes megabytes */
void print_mem(long long get_mem_size) {
   if (get_mem_size>1023*1024*1024) {
      printf("%.2gPB\n",
	   ((double)get_mem_size)/(1024.0*1024.0*1024.0));
    }
   else if (get_mem_size>1023*1024*10) {
      printf("%.3gTB\n",
	   ((double)get_mem_size)/(1024.0*1024.0));
    }
   else if (get_mem_size>1023*1024) {
      printf("%.2gTB\n",
	   ((double)get_mem_size)/(1024.0*1024.0));
    }
    else if (get_mem_size>1023*10) {
       printf("%.3gGB\n",
	    ((double)get_mem_size/1024.0));
    }
    else if (get_mem_size>1023) {
       printf("%.2gGB\n",
	    ((double)get_mem_size/1024.0));
    } else {
      printf("%lldM\n",get_mem_size);
    }
}

int main(int argc, char **argv) {

  long long mem_size=0;

  mem_size=1024*1024;
   
  mem_size=get_mem_size_iomem();
  printf("IOMEM:\n");
  printf("\traw:    %lld\n",mem_size);
  printf("\tpretty: ");
  print_mem(mem_size);
  mem_size=get_mem_size_stat();   
  printf("KCORE:\n");
  printf("\traw:    %lld\n",mem_size);
  printf("\tpretty: ");
  print_mem(mem_size);
  mem_size=get_mem_size_sysinfo();
  printf("SYSINFO:\n");
  printf("\traw:    %lld\n",mem_size);
  printf("\tpretty: ");
  print_mem(mem_size);   
  mem_size=get_mem_size_meminfo();   
  printf("MEMINFO:\n");
  printf("\traw:    %lld\n",mem_size);
  printf("\tpretty: ");
  print_mem(mem_size);   
  mem_size=get_mem_size();   
  printf("DEFAULT:\n");
  printf("\traw:    %lld\n",mem_size);
  printf("\tpretty: ");
  print_mem(mem_size);   

   
  return 0;
}
