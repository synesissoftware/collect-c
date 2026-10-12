/* /////////////////////////////////////////////////////////////////////////
 * File:    src/hash_fns.c
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

#include <collect-c/hash.h>

#include <assert.h>


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#define COLLECT_C_HASH_DJB2_MULTIPLIER_                     (33ull)
#define COLLECT_C_HASH_DJB2_SEED_                           (5381ull)

#define COLLECT_C_HASH_FNV1A_OFFSET_BASIS_                  (0xcbf29ce484222325ull)
#define COLLECT_C_HASH_FNV1A_PRIME_                         (0x00000100000001b3ull)

#define COLLECT_C_HASH_SDBM_MULTIPLIER_                     (65599ull) /* == 0x1003f */
#define COLLECT_C_HASH_SDBM_SEED_                           (0ull)


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

collect_c_hash_t
collect_c_hash_djb2_mbuf(
    void const* key
,   size_t      key_len
,   void*       param
)
{
    uint8_t const*  p;
    size_t          i;
    collect_c_hash_t hash;

    assert(0 == key_len || NULL != key);

    ((void)param);

    p = (uint8_t const*)key;
    hash = COLLECT_C_HASH_DJB2_SEED_;

    for (i = 0; i != key_len; ++i)
    {
        hash = (hash * COLLECT_C_HASH_DJB2_MULTIPLIER_) + (collect_c_hash_t)p[i];
    }

    return hash;
}

collect_c_hash_t
collect_c_hash_fnv1a_mbuf(
    void const* key
,   size_t      key_len
,   void*       param
)
{
    uint8_t const*  p;
    size_t          i;
    collect_c_hash_t hash;

    assert(0 == key_len || NULL != key);

    ((void)param);

    p = (uint8_t const*)key;
    hash = COLLECT_C_HASH_FNV1A_OFFSET_BASIS_;

    for (i = 0; i != key_len; ++i)
    {
        hash ^= (collect_c_hash_t)p[i];
        hash *= COLLECT_C_HASH_FNV1A_PRIME_;
    }

    return hash;
}

collect_c_hash_t
collect_c_hash_sdbm_mbuf(
    void const* key
,   size_t      key_len
,   void*       param
)
{
    uint8_t const*  p;
    size_t          i;
    collect_c_hash_t hash;

    assert(0 == key_len || NULL != key);

    ((void)param);

    p = (uint8_t const*)key;
    hash = COLLECT_C_HASH_SDBM_SEED_;

    for (i = 0; i != key_len; ++i)
    {
        hash = (hash * COLLECT_C_HASH_SDBM_MULTIPLIER_) + (collect_c_hash_t)p[i];
    }

    return hash;
}


/* ///////////////////////////// end of file //////////////////////////// */

