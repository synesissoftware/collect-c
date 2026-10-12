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
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

/** 64-bit djb2 hash of a byte sequence.
 *
 * @param key Pointer to the first byte of the key. May be NULL when
 *  @c key_len is 0;
 * @param key_len Number of bytes to hash;
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return The 64-bit djb2 digest of the byte sequence.
 *
 * @note Seed: 5381; multiplier: 33 (@c hash = hash * 33 + byte).
 *
 * @pre @c 0 == key_len || NULL != key
 */
collect_c_hash_t
collect_c_hash_djb2(
    void const* key
,   size_t      key_len
,   void*       param
);

/** 64-bit FNV-1a hash of a byte sequence.
 *
 * @param key Pointer to the first byte of the key. May be NULL when
 *  @c key_len is 0;
 * @param key_len Number of bytes to hash;
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return The 64-bit FNV-1a digest of the byte sequence.
 *
 * @note Offset basis: @c 0xcbf29ce484222325ULL; prime: @c 0x100000001b3ULL.
 *
 * @pre @c 0 == key_len || NULL != key
 */
collect_c_hash_t
collect_c_hash_fnv1a(
    void const* key
,   size_t      key_len
,   void*       param
);

/** 64-bit SDBM hash of a byte sequence.
 *
 * @param key Pointer to the first byte of the key. May be NULL when
 *  @c key_len is 0;
 * @param key_len Number of bytes to hash;
 * @param param Unused (reserved for callback compatibility). May be NULL;
 *
 * @return The 64-bit SDBM digest of the byte sequence.
 *
 * @note Seed: 0; multiplier: 65599 (@c hash = hash * 65599 + byte).
 *
 * @pre @c 0 == key_len || NULL != key
 */
collect_c_hash_t
collect_c_hash_sdbm(
    void const* key
,   size_t      key_len
,   void*       param
);


#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

