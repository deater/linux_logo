/*--------------------------------------------------------------------------
 LINUX LOGO 0.1 -- Creates a Nifty Logo With some System Info - 9 July 1997
     by Vince Weaver (weave@eng.umd.edu, http://www.glue.umd.edu/~weave )
		  
  perfect if you want a Penguin on Boot Up, but not in the kernel.
  Just stick this file in the rc files somewhere.
 
  The file is ANSI escape sequences, so it should work OK with most terminals.
 
  BUGS: Not tested with non-x86 proc files
        Will SMP work?  It would be nice to show both CPU's, but I don't
                        have a SMP box to work with.
 
 The image was created using the logo.xpm from the Linux distribution,
     ppm2ansi by Carsten Haitzler -- http://www.cse.unsw.edu.au/~s2154962/
     and was hand edited using THEDRAW under dosemu 0.66.7
 
-------------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define ESCAPE '\033'

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


int main(int argc,char **argv)
{
   FILE *fff;
   char line[40][50];
   int i,num_words;
    
   if(argc!=1) {
     printf("\nLinux Logo Version 0.1 -- by Vince Weaver (weave@eng.umd.edu)\n");  
     printf("   Newest Versions at http://www.glue.umd.edu/vmwprod and Sunsite\n\n");
     return 1; 		   
   }
   
   
/* Print graphic made with ppm2ansi and THEDRAW [under dosemu] */   
   
   printf("[40m[40m\n");
   printf("[0;1;30;44m........[37m.%c[40m      %c[44m.[30m......[40m\n",ESCAPE,ESCAPE);
   printf("[44m........[40m        %c[44m......[2C[47mLL[5CIIIIII[1CNN[3CNN[1CUU[2CUU[1CXX[2CXX[40m\n",ESCAPE);
   printf("[30;44m........[40m [47m  [40m [47m  %c[40m  [44m......[2C[47mLL[7CII[3CNNN[2CNN[1CUU[2CUU[2CXXXX[40m\n",ESCAPE);
   printf("[30;44m........[40m [47m [40m   %c[47m [40m   [44m.....[2C[47mLL[7CII[3CNNNN[1CNN[1CUU[2CUU[3CXX[40m\n",ESCAPE);
   printf("[30;44m........[40m  [43m    [40m   [44m.....[2C[47mLL[7CII[3CNN[1CNNNN[1CUU[2CUU[2CXXXX[40m\n");
   printf("[30;44m........[40m [0;30;43m`----'[40m  [1;44m.....[2C[47mLLLLLL[1CIIIIII[1CNN[3CNN[2CUUUU[2CXX[2CXX[40m\n");
   printf("[30;44m........[40m [47m.[37m....[30m.[40m   [44m....[40m\n");
   printf("[44m.......[40m [47m...:...:[40m    [44m..[40m");

/* Get the linux version and some other info from /proc/version */   
   fff=fopen("/proc/version","r");
     num_words=0;
     while( (fscanf(fff,"%s",&line[num_words]))!=EOF) num_words++;
   fclose(fff);

/* Print Version */
   printf("%c[2C%c[37;40mVersion %s%c[30;40m\n",ESCAPE,ESCAPE,line[2],ESCAPE);
   printf("[44m.......[40m [47m.......::[40m    [44m.[40m");

/* Print Compile Time */
   printf("%c[2C%c[37;40mCompiled %s %s %s",ESCAPE,ESCAPE,line[8],line[9],line[10]);
   printf(" %s %s %s%c[30;40m\n",line[11],line[12],line[13],ESCAPE);
   printf("[44m.......[40m [47m.........[40m    [37;44m.[40m");

/* Print CPU Type and BogoMips [how does SMP work with this? */   
   fff=fopen("/proc/cpuinfo","r");
   while ( fscanf(fff,"%s",&line[0])!=EOF) {
      if ( !(strcmp(line[0],"cpu")) ){
	   fscanf(fff,"%s%s",&line[1],&line[2]);
	   printf("%c[2C%s ",ESCAPE,line[2]);
      }
      if ( !(strcmp(line[0],"model")) ) {
	   fscanf(fff,"%s",&line[1]);
	   read_string_from_disk(fff,&line[2]);
	   printf("%s\n",line[2]);
	   printf("%c[44m......%c[40m %c[47m..........%c[40m     ",ESCAPE,ESCAPE,ESCAPE,ESCAPE);
      }
      if ( !(my_string_comp(line[0],"bogomips")) ) {
	   fscanf(fff,"%s%s",&line[1],&line[2]);
	   printf("%c[2C%s Bogomips\n",ESCAPE,line[2]);
      }
   }
   fclose(fff);
   printf("[44m.....[40m  [47m..........[40m     ");
   
/* Print Host Name [domain name wasn't working.....?] */   
   gethostname(&line[0],45);
   printf("%c[2C%s\n",ESCAPE,line[0]);
   printf("[44m.....[43m   [30;47m........[43m   [40m   \n");
   printf("[44m...[43m     [47m........[43m    [40m  \n");
   printf("[44m.[43m       [47m........[43m      %c[40m\n",ESCAPE);
   printf("[44m.[43m       [40m [47m......[40m [43m      %c[40m\n",ESCAPE);
   printf("[0m[255D\n");
   return 0;
}
