/* Handles arm64 (aarch64)  chips on Linux architecture   */
/* by Vince Weaver <vince@deater.net>        */

/* Heavily based on the util-linux code by Riku Voipio */
/* see sys-utils/lscpu-arm.c */


/* ARM is lots of fun!  Possibly even worse than x86 */
/* + There isn't consistent vendor/model strings so we contain */
/*   a full list ourselves (thanks, kernel) */
/* + big/LITTLE means there can be multiple processor types */
/*   and even multiple vendors in a machine.  And some vendors */
/*   report this weird (both on one line rather than separate? */
/* + There is a Processor field and a processor field, which means */
/*   different things */
/* + The ARM devels got so annoyed with questions about BOGOMIPS */
/*   that they removed the field altogether, which messes with the parsing */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

/* Currently maxes out at 2 for Big/Little setups? */
/* I have one cpuinfo with 3?  Is it real? */
#define MAX_IMPLEMENTERS	3
#define MAX_PARTS		3

/* Known Vendors */
static struct vendor_list_type {
	int id;
	char name[20];
} vendor_list[] = {
	{ 0x41,	"ARM"},
	{ 0x42,	"Broadcom"},
	{ 0x43,	"Cavium"},
	{ 0x44,	"DEC"},
	{ 0x48,	"HiSilicon"},
	{ 0x4d, "Motorola"}, /* Freescale? */
	{ 0x4e,	"Nvidia"},
	{ 0x50,	"APM"},
	{ 0x51,	"Qualcomm"},
	{ 0x53,	"Samsung"},
	{ 0x54,	"TI"},	/* Texas Instruments */
	{ 0x56,	"Marvell"},
	{ 0x61,	"Apple"},
	{ 0x66,	"Faraday"},
	{ 0x69,	"Intel"},
	{ -1,	"Unknown"},
};


static void string_append(int which,char *new_part,char *string) {

	if (which==0) {
		strncpy(string,new_part,BUFSIZ);
	}
	else {
		strncat(string,"/",2);
		strncat(string,new_part,BUFSIZ);
	}
}

static void handle_arm(int which,int part,char *model_string) {

	switch(part) {
		case 0x810:
			string_append(which,"ARM810",model_string);
			break;
		case 0x920:
			string_append(which,"ARM920",model_string);
			break;
		case 0x922:
			string_append(which,"ARM922",model_string);
			break;
		case 0x926:
			string_append(which,"ARM926",model_string);
			break;
		case 0x940:
			string_append(which,"ARM940",model_string);
			break;
		case 0x946:
			string_append(which,"ARM946",model_string);
			break;
		case 0x966:
			string_append(which,"ARM966",model_string);
			break;
		case 0xa20:
			string_append(which,"ARM1020",model_string);
			break;
		case 0xa22:
			string_append(which,"ARM1022",model_string);
			break;
		case 0xa26:
			string_append(which,"ARM1026",model_string);
			break;
		case 0xb02:
			string_append(which,"ARM11 MPCore",model_string);
			break;
		case 0xb36:
			string_append(which,"ARM1136",model_string);
			break;
		case 0xb56:
			string_append(which,"ARM1156",model_string);
			break;
		case 0xb76:
			string_append(which,"ARM1176",model_string);
			break;
		case 0xc05:
			string_append(which,"Cortex-A5",model_string);
			break;
		case 0xc07:
			string_append(which,"Cortex-A7",model_string);
			break;
		case 0xc08:
			string_append(which,"Cortex-A8",model_string);
			break;
		case 0xc09:
			string_append(which,"Cortex-A9",model_string);
			break;
		case 0xc0d:	/* Originally A12 */
			string_append(which,"Cortex-A17",model_string);
			break;
		case 0xc0e:
			string_append(which,"Cortex-A17",model_string);
			break;
		case 0xc0f:
			string_append(which,"Cortex-A15",model_string);
			break;
		case 0xc14:
			string_append(which,"Cortex-R4",model_string);
			break;
		case 0xc15:
			string_append(which,"Cortex-R5",model_string);
			break;
		case 0xc17:
			string_append(which,"Cortex-R7",model_string);
			break;
		case 0xc18:
			string_append(which,"Cortex-R8",model_string);
			break;
		case 0xc20:
			string_append(which,"Cortex-M0",model_string);
			break;
		case 0xc21:
			string_append(which,"Cortex-M1",model_string);
			break;
		case 0xc23:
			string_append(which,"Cortex-M3",model_string);
			break;
		case 0xc24:
			string_append(which,"Cortex-M4",model_string);
			break;
		case 0xc27:
			string_append(which,"Cortex-M7",model_string);
			break;
		case 0xc60:
			string_append(which,"Cortex-M0+",model_string);
			break;
		case 0xd00:
			string_append(which,"Foundation",model_string);
			break;
		case 0xd01:
			string_append(which,"Cortex-A32",model_string);
			break;
		case 0xd03:
			string_append(which,"Cortex-A53",model_string);
			break;
		case 0xd04:
			string_append(which,"Cortex-A35",model_string);
			break;
		case 0xd05:
			string_append(which,"Cortex-A55",model_string);
			break;
		case 0xd07:
			string_append(which,"Cortex-A57",model_string);
			break;
		case 0xd08:
			string_append(which,"Cortex-A72",model_string);
			break;
		case 0xd09:
			string_append(which,"Cortex-A73",model_string);
			break;
		case 0xd0a:
			string_append(which,"Cortex-A75",model_string);
			break;
		case 0xd0b:
			string_append(which,"Cortex-A76",model_string);
			break;
		case 0xd0c:
			string_append(which,"Neoverse-N1",model_string);
			break;
		case 0xd13:
			string_append(which,"Cortex-R52",model_string);
			break;
		case 0xd20:
			string_append(which,"Cortex-M23",model_string);
			break;
		case 0xd21:
			string_append(which,"Cortex-M33",model_string);
			break;
		case 0xd4a:
			string_append(which,"Neoverse-E1",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_broadcom(int which, int part,char *model_string) {

	switch(part) {

		case 0x0f:
			string_append(which,"Brahma B15",model_string);
			break;
		case 0x100:
			string_append(which,"Brahma B53",model_string);
			break;
		case 0x516:
			string_append(which,"ThunderX2",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_cavium(int which, int part,char *model_string) {

	switch(part) {
		case 0x0a0:
			string_append(which,"ThunderX",model_string);
			break;
		case 0x0a1:
			string_append(which,"ThunderX 88XX",model_string);
			break;
		case 0x0a2:
			string_append(which,"ThunderX 81XX",model_string);
			break;
		case 0x0a3:
			string_append(which,"ThunderX 83XX",model_string);
			break;
		case 0x0af:
			string_append(which,"ThunderX2 99XX",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}


static void handle_dec(int which, int part,char *model_string) {

	switch(part) {
		case 0xa10:
			string_append(which,"SA110",model_string);
			break;
		case 0xa11:
			string_append(which,"SA1100",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_hisilicon(int which, int part,char *model_string) {

	switch(part) {
		case 0xd01:
			string_append(which,"Kunpeng-920",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_nvidia(int which, int part,char *model_string) {

	switch(part) {
		case 0x000:
			string_append(which,"Denver",model_string);
			break;
		case 0x003:
			string_append(which,"Denver 2",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_apm(int which, int part,char *model_string) {

	switch(part) {
		case 0x000:
			string_append(which,"X-Gene",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_qualcomm(int which, int part,char *model_string) {

	switch(part) {
		case 0x00f:
			string_append(which,"Scorpion",model_string);
			break;
		case 0x02d:
			string_append(which,"Scorpion",model_string);
			break;
		case 0x04d:
			string_append(which,"Krait",model_string);
			break;
		case 0x06f:
			string_append(which,"Krait",model_string);
			break;
		case 0x201:
			string_append(which,"Kryo",model_string);
			break;
		case 0x205:
			string_append(which,"Kryo",model_string);
			break;
		case 0x211:
			string_append(which,"Kryo",model_string);
			break;
		case 0x800:
			string_append(which,"Falkor V1/Kryo",model_string);
			break;
		case 0x801:
			string_append(which,"Kryo V2",model_string);
			break;
		case 0xc00:
			string_append(which,"Falkor",model_string);
			break;
		case 0xc01:
			string_append(which,"Saphira",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_samsung(int which, int part,char *model_string) {

	switch(part) {
		case 0x001:
			string_append(which,"Exynos-M1",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_ti(int which, int part,char *model_string) {

	switch(part) {
		case 0x915:
			string_append(which,"OMAP",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_marvell(int which, int part,char *model_string) {

	switch(part) {
		case 0x0131:
			string_append(which,"Feroceon 88FR131",model_string);
			break;
		case 0x0581:
			string_append(which,"PJ4/PJ4b",model_string);
			break;
		case 0x0584:
			string_append(which,"PJ4B-MP",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_faraday(int which, int part,char *model_string) {

	switch(part) {
		case 0x0526:
			string_append(which,"FA526",model_string);
			break;
		case 0x0626:
			string_append(which,"FA626",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

static void handle_intel(int which, int part,char *model_string) {

	switch(part) {
		case 0x200:
			string_append(which,"i80200",model_string);
			break;
		case 0x210:
			string_append(which,"PXA250A",model_string);
			break;
		case 0x212:
			string_append(which,"PXA210A",model_string);
			break;
		case 0x242:
			string_append(which,"i80321-400",model_string);
			break;
		case 0x243:
			string_append(which,"i80321-600",model_string);
			break;
		case 0x290:
			string_append(which,"PXA250B/PXA26x",model_string);
			break;
		case 0x292:
			string_append(which,"PXA210B",model_string);
			break;
		case 0x2c2:
			string_append(which,"i80321-400-B0",model_string);
			break;
		case 0x2c3:
			string_append(which,"i80321-600-B0",model_string);
			break;
		case 0x2d0:
			string_append(which,"PXA250C/PXA255/PXA26x",model_string);
			break;
		case 0x2d2:
			string_append(which,"PXA210C",model_string);
			break;
		case 0x2e2:
		case 0x2e3:
			string_append(which,"XScale",model_string);
			break;
		case 0x411:
			string_append(which,"PXA27x",model_string);
			break;
		case 0x41c:
			string_append(which,"IPX425-533",model_string);
			break;
		case 0x41d:
			string_append(which,"IPX425-400",model_string);
			break;
		case 0x41f:
			string_append(which,"IPX425-266",model_string);
			break;
		case 0x660:
			string_append(which,"XScale",model_string);
			break;
		case 0x682:
			string_append(which,"PXA32x",model_string);
			break;
		case 0x683:
			string_append(which,"PXA930/PXA935",model_string);
			break;
		case 0x688:
			string_append(which,"PXA30x",model_string);
			break;
		case 0x689:
			string_append(which,"PXA31x",model_string);
			break;
		case 0xb11:
			string_append(which,"SA1110",model_string);
			break;
		case 0xc12:
			string_append(which,"IPX1200",model_string);
			break;
		default:
			string_append(which,"Unknown",model_string);
			break;
	}
}

int get_cpu_info(struct cpu_info_type *cpu_info) {

	FILE *fff;
	char temp_string[BUFSIZ];
	char vendor_string[BUFSIZ],model_string[BUFSIZ],hardware_string[BUFSIZ];
	int cpu_count=0;
	float bogomips=0.0;
	int implementers[MAX_IMPLEMENTERS]={-1};
	int parts[MAX_PARTS]={-1},parts_vendor[MAX_PARTS]={-1};
	int num_implementers=0,num_parts=0;
	int i,v,temp_value,temp_vendor;

	vendor_string[0]=model_string[0]=hardware_string[0]=0;

	/* We get all of our info here from /proc/cpuinfo */
	if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {

		while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {

			if (( !(strncmp(temp_string,"CPU implementer",15))) ||
			   ( !(strncmp(temp_string,"CPU implementor",15)))) {
				strncpy(vendor_string,
					parse_line(temp_string),
					BUFSIZ-1);
				clip_lf(vendor_string,BUFSIZ);

				temp_value=strtol(vendor_string,NULL,16);
				for(i=0;i<num_implementers;i++) {
					if (temp_value==implementers[i]) break;
				}
				if (i==num_implementers) {
					implementers[num_implementers]=temp_value;
					num_implementers++;
				}
			}

			if ( !(strncmp(temp_string,"CPU part",8))) {
				strncpy(vendor_string,
					parse_line(temp_string),
					BUFSIZ-1);
				clip_lf(vendor_string,BUFSIZ);

				temp_value=strtol(vendor_string,NULL,16);
				if (num_implementers==0) {
					temp_vendor=-1;
				}
				else {
					temp_vendor=implementers[num_implementers-1];
				}

				for (i=0;i<num_parts;i++) {
					if ((temp_value==parts[i]) &&
						(temp_vendor=parts_vendor[i])) {
						break;
					}
				}

				if (i==num_parts) {
					parts[num_parts]=temp_value;
					parts_vendor[num_parts]=temp_vendor;
					num_parts++;
				}
			}


			if ( !(strncmp(temp_string,"Hardware",8))) {
				strncpy(model_string,
					parse_line(temp_string),
					BUFSIZ-1);
				clip_lf(model_string,BUFSIZ);
			}

			if ( !(strncmp(temp_string,"cpu",3))) {
				strncpy(model_string,
					parse_line(temp_string),
					BUFSIZ-1);
				clip_lf(model_string,BUFSIZ);
			}

			/* ARM no longer supports bogomips, urgh */
			if ( !(strncmp(temp_string,"bogomips",8)) ||
				!(strncmp(temp_string,"BogoMips",8)) ||
				!(strncmp(temp_string,"BogoMIPS",8))) {

				bogomips+=atof(parse_line(temp_string));
			}

			if ( !(strncmp(temp_string,"processor",9))) {
			  	/* Cheating way to detect number of CPUs */

				cpu_count++;
			}

		}
	}

	/* Attempt to Pretty-print as Linux doesn't do this for us */
	/* Based on sys-utils/lscpu-arm.c from the util-linux package */

	/* Make a string with all vendors in it */
	for(i=0;i<num_implementers;i++) {
		v=0;
		while(vendor_list[v].id!=-1) {
			if (implementers[i]==vendor_list[v].id) {
				string_append(i,vendor_list[v].name,
							vendor_string);
				break;
			}
			v++;
		}
		if (vendor_list[v].id==-1) {
			string_append(i,"Unknown",vendor_string);
		}
	}

	/* Probably an old chip, guess ARM */
	if (num_implementers==0) {
		string_append(i,"ARM",vendor_string);
	}

	strncpy_truncate(cpu_info->chip_vendor,vendor_string,
				SYSINFO_CHIP_VENDOR_SIZE);


	/* Make a string with all parts in it */
	for(i=0;i<num_parts;i++) {
		switch(parts_vendor[i]) {
			case 0x41: /* ARM */
				handle_arm(i,parts[i],model_string);
				break;

			case 0x42: /* Broadcom */
				handle_broadcom(i,parts[i],model_string);
				break;

			case 0x43: /* Cavium */
				handle_cavium(i,parts[i],model_string);
				break;

			case 0x44: /* DEC */
				handle_dec(i,parts[i],model_string);
				break;

			case 0x48: /* HiSilicon */
				handle_hisilicon(i,parts[i],model_string);
				break;

			case 0x4e: /* Nvidia */
				handle_nvidia(i,parts[i],model_string);
				break;

			case 0x50: /* APM */
				handle_apm(i,parts[i],model_string);
				break;

			case 0x51: /* Qualcomm */
				handle_qualcomm(i,parts[i],model_string);
				break;

			case 0x53: /* Samsung */
				handle_samsung(i,parts[i],model_string);
				break;

			case 0x54: /* TI */
				handle_ti(i,parts[i],model_string);
				break;

			case 0x56: /* Marvell */
				handle_marvell(i,parts[i],model_string);
				break;

			case 0x66: /* Faraday */
				handle_faraday(i,parts[i],model_string);
				break;

			case 0x69: /* Intel */
				handle_intel(i,parts[i],model_string);
				break;

			default:
				break;
		}
	}


	strncpy_truncate(cpu_info->chip_type,model_string,
				SYSINFO_CHIP_TYPE_SIZE);

	/* Older cpuinfo don't always do this right */
	if (cpu_count==0) cpu_count=1;

	cpu_info->num_cpus=cpu_count;
	cpu_info->megahertz=0.0;
	cpu_info->bogomips=bogomips;



	return 0;
}

int get_hardware(char *hardware_string) {

	char temp_string[BUFSIZ];
	FILE *fff;

	if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {

		while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {

			if (!(strncmp(temp_string,"Hardware",8))) {
				strncpy(hardware_string,parse_line(temp_string),
					SYSINFO_HARDWARE_STRING_SIZE);
			}
		}
	}
	return 1;
}

	/* Some architectures might have better ways of detecting RAM size */
long long get_arch_specific_mem_size(void) {
	/* We have no special way of detecting RAM */
	return 0;
}
