     /* */
char *get_sysinfo_version(char *version);
int get_numeric_sysinfo_version(void);

#define SYSINFO_OS_NAME_SIZE     32
#define SYSINFO_OS_VERSION_SIZE  32
#define SYSINFO_OS_REVISION_SIZE 64
#define SYSINFO_HOSTNAME_SIZE    64
#define SYSINFO_DOMAIN_SIZE      64
#define SYSINFO_CHIP_VENDOR_SIZE 32
#define SYSINFO_CHIP_TYPE_SIZE   64
#define SYSINFO_HARDWARE_STRING_SIZE 64

struct os_info_type {
    char os_name[SYSINFO_OS_NAME_SIZE+1]; 
    char os_version[SYSINFO_OS_VERSION_SIZE+1]; 
    char os_revision[SYSINFO_OS_REVISION_SIZE+1];
};

int get_os_info(struct os_info_type *os_info);


/* host name */
char *get_host_name(char *hostname,char *domain);

 /* uptime  (seconds the system has been up) */
int get_uptime(void);

/* load average  ( average load across 1, 5 and 15 minutes) */
void get_load_average(float *load_1,float *load_5,float *load_15);

struct cpu_info_type {
   int num_cpus;
   float megahertz;
   float bogomips;
   char chip_vendor[SYSINFO_CHIP_VENDOR_SIZE+1];
   char chip_type[SYSINFO_CHIP_TYPE_SIZE+1];
};

int get_cpu_info(struct cpu_info_type *cpu_info);

    /* Hardware info string */
int get_hardware_info(char *hardware_string);

#define MEM_USE_SYSINFO -1
#define MEM_USE_MEMINFO -2

    /* mem_size (in megabytes)  (will this work on > 4Gig systems?) */
long long get_mem_size(void);
   
    /* Use if the generic mem_size routines don't work for your arch */
long long get_arch_specific_mem_size(void);
long long get_mem_size_sysinfo(void);
long long get_mem_size_meminfo(void);
long long get_mem_size_iomem(void);
long long get_mem_size_stat(void);

/* Debugging */
int set_cpuinfo_file(char *filename);
void set_pretty_printing(int value);

