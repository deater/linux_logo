char *get_cpuinfo_file(void);
char *parse_line(char *string);
char *clip_lf(char *string,int size);
int get_pretty_printing(void);

float fix_megahertz(int factor,float megahertz);
float external_bogomips();
