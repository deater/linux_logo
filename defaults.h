/* This is the defaults.h file that can be used to change the default *\ 
\* Behavior of the linux_logo program.                                */ 

#define S_(String) String

/* This is the default format of the output file.  See the README for more *\ 
\* information on how to configure this option.                            */ 
char DEFAULT_BANNER_FORMAT[]= S_("#O Version #V, Compiled #C\n#N #M #X #T #P, #R RAM, #B Bogomips Total\n#H\n"); 
char DEFAULT_CLASSIC_FORMAT[]= S_("#O Version #V\nCompiled #C\n#N #M #X #T #P, #R RAM\n#B Bogomips Total\n#H\n"); 
/* Plural.  With the coming of i18n you really can't assume plurals can */ 
/* be made by adding a string to the end.  Hopefully this can fix it for*/ 
/* most languages.  If they require significant word-order changes then */ 
/* we'll have even more problems */ 
char PROCESSOR_SINGULAR[]= S_("Processor"); 
char PROCESSOR_PLURAL[]= S_("Processors"); 
 
/* This option picks the default mode of the linux_logo program.            *\ 
\* If a 1 is picked, banner mode will be the default mode.                  */ 
/* If a 0 is picked, classic mode (which some people prefer)will be default.*\ 
\* You can over-ride this with the -banner and -classic switches.           */ 
#define DEFAULT_BANNER_MODE 1 
 
/* By setting this option to 1 you can have linux_logo always run in plain *\ 
\* ascii mode.                                                             */ 
#define DEFAULT_PLAIN_ASCII 0 
 
/* This mode sets whether you want the output to defaultly display uptime  *\ 
\* or not.  This is useful if you are using linux_logo on the fly or in    */ 
/* Server side includes; it is not useful if you are using linux_logo to   *\ 
\* create motd or issue files.                                             */ 
#define DEFAULT_SHOW_UPTIME 0 
 
/* This mode sets whether you want the load average displayed by default. */ 
#define DEFAULT_SHOW_LOAD 0 
 
/* Default width of the screen.  Change if you wish to use a wider screen. *\ 
\* This option is over-ridden by the -wX command line option               */ 
#define DEFAULT_WIDTH 80  
 
/* This Option, if set to 1, makes the Classic Logo appear better to some *\ 
\* people.  It has no affect on the banner mode logo.                     */ 
#define DEFAULT_NO_PERIODS 0  
 
/* This option will make linux_logo default to saving cursor position *\ 
\* if the option is set to 1.                                         */ 
#define DEFAULT_PRESERVE_XY 0  
 
/* This option will make linux_logo skip the bogo-mips check on non-linux   *\ 
\* platforms if set to 1.  This can speed up linux_logo on these platforms. */ 
#define DEFAULT_SKIP_BOGOMIPS 0 
 
/* Use this to set the default offset of the logo towards the middle of the *\ 
\* screen.  You can override this with the -oNum command line option.       */ 
#define DEFAULT_OFFSET 0 
 
/* By setting this option to 1, linux_logo will default to clearing the *\ 
\* screen each time the program is run.                                 */ 
#define DEFAULT_WIPE_SCREEN 0 
 
/* By setting this the logo only is 79 chars wide, instead of 80... this *\ 
\* can help if linux_logo used to make an issue.net file                 */ 
#define DEFAULT_NARROW_LOGO 0 
 
/* By setting this, the system_info parsing will "tidy up" your system   *\ 
\* info.  That is, try to make Megahertz's on proper numbers, report your*/ 
/* K6 as a K6 and not the "AMD K6 with Multimedia Extensions" that       *\ 
\* /proc/cpuinfo returns, etc.  I reccommend you keep it on.             */ 
#define DEFAULT_PRETTY_OUTPUT 1 
 
