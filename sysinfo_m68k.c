/* getsysinfo.c                                                   *\
\* I was trying to make this easier to add other platforms/       */
/* architectures.  Feel free to add yours, and send me the patch. *\
\*----------------------------------------------------------------*/
/* Initial gernic Linux and Irix -- Vince Weaver                  *\
\* Added Linux mc6800 support    -- Christian Marillat            */
/* Added Cyrix 6x86 support"     -- Adam J. Thornton              */

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <string.h>

int external_bogomips(char *bogomips_total);

/* The following utility functions stolen from my game SEABATTLE */

int my_string_comp(const char *cs, const char *ct)
{                         /* partly borrowed /usr/src/linux/lib/string.c */
      register signed char __res;   /* like strcmp, but case-insensitive    */
   
      while(1) {
	 if ((__res = toupper(*cs)-toupper(*ct++))!=0 || !*cs++) break;
      }
      return __res;
}

char *read_string_from_disk(FILE *fff,char *string)
{                                 /* My own, SUPERIOR version of fgets */
     int ch,i=0;                     /* Handles \n much better */
     char temp[150];
   
     strcpy(temp,"");
     while ((ch=fgetc(fff))==' ');
     while ( (ch!='\n') && (ch!=EOF) ) {
	        temp[i]=ch; i++;
	        ch=fgetc(fff);
     }
     if(ch==EOF) return NULL;
     temp[i]='\0';
     strcpy(string,temp);
     return string;
}


void get_os_info(char *os_name,char *os_version,char *os_revision,char *host_name)
{  
   struct utsname buf;
   uname( &buf);

   strcpy(os_name,buf.sysname);
   strcpy(os_version,buf.release);   
   strcpy(os_revision,buf.version);
   strcpy(os_revision,"Compiled ");
   strcat(os_revision,buf.version);
   strcpy(host_name,buf.nodename);
   /* 
   printf("machine: %s\n",buf.machine);
   printf("domain:  %s\n",buf.domainname);*/
 }
    

void get_hardware_info(char *cpuinfo,char *bogo_total,int skip_bogomips)
{
   FILE *fff;
   int cpus=0;
   struct stat buff;
   long long int mem;
   float bogomips=0.0;
   char temp_string2[40],model[15],vendor[30],chip[20];
   char model2[15],clock[30];
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */
   
   if ((fff=fopen("/proc/hardware","r") )!=NULL) {
      while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	 if (cpus==0) {
	    if ( !(strcmp(temp_string2,"Model:")) ) {
		 fscanf(fff,"%s",(char *)&model);
		 fscanf(fff,"%s",(char *)&model2);

	    }
            if ( !(strcmp(temp_string2,"CPU:")) ){
		 fscanf(fff,"%s",(char *)&chip);
	    }
			if ( !(strcmp(temp_string2,"Clocking:")) ) {
			fscanf(fff,"%s",(char *)&clock);
		}
	 }
	 if ( !(my_string_comp(temp_string2,"BogoMips:")) ) {
	    fscanf(fff,"%f",&bogomips);
	 }
      }
   }

   stat("/proc/kcore",&buff);
   mem=buff.st_size;
   mem/=1024; mem/=1024;
	    
      sprintf(cpuinfo,"%s @ %s %s %s %ldM RAM",chip,clock,model,model2,(long int)mem);
      sprintf(bogo_total,"%.2f Bogomips",bogomips);      
    
      /* End Linux */
 	 
}
      
