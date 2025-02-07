/* /////////////////////////////////////////////////////////////////////////
 * File:    include/collect-c/common/version.h
 *
 * Purpose: Common elements for collect-c library.
 *
 * Created: 4th February 2025
 * Updated: 4th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifdef __cplusplus
# error All collect-c components are C only
#endif

#ifndef __STDC_VERSION__
# error All collect-c components require Standard C
#endif

#if __STDC_VERSION__ < 199901L
# error collect-c requires C99 or later. If you are using Visual C++ you are encouraged to experiment with _MSC_VER for compatiblity and then submit patches to this project
#endif


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#define COLLECT_C_VER_MAJOR         0
#define COLLECT_C_VER_MINOR         0
#define COLLECT_C_VER_PATCH         0
#define COLLECT_C_VER_ALPHABETA     1

#define COLLECT_C_VER \
    (0\
        |   (   COLLECT_C_VER_MAJOR       << 24   ) \
        |   (   COLLECT_C_VER_MINOR       << 16   ) \
        |   (   COLLECT_C_VER_PATCH       <<  8   ) \
        |   (   COLLECT_C_VER_ALPHABETA   <<  0   ) \
    )


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

/** Obtains the value of COLLECT_C_VER at the time of compilation of the
 * library.
 */
uint32_t
collect_c_api_version(void);



/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

