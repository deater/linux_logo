/* Yes this is a bit messy, but it cleans up the makefile a bit *\
\* The C-Preproccessor can be out friend ;)                     */

#ifdef MREE
#undef MREE 
#endif

#if defined(linux)
#define MREE 1

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
#else
#include "sysinfo_ix86.c"
#endif 
       /* Linux */
#elif defined (SGI)
#define MREE 1
#include "sysinfo_irix.c"
#endif

#ifdef AIX
#define MREE 1
#include "sysinfo_aix.c"
#endif

#ifndef MREE
#include "sysinfo_default.c"
#endif
