     /* */
char *get_sysinfo_version(char *version);
int get_numeric_sysinfo_version(void);

    
typedef struct {
    char os_name[33]; 
    char os_version[33]; 
    char os_revision[65];
} os_info_t;

int get_os_info(os_info_t *os_info);


/* host name */
char *get_host_name(char hostname[65],char domain[65]);

 /* uptime  (seconds the system has been up) */
int get_uptime(void);

/* load average  ( average load across 1, 5 and 15 minutes) */
void get_load_average(float *load_1,float *load_5,float *load_15);

typedef struct {
   int num_cpus;
   float megahertz;
   float bogomips;
   char chip_vendor[33];
   char chip_type[64];
} cpu_info_t;

int get_cpu_info(cpu_info_t *cpu_info);

    /* Hardware info string */
int get_hardware_info(char hardware_string[65]);

    /* mem_size (in megabytes)  (will this work on > 4Gig systems?) */
long int get_mem_size(void);
   
    /* Use if the generic mem_size routines don't work for your arch */
long int get_arch_specific_mem_size(void);

/* Debugging */
int set_cpuinfo_file(char *filename);
void set_pretty_printing(int value);
long int get_mem_size_sysinfo(void);
