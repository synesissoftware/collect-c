/* /////////////////////////////////////////////////////////////////////////
 * File:    src/htab.c
 *
 * Purpose: Hash-table container.
 *
 * Created: 12th October 2026
 * Updated: 12th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/htab.h>

#include <assert.h>
#include <errno.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * helpers
 */

/** Rounds @c i up to a @c collect_c_common_node_data_t boundary.
 */
static
size_t
collect_c_htab_align_up_(
    size_t  i
)
{
    size_t const cbAlign = sizeof(collect_c_common_node_data_t);

    return (i + (cbAlign - 1)) & ~(cbAlign - 1);
}

/** Key storage span for a node: table-cached when keys are fixed-size.
 *
 * @param ht The hash table;
 * @param key_len The key length;
 *
 * @return Aligned span of the key region within node storage.
 */
static
size_t
collect_c_htab_key_span_for_(
    collect_c_htab_t const* ht
,   size_t                  key_len
)
{
    return (0 != ht->key_size) ? ht->key_span : collect_c_htab_align_up_(key_len);
}

/** Obtains the mutating pointer to the key for the given node.
 *
 * @param node The node;
 *
 * @return Pointer to the key octets within @c node->data.
 */
static
void*
collect_c_htab_node_key_ptr_(
    collect_c_htab_node_t*  node
)
{
    return &node->data[0];
}

/** Obtains the non-mutating pointer to the key for the given node.
 *
 * @param node The node;
 *
 * @return Pointer to the key octets within @c node->data.
 */
static
void const*
collect_c_htab_node_ckey_ptr_(
    collect_c_htab_node_t const*    node
)
{
    return &node->data[0];
}

/** Obtains the mutating pointer to the value for the given node.
 *
 * @param ht The hash table;
 * @param node The node;
 *
 * @return Pointer to the value octets after the aligned key span.
 */
static
void*
collect_c_htab_node_val_ptr_(
    collect_c_htab_t const* ht
,   collect_c_htab_node_t*  node
)
{
    return ((unsigned char*)&node->data[0]) + collect_c_htab_key_span_for_(ht, node->key_len);
}

/** Calculates the number of bytes to allocate for a node.
 *
 * @param ht The hash table;
 * @param key_len The key length;
 * @param val_len The value length;
 *
 * @return Total allocation size including aligned key span and value.
 */
static
size_t
collect_c_htab_node_bytes_(
    collect_c_htab_t const* ht
,   size_t                  key_len
,   size_t                  val_len
)
{
    return offsetof(collect_c_htab_node_t, data) + collect_c_htab_key_span_for_(ht, key_len) + val_len;
}

/** Allocates a node for the given key and value.
 *
 * @param ht The hash table;
 * @param key Pointer to the key's memory;
 * @param key_len The key length;
 * @param val Pointer to the value's memory;
 * @param val_len The value length;
 *
 * @return Newly allocated node, or nullptr on allocation failure.
 */
static
collect_c_htab_node_t*
collect_c_htab_alloc_node_(
    collect_c_htab_t*   ht
,   void const*         key
,   size_t              key_len
,   void const*         val
,   size_t              val_len
)
{
    size_t const                    cb      =   collect_c_htab_node_bytes_(ht, key_len, val_len);
    collect_c_htab_node_t* const    node    =   (*ht->mem_api.pfn_alloc)(ht->mem_api.param, cb);

    if (NULL == node)
    {
        return NULL;
    }

    node->next      =   NULL;
    node->key_len   =   key_len;
    node->val_len   =   val_len;

    if (0 != key_len)
    {
        memcpy(collect_c_htab_node_key_ptr_(node), key, key_len);
    }

    if (0 != val_len)
    {
        memcpy(collect_c_htab_node_val_ptr_(ht, node), val, val_len);
    }

    return node;
}

/** Invokes @c pfn_free for the given node when a free callback is set.
 *
 * @param ht The hash table;
 * @param node The node whose key/value are passed to the callback;
 */
static
void
collect_c_htab_invoke_free_(
    collect_c_htab_t*       ht
,   collect_c_htab_node_t*  node
)
{
    if (NULL != ht->pfn_free)
    {
        (*ht->pfn_free)(
            collect_c_htab_node_key_ptr_(node)
        ,   node->key_len
        ,   collect_c_htab_node_val_ptr_(ht, node)
        ,   node->val_len
        ,   ht->param
        );
    }
}

/** Releases a node's memory.
 *
 * @param ht The hash table;
 * @param node The node;
 */
static
void
collect_c_htab_free_node_(
    collect_c_htab_t*       ht
,   collect_c_htab_node_t*  node
)
{
    size_t const cb = collect_c_htab_node_bytes_(ht, node->key_len, node->val_len);

    (*ht->mem_api.pfn_free)(ht->mem_api.param, node, cb);
}

/** Calculates the bucket index for the given key.
 *
 * @param ht The hash table;
 * @param key Pointer to the key's memory;
 * @param key_len The key length;
 *
 * @return Bucket index in @c [0, ht->bucket_count).
 */
static
size_t
collect_c_htab_bucket_ix_(
    collect_c_htab_t const* ht
,   void const*             key
,   size_t                  key_len
)
{
    collect_c_hash_t const  h   =   (*ht->pfn_hash)(key, key_len, ht->param);

    assert(0 != ht->bucket_count);

    return (size_t)(h % (collect_c_hash_t)ht->bucket_count);
}

/** Looks up the node for the given key.
 *
 * @param ht The hash table;
 * @param key Pointer to the key's memory;
 * @param key_len The key length;
 *
 * @return Matching node, or nullptr if not found.
 */
static
collect_c_htab_node_t*
collect_c_htab_lookup_node_(
    collect_c_htab_t const* ht
,   void const*             key
,   size_t                  key_len
)
{
    size_t const            ix      =   collect_c_htab_bucket_ix_(ht, key, key_len);
    collect_c_htab_node_t*  node    =   ht->buckets[ix];

    while (NULL != node)
    {
        if (0 == (*ht->pfn_compare)(
                key
            ,   key_len
            ,   collect_c_htab_node_ckey_ptr_(node)
            ,   node->key_len
            ,   ht->param
            )
        )
        {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

/** Finds the chain link for @c key, or the insertion link if absent.
 *
 * @param ht The hash table;
 * @param key Pointer to the key's memory;
 * @param key_len The key length;
 *
 * @return Address of the bucket head or of a node's @c next field. On a
 *  hit, @c *link is the matching node; on a miss, @c *link is nullptr and
 *  ready for insertion.
 */
static
collect_c_htab_node_t**
collect_c_htab_find_link_(
    collect_c_htab_t*   ht
,   void const*         key
,   size_t              key_len
)
{
    size_t const                ix      =   collect_c_htab_bucket_ix_(ht, key, key_len);
    collect_c_htab_node_t**     link    =   &ht->buckets[ix];

    while (NULL != *link)
    {
        collect_c_htab_node_t* const node = *link;

        if (0 == (*ht->pfn_compare)(
                key
            ,   key_len
            ,   collect_c_htab_node_ckey_ptr_(node)
            ,   node->key_len
            ,   ht->param
            )
        )
        {
            return link;
        }

        link = &node->next;
    }

    return link;
}

/** Releases all nodes and their associated memory.
 *
 * @param ht The hash table;
 */
static
void
collect_c_htab_clear_nodes_(
    collect_c_htab_t*   ht
)
{
    size_t  i;

    if (NULL == ht->buckets)
    {
        return;
    }

    for (i = 0; i != ht->bucket_count; ++i)
    {
        collect_c_htab_node_t* node = ht->buckets[i];

        ht->buckets[i] = NULL;

        while (NULL != node)
        {
            collect_c_htab_node_t* const next = node->next;

            collect_c_htab_invoke_free_(ht, node);
            collect_c_htab_free_node_(ht, node);

            node = next;
        }
    }

    ht->size = 0;
}

/** Rehashes the instance into a bucket array of @c new_bucket_count.
 *
 * @param ht The hash table;
 * @param new_bucket_count The new bucket count. Must not be 0;
 *
 * @return 0 on success; @c ENOMEM if allocation fails (state unchanged).
 *
 * @note Always allocates a fresh bucket array so existing chains can be
 *  redistributed under the new modulus, then frees the old array.
 */
static
int
collect_c_htab_rehash_(
    collect_c_htab_t*   ht
,   size_t              new_bucket_count
)
{
    collect_c_htab_node_t** old_buckets;
    size_t                  old_bucket_count;
    collect_c_htab_node_t** new_buckets;
    size_t                  i;
    size_t const            cb_new  =   new_bucket_count * sizeof(collect_c_htab_node_t*);

    assert(NULL != ht);
    assert(0 != new_bucket_count);

    if (new_bucket_count == ht->bucket_count && NULL != ht->buckets)
    {
        return 0;
    }

    new_buckets = (*ht->mem_api.pfn_alloc)(ht->mem_api.param, cb_new);

    if (NULL == new_buckets)
    {
        return ENOMEM;
    }

    memset(new_buckets, 0, cb_new);

    old_buckets         =   ht->buckets;
    old_bucket_count    =   ht->bucket_count;

    ht->buckets         =   new_buckets;
    ht->bucket_count    =   new_bucket_count;

    if (NULL != old_buckets)
    {
        for (i = 0; i != old_bucket_count; ++i)
        {
            collect_c_htab_node_t* node = old_buckets[i];

            while (NULL != node)
            {
                collect_c_htab_node_t* const    next    =   node->next;
                size_t const                    ix      =   collect_c_htab_bucket_ix_(
                                                                ht
                                                            ,   collect_c_htab_node_key_ptr_(node)
                                                            ,   node->key_len
                                                            );

                node->next = ht->buckets[ix];
                ht->buckets[ix] = node;

                node = next;
            }
        }

        (*ht->mem_api.pfn_free)(
            ht->mem_api.param
        ,   old_buckets
        ,   old_bucket_count * sizeof(collect_c_htab_node_t*)
        );
    }

    return 0;
}

/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

uint32_t
collect_c_htab_version(void)
{
    return COLLECT_C_HTAB_VER;
}

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
)
{
    size_t  n;

    assert(NULL != ht);
    assert(NULL != pfn_hash);
    assert(NULL != pfn_compare);

    n = (0 != initial_buckets) ? initial_buckets : COLLECT_C_HTAB_BUCKETS_DEFAULT;

    ht->mem_api.pfn_alloc   =   collect_c_mem_std_alloc;
    ht->mem_api.pfn_realloc =   collect_c_mem_std_realloc;
    ht->mem_api.pfn_free    =   collect_c_mem_std_free;
    ht->mem_api.param       =   NULL;

    ht->key_size        =   key_size;
    ht->key_span        =   (0 != key_size) ? collect_c_htab_align_up_(key_size) : 0;
    ht->val_size        =   val_size;
    ht->bucket_count    =   0;
    ht->size            =   0;
    ht->load_factor_max =   COLLECT_C_HTAB_LOAD_FACTOR_DEFAULT;
    ht->flags           =   0;
    ht->reserved0       =   0;
    ht->buckets         =   NULL;
    ht->pfn_hash        =   pfn_hash;
    ht->pfn_compare     =   pfn_compare;
    ht->pfn_free        =   pfn_free;
    ht->param           =   param;

    return collect_c_htab_rehash_(ht, n);
}

int
collect_c_htab_allocate_storage(
    collect_c_htab_t*   ht
,   size_t              bucket_count
)
{
    size_t  n;

    assert(NULL != ht);

    n = (0 != bucket_count) ? bucket_count : COLLECT_C_HTAB_BUCKETS_DEFAULT;

    if (n < ht->bucket_count)
    {
        n = ht->bucket_count;
    }

    return collect_c_htab_rehash_(ht, n);
}

void
collect_c_htab_free_storage(
    collect_c_htab_t*   ht
)
{
    assert(NULL != ht);

    collect_c_htab_clear_nodes_(ht);

    if (NULL != ht->buckets)
    {
        (*ht->mem_api.pfn_free)(
            ht->mem_api.param
        ,   ht->buckets
        ,   ht->bucket_count * sizeof(collect_c_htab_node_t*)
        );

        ht->buckets         =   NULL;
        ht->bucket_count    =   0;
    }
}

void
collect_c_htab_clear(
    collect_c_htab_t*   ht
)
{
    assert(NULL != ht);

    collect_c_htab_clear_nodes_(ht);
}

int
collect_c_htab_insert(
    collect_c_htab_t*   ht
,   void const*         key
,   size_t              key_len
,   void const*         val
,   size_t              val_len
,   void**              inserted_val_ptr
,   int*                item_was_inserted
)
{
    collect_c_htab_node_t** link;
    collect_c_htab_node_t*  node;
    int                     r;

    assert(NULL != ht);
    assert(0 == key_len || NULL != key);
    assert(0 == val_len || NULL != val);
    assert(NULL != ht->pfn_hash);
    assert(NULL != ht->pfn_compare);
    assert(0 == ht->key_size || key_len == ht->key_size);
    assert(0 == ht->val_size || val_len == ht->val_size);

    if (NULL == ht->buckets || 0 == ht->bucket_count)
    {
        r = collect_c_htab_rehash_(ht, COLLECT_C_HTAB_BUCKETS_DEFAULT);

        if (0 != r)
        {
            return r;
        }
    }

    link = collect_c_htab_find_link_(ht, key, key_len);

    if (NULL != *link)
    {
        collect_c_htab_node_t* const    old     =   *link;
        collect_c_htab_node_t* const    fresh   =   collect_c_htab_alloc_node_(ht, key, key_len, val, val_len);

        if (NULL == fresh)
        {
            return ENOMEM;
        }

        fresh->next =   old->next;
        *link       =   fresh;

        collect_c_htab_invoke_free_(ht, old);
        collect_c_htab_free_node_(ht, old);

        if (NULL != inserted_val_ptr)
        {
            *inserted_val_ptr = collect_c_htab_node_val_ptr_(ht, fresh);
        }

        if (NULL != item_was_inserted)
        {
            *item_was_inserted = 0;
        }

        return 0;
    }
    else
    {
        size_t const    prospective =   ht->size + 1u;
        float const     load        =   (float)prospective / (float)ht->bucket_count;

        if (load > ht->load_factor_max)
        {
            size_t const next = ht->bucket_count * 2u;

            r = collect_c_htab_rehash_(ht, (0 != next) ? next : COLLECT_C_HTAB_BUCKETS_DEFAULT);

            if (0 != r)
            {
                return r;
            }

            link = collect_c_htab_find_link_(ht, key, key_len);
        }

        node = collect_c_htab_alloc_node_(ht, key, key_len, val, val_len);

        if (NULL == node)
        {
            return ENOMEM;
        }

        node->next  =   NULL;
        *link       =   node;
        ++ht->size;

        if (NULL != inserted_val_ptr)
        {
            *inserted_val_ptr = collect_c_htab_node_val_ptr_(ht, node);
        }

        if (NULL != item_was_inserted)
        {
            *item_was_inserted = 1;
        }

        return 0;
    }
}

int
collect_c_htab_find(
    collect_c_htab_t const* ht
,   void const*             key
,   size_t                  key_len
,   void**                  found_val_ptr
)
{
    assert(NULL != ht);
    assert(0 == key_len || NULL != key);

    if (NULL == ht->buckets || 0 == ht->bucket_count)
    {
        if (NULL != found_val_ptr)
        {
            *found_val_ptr = NULL;
        }

        return ENOENT;
    }
    else
    {
        collect_c_htab_node_t* const node = collect_c_htab_lookup_node_(ht, key, key_len);

        if (NULL == node)
        {
            if (NULL != found_val_ptr)
            {
                *found_val_ptr = NULL;
            }

            return ENOENT;
        }
        else
        {
            if (NULL != found_val_ptr)
            {
                *found_val_ptr = collect_c_htab_node_val_ptr_(ht, node);
            }

            return 0;
        }
    }
}

int
collect_c_htab_remove(
    collect_c_htab_t*   ht
,   void const*         key
,   size_t              key_len
)
{
    assert(NULL != ht);
    assert(0 == key_len || NULL != key);

    if (NULL == ht->buckets || 0 == ht->bucket_count)
    {
        return ENOENT;
    }
    else
    {
        collect_c_htab_node_t** const link = collect_c_htab_find_link_(ht, key, key_len);

        if (NULL == *link)
        {
            return ENOENT;
        }
        else
        {
            collect_c_htab_node_t* const node = *link;

            *link = node->next;

            collect_c_htab_invoke_free_(ht, node);
            collect_c_htab_free_node_(ht, node);

            --ht->size;

            return 0;
        }
    }
}

size_t
collect_c_htab_size(
    collect_c_htab_t const* ht
)
{
    assert(NULL != ht);

    return ht->size;
}

size_t
collect_c_htab_bucket_count(
    collect_c_htab_t const* ht
)
{
    assert(NULL != ht);

    return ht->bucket_count;
}

void
collect_c_htab_sizes(
    collect_c_htab_t const* ht
,   size_t*                 num_elements
,   size_t*                 num_buckets
)
{
    assert(NULL != ht);

    if (NULL != num_elements)
    {
        *num_elements = ht->size;
    }

    if (NULL != num_buckets)
    {
        *num_buckets = ht->bucket_count;
    }
}


int
collect_c_htab_for_each(
    collect_c_htab_t*           ht
,   collect_c_htab_pfn_visit_t  pfn_visit
,   void*                       param
)
{
    size_t  i;

    assert(NULL != ht);
    assert(NULL != pfn_visit);

    if (NULL == ht->buckets)
    {
        return 0;
    }

    for (i = 0; i != ht->bucket_count; ++i)
    {
        collect_c_htab_node_t* node;

        for (node = ht->buckets[i]; NULL != node; node = node->next)
        {
            int const r = (*pfn_visit)(
                collect_c_htab_node_key_ptr_(node)
            ,   node->key_len
            ,   collect_c_htab_node_val_ptr_(ht, node)
            ,   node->val_len
            ,   param
            );

            if (0 != r)
            {
                return r;
            }
        }
    }

    return 0;
}


/* ///////////////////////////// end of file //////////////////////////// */

