/* Yes this is a bit messy, but it cleans up the makefile a bit *\
\* The C-Preproccessor can be out friend ;)                     */

#if defined (linux)

#if defined(__mips__)
#include "sysinfo_mips.c"
#elif defined(__PPC__)
#include "sysinfo_ppc.c"
/* #elif defined(mc68000) -Acpu(m68k)*/
#elif (defined(mc68000) || #cpu(m68k))
#include "sysinfo_m68k.c"
#elif  defined(__alpha__)
#include "sysinfo_alpha.c"
#elif defined(__sparc__)
#include "sysinfo_sparc.c"
#elif defined(__arm__)
#include "sysinfo_arm.c"
#else
#include "sysinfo_ix86.c"
#endif 

#else
#include "sysinfo_default.c"
#endif
