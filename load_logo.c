#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logo_types.h"

    /* Loads a logo from disk */
struct logo_info *load_logo_from_disk(char *filename) {

	struct logo_info *new_logo;
	int logo_start=0,ascii_logo_start=0;
	int ascii_size=0,size=0;
	char temp_st[BUFSIZ];
	FILE *fff;
	char *result;

	new_logo=calloc(1,sizeof(struct logo_info));

	if (new_logo==NULL) {
		printf("Error allocating memory!\n");
		return NULL;
	}

	fff=fopen(filename,"r");
	if (fff==NULL) {
		printf("Error loading logo: %s\n",filename);
		return NULL;
	}

	new_logo->logo=NULL;
	new_logo->ascii_logo=NULL;

	while (!feof(fff)) {
		result=fgets(temp_st,BUFSIZ,fff);
		if (result==NULL) {
			break;
		}

		if (!strncmp(temp_st,"END_LOGO",8)) logo_start=0;
		if (!strncmp(temp_st,"END_ASCII_LOGO",14)) ascii_logo_start=0;

		if (logo_start) {
			size+=strlen(temp_st);

			if (new_logo->logo==NULL) {
				new_logo->logo=strdup(temp_st);
			}
			else {
				new_logo->logo=realloc(new_logo->logo,size+1);
				if (new_logo->logo==NULL) {
					printf("realloc of size %d failed!\n",size+1);
					return NULL;
				}
				memcpy(new_logo->logo+strlen(new_logo->logo),
						temp_st,strlen(temp_st)+1);
			}
			new_logo->ysize++;
		}

		if (ascii_logo_start) {
			ascii_size+=strlen(temp_st);

			if (new_logo->ascii_logo==NULL) {
				new_logo->ascii_logo=strdup(temp_st);
			}
			else {
				new_logo->ascii_logo=realloc(
					new_logo->ascii_logo,ascii_size+1);
				memcpy( new_logo->ascii_logo
						+strlen(new_logo->ascii_logo),
						temp_st,strlen(temp_st)+1);
			}
			new_logo->ascii_ysize++;
		}

		if (!strncmp(temp_st,"BEGIN_ASCII_LOGO",16)) {
			ascii_logo_start=1;
		}
		if (!strncmp(temp_st,"BEGIN_LOGO",10)) {
			logo_start=1;
		}

		if ( (!ascii_logo_start) && (!logo_start) ) {
			if (!strncmp(temp_st,"SYSINFO_POSITION",16)) {
				if (!strncmp(temp_st+17,"bottom",6)) {
					new_logo->sysinfo_position=
								SYSINFO_BOTTOM;
				}
				if (!strncmp(temp_st+17,"right",5)) {
					new_logo->sysinfo_position=
								SYSINFO_RIGHT;
				}
			}
			if (!strncmp(temp_st,"DESCRIPTION_STRING",18)) {
				new_logo->description=strdup(temp_st+19);
				new_logo->description[strlen(
						new_logo->description)-1]=0;
			}
			if (!strncmp(temp_st,"NAME",4)) {
				new_logo->name=strdup(temp_st+5);
				new_logo->name[strlen(new_logo->name)-1]=0;
			}
		}
	}

	new_logo->next_logo=NULL;

	fclose(fff);

	return new_logo;
}
