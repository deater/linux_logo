#define SYSINFO_RIGHT  0
#define SYSINFO_BOTTOM 1

struct logo_info {
      char *description;
      char *name;
      int default_is_ascii;
      int sysinfo_position;
      char *logo;
      char *ascii_logo;
      int ysize;
      int ascii_ysize;
      struct logo_info *next_logo;
};
