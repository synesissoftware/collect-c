/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.compare_fns/entry.c
 *
 * Purpose: Unit-test for stock key-comparison functions.
 *
 * Created: 12th October 2026
 * Updated: 12th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/compare.h>

#include <xtests/terse-api.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_collect_c_compare_bytes_EMPTY(void);
static void TEST_collect_c_compare_bytes_EQUAL(void);
static void TEST_collect_c_compare_bytes_UNEQUAL(void);
static void TEST_collect_c_compare_bytes_LENGTH_MISMATCH(void);
static void TEST_collect_c_compare_bytes_NULL_ZERO_LEN(void);
static void TEST_collect_c_compare_bytes_PARAM_IGNORED(void);

static void TEST_collect_c_compare_mbuf_EMPTY(void);
static void TEST_collect_c_compare_mbuf_EQUAL(void);
static void TEST_collect_c_compare_mbuf_UNEQUAL(void);
static void TEST_collect_c_compare_mbuf_LENGTH_MISMATCH(void);
static void TEST_collect_c_compare_mbuf_NULL_ZERO_LEN(void);
static void TEST_collect_c_compare_mbuf_PARAM_IGNORED(void);

static void TEST_collect_c_compare_mbuf_case_EMPTY(void);
static void TEST_collect_c_compare_mbuf_case_EQUAL_IGNORE_CASE(void);
static void TEST_collect_c_compare_mbuf_case_UNEQUAL(void);
static void TEST_collect_c_compare_mbuf_case_LENGTH_MISMATCH(void);
static void TEST_collect_c_compare_mbuf_case_NULL_ZERO_LEN(void);
static void TEST_collect_c_compare_mbuf_case_PARAM_IGNORED(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.compare_fns", verbosity))
    {
        XTESTS_RUN_CASE(TEST_collect_c_compare_bytes_EMPTY);
        XTESTS_RUN_CASE(TEST_collect_c_compare_bytes_EQUAL);
        XTESTS_RUN_CASE(TEST_collect_c_compare_bytes_UNEQUAL);
        XTESTS_RUN_CASE(TEST_collect_c_compare_bytes_LENGTH_MISMATCH);
        XTESTS_RUN_CASE(TEST_collect_c_compare_bytes_NULL_ZERO_LEN);
        XTESTS_RUN_CASE(TEST_collect_c_compare_bytes_PARAM_IGNORED);

        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_EMPTY);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_EQUAL);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_UNEQUAL);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_LENGTH_MISMATCH);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_NULL_ZERO_LEN);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_PARAM_IGNORED);

        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_case_EMPTY);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_case_EQUAL_IGNORE_CASE);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_case_UNEQUAL);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_case_LENGTH_MISMATCH);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_case_NULL_ZERO_LEN);
        XTESTS_RUN_CASE(TEST_collect_c_compare_mbuf_case_PARAM_IGNORED);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

static void TEST_collect_c_compare_bytes_EMPTY(void)
{
    {
        TEST_INT_EQ(0, collect_c_compare_bytes("", 0, "", 0, NULL));
    }
}

static void TEST_collect_c_compare_bytes_EQUAL(void)
{
    {
        uint8_t const a[] = { 0x01, 0x02, 0xff };
        uint8_t const b[] = { 0x01, 0x02, 0xff };

        TEST_INT_EQ(0, collect_c_compare_bytes(a, sizeof(a), b, sizeof(b), NULL));
    }

    {
        char const* const s = "foobar";

        TEST_INT_EQ(0, collect_c_compare_bytes(s, 6, s, 6, NULL));
    }
}

static void TEST_collect_c_compare_bytes_UNEQUAL(void)
{
    {
        uint8_t const a[] = { 0x01, 0x02 };
        uint8_t const b[] = { 0x01, 0x03 };

        TEST_INT_NE(0, collect_c_compare_bytes(a, sizeof(a), b, sizeof(b), NULL));
    }
}

static void TEST_collect_c_compare_bytes_LENGTH_MISMATCH(void)
{
    {
        char const* const s = "foo";

        /* Equal prefix: shorter is less. */
        TEST_INT_LT(0, collect_c_compare_bytes(s, 2, s, 3, NULL));
        TEST_INT_GT(0, collect_c_compare_bytes(s, 3, s, 2, NULL));
    }

    {
        /* Content decides before length: "zz" > "aaa". */
        char const* const a = "zz";
        char const* const b = "aaa";

        TEST_INT_GT(0, collect_c_compare_bytes(a, 2, b, 3, NULL));
        TEST_INT_LT(0, collect_c_compare_bytes(b, 3, a, 2, NULL));
    }
}

static void TEST_collect_c_compare_bytes_NULL_ZERO_LEN(void)
{
    {
        TEST_INT_EQ(0, collect_c_compare_bytes(NULL, 0, NULL, 0, NULL));
        TEST_INT_EQ(0, collect_c_compare_bytes(NULL, 0, "", 0, NULL));
        TEST_INT_EQ(0, collect_c_compare_bytes("", 0, NULL, 0, NULL));
    }
}

static void TEST_collect_c_compare_bytes_PARAM_IGNORED(void)
{
    {
        int                 sentinel = 0;
        char const* const   s        = "ab";

        TEST_INT_EQ(
            collect_c_compare_bytes(s, 2, s, 2, NULL)
        ,   collect_c_compare_bytes(s, 2, s, 2, &sentinel)
        );
    }
}

static void TEST_collect_c_compare_mbuf_EMPTY(void)
{
    {
        TEST_INT_EQ(0, collect_c_compare_mbuf("", 0, "", 0, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_EQUAL(void)
{
    {
        char const* const a = "Hello";
        char const* const b = "Hello";

        TEST_INT_EQ(0, collect_c_compare_mbuf(a, 5, b, 5, NULL));
    }

    {
        /* Embedded NUL counts: length-3 buffers match octet-for-octet. */
        char const a[] = { 'a', '\0', 'b' };
        char const b[] = { 'a', '\0', 'b' };

        TEST_INT_EQ(0, collect_c_compare_mbuf(a, 3, b, 3, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_UNEQUAL(void)
{
    {
        char const* const a = "Hello";
        char const* const b = "Hallo";

        TEST_INT_NE(0, collect_c_compare_mbuf(a, 5, b, 5, NULL));
    }

    {
        char const* const a = "Test";
        char const* const b = "test";

        TEST_INT_NE(0, collect_c_compare_mbuf(a, 4, b, 4, NULL));
    }

    {
        /* Embedded NUL: differs from a shorter "ab" only by length; here
         * same length but different after the NUL. */
        char const a[] = { 'a', '\0', 'b' };
        char const b[] = { 'a', '\0', 'c' };

        TEST_INT_NE(0, collect_c_compare_mbuf(a, 3, b, 3, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_LENGTH_MISMATCH(void)
{
    {
        char const* const s = "foo";

        TEST_INT_LT(0, collect_c_compare_mbuf(s, 2, s, 3, NULL));
        TEST_INT_GT(0, collect_c_compare_mbuf(s, 3, s, 2, NULL));
    }

    {
        char const* const a = "zz";
        char const* const b = "aaa";

        TEST_INT_GT(0, collect_c_compare_mbuf(a, 2, b, 3, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_NULL_ZERO_LEN(void)
{
    {
        TEST_INT_EQ(0, collect_c_compare_mbuf(NULL, 0, NULL, 0, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_PARAM_IGNORED(void)
{
    {
        int                 sentinel = 0;
        char const* const   s        = "xy";

        TEST_INT_EQ(
            collect_c_compare_mbuf(s, 2, s, 2, NULL)
        ,   collect_c_compare_mbuf(s, 2, s, 2, &sentinel)
        );
    }
}

static void TEST_collect_c_compare_mbuf_case_EMPTY(void)
{
    {
        TEST_INT_EQ(0, collect_c_compare_mbuf_case("", 0, "", 0, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_case_EQUAL_IGNORE_CASE(void)
{
    {
        char const* const a = "Test";
        char const* const b = "tEsT";

        TEST_INT_EQ(0, collect_c_compare_mbuf_case(a, 4, b, 4, NULL));
    }

    {
        char const* const a = "FOOBAR";
        char const* const b = "foobar";

        TEST_INT_EQ(0, collect_c_compare_mbuf_case(a, 6, b, 6, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_case_UNEQUAL(void)
{
    {
        char const* const a = "foo";
        char const* const b = "bar";

        TEST_INT_NE(0, collect_c_compare_mbuf_case(a, 3, b, 3, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_case_LENGTH_MISMATCH(void)
{
    {
        char const* const a = "ab";
        char const* const b = "ABC";

        /* Equal ignoring case on common prefix; shorter is less. */
        TEST_INT_LT(0, collect_c_compare_mbuf_case(a, 2, b, 3, NULL));
        TEST_INT_GT(0, collect_c_compare_mbuf_case(b, 3, a, 2, NULL));
    }

    {
        char const* const a = "ZZ";
        char const* const b = "aaa";

        TEST_INT_GT(0, collect_c_compare_mbuf_case(a, 2, b, 3, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_case_NULL_ZERO_LEN(void)
{
    {
        TEST_INT_EQ(0, collect_c_compare_mbuf_case(NULL, 0, NULL, 0, NULL));
    }
}

static void TEST_collect_c_compare_mbuf_case_PARAM_IGNORED(void)
{
    {
        int                 sentinel = 0;
        char const* const   a        = "Ab";
        char const* const   b        = "aB";

        TEST_INT_EQ(
            collect_c_compare_mbuf_case(a, 2, b, 2, NULL)
        ,   collect_c_compare_mbuf_case(a, 2, b, 2, &sentinel)
        );
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

