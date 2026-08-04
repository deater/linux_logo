/* Handles loongarch chips on Linux architecture  */
/* by JiaLing Zhang <zhangjialing@loongson.cn>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

	FILE *fff;
	char temp_string[BUFSIZ];
	char vendor_string[BUFSIZ],model_string[BUFSIZ];
	int cpu_count=0;
	float megahertz=0.0,bogomips=0.0;

	vendor_string[0]=model_string[0]=0;

	strncpy(vendor_string,"Loongson",9);

	/* We get all of our info here from /proc/cpuinfo */
	if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {

		while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {

			if (cpu_count==0) {

				if ( !(strncmp(temp_string,"Model Name",10))) {
					strncpy(model_string,parse_line(temp_string),BUFSIZ-1);
					clip_lf(model_string,BUFSIZ);
				}

				if ( !(strncmp(temp_string,"CPU MHz",7))) {
					megahertz=atof(parse_line(temp_string));
				}
			}

			/* Ugh why must people play with capitalization */
			if ( !(strncmp(temp_string,"bogomips",8)) ||
					!(strncmp(temp_string,"BogoMips",8)) ||
					!(strncmp(temp_string,"BogoMIPS",8))) {
				bogomips+=atof(parse_line(temp_string));
				cpu_count++;  /* Cheating way to detect number of CPUs */
			}
		}
	}

	strncpy_truncate(cpu_info->chip_vendor,vendor_string,SYSINFO_CHIP_VENDOR_SIZE);
	strncpy_truncate(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);

	cpu_info->num_cpus=cpu_count;
	cpu_info->megahertz=megahertz;
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
