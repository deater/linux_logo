/* This is the defaults.h file that can be used to change the default *\
\* Behavior of the linux_logo program.                                */

/* This option picks the default mode of the linux_logo program.            *\
\* If a 1 is picked, banner mode will be the default mode.                  */
/* If a 0 is picked, classic mode (which some people prefer)will be default.*\
\* You can over-ride this with the -banner and -classic switches.           */
#define DEFAULT_BANNER_MODE 1

/* By setting this option to 1 you can have linux_logo always run in plain *\
\* ascii mode.                                                             */
#define DEFAULT_PLAIN_ASCII 0

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

/* If you don't like the standard look of linux_logo you can alter the two *\
\* defines below.  Be sure to use only one character, and they must be     */
/* Surrounded by single quotes.  You can over-ride these defaults with the *\
\* -rX and -kX command line options.                                       */
#define DEFAULT_SYMBOL '#'
#define DEFAULT_SYMBOL_BGND '#'

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
