/* sysinfo_common.h                                               *\
\* Just trying to cut back on overly-repeated code.  --vmw        */


/* The following utility functions stolen from my game SEABATTLE */

int my_string_comp(const char *cs, const char *ct)
{                           /* partly borrowed /usr/src/linux/lib/string.c */
      register signed char __res;  /* like strcmp, but case-insensitive    */
   
      while(1) {
	 if ((__res = toupper(*cs)-toupper(*ct++))!=0 || !*cs++) break;
      }
      return __res;
}

char *read_string_from_disk(FILE *fff,char *string)
{                                 /* My own version of fgets */
     int ch,i=0;                  /* Handles \n much better */
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

char *linux_get_proc_uptime (char *string) 
{            /* This code modeled on the linux sh-utils 1.16 uptime.c code */
    FILE *fff;
    float uptime_seconds;
    int up_days,up_hrs,up_mins;
    char temp_string[BUFSIZ];
   
    fff=fopen("/proc/uptime","r");
    if (fff!=NULL) {
       fscanf(fff,"%f",&uptime_seconds);
       fclose (fff);
    
       up_days=uptime_seconds/86400;
       up_hrs=(uptime_seconds-(up_days*86400))/3600;
       up_mins=(uptime_seconds-(up_days*86400)-(up_hrs*3600))/60;
  
       if (up_days<=0) 
	  sprintf(temp_string,"Uptime %d %s %d %s",
		  up_hrs,(up_hrs==1 ? "hour":"hours"),
		  up_mins,(up_mins==1 ? "minute":"minutes"));
       else 
	  sprintf(temp_string,"Uptime %d %s %d %s %d %s",
		  up_days,(up_days==1 ? "day":"days"),
		  up_hrs,(up_hrs==1 ? "hour":"hours"),
		  up_mins,(up_mins==1 ? "minute":"minutes"));
       strcpy(string,temp_string);
       return string;
    }
    return NULL;
}

    /* Code contributed by Anders Rundegren <anders@rundegren.com> */
char *linux_get_proc_loadavg(char *string)
{
    FILE *fff;
    float load_1;
    float load_5;
    float load_15;
    char temp_string[BUFSIZ];
   
    fff=fopen("/proc/loadavg","r");
    if (fff!=NULL) {
       fscanf(fff,"%f" "%f" "%f", &load_1, &load_5, &load_15);
       fclose (fff);
	
       sprintf(temp_string,"Load average %4.2f, %4.2f, %4.2f",
	       load_1,load_5,load_15);
       strcpy(string,temp_string);
       return string;
    }
    return NULL;
}
   
char *get_loadavg_noproc(char *string) 
{
    /* Unfortunately it seems getting the load-average is platform *\
    \* dependant.                                                  */
    strncpy(string,"\0",1);
    return string;  
}

char *utmp_get_uptime(char *string) 
{
    /* To implement uptime on architechtures w/o a /proc/uptime one *\
    \* has to scan the /var/utmp file.  Very annoying thing to do   */
    /* Check out the linux sh-utils uptime.c for a reference.       *\
    \* Currently not implemented here.                              */
    strncpy(string,"\0",1);
    return string;
   
}
