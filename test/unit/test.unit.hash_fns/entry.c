/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.hash_fns/entry.c
 *
 * Purpose: Unit-test for stock hashing functions.
 *
 * Created: 12th October 2026
 * Updated: 30th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/hash.h>

#include <xtests/terse-api.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_collect_c_hash_djb2_mbuf_EMPTY(void);
static void TEST_collect_c_hash_djb2_mbuf_KNOWN_VECTORS(void);
static void TEST_collect_c_hash_djb2_mbuf_NULL_KEY_ZERO_LEN(void);
static void TEST_collect_c_hash_djb2_mbuf_PARAM_IGNORED(void);

static void TEST_collect_c_hash_fnv1a_mbuf_EMPTY(void);
static void TEST_collect_c_hash_fnv1a_mbuf_KNOWN_VECTORS(void);
static void TEST_collect_c_hash_fnv1a_mbuf_NULL_KEY_ZERO_LEN(void);
static void TEST_collect_c_hash_fnv1a_mbuf_PARAM_IGNORED(void);

static void TEST_collect_c_hash_sdbm_mbuf_EMPTY(void);
static void TEST_collect_c_hash_sdbm_mbuf_KNOWN_VECTORS(void);
static void TEST_collect_c_hash_sdbm_mbuf_NULL_KEY_ZERO_LEN(void);
static void TEST_collect_c_hash_sdbm_mbuf_PARAM_IGNORED(void);

static void TEST_collect_c_hash_PEERS_DIFFER(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER(__FILE__, verbosity))
    {
        XTESTS_RUN_CASE(TEST_collect_c_hash_djb2_mbuf_EMPTY);
        XTESTS_RUN_CASE(TEST_collect_c_hash_djb2_mbuf_KNOWN_VECTORS);
        XTESTS_RUN_CASE(TEST_collect_c_hash_djb2_mbuf_NULL_KEY_ZERO_LEN);
        XTESTS_RUN_CASE(TEST_collect_c_hash_djb2_mbuf_PARAM_IGNORED);

        XTESTS_RUN_CASE(TEST_collect_c_hash_fnv1a_mbuf_EMPTY);
        XTESTS_RUN_CASE(TEST_collect_c_hash_fnv1a_mbuf_KNOWN_VECTORS);
        XTESTS_RUN_CASE(TEST_collect_c_hash_fnv1a_mbuf_NULL_KEY_ZERO_LEN);
        XTESTS_RUN_CASE(TEST_collect_c_hash_fnv1a_mbuf_PARAM_IGNORED);

        XTESTS_RUN_CASE(TEST_collect_c_hash_sdbm_mbuf_EMPTY);
        XTESTS_RUN_CASE(TEST_collect_c_hash_sdbm_mbuf_KNOWN_VECTORS);
        XTESTS_RUN_CASE(TEST_collect_c_hash_sdbm_mbuf_NULL_KEY_ZERO_LEN);
        XTESTS_RUN_CASE(TEST_collect_c_hash_sdbm_mbuf_PARAM_IGNORED);

        XTESTS_RUN_CASE(TEST_collect_c_hash_PEERS_DIFFER);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

static void TEST_collect_c_hash_djb2_mbuf_EMPTY(void)
{
    {
        char const* const   s       =   "";
        collect_c_hash_t const h    =   collect_c_hash_djb2_mbuf(s, 0, NULL);

        TEST_INT_EQ(5381ull, h);
    }
}

static void TEST_collect_c_hash_djb2_mbuf_KNOWN_VECTORS(void)
{
    {
        char const* const   s       =   "a";
        collect_c_hash_t const h    =   collect_c_hash_djb2_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x2b606ull, h);
    }

    {
        char const* const   s       =   "foo";
        collect_c_hash_t const h    =   collect_c_hash_djb2_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0xb887389ull, h);
    }

    {
        char const* const   s       =   "foobar";
        collect_c_hash_t const h    =   collect_c_hash_djb2_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x652fde460beull, h);
    }

    {
        char const* const   s       =   "Hello, world!";
        collect_c_hash_t const h    =   collect_c_hash_djb2_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x51f27069e18796aeull, h);
    }
}

static void TEST_collect_c_hash_djb2_mbuf_NULL_KEY_ZERO_LEN(void)
{
    {
        collect_c_hash_t const h = collect_c_hash_djb2_mbuf(NULL, 0, NULL);

        TEST_INT_EQ(5381ull, h);
    }
}

static void TEST_collect_c_hash_djb2_mbuf_PARAM_IGNORED(void)
{
    {
        int                 sentinel = 0;
        char const* const   s        = "foobar";
        collect_c_hash_t const h_null = collect_c_hash_djb2_mbuf(s, strlen(s), NULL);
        collect_c_hash_t const h_ptr  = collect_c_hash_djb2_mbuf(s, strlen(s), &sentinel);

        TEST_INT_EQ(h_null, h_ptr);
    }
}

static void TEST_collect_c_hash_fnv1a_mbuf_EMPTY(void)
{
    {
        char const* const   s       =   "";
        collect_c_hash_t const h    =   collect_c_hash_fnv1a_mbuf(s, 0, NULL);

        TEST_INT_EQ(0xcbf29ce484222325ull, h);
    }
}

static void TEST_collect_c_hash_fnv1a_mbuf_KNOWN_VECTORS(void)
{
    {
        char const* const   s       =   "a";
        collect_c_hash_t const h    =   collect_c_hash_fnv1a_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0xaf63dc4c8601ec8cull, h);
    }

    {
        char const* const   s       =   "foo";
        collect_c_hash_t const h    =   collect_c_hash_fnv1a_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0xdcb27518fed9d577ull, h);
    }

    {
        char const* const   s       =   "foobar";
        collect_c_hash_t const h    =   collect_c_hash_fnv1a_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x85944171f73967e8ull, h);
    }

    {
        char const* const   s       =   "Hello, world!";
        collect_c_hash_t const h    =   collect_c_hash_fnv1a_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x38d1334144987bf4ull, h);
    }

    {
        uint8_t const       bytes[] =   { 0x00, 0xff, 0x7f };
        collect_c_hash_t const h    =   collect_c_hash_fnv1a_mbuf(bytes, sizeof(bytes), NULL);
        collect_c_hash_t const h2   =   collect_c_hash_fnv1a_mbuf(bytes, sizeof(bytes), NULL);

        TEST_INT_EQ(h, h2);
        TEST_INT_NE(0xcbf29ce484222325ull, h);
    }
}

static void TEST_collect_c_hash_fnv1a_mbuf_NULL_KEY_ZERO_LEN(void)
{
    {
        collect_c_hash_t const h = collect_c_hash_fnv1a_mbuf(NULL, 0, NULL);

        TEST_INT_EQ(0xcbf29ce484222325ull, h);
    }
}

static void TEST_collect_c_hash_fnv1a_mbuf_PARAM_IGNORED(void)
{
    {
        int                 sentinel = 0;
        char const* const   s        = "foobar";
        collect_c_hash_t const h_null = collect_c_hash_fnv1a_mbuf(s, strlen(s), NULL);
        collect_c_hash_t const h_ptr  = collect_c_hash_fnv1a_mbuf(s, strlen(s), &sentinel);

        TEST_INT_EQ(h_null, h_ptr);
    }
}

static void TEST_collect_c_hash_sdbm_mbuf_EMPTY(void)
{
    {
        char const* const   s       =   "";
        collect_c_hash_t const h    =   collect_c_hash_sdbm_mbuf(s, 0, NULL);

        TEST_INT_EQ(0ull, h);
    }
}

static void TEST_collect_c_hash_sdbm_mbuf_KNOWN_VECTORS(void)
{
    {
        char const* const   s       =   "a";
        collect_c_hash_t const h    =   collect_c_hash_sdbm_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x61ull, h);
    }

    {
        char const* const   s       =   "foo";
        collect_c_hash_t const h    =   collect_c_hash_sdbm_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x6632a94926ull, h);
    }

    {
        char const* const   s       =   "foobar";
        collect_c_hash_t const h    =   collect_c_hash_sdbm_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x430d469aa6437b0dull, h);
    }

    {
        char const* const   s       =   "Hello, world!";
        collect_c_hash_t const h    =   collect_c_hash_sdbm_mbuf(s, strlen(s), NULL);

        TEST_INT_EQ(0x1659ed9bcf856ff5ull, h);
    }
}

static void TEST_collect_c_hash_sdbm_mbuf_NULL_KEY_ZERO_LEN(void)
{
    {
        collect_c_hash_t const h = collect_c_hash_sdbm_mbuf(NULL, 0, NULL);

        TEST_INT_EQ(0ull, h);
    }
}

static void TEST_collect_c_hash_sdbm_mbuf_PARAM_IGNORED(void)
{
    {
        int                 sentinel = 0;
        char const* const   s        = "foobar";
        collect_c_hash_t const h_null = collect_c_hash_sdbm_mbuf(s, strlen(s), NULL);
        collect_c_hash_t const h_ptr  = collect_c_hash_sdbm_mbuf(s, strlen(s), &sentinel);

        TEST_INT_EQ(h_null, h_ptr);
    }
}

static void TEST_collect_c_hash_PEERS_DIFFER(void)
{
    {
        char const* const   s   =   "foobar";
        collect_c_hash_t const djb2  = collect_c_hash_djb2_mbuf(s, strlen(s), NULL);
        collect_c_hash_t const fnv1a = collect_c_hash_fnv1a_mbuf(s, strlen(s), NULL);
        collect_c_hash_t const sdbm  = collect_c_hash_sdbm_mbuf(s, strlen(s), NULL);

        TEST_INT_NE(djb2, fnv1a);
        TEST_INT_NE(djb2, sdbm);
        TEST_INT_NE(fnv1a, sdbm);
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

