/* Re-written from scratch 3 March 2001      */
/* Handles x86 and x86_64 chips on Linux     */
/* by Vince Weaver <vince@deater.net>        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* atof */

#include "../sysinfo.h"
#include "../include/generic.h"

int get_cpu_info(struct cpu_info_type *cpu_info) {

    FILE *fff;
    char temp_string[BUFSIZ];
    char vendor_string[BUFSIZ],model_string[BUFSIZ],plain_model[BUFSIZ];
    int family=0;
    int cpu_count=0;
    float megahertz=0.0,bogomips=0.0;
   
    vendor_string[0]=model_string[0]=plain_model[0]=0;
 
       /* We get all of our info here from /proc/cpuinfo */
    if ((fff=fopen(get_cpuinfo_file(),"r") )!=NULL) {
       
       while ( (fgets(temp_string,BUFSIZ,fff)!=NULL) ) {
	     
	  if (cpu_count==0) {	/* Assume all CPU's in SMP system are the same */
	     if ( !(strncmp(temp_string,"vendor_id",9)) ||
		  !(strncmp(temp_string,"vid",3)) ) { /* 1.2.13 kernels */
		strncpy(vendor_string,parse_line(temp_string),BUFSIZ);
	     }
     
	     if ( !(strncmp(temp_string,"model name",9)) ) {
		strncpy(model_string,parse_line(temp_string),BUFSIZ);
		clip_lf(model_string,BUFSIZ);
	     }
	     else {  /* for older (pre 2.2.x) kernels */
		if (!(strncmp(temp_string,"model",5))) {
		   if (strncmp(parse_line(temp_string),"unknown",7)) {
		      strncpy(plain_model,parse_line(temp_string),BUFSIZ);
		      clip_lf(plain_model,BUFSIZ);
		   }
		}
	     }
	     
	     if ( !(strncmp(temp_string,"cpu MHz",6))) {
		megahertz=atof(parse_line(temp_string));
	     }
	     
	        /* We use this for cpu's w/o cpuinfo (386s and 486s) */
	     if ( !(strncmp(temp_string,"cpu family",10))) {
		   family=atoi(parse_line(temp_string));
	     }
	        /* Old legacy stuff (2.0.x kernels and earlier) */
	     if ( !(strncmp(temp_string,"cpu   ",5))) {
		strncpy(plain_model,parse_line(temp_string),BUFSIZ);
		clip_lf(plain_model,255);
	     }
	     
	  }
	     /* Ugh why must people play with capitalization */
	  if ( !(strncmp(temp_string,"bogomips",8)) ||
	       !(strncmp(temp_string,"BogoMips",8)) ||
	       !(strncmp(temp_string,"BogoMIPS",8))) {
	     bogomips+=atof(parse_line(temp_string));
	     cpu_count++;  /* Cheating way to detect number of intel CPU's */
	  }
       }
    }
       
       /* Re-arrange some of the strings for best results  */
    if (model_string[0]==0) {  /* For 1.2.13 Kernels */
       if (strlen(plain_model)>1) strncpy(model_string,plain_model,BUFSIZ);
    }

    if (model_string[0]==0) {
       strncpy(cpu_info->chip_type,"Unknown",9);
    }
    else strncpy(cpu_info->chip_type,model_string,SYSINFO_CHIP_TYPE_SIZE);
   
    strncpy(cpu_info->chip_vendor,"Unknown",9);
  
    /* sanity */
    if (cpu_count<1) cpu_count=1;
   
    cpu_info->num_cpus=cpu_count;
    cpu_info->bogomips=bogomips;
   
    /*********************************************************/
    /* Vendor specific fixups                                */
    /*********************************************************/
   
       
       /* ************** */
       /* AMD chips      */
       /* ************** */
    if (!strncmp(vendor_string,"AuthenticAMD",12)) {
       strncpy(cpu_info->chip_vendor,"AMD",4);

          /* Clean-up K6 model info */
       if (strstr(model_string,"K6")!=NULL) {
	     /* Default to K6 */
	  strncpy(cpu_info->chip_type,"K6",3);
      
	     /* Original K6 */
	  if ( !(strncmp(model_string,"AMD-K6tm",8)))
	     strncpy(cpu_info->chip_type,"K6",3);
	
	     /* Counter-intuitive, but the following are correct */
	  if ( !(strncmp(model_string,"AMD-K6(tm)-III",14)))
	     strncpy(cpu_info->chip_type,"K6-2+",6);
	  else if ( !(strncmp(model_string,"AMD-K6(tm)",10)))
	     strncpy(cpu_info->chip_type,"K6-2",5);
	  if ( !(strncmp(model_string,"K6-2",4)))
	     strncpy(cpu_info->chip_type,"K6-2",5);
	  if (strstr(model_string,"3D+")!=NULL) {
	     strncpy(cpu_info->chip_type,"K6-III",7);
	  }
       }
     
          /* K5 Chips */
       if (strstr(model_string,"K5")!=NULL) {
	  strncpy(cpu_info->chip_type,"K5",3);
       }
 
          /* Some old K7's were reported as such */
       if (strstr(model_string,"K7")!=NULL) {
	  strncpy(cpu_info->chip_type,"Athlon",7);
       }

          /* Athlons */
       if (strstr(model_string,"Athlon(tm) 64 X2")!=NULL) {
	  strncpy(cpu_info->chip_type,"Athlon 64 X2",13);
       } else
       if (strstr(model_string,"Athlon(tm) 64")!=NULL) {
	  strncpy(cpu_info->chip_type,"Athlon 64",10);
       } else
       if (strstr(model_string,"Athlon")!=NULL) {
	  strncpy(cpu_info->chip_type,"Athlon",7);
       }
       
          /* Specialty Athlons */
          /* Should we support MP too?  */
          /* Should we decode the Speed ratings too (ie, 1600+) */
       if (!strncmp(model_string,"AMD Athlon",10)) {
	  if (strstr(model_string,"XP")!=NULL) {
	     strncpy(cpu_info->chip_type,"Athlon XP",10);
	  }
       }

          /* Durons */
       if (strstr(model_string,"Duron")!=NULL) {
	  strncpy(cpu_info->chip_type,"Duron",6);
       }
          
          /* Unknown */
       if (strstr(model_string,"Unknown")!=NULL) {
	  strncpy(cpu_info->chip_type,"Unknown",8);
       }
       
          /* Sempron */
       if (strstr(model_string,"Sempron")!=NULL) {
	 strncpy(cpu_info->chip_type,"Sempron",8);
       }
       
          /* Turion */
       if (strstr(model_string,"Turion")!=NULL) {
	 strncpy(cpu_info->chip_type,"Turion",8);
       }       
              
          /* Opterons */
       if (strstr(model_string,"Opteron")!=NULL) {
	  strncpy(cpu_info->chip_type,"Opteron",8);
       }
       
          /* Phenom */
       if (strstr(model_string,"Phenom")!=NULL) {
	  if (strstr(model_string,"II")!=NULL) {
	     strncpy(cpu_info->chip_type,"Phenom II",10);
	  }
	  else {
	     strncpy(cpu_info->chip_type,"Phenom",7);
	  }
       }
       
          /* Geode */
       if ( strstr(model_string,"Geode")!=NULL) {
	  strncpy(cpu_info->chip_type,"Geode",6);
       }
       
          /* Handle BIOS/kernel too old issue */
       if ( strstr(model_string,"unknown")!=NULL) {
	  strncpy(cpu_info->chip_type,"Unknown",8);
       }
       
          /* Work around old kernels */
       if (model_string[0]==0) {
	  if (plain_model[0]=='9') {
	     strncpy(cpu_info->chip_type,"K6-III",7);
	  }
       }
       
    }
   
       /* ***************** */
       /* Centaur Chips     */
       /* ***************** */
    if ( !(strncmp(vendor_string,"CentaurHauls",12))) {
		  /* centaur cpuid is fully customizable, but this */
                  /* should catch the common case                  */
       strncpy(cpu_info->chip_vendor,"Centaur",8);
       
       /* Should this be generic?  We should never print processor twice */
       {
	  char *processor_begin;
          processor_begin=strstr(model_string," Processor");
	  if (processor_begin!=NULL) {
	     /* I hate to use pointer math */
	    model_string[(processor_begin-model_string)]='\0';
	  }
       }

   
          /* Is this the proper thing to do?  It looks silly */
          /* calling them "Centaur VIA" chips                */
       if (strstr(model_string,"VIA")!=NULL) {
	  strncpy(cpu_info->chip_vendor,"VIA",4);	  
	  strncpy(cpu_info->chip_type,model_string+4,SYSINFO_CHIP_TYPE_SIZE);
       }
       
          /* Esther */
       if (strstr(model_string,"Esther")!=NULL) {
	  strncpy(cpu_info->chip_type,"Esther",7);
       }       

    }

       /* *************** */
       /* Cyrix Chips     */
       /* *************** */
    if (!strncmp(vendor_string,"CyrixInstead",12)) {
       strncpy(cpu_info->chip_vendor,"Cyrix",6);

       if ( strstr(model_string,"MediaGX")!=NULL) 
	  strncpy(cpu_info->chip_type,"MediaGX",8);

       if ( strstr(model_string,"Geode")!=NULL)
	  strncpy(cpu_info->chip_type,"Geode",6);
       
       if ( !(strncmp(model_string,"6x86",4)))
	  strncpy(cpu_info->chip_type,"6x86",5);
       if ( !(strncmp(model_string,"6x86M",5)))
	  strncpy(cpu_info->chip_type,"6x86MMX",8);
       
       if ( !(strncmp(model_string,"M III",5))) 
	  strncpy(cpu_info->chip_type,"MIII",5);
       else if ( !(strncmp(model_string,"M II",4))) 
	  strncpy(cpu_info->chip_type,"MII",4);
    }

       /* *************** */
       /* Intel Chips     */
       /* *************** */
    if (!strncmp(vendor_string,"GenuineIntel",12)) {
       strncpy(cpu_info->chip_vendor,"Intel",6);

          /* Handle the various Pentium types */
       if (!(strncmp(model_string,"Pentium",7))) {
	  if (strstr(model_string,"75")!=NULL) {
	     strncpy(cpu_info->chip_type,"Pentium",8);
	  }
	  if (strstr(model_string,"90")!=NULL) {
	     strncpy(cpu_info->chip_type,"Pentium",8);  
	  }
	  if (strstr(model_string,"Pro")!=NULL) {
	     strncpy(cpu_info->chip_type,"Pentium Pro",12);
	  }
	  if (strstr(model_string,"II")!=NULL) {
	     strncpy(cpu_info->chip_type,"Pentium II",11);
	  }
	  if (strstr(model_string,"III")!=NULL) {
	     strncpy(cpu_info->chip_type,"Pentium III",12);
	  }
	  if (strstr(model_string,"IV")!=NULL) {
	     strncpy(cpu_info->chip_type,"Pentium IV",11);
	  }
       }
       
          /* Now handle the ones with annoying (R) and (TM) */
       if (strstr(model_string,"Pentium(R) M")!=NULL) {
	  strncpy(cpu_info->chip_type,"Pentium M",10);
       }
       if (strstr(model_string,"Pentium(R) III")!=NULL) {
	  strncpy(cpu_info->chip_type,"Pentium III",12);
       }
       
       if (strstr(model_string,"Xeon(TM) MP")!=NULL) {
	  strncpy(cpu_info->chip_type,"Xeon MP",8);
       }
              
          /* Atom */
       if (strstr(model_string,"Atom")!=NULL) {
	  strncpy(cpu_info->chip_type,"Atom",5);
       }       
       

          /* Should we handle all the various Celeron */
          /* types separately??                       */
       if (strstr(model_string,"Celeron(R) M")!=NULL) {
	  strncpy(cpu_info->chip_type,"Celeron M",10);
       }
       else if (strstr(model_string,"Celeron")!=NULL) {
	  strncpy(cpu_info->chip_type,"Celeron",8);
       }
       
          /* Handle values with annoying "Intel(R)" */
       if (strstr(model_string,"Intel(R)")!=NULL) {
       
	     /* Pentium 4 */
	  if (strstr(model_string,"Pentium(R) 4")) {
	     strncpy(cpu_info->chip_type,"Pentium 4",10);
          }
	  
	     /* Pentium D */
	  if (strstr(model_string,"Pentium(R) D")) {
	     strncpy(cpu_info->chip_type,"Pentium D",10);
          }	  
	     /* Mobile P4 */
	  if (strstr(model_string,"Mobile Intel(R) Pentium(R) 4")!=NULL) {
             if (strstr(model_string,"Pentium(R) 4 - M")!=NULL) {
	        strncpy(cpu_info->chip_type,"Pentium 4 M",12);
	     }
	     else {
	        strncpy(cpu_info->chip_type,"Mobile Pentium 4",17);
	     }
	  }

	     /* Xeons */
             /* TODO - determine base type based on model id? */
          if ( !(strncmp(model_string,"Intel(R) Xeon(TM) CPU",21)) ||
	       !(strncmp(model_string,"Intel(R) Xeon(R) CPU",20)) || 
	       !(strncmp(model_string,"Intel(R) Genuine CPU",20)) || 
	       !(strncmp(model_string,"Genuine Intel(R) CPU",20)) || 
	       !(strncmp(model_string,"Intel(R) XEON(TM)",17)) ||
	       !(strncmp(model_string,"Intel(R) Xeon(TM)",17))
	      ) {
	     strncpy(cpu_info->chip_type,"Pentium Xeon",16);
          }
	     /* Nehalem */
          if (strstr(model_string,"Core(TM) i7")!=NULL) {
	     strncpy(cpu_info->chip_type,"i7",3);
	  }
          if (strstr(model_string,"Core(TM) i5")!=NULL) {
	     strncpy(cpu_info->chip_type,"i5",3);
	  }
          if (strstr(model_string,"Core(TM) i3")!=NULL) {
	     strncpy(cpu_info->chip_type,"i3",3);
	  }
	     /* Core and Core2 */
	  if (strstr(model_string,"Core(TM)2 Duo")!=NULL) {
	     strncpy(cpu_info->chip_type,"Core2 Duo",10);
	  }
	  else if (strstr(model_string,"Core(TM)2 Quad")!=NULL) {
	     strncpy(cpu_info->chip_type,"Core2 Quad",11);
	  }
	  else if (strstr(model_string,"Core(TM)2")!=NULL) {
	     strncpy(cpu_info->chip_type,"Core2",6);
	  }	 /* UGH!  I hate this stupid TXXX naming */
	  else if ( (strstr(model_string,"T2300")!=NULL) ||
		    (strstr(model_string,"T2400")!=NULL) ||
		    (strstr(model_string,"T2500")!=NULL) ||
		    (strstr(model_string,"T2600")!=NULL)) {
	     strncpy(cpu_info->chip_type,"Core Duo",9);
          }
	  else if (strstr(model_string,"E2180")!=NULL) {
	    strncpy(cpu_info->chip_type,"Core2 Duo",10);
	  }
	  else if (strstr(model_string,"2140")!=NULL) {
	    strncpy(cpu_info->chip_type,"Core2 Duo",10);
	  }
       }
    
          /* Fix up some older kernels */
       if (model_string[0]==0) {
	  if (plain_model[0]=='5') {  
	     strncpy(cpu_info->chip_type,"Pentium II",11);  
	  }
       }
       if (!strncmp(model_string,"00/07",5)) {
	  strncpy(cpu_info->chip_type,"Pentium III",12);
       }
          
          /* Virtual Machines simulated by QEMU */
       if (strstr(model_string,"QEMU")!=NULL) {
	  strncpy(cpu_info->chip_type,"QEMU Virtual",13);
       }
    }
   
       /* ************* */
       /* NexGen        */
       /* ************* */
    if ( !(strncmp(vendor_string,"NexGenDriven",12))) {
       strncpy(cpu_info->chip_vendor,"NexGen",7);
    }
    
       /* ********************************** */
       /* National Semiconductor Geode Chips */
       /* ********************************** */
    if ( !(strncmp(vendor_string,"Geode by NSC",12))) {
       strncpy(cpu_info->chip_vendor,"NSC",4);
       strncpy(cpu_info->chip_type,"Geode",6);
    }
   
       /* ************* */
       /* Rise          */
       /* ************* */
    if ( !(strncmp(vendor_string,"RiseRiseRise",12))) {
       strncpy(cpu_info->chip_vendor,"Rise",5);
    }

       /* ************** */
       /* SiS            */
       /* ************** */
    if ( !(strncmp(vendor_string,"SiS SiS SiS",11))) {
       strncpy(cpu_info->chip_vendor,"SiS",4);
       if (family!=0) {
	  sprintf(temp_string,"%i86",family);
          strncpy(cpu_info->chip_type,temp_string,4);
       }
    }

   
       /* ************* */
       /* Transmeta     */
       /* ************* */
    if ( !(strncmp(vendor_string,"GenuineTMx86",12)) ||
	 !(strncmp(vendor_string,"TransmetaCPU",12))) {
       strncpy(cpu_info->chip_vendor,"Transmeta",10); 
	    
       if (strstr(model_string,"Crusoe")!=NULL) {
	  strncpy(cpu_info->chip_type,"Crusoe",7);
       }
    }
   
       /* ************** */
       /* UMC            */
       /* ************** */
    if ( !(strncmp(vendor_string,"UMC UMC UMC",11))) {
       strncpy(cpu_info->chip_vendor,"UMC",4);
       
       if (!(strncmp(model_string,"SX",2))) {
	  strncpy(cpu_info->chip_type,"486SX",6);
       }
    }

       /* ************** */
       /* Vortex         */
       /* ************** */
    if ( !(strncmp(vendor_string,"Vortex86 SoC",11))) {
       strncpy(cpu_info->chip_vendor,"Vortex",7);
       if (family!=0) {
	  sprintf(temp_string,"%i86",family);
          strncpy(cpu_info->chip_type,temp_string,4);
       }
    }
   
       /* ****************************** */
       /* Try to handle cpus w/o cpuinfo */
       /* ****************************** */
    if ( !(strncmp(cpu_info->chip_vendor,"Unknown",7))) {
       if (family!=0) {
	  sprintf(temp_string,"%i86",family);
          strncpy(cpu_info->chip_type,temp_string,4);
       }
       else {
	  if (plain_model!=NULL) { 
	     strncpy(cpu_info->chip_type,plain_model,SYSINFO_CHIP_TYPE_SIZE);
	  }
       }
    }
   
    cpu_info->megahertz=0.0;
   
       /* Handle Pretty_printing */
    if (get_pretty_printing()) {
       /* Fix MHz */
       if (megahertz>0.0)
          cpu_info->megahertz=fix_megahertz(25,megahertz);
    }
    else {
       /* restore RAW vendor string.  Do we want this? */
       strncpy(cpu_info->chip_type,model_string,64);
       cpu_info->megahertz=megahertz;
    }

    return 0;   
}

    /* Not implemented on ix86 */
int get_hardware_info(char *hardware_string) {
   
    return 0;
}

    /* Some architectures might have better ways of detecting RAM size */
long long get_arch_specific_mem_size(void) {
    /* We have no special way of detecting RAM */
    return 0;
}
