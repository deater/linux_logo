/* Re-written from scratch 3 March 2001      */
/* Handles intel chips on Linux architecture */
/* by Vince Weaver <vince@deater.net>        */
/* update for IA-64 architecture 	     */
/* by Ludo <lfrancois@mandrakesoft.com>	     */

#include <stdio.h>

#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

  FILE	*fff;
  char	temp_string[BUFSIZ];
  char	vendor_string[BUFSIZ], model_string[BUFSIZ], plain_model[BUFSIZ];
  char	family_string[BUFSIZ];
  int	rev_int = 0;
  int	cpu_count = 0;
  float megahertz = 0.0, bogomips = 0.0;
   
  vendor_string[0] = model_string[0] = plain_model[0] = family_string[0] = 0;
 
  /* We get all of our info here from /proc/cpuinfo */
  if ((fff = fopen(get_cpuinfo_file(), "r")) != NULL) {
     
      while ((fgets(temp_string, BUFSIZ, fff) != NULL)) {
	     /* Assume all CPU's in SMP system are the same */
	  if (cpu_count == 0) {
	      if (!(strncmp(temp_string, "vendor", 6)))
		 strncpy(vendor_string, parse_line(temp_string), BUFSIZ);
    	      if (!strncmp(temp_string, "family", strlen("family")))
		 strncpy(family_string, parse_line(temp_string), BUFSIZ);
	      /*
	      ** note for later check the model line
	      ** now this line is useless
	      */
	      if (!(strncmp(temp_string, "model", strlen("model")))) 
		 strncpy(model_string, parse_line(temp_string), BUFSIZ);
	      if (!(strncmp(temp_string, "revision", strlen("revision"))))
		 rev_int = atoi(parse_line(temp_string));
	      if (!(strncmp(temp_string, "cpu MHz", 6)))
		 megahertz = atof(parse_line(temp_string));
	  }
	  /* Ugh why must people play with capitalization */
	  if ( !(strncmp(temp_string, "bogomips", 8)) ||
	       !(strncmp(temp_string, "BogoMips", 8)) ||
	       !(strncmp(temp_string, "BogoMIPS", 8))) 
	    {
	      bogomips += atof(parse_line(temp_string));
	      cpu_count++;  /* Cheating way to detect number of intel CPU's */
	    }
	}
    }
       
  /* Re-arrange some of the strings for best results  */
  if (family_string[0] == 0) 
    strncpy(cpu_info->chip_type, "Unknown", 9);
  else 
    strncpy(cpu_info->chip_type, family_string, SYSINFO_CHIP_TYPE_SIZE);
   
  strncpy(cpu_info->chip_vendor, "Unknown", 9);
  
  cpu_info->num_cpus = cpu_count;
  cpu_info->bogomips = bogomips;
   
  /*********************************************************/
  /* Vendor specific fixups                                */
  /*********************************************************/
   
       
  /* *************** */
  /* Intel Chips     */
  /* *************** */
  if (!strncmp(vendor_string,"GenuineIntel",12)) {
     
      strncpy(cpu_info->chip_vendor,"Intel",6);
     
      if (rev_int >= 1 && rev_int <= 4)
	snprintf(cpu_info->chip_type,SYSINFO_CHIP_TYPE_SIZE, 
		"%s [B%i]", 
		clip_lf(family_string, BUFSIZ), rev_int - 1);
      else if (rev_int == 5)
	snprintf(cpu_info->chip_type,SYSINFO_CHIP_TYPE_SIZE,
		"%s [C0]", 
		clip_lf(family_string, BUFSIZ));
      else
	snprintf(cpu_info->chip_type,SYSINFO_CHIP_TYPE_SIZE,
		"%s", 
		clip_lf(family_string, BUFSIZ));
    }
   
  cpu_info->megahertz = 0.0;
   
  /* Handle Pretty_printing */
  if (get_pretty_printing()) {
      /* Fix MHz */
      /* should we still do this? */
      if (megahertz > 0.0)
	cpu_info->megahertz = fix_megahertz(25, megahertz);
    }
  else {
      /* restore RAW vendor string.  Do we want this? */
      strncpy(cpu_info->chip_type, model_string, SYSINFO_CHIP_TYPE_SIZE);
      cpu_info->megahertz = megahertz;
    }

  return 0;   
}

    /* Not implemented on ix86 nor on ia64 */
int get_hardware_info(char *hardware_string) {
    return 0;
}

    /* Some architectures might have better ways of detecting RAM size */
long long get_arch_specific_mem_size(void) {
       /* We have no special way of detecting RAM */
       return 0;
}
