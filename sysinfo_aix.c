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
   strcpy(os_version,buf.version);   /* switched around, JSt */
   strcpy(os_revision,"Revision ");
   strcat(os_revision,buf.release);
   strcpy(host_name,buf.nodename);
   
  /* printf("machine: %s\n",buf.machine);
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
   char temp_string[80],bogomips_total[30];
   float total_bogo=0.0;
   /*Anyone have more than 9 cpu's yet?*/	
	char ordinal[10][10]={"Zero","One","Two","Three","Four","Five","Six",
	                      "Seven","Eight","Nine"};
   
/* Print CPU Type and BogoMips -- Handles SMP Correctly now            *\  
\* To debug other architectures, create copies of the  proc files and  */ 
/*   fopen() them.                                                    */

       /*      sprintf(cpuinfo,"Unknown CPU");*/
    if ((fff=popen("lsattr -El proc0","r") )!=NULL) {
       while ( fscanf(fff,"%s",(char *)&temp_string2)!=EOF) {
	  if ( !(strcmp(temp_string2,"type")) ) { 
/* moved &chip, JSt
   output of lsattr -El proc0 gives
state enable Processor state False
type  POWER2 Processor type  False
   on AIX 4.1.5 systems
 */
	     fscanf(fff,"%s%s%s%s%s",(char *)&chip,(char *)&temp_string,
		   (char *)&temp_string,(char *)&temp_string,(char *)&temp_string);
	  }
       }
    }
    pclose(fff);

/* count cpus ... ugly using wc, JSt */
    if ((fff=popen("lsdev -Cc processor -SA|wc -l","r") )!=NULL) {
	     fscanf(fff,"%d",&cpus);
	  }
    pclose(fff);
/* check mem, JSt */
    if ((fff=popen("lsattr -E -l sys0 -a realmem -F value","r") )!=NULL) {
	     fscanf(fff,"%ld",&mem);
	  }
    pclose(fff);

    sprintf(cpuinfo,"%s %s Processor%s %ldM RAM",ordinal[cpus],chip,(cpus>1)?"s,":",",mem/1024);
  	    
      if (!skip_bogomips)
         if ( (external_bogomips( (char *)&bogomips_total))==-1 )
         sprintf(bogo_total," ");
         else sprintf(bogo_total,"%s Bogomips Total",bogomips_total);
      else sprintf(bogo_total," ");
	 
}
      
