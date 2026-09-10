/* /////////////////////////////////////////////////////////////////////////
 * File:    src/string_fns.c
 *
 * Purpose: Common string helpers.
 *
 * Created: 19th February 2025
 * Updated: 10th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common.h>

#include <string.h>

#if 0
#elif 0 ||\
      defined(__MACH__) ||\
      defined(__linux) ||\
      defined(__linux__) ||\
      defined(linux) ||\
      0

# ifndef STLSOFT_INCL_H_STRINGS
#  define STLSOFT_INCL_H_STRINGS
#  include <strings.h>
# endif /* !STLSOFT_INCL_H_STRINGS */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * internal functions & macros
 */

#ifndef STLSOFT_NS_GLOBAL_
# define STLSOFT_NS_GLOBAL_(x)                              x
#endif


/* *************************************
 * stricmp - this borrowed from STLSoft's include/stlsoft/api/external/string.h, which I also wrote
 */

#ifndef STLSOFT_API_EXTERNAL_string_stricmp
# if 0

  /* Microsoft Visual C++ */
# elif defined(_MSC_VER)

#  define STLSOFT_API_EXTERNAL_string_stricmp               STLSOFT_NS_GLOBAL_(_stricmp)

  /* Borland */
# elif defined(__BORLANDC__)

#  if 0
#  elif !defined(__STDC__)

#   define STLSOFT_API_EXTERNAL_string_stricmp              STLSOFT_NS_GLOBAL_(stricmp)
#  elif !defined(__MFC_COMPAT__)

#   define STLSOFT_API_EXTERNAL_string_stricmp              STLSOFT_NS_GLOBAL_(_stricmp)
#  endif

  /* Digital Mars */
# elif defined(__DMC__)

#  define STLSOFT_API_EXTERNAL_string_stricmp               STLSOFT_NS_GLOBAL_(stricmp)

  /* GCC / MinGW */
# elif defined(__GNUC__)

#  if 0
#  elif 0 ||\
        defined(__MINGW32__) ||\
        defined(__MINGW64__) ||\
        defined(__MINGW__) ||\
        0

#   define STLSOFT_API_EXTERNAL_string_stricmp              STLSOFT_NS_GLOBAL_(_stricmp)
#  elif defined(STLSOFT_INCL_H_STRINGS)

#   define STLSOFT_API_EXTERNAL_string_stricmp              STLSOFT_NS_GLOBAL_(strcasecmp)
#  endif

  /* Intel (Windows) */
# elif defined(__INTEL_COMPILER) &&\
       defined(_WIN32)

#  if 0
#  elif !defined(__STDC__)

#   define STLSOFT_API_EXTERNAL_string_stricmp              STLSOFT_NS_GLOBAL_(stricmp)
#  else

#   define STLSOFT_API_EXTERNAL_string_stricmp              STLSOFT_NS_GLOBAL_(_stricmp)
#  endif

  /* Metrowerks CodeWarrior */
# elif defined(__MWERKS__)

#  define STLSOFT_API_EXTERNAL_string_stricmp               STLSOFT_NS_GLOBAL_(stricmp)

  /* Watcom */
# elif defined(__WATCOMC__)

#  define STLSOFT_API_EXTERNAL_string_stricmp               STLSOFT_NS_GLOBAL_(_stricmp)
# endif
#endif /* !STLSOFT_API_EXTERNAL_string_stricmp */

#ifndef STLSOFT_API_EXTERNAL_string_stricmp
# ifdef _WIN32
#  define STLSOFT_API_EXTERNAL_string_stricmp               STLSOFT_NS_GLOBAL_(lstrcmpiA)
# endif
#endif /* !STLSOFT_API_EXTERNAL_string_stricmp */


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef STLSOFT_API_EXTERNAL_string_stricmp

int
collect_c_stricmp(
    char const*     lhs
,   char const*     rhs
)
{
    return STLSOFT_API_EXTERNAL_string_stricmp(lhs, rhs);
}
#endif


/* ///////////////////////////// end of file //////////////////////////// */

