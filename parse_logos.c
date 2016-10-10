#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include "logo_types.h"
#include "load_logo.h"


    /* People can put all kind of strange and wonderful ascii */
    /* characters in a logo that would royally confuse printf */
int sanitize_print_string(char *string, FILE *fff) {
   
   int i;
   
   for(i=0;i<strlen(string);i++) {
      switch (string[i]) {
       case '\n': fputc('\\',fff);
                  fputc('n',fff);
                  break;
       case '\t': fputc('\\',fff);
	          fputc('t',fff);
	          break;
       case '\\': fputc('\\',fff);
	          fputc('\\',fff);
	          break;
       case '%':  fputc('\\',fff);  /* I think %% works as well */
	          fputc('%',fff);
	          break;
       case '\r': fputc('\\',fff);
	          fputc('r',fff);
	          break;
       case '"':  fputc('\\',fff);
	          fputc('"',fff);
	          break;
       case '\'': fputc('\\',fff);
	          fputc('\'',fff);
	          break;
       default:   fputc(string[i],fff);
      }
   }
   return 0;
}

static void free_logo_memory(struct logo_info *logo_ptr) {
    free(logo_ptr->description);
    free(logo_ptr->name);
    free(logo_ptr->logo);
    free(logo_ptr->ascii_logo);
    free(logo_ptr);
}

int main(int argc, char **argv) {
 
    FILE *fff,*ggg;
 
    struct logo_info *logo_info_temp;
   
    char temp_st[BUFSIZ];
   
    int logo_number = 0;

#ifdef sun
    fff=fopen("logo_config.Solaris","r");   
#elif __sgi__
    fff=fopen("logo_config.Irix","r");
#elif __FreeBSD__
    fff=fopen("logo_config.BSD","r");
#else   
    fff=fopen("logo_config","r");
#endif   
    ggg=fopen("load_logos.h","w");

    printf("\nParsing logos from file \"logo_config\"...\n"); 
   
    if (ggg==NULL) {
       printf("\nError!  Cannot create load_logos.h!!!!\n\n");
       return 3;
    }
   
    if (fff==NULL) {
       printf("\nWarning!  No logo_config file exists!  No Logos will "
	      "be compiled in!\n\n");
       fprintf(ggg,"/* No logos configured */\n");
       fclose(ggg);
       return 4;
    }
    
    while (!feof(fff)) {
       if ( fgets(temp_st,BUFSIZ,fff) !=NULL) {
             /* Skip comments */
	  if (temp_st[0]=='#') break;
	  
	  temp_st[strlen(temp_st)-1]='\0';  /* Stupid fgets */
	  
	     /* Skip Empty lines in logo_config file */
	  if (strlen(temp_st)<1) break;
	  
          logo_info_temp=load_logo_from_disk(temp_st);
	  if (logo_info_temp==NULL) break;
	    
	  fprintf(ggg,"\t\t/* %s -- %s */\n",temp_st,logo_info_temp->description);
          fprintf(ggg,"\tnew_logo=calloc(1,sizeof(struct logo_info));\n");
	  
	  if (logo_number==0) {
	     fprintf(ggg,"\n\tlogo_info_head=new_logo;\n\n");  	     
	  } else {
	     fprintf(ggg,"\n\tlogo_info_tail->next_logo=new_logo;\n\n");
	  }
	  
	  fprintf(ggg,"\tnew_logo->description=strdup(\"%s\");\n",
		      logo_info_temp->description);
	  fprintf(ggg,"\tnew_logo->name=strdup(\"%s\");\n",logo_info_temp->name);
	  fprintf(ggg,"\tnew_logo->logo=strdup(\"");
	  sanitize_print_string(logo_info_temp->logo,ggg);
	  fprintf(ggg,"\");\n");
	  fprintf(ggg,"\tnew_logo->ysize=%d;\n",logo_info_temp->ysize);
	  fprintf(ggg,"\tnew_logo->ascii_logo=strdup(\"");
	  sanitize_print_string(logo_info_temp->ascii_logo,ggg);
	  fprintf(ggg,"\");\n");
	  fprintf(ggg,"\tnew_logo->ascii_ysize=%d;\n",
		      logo_info_temp->ascii_ysize);
	  fprintf(ggg,"\tnew_logo->sysinfo_position=%d;\n",
		      logo_info_temp->sysinfo_position);
	  fprintf(ggg,"\tnew_logo->next_logo=NULL;\n");
	  fprintf(ggg,"\tlogo_info_tail=new_logo;\n");
          logo_number++;
	  printf("+ Added logo %s containing \"%s\"\n",
		 temp_st,logo_info_temp->description);
             /* Free memory allocated to current logo */
          free_logo_memory(logo_info_temp);
       }
    } 
    printf("\n");
    return 0;
}
