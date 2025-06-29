/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/util/limits.h
 *
 * Purpose: Type-detecting limits functions/macros.
 *
 * Created: 7th June 2025
 * Updated: 7th June 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <limits.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros (internal)
 */

#define COLLECT_C_LIMIT_ENTRY_short_(slim, ulim)            \
                                                            \
               short        : (slim),                       \
               short      * : (slim),                       \
               short const* : (slim),                       \
      unsigned short        : (ulim),                       \
      unsigned short      * : (ulim),                       \
      unsigned short const* : (ulim),

#define COLLECT_C_LIMIT_ENTRY_int_(slim, ulim)              \
                                                            \
                 int        : (slim),                       \
                 int      * : (slim),                       \
                 int const* : (slim),                       \
        unsigned int        : (ulim),                       \
        unsigned int      * : (ulim),                       \
        unsigned int const* : (ulim),

#define COLLECT_C_LIMIT_ENTRY_long_(slim, ulim)             \
                                                            \
                long        : (slim),                       \
                long      * : (slim),                       \
                long const* : (slim),                       \
       unsigned long        : (ulim),                       \
       unsigned long      * : (ulim),                       \
       unsigned long const* : (ulim),

#define COLLECT_C_LIMIT_ENTRY_long_long_(slim, ulim)        \
                                                            \
           long long        : (slim),                       \
           long long      * : (slim),                       \
           long long const* : (slim),                       \
  unsigned long long        : (ulim),                       \
  unsigned long long      * : (ulim),                       \
  unsigned long long const* : (ulim),

#if 0
#elif defined(__clang__)

# undef COLLECT_C_LIMIT_ENTRY_short_
# define COLLECT_C_LIMIT_ENTRY_short_(slim, ulim)
# undef COLLECT_C_LIMIT_ENTRY_int_
# define COLLECT_C_LIMIT_ENTRY_int_(slim, ulim)
# undef COLLECT_C_LIMIT_ENTRY_long_long_
# define COLLECT_C_LIMIT_ENTRY_long_long_(slim, ulim)
#elif defined(__GNUC__)

#elif defined(_MSC_VER)

#endif


#define COLLECT_C_LIMITS_maximum_(v)    ( _Generic((v),     \
                                                            \
             int8_t         : (INT8_MAX),                   \
             int8_t      *  : (INT8_MAX),                   \
             int8_t const*  : (INT8_MAX),                   \
            uint8_t         : (UINT8_MAX),                  \
            uint8_t      *  : (UINT8_MAX),                  \
            uint8_t const*  : (UINT8_MAX),                  \
                                                            \
             int16_t        : (INT16_MAX),                  \
             int16_t      * : (INT16_MAX),                  \
             int16_t const* : (INT16_MAX),                  \
            uint16_t        : (UINT16_MAX),                 \
            uint16_t      * : (UINT16_MAX),                 \
            uint16_t const* : (UINT16_MAX),                 \
                                                            \
             int32_t        : (INT32_MAX),                  \
             int32_t      * : (INT32_MAX),                  \
             int32_t const* : (INT32_MAX),                  \
            uint32_t        : (UINT32_MAX),                 \
            uint32_t      * : (UINT32_MAX),                 \
            uint32_t const* : (UINT32_MAX),                 \
                                                            \
             int64_t        : (INT64_MAX),                  \
             int64_t      * : (INT64_MAX),                  \
             int64_t const* : (INT64_MAX),                  \
            uint64_t        : (UINT64_MAX),                 \
            uint64_t      * : (UINT64_MAX),                 \
            uint64_t const* : (UINT64_MAX),                 \
                                                            \
    COLLECT_C_LIMIT_ENTRY_short_(LONG_MAX, ULONG_MAX)       \
                                                            \
    COLLECT_C_LIMIT_ENTRY_int_(LONG_MAX, ULONG_MAX)         \
                                                            \
    COLLECT_C_LIMIT_ENTRY_long_(LONG_MAX, ULONG_MAX)        \
                                                            \
    COLLECT_C_LIMIT_ENTRY_long_long_(LLONG_MAX, ULLONG_MAX) \
                                                            \
                char   **** : 0                             \
))

#define COLLECT_C_LIMITS_minimum_(v)    ( _Generic((v),     \
                                                            \
             int8_t         : (INT8_MIN),                   \
             int8_t      *  : (INT8_MIN),                   \
             int8_t const*  : (INT8_MIN),                   \
            uint8_t         : (0),                          \
            uint8_t      *  : (0),                          \
            uint8_t const*  : (0),                          \
                                                            \
             int16_t        : (INT16_MIN),                  \
             int16_t      * : (INT16_MIN),                  \
             int16_t const* : (INT16_MIN),                  \
            uint16_t        : (0),                          \
            uint16_t      * : (0),                          \
            uint16_t const* : (0),                          \
                                                            \
             int32_t        : (INT32_MIN),                  \
             int32_t      * : (INT32_MIN),                  \
             int32_t const* : (INT32_MIN),                  \
            uint32_t        : (0),                          \
            uint32_t      * : (0),                          \
            uint32_t const* : (0),                          \
                                                            \
             int64_t        : (INT64_MIN),                  \
             int64_t      * : (INT64_MIN),                  \
             int64_t const* : (INT64_MIN),                  \
            uint64_t        : (0),                          \
            uint64_t      * : (0),                          \
            uint64_t const* : (0),                          \
                                                            \
    COLLECT_C_LIMIT_ENTRY_short_(LONG_MIN, 0)               \
                                                            \
    COLLECT_C_LIMIT_ENTRY_int_(LONG_MIN, 0)                 \
                                                            \
    COLLECT_C_LIMIT_ENTRY_long_(LONG_MIN, 0)                \
                                                            \
    COLLECT_C_LIMIT_ENTRY_long_long_(LLONG_MIN, 0)          \
                                                            \
                char   **** : 0                             \
))


#define COLLECT_C_LIMITS_maximum(v)                         COLLECT_C_LIMITS_maximum_(v)

#define COLLECT_C_LIMITS_minimum(v)                         COLLECT_C_LIMITS_minimum_(v)


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

