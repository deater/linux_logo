#ifndef linux
/*
 *                Standalone BogoMips program
 *
 * Based on code Linux kernel code in init/main.c and
 * include/linux/delay.h
 *
 * For more information on interpreting the results, see the BogoMIPS
 * Mini-HOWTO document.
 *
 * version: 1.3 
 *  author: Jeff Tranter (Jeff_Tranter@Mitel.COM)
 *
 * Modified for inclusion with Linux_Logo -- Vince Weaver
 */

#include <stdio.h>
#include <time.h>

/* portable version */
static void delay(int loops)
{
  long i;
  for (i = loops; i >= 0 ; i--)
    ;
}

int external_bogomips(char *bogomips_total)
{
  unsigned long loops_per_sec = 1;
  unsigned long ticks;
  
  while ((loops_per_sec <<= 1)) {
    ticks = clock();
    delay(loops_per_sec);
    ticks = clock() - ticks;
    if (ticks >= CLOCKS_PER_SEC) {
      loops_per_sec = (loops_per_sec / ticks) * CLOCKS_PER_SEC;
      sprintf(bogomips_total,"%lu.%02lu",
 	     loops_per_sec/500000,
	     (loops_per_sec/5000) % 100
	     );
         return 0;
    }
  }
  return -1;
}

#endif
