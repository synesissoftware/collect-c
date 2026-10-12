/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/htab.h
 *
 * Purpose: Hash-table container.
 *
 * Created: 12th October 2026
 * Updated: 12th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifdef __cplusplus
/* NOTE: C++ functionality is provided opt-in and at the users' own risk for
 * the purpose of faciliting functional/performance testing with C++
 */

 # ifndef COLLECT_C_HTAB_SUPPRESS_CXX_WARNING
#  error This file not currently compatible with C++ compilation
# endif
#endif


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#define COLLECT_C_HTAB_VER_MAJOR        0
#define COLLECT_C_HTAB_VER_MINOR        0
#define COLLECT_C_HTAB_VER_PATCH        0
#define COLLECT_C_HTAB_VER_ALPHABETA    1

#define COLLECT_C_HTAB_VER \
    (0\
        |   (   COLLECT_C_HTAB_VER_MAJOR        << 24   ) \
        |   (   COLLECT_C_HTAB_VER_MINOR        << 16   ) \
        |   (   COLLECT_C_HTAB_VER_PATCH        <<  8   ) \
        |   (   COLLECT_C_HTAB_VER_ALPHABETA    <<  0   ) \
    )


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common.h>
#include <collect-c/hash.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * API constants
 */

/** Default maximum load factor before bucket growth. */
#define COLLECT_C_HTAB_LOAD_FACTOR_DEFAULT                  (0.75f)

/** Default initial bucket count when @c 0 is requested. */
#define COLLECT_C_HTAB_BUCKETS_DEFAULT                      (8u)


/* /////////////////////////////////////////////////////////////////////////
 * API types
 */

struct collect_c_htab_node_t;
#ifndef __cplusplus
typedef struct collect_c_htab_node_t                        collect_c_htab_node_t;
#endif

/** Represents a hash-table chain node.
 *
 * @note Key octets occupy the start of @c data; the value follows after an
 *  alignment span of @c collect_c_common_node_data_t (see @c key_span on
 *  the owning table when keys are fixed-size).
 */
struct collect_c_htab_node_t
{
    collect_c_htab_node_t*          next;               /*! Next node in the bucket chain. */
    size_t                          key_len;            /*! Length of the stored key in octets. */
    size_t                          val_len;            /*! Length of the stored value in octets. */
    collect_c_common_node_data_t    data[1];            /*! Flexible storage for key then value. */
};

/** Hash function callback.
 *
 * @param key Pointer to the key. May be nullptr when @c key_len is 0;
 * @param key_len Length of @c key in octets;
 * @param param Context from the owning @c collect_c_htab_t. May be nullptr;
 *
 * @return Hash value used for bucket selection.
 *
 * @note Stock helpers in **collect-c/hash.h** implement this contract.
 */
typedef collect_c_hash_t
(*collect_c_htab_pfn_hash_t)(
    void const* key
,   size_t      key_len
,   void*       param
);

/** Key comparison callback (ordering relation).
 *
 * @param k1 Pointer to the left-hand key. May be nullptr when @c k1_len is
 *  0;
 * @param k1_len Length of @c k1 in octets;
 * @param k2 Pointer to the right-hand key. May be nullptr when @c k2_len is
 *  0;
 * @param k2_len Length of @c k2 in octets;
 * @param param Context from the owning @c collect_c_htab_t. May be nullptr;
 *
 * @return Negative if @c k1 is ordered before @c k2; 0 if equivalent;
 *  positive if @c k1 is ordered after @c k2. Magnitude is not significant.
 *
 * @note Stock helpers in **collect-c/compare.h** implement this contract.
 *  The hash-table uses only equivalence (@c 0 vs non-zero) for lookup,
 *  insert, and erase; a full ordering remains useful for consistent
 *  call-sites and for future ordered views.
 */
typedef int
(*collect_c_htab_pfn_key_compare_t)(
    void const* k1
,   size_t      k1_len
,   void const* k2
,   size_t      k2_len
,   void*       param
);

/** Callback function that, if attached to instance, will be called back for
 * each element upon its erasure or replacement by any of the API functions.
 *
 * @param key Pointer to the stored key. May be nullptr when @c key_len is
 *  0;
 * @param key_len Length of @c key in octets;
 * @param val Pointer to the stored value. May be nullptr when @c val_len is
 *  0;
 * @param val_len Length of @c val in octets;
 * @param param Context from the owning @c collect_c_htab_t. May be nullptr;
 */
typedef void
(*collect_c_htab_pfn_element_free_t)(
    void*       key
,   size_t      key_len
,   void*       val
,   size_t      val_len
,   void*       param
);

/** Prototype for callback function for entries visited by
 * @c collect_c_htab_for_each.
 *
 * @param key Pointer to the stored key. May be nullptr when @c key_len is
 *  0;
 * @param key_len Length of @c key in octets;
 * @param val Pointer to the stored value. May be nullptr when @c val_len is
 *  0;
 * @param val_len Length of @c val in octets;
 * @param param Caller context passed to @c collect_c_htab_for_each. May be
 *  nullptr;
 *
 * @return 0 to continue; non-zero to abort the walk (returned to the
 *  caller).
 */
typedef int
(*collect_c_htab_pfn_visit_t)(
    void const* key
,   size_t      key_len
,   void*       val
,   size_t      val_len
,   void*       param
);

/** Represents a hash-table container with separate chaining.
 */
struct collect_c_htab_t
{
    collect_c_mem_api_t                 mem_api;            /*! Memory allocation API. */
    size_t                              key_size;           /*! Fixed key size, or 0 for variable-length keys. */
    size_t                              key_span;           /*! Aligned key storage span when key_size != 0; else 0. */
    size_t                              val_size;           /*! Fixed value size, or 0 for variable-length values. */
    size_t                              bucket_count;       /*! The number of buckets. */
    size_t                              size;               /*! The number of elements currently stored. */
    float                               load_factor_max;    /*! Maximum load factor before bucket growth. */
    int32_t                             flags;              /*! Control flags. */
    int32_t                             reserved0;          /*! Reserved field. */
    collect_c_htab_node_t**             buckets;            /*! Pointer to the bucket array. */
    collect_c_htab_pfn_hash_t           pfn_hash;           /*! Hash function. */
    collect_c_htab_pfn_key_compare_t    pfn_compare;        /*! Key comparison function. */
    collect_c_htab_pfn_element_free_t   pfn_free;           /*! Custom function to be invoked when element erased/replaced. */
    void*                               param;              /*! Custom parameter to be passed to callback invocations. */
};
#ifndef __cplusplus
typedef struct collect_c_htab_t                             collect_c_htab_t;
#endif


/* /////////////////////////////////////////////////////////////////////////
 * language
 */

#ifdef __cplusplus
extern "C" {
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

/** Obtains the value of COLLECT_C_HTAB_VER at the time of compilation of
 * the library.
 */
uint32_t
collect_c_htab_version(void);

/** Initialise an uninitialised hash-table instance.
 *
 * @param ht Instance to initialise. Must not be nullptr;
 * @param key_size Fixed key size, or 0 for variable-length keys;
 * @param val_size Fixed value size, or 0 for variable-length values;
 * @param initial_buckets Initial bucket count (0 selects the default);
 * @param pfn_hash Hash callback. Must not be nullptr;
 * @param pfn_compare Key comparison callback. Must not be nullptr;
 * @param pfn_free Optional element free callback. May be nullptr;
 * @param param Context passed to callbacks. May be nullptr;
 *
 * @return 0 on success; @c ENOMEM if bucket allocation fails.
 *
 * @pre nullptr != ht
 * @pre nullptr != pfn_hash
 * @pre nullptr != pfn_compare
 */
int
collect_c_htab_init(
    collect_c_htab_t*                   ht
,   size_t                              key_size
,   size_t                              val_size
,   size_t                              initial_buckets
,   collect_c_htab_pfn_hash_t           pfn_hash
,   collect_c_htab_pfn_key_compare_t    pfn_compare
,   collect_c_htab_pfn_element_free_t   pfn_free
,   void*                               param
);

/** Pre-allocate or expand bucket capacity (rehashes existing entries).
 *
 * @param ht The hash table. Must not be nullptr;
 * @param bucket_count Requested bucket count (0 selects the default). If
 *  less than the current count, the current count is retained;
 *
 * @return 0 on success; @c ENOMEM if allocation fails (state unchanged).
 *
 * @pre nullptr != ht
 */
int
collect_c_htab_allocate_storage(
    collect_c_htab_t*   ht
,   size_t              bucket_count
);

/** Free all entries and bucket storage; reset @c ht to an empty,
 * unallocated state (callbacks retained).
 *
 * @param ht The hash table. Must not be nullptr;
 *
 * @pre nullptr != ht
 */
void
collect_c_htab_free_storage(
    collect_c_htab_t*   ht
);

/** Remove all entries (invoking @c pfn_free when set); retain buckets.
 *
 * @param ht The hash table. Must not be nullptr;
 *
 * @pre nullptr != ht
 */
void
collect_c_htab_clear(
    collect_c_htab_t*   ht
);

/** Insert or replace a key/value pair.
 *
 * @param ht The hash table. Must not be nullptr;
 * @param key Pointer to the key. May be nullptr when @c key_len is 0;
 * @param key_len Length of @c key in octets;
 * @param val Pointer to the value. May be nullptr when @c val_len is 0;
 * @param val_len Length of @c val in octets;
 * @param inserted_val_ptr Optional; receives pointer to the stored value;
 * @param item_was_inserted Optional; set to non-zero if a new entry was
 *  inserted, or to 0 if an existing entry was replaced;
 *
 * @return 0 on success; @c ENOMEM on allocation failure (state unchanged).
 *
 * @pre nullptr != ht
 * @pre 0 == key_len || nullptr != key
 * @pre 0 == val_len || nullptr != val
 * @pre 0 == ht->key_size || key_len == ht->key_size
 * @pre 0 == ht->val_size || val_len == ht->val_size
 */
int
collect_c_htab_insert(
    collect_c_htab_t*   ht
,   void const*         key
,   size_t              key_len
,   void const*         val
,   size_t              val_len
,   void**              inserted_val_ptr
,   int*                item_was_inserted
);

/** Find an entry by key.
 *
 * @param ht The hash table. Must not be nullptr;
 * @param key Pointer to the key. May be nullptr when @c key_len is 0;
 * @param key_len Length of @c key in octets;
 * @param found_val_ptr Optional; receives pointer to the stored value;
 *
 * @return 0 if found; @c ENOENT if not found.
 *
 * @pre nullptr != ht
 * @pre 0 == key_len || nullptr != key
 *
 * @note If nullptr != found_val_ptr then *found_val_ptr = nullptr if the
 *  key is not found.
 */
int
collect_c_htab_find(
    collect_c_htab_t const* ht
,   void const*             key
,   size_t                  key_len
,   void**                  found_val_ptr
);

/** Remove the entry matching @c key.
 *
 * @param ht The hash table. Must not be nullptr;
 * @param key Pointer to the key. May be nullptr when @c key_len is 0;
 * @param key_len Length of @c key in octets;
 *
 * @return 0 if removed; @c ENOENT if not found.
 *
 * @pre nullptr != ht
 * @pre 0 == key_len || nullptr != key
 */
int
collect_c_htab_remove(
    collect_c_htab_t*   ht
,   void const*         key
,   size_t              key_len
);

/** Obtains the number of elements currently stored.
 *
 * @param ht The hash table. Must not be nullptr;
 *
 * @return The element count.
 *
 * @pre nullptr != ht
 */
size_t
collect_c_htab_size(
    collect_c_htab_t const* ht
);

/** Obtains the number of buckets.
 *
 * @param ht The hash table. Must not be nullptr;
 *
 * @return The bucket count.
 *
 * @pre nullptr != ht
 */
size_t
collect_c_htab_bucket_count(
    collect_c_htab_t const* ht
);

/** Obtains element count and/or bucket count.
 *
 * @param ht The hash table. Must not be nullptr;
 * @param num_elements Optional; receives @c collect_c_htab_size;
 * @param num_buckets Optional; receives @c collect_c_htab_bucket_count;
 *
 * @pre nullptr != ht
 */
void
collect_c_htab_sizes(
    collect_c_htab_t const* ht
,   size_t*                 num_elements
,   size_t*                 num_buckets
);

/** Visit each entry. Stops early if @c pfn_visit returns non-zero.
 *
 * @param ht The hash table. Must not be nullptr;
 * @param pfn_visit Visit callback. Must not be nullptr;
 * @param param Context passed to @c pfn_visit. May be nullptr;
 *
 * @return 0 if all visited; otherwise the non-zero visit result.
 *
 * @pre nullptr != ht
 * @pre nullptr != pfn_visit
 */
int
collect_c_htab_for_each(
    collect_c_htab_t*           ht
,   collect_c_htab_pfn_visit_t  pfn_visit
,   void*                       param
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

