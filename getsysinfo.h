struct os_info_type {
      char *os_name;
      char *os_version;
      char *os_revision;
      char *host_name;
      char *uptime;
      char *load_average;
};

struct hw_info_type {
      int num_cpus;
      char *bogo_total;
      char *megahertz;
      char *cpu_vendor;
      char *cpu_type;
      char *mem_size;
};

struct linux_logo_info_type {
       int width;
       int no_periods;
       int preserve_xy;
       int skip_bogomips;
       int offset;
       int plain_ascii;
       int banner_mode;
       int wipe_screen;
       int show_uptime;
       int show_load;
       int narrow_logo;
       int display_logo_only;
       int display_sysinfo_only;
       int display_usertext;
       int custom_format;
       int pretty_output;
       char symbol;
       char symbol_bgnd;
       char *user_text;
       char *cpuinfo_file;
       char *format;
};

void get_os_info(struct os_info_type *os_info);
void get_hw_info(struct hw_info_type *hw_info,
		 struct linux_logo_info_type *logo_info);
