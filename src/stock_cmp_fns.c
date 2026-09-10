/* /////////////////////////////////////////////////////////////////////////
 * File:    src/stock_cmp_fns.c
 *
 * Purpose: Stock comparison functions.
 *
 * Created: 19th February 2025
 * Updated: 10th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common.h>

#include <assert.h>
#include <stdint.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * internal functions & macros
 */

static
int
collect_c_fn_cmp_undiscriminated_(
    void const* _lhs
,   void const* _rhs
)
{
    ((void)_lhs);
    ((void)_rhs);

    assert(!"undiscriminated comparison type");

    return 0;
}

static
int
collect_c_fn_cmp_int16_(
    void const* pe_lhs
,   void const* pe_rhs
)
{
    int16_t const   lhs =   *(int16_t const*)pe_lhs;
    int16_t const   rhs =   *(int16_t const*)pe_rhs;

    if (lhs < rhs)
    {
        return -1;
    }

    if (lhs > rhs)
    {
        return +1;
    }

    return 0;
}

static
int
collect_c_fn_cmp_int32_(
    void const* pe_lhs
,   void const* pe_rhs
)
{
    int32_t const   lhs =   *(int32_t const*)pe_lhs;
    int32_t const   rhs =   *(int32_t const*)pe_rhs;

    if (lhs < rhs)
    {
        return -1;
    }

    if (lhs > rhs)
    {
        return +1;
    }

    return 0;
}

static
int
collect_c_fn_cmp_int64_(
    void const* pe_lhs
,   void const* pe_rhs
)
{
    int64_t const   lhs =   *(int64_t const*)pe_lhs;
    int64_t const   rhs =   *(int64_t const*)pe_rhs;

    if (lhs < rhs)
    {
        return -1;
    }

    if (lhs > rhs)
    {
        return +1;
    }

    return 0;
}

static
int
collect_c_fn_cmp_uint16_(
    void const* pe_lhs
,   void const* pe_rhs
)
{
    uint16_t const  lhs =   *(uint16_t const*)pe_lhs;
    uint16_t const  rhs =   *(uint16_t const*)pe_rhs;

    if (lhs < rhs)
    {
        return -1;
    }

    if (lhs > rhs)
    {
        return +1;
    }

    return 0;
}

static
int
collect_c_fn_cmp_uint32_(
    void const* pe_lhs
,   void const* pe_rhs
)
{
    uint32_t const  lhs =   *(uint32_t const*)pe_lhs;
    uint32_t const  rhs =   *(uint32_t const*)pe_rhs;

    if (lhs < rhs)
    {
        return -1;
    }

    if (lhs > rhs)
    {
        return +1;
    }

    return 0;
}

static
int
collect_c_fn_cmp_uint64_(
    void const* pe_lhs
,   void const* pe_rhs
)
{
    uint64_t const  lhs =   *(uint64_t const*)pe_lhs;
    uint64_t const  rhs =   *(uint64_t const*)pe_rhs;

    if (lhs < rhs)
    {
        return -1;
    }

    if (lhs > rhs)
    {
        return +1;
    }

    return 0;
}


#define CLC_INTERNAL_SIGNED_SELECT_(n)                      ((8 == (n)) ? collect_c_fn_cmp_int64_ : (4 == (n)) ? collect_c_fn_cmp_int32_ : collect_c_fn_cmp_int16_)
#define CLC_INTERNAL_UNSIGNED_SELECT_(n)                    ((8 == (n)) ? collect_c_fn_cmp_uint64_ : (4 == (n)) ? collect_c_fn_cmp_uint32_ : collect_c_fn_cmp_uint16_)

#define CLC_INTERNAL_SELECT_CMPFN_FOR_SIZED_T_(t_int)  _Generic(((t_int)0), \
                                                                            \
     int64_t : collect_c_fn_cmp_int64_,                                     \
     int32_t : collect_c_fn_cmp_int32_,                                     \
     int16_t : collect_c_fn_cmp_int16_,                                     \
    uint64_t : collect_c_fn_cmp_uint64_,                                    \
    uint32_t : collect_c_fn_cmp_uint32_,                                    \
    uint16_t : collect_c_fn_cmp_uint16_,                                    \
     default : collect_c_fn_cmp_undiscriminated_                            \
)

#define CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(t_int)   _Generic(((t_int)0), \
                                                                                \
                  short : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
                    int : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
                   long : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
              long long : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
         unsigned short : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
           unsigned int : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
          unsigned long : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
     unsigned long long : CLC_INTERNAL_SIGNED_SELECT_(sizeof(t_int)),           \
                default : collect_c_fn_cmp_undiscriminated_                     \
)


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

int
collect_c_fn_cmp_int16(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef int16_t                                         int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_SIZED_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_int32(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef int32_t                                         int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_SIZED_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_int64(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef int64_t                                         int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_SIZED_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_uint16(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef uint16_t                                        int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_SIZED_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_uint32(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef uint32_t                                        int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_SIZED_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_uint64(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef uint64_t                                        int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_SIZED_T_(int_t)(pe_lhs, pe_rhs);
}


int
collect_c_fn_cmp_short(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef short                                           int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_int(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef int                                             int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_long(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef long                                            int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_llong(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef long long                                       int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_ushort(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef unsigned short                                  int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_uint(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef unsigned int                                    int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_ulong(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef unsigned long                                   int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}

int
collect_c_fn_cmp_ullong(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    typedef unsigned long long                              int_t;

    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(int_t) == el_size);

    ((void)&el_size);

    return CLC_INTERNAL_SELECT_CMPFN_FOR_BUILT_IN_T_(int_t)(pe_lhs, pe_rhs);
}



int
collect_c_fn_cmp_ccs(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(char const*) == el_size);

    ((void)&el_size);

    {
        char const* const   lhs =   *(char const* const*)pe_lhs;
        char const* const   rhs =   *(char const* const*)pe_rhs;

        if (NULL == lhs)
        {
            if (NULL == rhs)
            {
                return 0;
            }
            else
            {
                if ('\0' == *rhs)
                {
                    return 0;
                }
                else
                {
                    return -1;
                }
            }
        }
        else
        {
            if (NULL == rhs)
            {
                if ('\0' == *lhs)
                {
                    return 0;
                }
                else
                {
                    return +1;
                }
            }
            else
            {
                return strcmp(lhs, rhs);
            }
        }
    }
}

int
collect_c_fn_cmp_ccs_ignore_case(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(0 != el_size);
    assert(sizeof(char const*) == el_size);

    ((void)&el_size);

    {
        char const* const   lhs =   *(char const* const*)pe_lhs;
        char const* const   rhs =   *(char const* const*)pe_rhs;

        if (NULL == lhs)
        {
            if (NULL == rhs)
            {
                return 0;
            }
            else
            {
                if ('\0' == *rhs)
                {
                    return 0;
                }
                else
                {
                    return -1;
                }
            }
        }
        else
        {
            if (NULL == rhs)
            {
                if ('\0' == *lhs)
                {
                    return 0;
                }
                else
                {
                    return +1;
                }
            }
            else
            {
                return collect_c_stricmp(lhs, rhs);
            }
        }
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

