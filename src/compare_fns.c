/* /////////////////////////////////////////////////////////////////////////
 * File:    src/compare_fns.c
 *
 * Purpose: Stock key-comparison functions (hashtable-oriented).
 *
 * Created: 12th October 2026
 * Updated: 12th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/compare.h>

#include <assert.h>
#include <stdint.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * helpers
 */

/* ASCII A-Z becomes a-z by adding 0x20. Every other octet is unchanged,
 * including values above 0x7F (locale-independent; not Unicode case-fold).
 */
static
uint8_t
collect_c_compare_ascii_fold_octet_(
    uint8_t octet
)
{
    if (octet >= (uint8_t)'A' && octet <= (uint8_t)'Z')
    {
        octet = (uint8_t)(octet + 0x20u);
    }

    return octet;
}


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

int
collect_c_compare_bytes(
    void const* k1
,   size_t      k1_len
,   void const* k2
,   size_t      k2_len
,   void*       param
)
{
    size_t const    n   =   (k1_len < k2_len) ? k1_len : k2_len;
    int             r;

    assert(0 == k1_len || NULL != k1);
    assert(0 == k2_len || NULL != k2);

    ((void)param);

    if (0 != n)
    {
        r = memcmp(k1, k2, n);

        if (0 != r)
        {
            return r;
        }
    }

    if (k1_len < k2_len)
    {
        return -1;
    }

    if (k1_len > k2_len)
    {
        return +1;
    }

    return 0;
}

int
collect_c_compare_mbuf(
    void const* k1
,   size_t      k1_len
,   void const* k2
,   size_t      k2_len
,   void*       param
)
{
    size_t const    n   =   (k1_len < k2_len) ? k1_len : k2_len;
    int             r;

    assert(0 == k1_len || NULL != k1);
    assert(0 == k2_len || NULL != k2);

    ((void)param);

    if (0 != n)
    {
        r = memcmp(k1, k2, n);

        if (0 != r)
        {
            return r;
        }
    }

    if (k1_len < k2_len)
    {
        return -1;
    }

    if (k1_len > k2_len)
    {
        return +1;
    }

    return 0;
}

int
collect_c_compare_mbuf_case(
    void const* k1
,   size_t      k1_len
,   void const* k2
,   size_t      k2_len
,   void*       param
)
{
    unsigned char const*    p1;
    unsigned char const*    p2;
    size_t const            n   =   (k1_len < k2_len) ? k1_len : k2_len;
    size_t                  i;

    assert(0 == k1_len || NULL != k1);
    assert(0 == k2_len || NULL != k2);

    ((void)param);

    p1 = (unsigned char const*)k1;
    p2 = (unsigned char const*)k2;

    for (i = 0; i != n; ++i)
    {
        uint8_t const   c1  =   collect_c_compare_ascii_fold_octet_(p1[i]);
        uint8_t const   c2  =   collect_c_compare_ascii_fold_octet_(p2[i]);

        if (c1 != c2)
        {
            return (c1 < c2) ? -1 : +1;
        }
    }

    if (k1_len < k2_len)
    {
        return -1;
    }

    if (k1_len > k2_len)
    {
        return +1;
    }

    return 0;
}


/* ///////////////////////////// end of file //////////////////////////// */

