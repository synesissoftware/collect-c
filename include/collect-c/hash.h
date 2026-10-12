/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/hash.h
 *
 * Purpose: Stock hashing functions.
 *
 * Created: 12th October 2026
 * Updated: 12th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <stddef.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */


/* /////////////////////////////////////////////////////////////////////////
 * API types
 */

/** 64-bit hash value type. */
typedef uint64_t                                            collect_c_hash_t;


/* /////////////////////////////////////////////////////////////////////////
 * language
 */

#ifdef __cplusplus
extern "C" {
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

/** 64-bit djb2 hash of a counted multibyte buffer (@c _mbuf).
 *
 * @param key Pointer to the first octet. May be NULL when @c key_len is 0;
 * @param key_len Number of octets to hash (embedded NULs count);
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return The 64-bit djb2 digest of the buffer.
 *
 * @note Seed: 5381; multiplier: 33 (@c hash = hash * 33 + byte).
 *
 * @pre @c 0 == key_len || NULL != key
 */
collect_c_hash_t
collect_c_hash_djb2_mbuf(
    void const* key
,   size_t      key_len
,   void*       param
);

/** 64-bit FNV-1a hash of a counted multibyte buffer (@c _mbuf).
 *
 * @param key Pointer to the first octet. May be NULL when @c key_len is 0;
 * @param key_len Number of octets to hash (embedded NULs count);
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return The 64-bit FNV-1a digest of the buffer.
 *
 * @note Offset basis: @c 0xcbf29ce484222325ULL; prime: @c 0x100000001b3ULL.
 *
 * @pre @c 0 == key_len || NULL != key
 */
collect_c_hash_t
collect_c_hash_fnv1a_mbuf(
    void const* key
,   size_t      key_len
,   void*       param
);

/** 64-bit SDBM hash of a counted multibyte buffer (@c _mbuf).
 *
 * @param key Pointer to the first octet. May be NULL when @c key_len is 0;
 * @param key_len Number of octets to hash (embedded NULs count);
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return The 64-bit SDBM digest of the buffer.
 *
 * @note Seed: 0; multiplier: 65599 (@c hash = hash * 65599 + byte).
 *
 * @pre @c 0 == key_len || NULL != key
 */
collect_c_hash_t
collect_c_hash_sdbm_mbuf(
    void const* key
,   size_t      key_len
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

