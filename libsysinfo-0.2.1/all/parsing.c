#include <string.h>

char empty_string[]="";

    /* take line, give back part after colon */
char *parse_line(char *string) {
    char *temp_ptr; 
    temp_ptr=strstr(string,":");
    if (temp_ptr==NULL) {
       /* special handling... atoi, aof, strcmp, etc can't handle NULL */
       /* Found via zzuf fuzz checking */
       return empty_string;
    }
       /* skip past the colon */
    temp_ptr++;
       /* skip any white-space */
    while (( temp_ptr[0] == ' ') || ( temp_ptr[0] == '\t')) temp_ptr++;
    return temp_ptr;
}

char *clip_lf(char *string,int size) {
    char *temp_ptr;
    int i=0;
   
    temp_ptr=string;
  
    while ((*temp_ptr!=0) && (*temp_ptr!='\n') && (i<(size-1))) {  
       i++;
       temp_ptr++;
    }
    *temp_ptr=0;
    return string;
}
