/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/compare.h
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

#include <stddef.h>


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */


/* /////////////////////////////////////////////////////////////////////////
 * language
 */

#ifdef __cplusplus
extern "C" {
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

/** Binary memory comparison of two key byte sequences.
 *
 * @param k1 Pointer to the first key. May be NULL when @c k1_len is 0;
 * @param k1_len Length of @c k1 in bytes;
 * @param k2 Pointer to the second key. May be NULL when @c k2_len is 0;
 * @param k2_len Length of @c k2 in bytes;
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return Negative, zero, or positive (magnitude not significant).
 *
 * @note Compares the common prefix with @c memcmp; if that is equal, the
 *  shorter sequence is less (prefix rule).
 *
 * @pre @c 0 == k1_len || NULL != k1
 * @pre @c 0 == k2_len || NULL != k2
 */
int
collect_c_compare_bytes(
    void const* k1
,   size_t      k1_len
,   void const* k2
,   size_t      k2_len
,   void*       param
);

/** Exact comparison of two counted multibyte buffers (@c _mbuf).
 *
 * @param k1 Pointer to the first key. May be NULL when @c k1_len is 0;
 * @param k1_len Length of @c k1 in octets (embedded NULs count);
 * @param k2 Pointer to the second key. May be NULL when @c k2_len is 0;
 * @param k2_len Length of @c k2 in octets;
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return Negative, zero, or positive (magnitude not significant).
 *
 * @note Compares the common prefix with @c memcmp (embedded NULs count); if
 *  that is equal, the shorter buffer is less (prefix rule).
 *
 * @pre @c 0 == k1_len || NULL != k1
 * @pre @c 0 == k2_len || NULL != k2
 */
int
collect_c_compare_mbuf(
    void const* k1
,   size_t      k1_len
,   void const* k2
,   size_t      k2_len
,   void*       param
);

/** Case-insensitive ASCII comparison of two counted multibyte buffers.
 *
 * @param k1 Pointer to the first key. May be NULL when @c k1_len is 0;
 * @param k1_len Length of @c k1 in octets;
 * @param k2 Pointer to the second key. May be NULL when @c k2_len is 0;
 * @param k2_len Length of @c k2 in octets;
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return Negative, zero, or positive (magnitude not significant).
 *
 * @note Compares the common prefix after ASCII @c tolower (@c _case); if
 *  that is equal, the shorter buffer is less (prefix rule).
 *
 * @pre @c 0 == k1_len || NULL != k1
 * @pre @c 0 == k2_len || NULL != k2
 */
int
collect_c_compare_mbuf_case(
    void const* k1
,   size_t      k1_len
,   void const* k2
,   size_t      k2_len
,   void*       param
);


/* /////////////////////////////////////////////////////////////////////////
 * language
 */

#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

