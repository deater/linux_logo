/* This is the defaults.h file that can be used to change the default *\
\* Behavior of the linux_logo program.                                */


/* This is the default format of the output file.  See the README for more *\
\* information on how to configure this option.                            */
#define DEFAULT_BANNER_FORMAT "#O Version #V, Compiled #C\n#N #M#X#T Processor#S, #R RAM, #B Bogomips Total\n#H\n"
#define DEFAULT_CLASSIC_FORMAT "#O Version #V\nCompiled #C\n#N #M#X#T Processor#S, #R RAM\n#B Bogomips Total\n#H\n"

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

/* If you don't like the standard look of linux_logo you can alter the two *\
\* defines below.  Be sure to use only one character, and they must be     */
/* Surrounded by single quotes.  You can over-ride these defaults with the *\
\* -rX and -kX command line options.                                       */
#define DEFAULT_SYMBOL '#'
#define DEFAULT_SYMBOL_BGND '#'

/* Here you can change the default language with which to say how many   *\
\* Processors you have.  I have invcluded the only two languages I know  */

#define ENGLISH 0
#define DEUTSCH 1
#define NEDERLANDS 2
#define ITALIANO 3
#define FRENCH 4
#define LANGUAGE ENGLISH

#if LANGUAGE==ENGLISH
char ordinal[11][10]={"Zero","One","Two","Three","Four","Five","Six",
	                                 "Seven","Eight","Nine","Many"};
#elif LANGUAGE==DEUTSCH
char ordinal[11][10]={"Null","Ein","Zwei","Drei","Vier","Fuenf","Sechs",
                                          "Sieben","Acht","Neun","Viele"};
#elif LANGUAGE==NEDERLANDS
char ordinal[11][10]={"Nul","Een","Twee","Drie","Vier","Vijf","Zes",
                                         "Zeven","Acht","Negen","Veel"};
#elif LANGUAGE==ITALIANO
char ordinal[11][10]={"Zero","Uno","Due","Tre","Quattro","Cinque","Sei",
                                            "Sette","Otto","Nove","Tanti"};
#elif LANGUAGE==FRENCH
char ordinal[11][10]={"Aucun","Un","Deux","Trois","Quatre","Cinq","Six",
                                          "Sept","Huit","Neuf","Plusieurs" };
#endif

/* --------------------Logo File Defaults-----------------------------------*/

/* You can replace the names of the files pointed to here as long as the  *\
\* files included have a similar [almost identical] setup to the standard */
/* Linux logo files.                                                      */

/* This file contains the ascii version of the classic logo */
#include "ascii_penguin.h"

/* This file contains the ascii version of the banner logo */
#include "ascii_banner.h"

/* This file contains the regular version of the banner logo */
#include "banner.h"

/* This is a bit messy.. it tries to auto-figure out what version     *\
\* of the classic logo to load.  There is the linux one, the AIX one, */
/* And the default non-linux one.                                     */
#ifdef IRIX_ANSI     /* It's easy to create a custom Ansi */
#include "irix.h"    /* Just make it with an ansi-making tool (i.e. TheDraw) */

#elif LINUX_ANSI     /* it MUST be 16 lines long.  Any width.  */
#include "penguin.h" /* The first 6 lines can be longer */

#elif AIX
#include "aix.h"

#elif CUSTOM_ANSI    /* The rest of the lines should be padded to the same */
#include "custom.h"  /* Length so the output is lined up*/
#else                /* Just stick quotation marks around the ansi (as in */
#include "non-linux.h" /*    the included .h files) and it should work */
#endif
