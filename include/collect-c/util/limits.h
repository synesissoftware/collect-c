/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/util/limits.h
 *
 * Purpose: Type-detecting limits functions/macros.
 *
 * Created: 7th June 2025
 * Updated: 10th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <limits.h>


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros (internal)
 */

/* Associations use only distinct built-in types. stdint typedefs (int16_t,
 * int64_t, size_t, …) match via their underlying type — listing both would
 * make _Generic reject duplicate compatible types (ABI-dependent).
 */

#define COLLECT_C_LIMITS_maximum_(v)    ( _Generic((v),     \
                                                            \
        signed char         : (SCHAR_MAX),                  \
        signed char      *  : (SCHAR_MAX),                  \
        signed char const*  : (SCHAR_MAX),                  \
      unsigned char         : (UCHAR_MAX),                  \
      unsigned char      *  : (UCHAR_MAX),                  \
      unsigned char const*  : (UCHAR_MAX),                  \
                                                            \
               short        : (SHRT_MAX),                   \
               short      * : (SHRT_MAX),                   \
               short const* : (SHRT_MAX),                   \
      unsigned short        : (USHRT_MAX),                  \
      unsigned short      * : (USHRT_MAX),                  \
      unsigned short const* : (USHRT_MAX),                  \
                                                            \
                 int        : (INT_MAX),                    \
                 int      * : (INT_MAX),                    \
                 int const* : (INT_MAX),                    \
        unsigned int        : (UINT_MAX),                   \
        unsigned int      * : (UINT_MAX),                   \
        unsigned int const* : (UINT_MAX),                   \
                                                            \
                long        : (LONG_MAX),                   \
                long      * : (LONG_MAX),                   \
                long const* : (LONG_MAX),                   \
       unsigned long        : (ULONG_MAX),                  \
       unsigned long      * : (ULONG_MAX),                  \
       unsigned long const* : (ULONG_MAX),                  \
                                                            \
           long long        : (LLONG_MAX),                  \
           long long      * : (LLONG_MAX),                  \
           long long const* : (LLONG_MAX),                  \
  unsigned long long        : (ULLONG_MAX),                 \
  unsigned long long      * : (ULLONG_MAX),                 \
  unsigned long long const* : (ULLONG_MAX),                 \
                                                            \
                char   **** : 0                             \
))

#define COLLECT_C_LIMITS_minimum_(v)    ( _Generic((v),     \
                                                            \
        signed char         : (SCHAR_MIN),                  \
        signed char      *  : (SCHAR_MIN),                  \
        signed char const*  : (SCHAR_MIN),                  \
      unsigned char         : (0),                          \
      unsigned char      *  : (0),                          \
      unsigned char const*  : (0),                          \
                                                            \
               short        : (SHRT_MIN),                   \
               short      * : (SHRT_MIN),                   \
               short const* : (SHRT_MIN),                   \
      unsigned short        : (0),                          \
      unsigned short      * : (0),                          \
      unsigned short const* : (0),                          \
                                                            \
                 int        : (INT_MIN),                    \
                 int      * : (INT_MIN),                    \
                 int const* : (INT_MIN),                    \
        unsigned int        : (0),                          \
        unsigned int      * : (0),                          \
        unsigned int const* : (0),                          \
                                                            \
                long        : (LONG_MIN),                   \
                long      * : (LONG_MIN),                   \
                long const* : (LONG_MIN),                   \
       unsigned long        : (0),                          \
       unsigned long      * : (0),                          \
       unsigned long const* : (0),                          \
                                                            \
           long long        : (LLONG_MIN),                  \
           long long      * : (LLONG_MIN),                  \
           long long const* : (LLONG_MIN),                  \
  unsigned long long        : (0),                          \
  unsigned long long      * : (0),                          \
  unsigned long long const* : (0),                          \
                                                            \
                char   **** : 0                             \
))


#define COLLECT_C_LIMITS_maximum(v)                         ((void)sizeof(v), COLLECT_C_LIMITS_maximum_(v))

#define COLLECT_C_LIMITS_minimum(v)                         ((void)sizeof(v), COLLECT_C_LIMITS_minimum_(v))


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */
