/* vmw_string.h                                               *\
\* string code I tend to re-use               .  --vmw        */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* The following utility functions taken from my game SEABATTLE */

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
        char temp[BUFSIZ];
   
        strcpy(temp,"");
        while ((ch=fgetc(fff))==' ');
        while ( (ch!='\n') && (ch!=EOF) && (i<((BUFSIZ-6)-1)) ) {
	   if (ch!='\r') {                
	      temp[i]=ch; 
	      i++;
	   }
	   ch=fgetc(fff);
	}
        if(ch==EOF) return NULL;
        temp[i]='\0';
        strncpy(string,temp,BUFSIZ-5);
        return string;
}

char *read_string_from_disk_exact(FILE *fff,char *string)
{                                 /* My own version of fgets */
        int ch,i=0;                  /* Handles \n much better */
        char temp[BUFSIZ];
   
        strcpy(temp,"");
        ch=fgetc(fff);
        while ( (ch!='\n') && (ch!=EOF) && (i<((BUFSIZ-6)-1)) ) {
	   if (ch!='\r') {
	      temp[i]=ch; 
	      i++;
	   }   
	   ch=fgetc(fff);
	}
        if(ch==EOF) return NULL;
        if (ch=='\n') {
	   temp[i]='\n';
	   temp[i+1]='\0';
	} 
        else temp[i]='\0';
        strncpy(string,temp,BUFSIZ-5);
        return string;
}


    /* a strcat that returns how big the result is. */
    /* AUDIT should I use strncat? */
int vmw_strcat(char *dest,char *src) {

    if (src!=NULL) {
       strcat(dest,src);
       return strlen(src);
    }
    else return 0;
}


