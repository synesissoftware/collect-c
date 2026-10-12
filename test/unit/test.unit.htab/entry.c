/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.htab/entry.c
 *
 * Purpose: Unit-test for Hash-table container.
 *
 * Created: 12th October 2026
 * Updated: 12th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/compare.h>
#include <collect-c/hash.h>
#include <collect-c/htab.h>

#include <xtests/terse-api.h>

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_aligned_value_after_uint32_key(void);
static void TEST_case_insensitive_keys(void);
static void TEST_collision_chain_remove_positions(void);
static void TEST_for_each_and_clear(void);
static void TEST_grow_under_load(void);
static void TEST_init_find_insert_remove(void);
static void TEST_overwrite_and_pfn_free(void);
static void TEST_sizes(void);


/* /////////////////////////////////////////////////////////////////////////
 * helpers
 */

static collect_c_hash_t
hash_always_zero(
    void const* key
,   size_t      key_len
,   void*       param
)
{
    ((void)key);
    ((void)key_len);
    ((void)param);

    return 0;
}

struct free_counter_t
{
    int count;
};
typedef struct free_counter_t free_counter_t;

static void
counting_free(
    void*       key
,   size_t      key_len
,   void*       val
,   size_t      val_len
,   void*       param
)
{
    free_counter_t* fc = (free_counter_t*)param;

    ((void)key);
    ((void)key_len);
    ((void)val);
    ((void)val_len);

    ++fc->count;
}

struct visit_count_t
{
    int count;
};
typedef struct visit_count_t visit_count_t;

static int
counting_visit(
    void const* key
,   size_t      key_len
,   void*       val
,   size_t      val_len
,   void*       param
)
{
    visit_count_t* vc = (visit_count_t*)param;

    ((void)key);
    ((void)key_len);
    ((void)val);
    ((void)val_len);

    ++vc->count;

    return 0;
}


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.htab", verbosity))
    {
        XTESTS_RUN_CASE(TEST_aligned_value_after_uint32_key);
        XTESTS_RUN_CASE(TEST_case_insensitive_keys);
        XTESTS_RUN_CASE(TEST_collision_chain_remove_positions);
        XTESTS_RUN_CASE(TEST_for_each_and_clear);
        XTESTS_RUN_CASE(TEST_grow_under_load);
        XTESTS_RUN_CASE(TEST_init_find_insert_remove);
        XTESTS_RUN_CASE(TEST_overwrite_and_pfn_free);
        XTESTS_RUN_CASE(TEST_sizes);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

static void TEST_aligned_value_after_uint32_key(void)
{
    collect_c_htab_t    ht;
    uint32_t const      key     =   7u;
    uint64_t const      val     =   UINT64_C(0x1122334455667788);
    void*               pval    =   NULL;

    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   sizeof(uint32_t)
    ,   sizeof(uint64_t)
    ,   4
    ,   collect_c_hash_djb2_mbuf
    ,   collect_c_compare_mbuf
    ,   NULL
    ,   NULL
    ));

    TEST_INT_EQ(0, collect_c_htab_insert(
        &ht
    ,   &key
    ,   sizeof(key)
    ,   &val
    ,   sizeof(val)
    ,   &pval
    ,   NULL
    ));
    TEST_PTR_NE(NULL, pval);
    TEST_INT_EQ(0, (int)((uintptr_t)pval % sizeof(collect_c_common_node_data_t)));
    TEST_BOOLEAN_TRUE(val == *(uint64_t*)pval);

    pval = NULL;
    TEST_INT_EQ(0, collect_c_htab_find(&ht, &key, sizeof(key), &pval));
    TEST_INT_EQ(0, (int)((uintptr_t)pval % sizeof(collect_c_common_node_data_t)));
    TEST_BOOLEAN_TRUE(val == *(uint64_t*)pval);

    collect_c_htab_free_storage(&ht);
}

static void TEST_init_find_insert_remove(void)
{
    collect_c_htab_t    ht;
    void*               pval = NULL;
    int                 inserted = -1;
    int                 v;

    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   0
    ,   sizeof(int)
    ,   4
    ,   collect_c_hash_djb2_mbuf
    ,   collect_c_compare_mbuf
    ,   NULL
    ,   NULL
    ));

    TEST_INT_EQ(0, collect_c_htab_size(&ht));
    TEST_INT_EQ(4, collect_c_htab_bucket_count(&ht));
    TEST_INT_EQ(ENOENT, collect_c_htab_find(&ht, "a", 1, &pval));

    v = 42;
    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "a", 1, &v, sizeof(v), &pval, &inserted));
    TEST_INT_EQ(1, inserted);
    TEST_INT_EQ(1, collect_c_htab_size(&ht));
    TEST_PTR_NE(NULL, pval);
    TEST_INT_EQ(42, *(int*)pval);

    pval = NULL;
    TEST_INT_EQ(0, collect_c_htab_find(&ht, "a", 1, &pval));
    TEST_INT_EQ(42, *(int*)pval);

    TEST_INT_EQ(0, collect_c_htab_remove(&ht, "a", 1));
    TEST_INT_EQ(0, collect_c_htab_size(&ht));
    TEST_INT_EQ(ENOENT, collect_c_htab_find(&ht, "a", 1, NULL));
    TEST_INT_EQ(ENOENT, collect_c_htab_remove(&ht, "a", 1));

    collect_c_htab_free_storage(&ht);
    TEST_INT_EQ(0, collect_c_htab_bucket_count(&ht));
}

static void TEST_overwrite_and_pfn_free(void)
{
    collect_c_htab_t    ht;
    free_counter_t      fc = { 0 };
    int                 v1 = 1;
    int                 v2 = 2;
    void*               pval = NULL;
    int                 inserted = -1;

    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   0
    ,   sizeof(int)
    ,   8
    ,   collect_c_hash_fnv1a_mbuf
    ,   collect_c_compare_bytes
    ,   counting_free
    ,   &fc
    ));

    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "k", 1, &v1, sizeof(v1), NULL, &inserted));
    TEST_INT_EQ(1, inserted);
    TEST_INT_EQ(0, fc.count);

    inserted = -1;
    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "k", 1, &v2, sizeof(v2), &pval, &inserted));
    TEST_INT_EQ(0, inserted);
    TEST_INT_EQ(1, fc.count);
    TEST_INT_EQ(2, *(int*)pval);
    TEST_INT_EQ(1, collect_c_htab_size(&ht));

    TEST_INT_EQ(0, collect_c_htab_remove(&ht, "k", 1));
    TEST_INT_EQ(2, fc.count);

    collect_c_htab_free_storage(&ht);
}

static void TEST_collision_chain_remove_positions(void)
{
    collect_c_htab_t    ht;
    int                 values[3] = { 10, 20, 30 };
    void*               pval = NULL;

    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   0
    ,   sizeof(int)
    ,   1
    ,   hash_always_zero
    ,   collect_c_compare_mbuf
    ,   NULL
    ,   NULL
    ));

    /* Keep a single bucket so all keys share one chain. */
    ht.load_factor_max = 100.0f;

    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "a", 1, &values[0], sizeof(int), NULL, NULL));
    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "b", 1, &values[1], sizeof(int), NULL, NULL));
    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "c", 1, &values[2], sizeof(int), NULL, NULL));
    TEST_INT_EQ(3, collect_c_htab_size(&ht));
    TEST_INT_EQ(1, collect_c_htab_bucket_count(&ht));

    /* Remove middle */
    TEST_INT_EQ(0, collect_c_htab_remove(&ht, "b", 1));
    TEST_INT_EQ(ENOENT, collect_c_htab_find(&ht, "b", 1, NULL));
    TEST_INT_EQ(0, collect_c_htab_find(&ht, "a", 1, &pval));
    TEST_INT_EQ(10, *(int*)pval);
    TEST_INT_EQ(0, collect_c_htab_find(&ht, "c", 1, &pval));
    TEST_INT_EQ(30, *(int*)pval);

    /* Remove head of remaining chain */
    TEST_INT_EQ(0, collect_c_htab_remove(&ht, "a", 1));
    TEST_INT_EQ(0, collect_c_htab_find(&ht, "c", 1, &pval));
    TEST_INT_EQ(30, *(int*)pval);

    /* Remove tail / sole remaining */
    TEST_INT_EQ(0, collect_c_htab_remove(&ht, "c", 1));
    TEST_INT_EQ(0, collect_c_htab_size(&ht));

    collect_c_htab_free_storage(&ht);
}

static void TEST_grow_under_load(void)
{
    collect_c_htab_t    ht;
    size_t              i;
    size_t const        n = 64;
    size_t              buckets_before;

    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   sizeof(size_t)
    ,   sizeof(size_t)
    ,   4
    ,   collect_c_hash_sdbm_mbuf
    ,   collect_c_compare_bytes
    ,   NULL
    ,   NULL
    ));

    buckets_before = collect_c_htab_bucket_count(&ht);

    for (i = 0; i != n; ++i)
    {
        TEST_INT_EQ(0, collect_c_htab_insert(&ht, &i, sizeof(i), &i, sizeof(i), NULL, NULL));
    }

    TEST_INT_EQ(n, collect_c_htab_size(&ht));
    TEST_INT_GT(0, buckets_before);
    TEST_INT_GT(buckets_before, collect_c_htab_bucket_count(&ht));

    for (i = 0; i != n; ++i)
    {
        void*   pval = NULL;

        TEST_INT_EQ(0, collect_c_htab_find(&ht, &i, sizeof(i), &pval));
        TEST_INT_EQ(i, *(size_t*)pval);
    }

    collect_c_htab_free_storage(&ht);
}

static void TEST_case_insensitive_keys(void)
{
    collect_c_htab_t    ht;
    int                 v = 7;
    void*               pval = NULL;

    /* Pair CI compare with a hash that does not scatter case variants
     * (stock *_mbuf hashes are case-sensitive; a future *_mbuf_case hash
     * would be the production pairing). */
    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   0
    ,   sizeof(int)
    ,   1
    ,   hash_always_zero
    ,   collect_c_compare_mbuf_case
    ,   NULL
    ,   NULL
    ));

    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "Key", 3, &v, sizeof(v), NULL, NULL));
    TEST_INT_EQ(0, collect_c_htab_find(&ht, "key", 3, &pval));
    TEST_INT_EQ(7, *(int*)pval);
    TEST_INT_EQ(0, collect_c_htab_find(&ht, "KEY", 3, &pval));
    TEST_INT_EQ(7, *(int*)pval);

    collect_c_htab_free_storage(&ht);
}

static void TEST_for_each_and_clear(void)
{
    collect_c_htab_t    ht;
    visit_count_t       vc = { 0 };
    free_counter_t      fc = { 0 };
    int                 a = 1;
    int                 b = 2;

    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   0
    ,   sizeof(int)
    ,   8
    ,   collect_c_hash_djb2_mbuf
    ,   collect_c_compare_mbuf
    ,   counting_free
    ,   &fc
    ));

    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "a", 1, &a, sizeof(a), NULL, NULL));
    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "b", 1, &b, sizeof(b), NULL, NULL));

    TEST_INT_EQ(0, collect_c_htab_for_each(&ht, counting_visit, &vc));
    TEST_INT_EQ(2, vc.count);

    collect_c_htab_clear(&ht);
    TEST_INT_EQ(0, collect_c_htab_size(&ht));
    TEST_INT_EQ(2, fc.count);
    TEST_INT_EQ(8, collect_c_htab_bucket_count(&ht));

    collect_c_htab_free_storage(&ht);
}

static void TEST_sizes(void)
{
    collect_c_htab_t    ht;
    size_t              num_elements = 99;
    size_t              num_buckets = 99;
    int                 v = 1;

    TEST_INT_EQ(0, collect_c_htab_init(
        &ht
    ,   0
    ,   sizeof(int)
    ,   4
    ,   collect_c_hash_djb2_mbuf
    ,   collect_c_compare_mbuf
    ,   NULL
    ,   NULL
    ));

    collect_c_htab_sizes(&ht, &num_elements, &num_buckets);
    TEST_INT_EQ(0, num_elements);
    TEST_INT_EQ(4, num_buckets);
    TEST_INT_EQ(num_elements, collect_c_htab_size(&ht));
    TEST_INT_EQ(num_buckets, collect_c_htab_bucket_count(&ht));

    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "a", 1, &v, sizeof(v), NULL, NULL));
    TEST_INT_EQ(0, collect_c_htab_insert(&ht, "b", 1, &v, sizeof(v), NULL, NULL));

    num_elements = 99;
    num_buckets = 99;
    collect_c_htab_sizes(&ht, &num_elements, NULL);
    TEST_INT_EQ(2, num_elements);
    TEST_INT_EQ(99, num_buckets);

    collect_c_htab_sizes(&ht, NULL, &num_buckets);
    TEST_INT_EQ(2, num_elements);
    TEST_INT_EQ(4, num_buckets);

    collect_c_htab_sizes(&ht, NULL, NULL);

    collect_c_htab_free_storage(&ht);
    collect_c_htab_sizes(&ht, &num_elements, &num_buckets);
    TEST_INT_EQ(0, num_elements);
    TEST_INT_EQ(0, num_buckets);
}


/* ///////////////////////////// end of file //////////////////////////// */

