/* Handles the regrettable RISC-V architecture   */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

	FILE *fff;
	char temp_string[BUFSIZ];
	char vendor_string[BUFSIZ],model_string[BUFSIZ],hardware_string[BUFSIZ];
	int cpu_count=0;
	float bogomips=0.0;

	vendor_string[0]=model_string[0]=hardware_string[0]=0;

	/* We get all of our info here from /proc/cpuinfo */
	if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {

		while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {

			/* No Processor string? */
			if ( !(strncmp(temp_string,"isa",3))) {
				strncpy(model_string,parse_line(temp_string),BUFSIZ-1);
                                clip_lf(model_string,BUFSIZ);
			}

			/* No Bogomips */

			/* Count cores with hart */
			if ( !(strncmp(temp_string,"hart",4))) {
				cpu_count++;
			}
		}
	}

	strncpy(vendor_string,"RISC-V",BUFSIZ);

	strncpy_truncate(cpu_info->chip_vendor,vendor_string,SYSINFO_CHIP_VENDOR_SIZE);
	strncpy_truncate(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);

	cpu_info->num_cpus=cpu_count;
	cpu_info->megahertz=0.0;
	cpu_info->bogomips=bogomips;

	return 0;

}

int get_hardware(char *hardware_string) {

	return -1;
}

/* Some architectures might have better ways of detecting RAM size */

long long get_arch_specific_mem_size(void) {
	/* We have no special way of detecting RAM */
	return 0;
}
