#include <stdio.h>

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

  long long i=0;

  for(i=0;i<10;i++) {
     print_mem(i);
  }

  for(i=1010;i<1050;i+=10) {
     print_mem(i);
  }

  print_mem(1024*1024);
  print_mem(128*1024);
  print_mem(500*1024);
  print_mem(512*1024);
  print_mem(600*1024);

  print_mem(2032504/1024);
  print_mem(4096*1024);
  print_mem(65536*1024);

  print_mem(128*1024*1024);
  print_mem(1024*1024*1024);

  return 0;
}
