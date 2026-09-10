/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.stock_cmp_fns/entry.c
 *
 * Purpose: Unit-test for stock comparison functions.
 *
 * Created: 19th February 2025
 * Updated: 19th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common.h>

#include <xtests/terse-api.h>

#include <limits.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_collect_c_fn_cmp_short(void);
static void TEST_collect_c_fn_cmp_int(void);
static void TEST_collect_c_fn_cmp_long(void);
static void TEST_collect_c_fn_cmp_llong(void);
static void TEST_collect_c_fn_cmp_int16(void);
static void TEST_collect_c_fn_cmp_int32(void);
static void TEST_collect_c_fn_cmp_int64(void);
static void TEST_collect_c_fn_cmp_uint64(void);

static void TEST_collect_c_fn_cmp_ccs(void);
static void TEST_collect_c_fn_cmp_ccs_ignore_case(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.stock_cmp_fns", verbosity))
    {
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_short);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_int);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_long);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_llong);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_int16);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_int32);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_int64);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_uint64);

        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_ccs);
        XTESTS_RUN_CASE(TEST_collect_c_fn_cmp_ccs_ignore_case);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

static void TEST_collect_c_fn_cmp_short(void)
{
    /* == */
    {
        {
            short const lhs =   0;
            short const rhs =   0;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            short const lhs =   -1;
            short const rhs =   -1;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            short const lhs =   +1;
            short const rhs =   +1;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            short const lhs =   SHRT_MIN;
            short const rhs =   SHRT_MIN;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            short const lhs =   SHRT_MAX;
            short const rhs =   SHRT_MAX;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            short const lhs =   -1;
            short const rhs =   +1;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            short const lhs =   -1;
            short const rhs =   0;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            short const lhs =   0;
            short const rhs =   +1;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            short const lhs =   SHRT_MIN;
            short const rhs =   SHRT_MAX;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            short const lhs =   +1;
            short const rhs =   -1;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            short const lhs =   +1;
            short const rhs =   0;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            short const lhs =   0;
            short const rhs =   -1;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            short const lhs =   SHRT_MAX;
            short const rhs =   SHRT_MIN;

            int const   r   =   collect_c_fn_cmp_short(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_int(void)
{
    /* == */
    {
        {
            int const   lhs =   0;
            int const   rhs =   0;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int const   lhs =   -1;
            int const   rhs =   -1;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int const   lhs =   +1;
            int const   rhs =   +1;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int const   lhs =   INT_MIN;
            int const   rhs =   INT_MIN;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int const   lhs =   INT_MAX;
            int const   rhs =   INT_MAX;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            int const   lhs =   -1;
            int const   rhs =   +1;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int const   lhs =   -1;
            int const   rhs =   0;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int const   lhs =   0;
            int const   rhs =   +1;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int const   lhs =   INT_MIN;
            int const   rhs =   INT_MAX;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            int const   lhs =   +1;
            int const   rhs =   -1;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int const   lhs =   +1;
            int const   rhs =   0;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int const   lhs =   0;
            int const   rhs =   -1;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int const   lhs =   INT_MAX;
            int const   rhs =   INT_MIN;

            int const   r   =   collect_c_fn_cmp_int(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_long(void)
{
    /* == */
    {
        {
            long const  lhs =   0;
            long const  rhs =   0;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long const  lhs =   -1;
            long const  rhs =   -1;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long const  lhs =   +1;
            long const  rhs =   +1;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long const  lhs =   LONG_MIN;
            long const  rhs =   LONG_MIN;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long const  lhs =   LONG_MAX;
            long const  rhs =   LONG_MAX;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            long const  lhs =   -1;
            long const  rhs =   +1;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            long const  lhs =   -1;
            long const  rhs =   0;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            long const  lhs =   0;
            long const  rhs =   +1;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            long const  lhs =   LONG_MIN;
            long const  rhs =   LONG_MAX;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            long const  lhs =   +1;
            long const  rhs =   -1;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            long const  lhs =   +1;
            long const  rhs =   0;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            long const  lhs =   0;
            long const  rhs =   -1;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            long const  lhs =   LONG_MAX;
            long const  rhs =   LONG_MIN;

            int const   r   =   collect_c_fn_cmp_long(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_llong(void)
{
    /* == */
    {
        {
            long long const lhs =   0;
            long long const rhs =   0;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long long const lhs =   -1;
            long long const rhs =   -1;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long long const lhs =   +1;
            long long const rhs =   +1;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long long const lhs =   LLONG_MIN;
            long long const rhs =   LLONG_MIN;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            long long const lhs =   LLONG_MAX;
            long long const rhs =   LLONG_MAX;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            long long const lhs =   -1;
            long long const rhs =   +1;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            long long const lhs =   -1;
            long long const rhs =   0;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            long long const lhs =   0;
            long long const rhs =   +1;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            long long const lhs =   LLONG_MIN;
            long long const rhs =   LLONG_MAX;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            long long const lhs =   +1;
            long long const rhs =   -1;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            long long const lhs =   +1;
            long long const rhs =   0;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            long long const lhs =   0;
            long long const rhs =   -1;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            long long const lhs =   LLONG_MAX;
            long long const rhs =   LLONG_MIN;

            int const   r   =   collect_c_fn_cmp_llong(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_int16(void)
{
    /* == */
    {
        {
            int16_t const   lhs =   0;
            int16_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int16_t const   lhs =   -1;
            int16_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int16_t const   lhs =   +1;
            int16_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int16_t const   lhs =   INT16_MIN;
            int16_t const   rhs =   INT16_MIN;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int16_t const   lhs =   INT16_MAX;
            int16_t const   rhs =   INT16_MAX;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            int16_t const   lhs =   -1;
            int16_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int16_t const   lhs =   -1;
            int16_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int16_t const   lhs =   0;
            int16_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int16_t const   lhs =   INT16_MIN;
            int16_t const   rhs =   INT16_MAX;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            int16_t const   lhs =   +1;
            int16_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int16_t const   lhs =   +1;
            int16_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int16_t const   lhs =   0;
            int16_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int16_t const   lhs =   INT16_MAX;
            int16_t const   rhs =   INT16_MIN;

            int const       r   =   collect_c_fn_cmp_int16(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_int32(void)
{
    /* == */
    {
        {
            int32_t const   lhs =   0;
            int32_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int32_t const   lhs =   -1;
            int32_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int32_t const   lhs =   +1;
            int32_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int32_t const   lhs =   INT32_MIN;
            int32_t const   rhs =   INT32_MIN;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int32_t const   lhs =   INT32_MAX;
            int32_t const   rhs =   INT32_MAX;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            int32_t const   lhs =   -1;
            int32_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int32_t const   lhs =   -1;
            int32_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int32_t const   lhs =   0;
            int32_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int32_t const   lhs =   INT32_MIN;
            int32_t const   rhs =   INT32_MAX;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            int32_t const   lhs =   +1;
            int32_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int32_t const   lhs =   +1;
            int32_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int32_t const   lhs =   0;
            int32_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int32_t const   lhs =   INT32_MAX;
            int32_t const   rhs =   INT32_MIN;

            int const       r   =   collect_c_fn_cmp_int32(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_int64(void)
{
    /* == */
    {
        {
            int64_t const   lhs =   0;
            int64_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int64_t const   lhs =   -1;
            int64_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int64_t const   lhs =   +1;
            int64_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int64_t const   lhs =   INT64_MIN;
            int64_t const   rhs =   INT64_MIN;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            int64_t const   lhs =   INT64_MAX;
            int64_t const   rhs =   INT64_MAX;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            int64_t const   lhs =   -1;
            int64_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int64_t const   lhs =   -1;
            int64_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int64_t const   lhs =   0;
            int64_t const   rhs =   +1;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            int64_t const   lhs =   INT64_MIN;
            int64_t const   rhs =   INT64_MAX;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            int64_t const   lhs =   +1;
            int64_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int64_t const   lhs =   +1;
            int64_t const   rhs =   0;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int64_t const   lhs =   0;
            int64_t const   rhs =   -1;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            int64_t const   lhs =   INT64_MAX;
            int64_t const   rhs =   INT64_MIN;

            int const       r   =   collect_c_fn_cmp_int64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_uint64(void)
{
    /* == */
    {
        {
            uint64_t const  lhs =   0;
            uint64_t const  rhs =   0;

            int const       r   =   collect_c_fn_cmp_uint64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            uint64_t const  lhs =   +1;
            uint64_t const  rhs =   +1;

            int const       r   =   collect_c_fn_cmp_uint64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            uint64_t const  lhs =   UINT_MAX;
            uint64_t const  rhs =   UINT_MAX;

            int const       r   =   collect_c_fn_cmp_uint64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            uint64_t const  lhs =   0;
            uint64_t const  rhs =   +1;

            int const       r   =   collect_c_fn_cmp_uint64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            uint64_t const  lhs =   0;
            uint64_t const  rhs =   UINT_MAX;

            int const       r   =   collect_c_fn_cmp_uint64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            uint64_t const  lhs =   +1;
            uint64_t const  rhs =   0;

            int const       r   =   collect_c_fn_cmp_uint64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            uint64_t const  lhs =   UINT_MAX;
            uint64_t const  rhs =   0;

            int const       r   =   collect_c_fn_cmp_uint64(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_ccs(void)
{
    /* == */
    {
        {
            char const* lhs =   NULL;
            char const* rhs =   NULL;

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "";
            char const* rhs =   "";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "";
            char const* rhs =   NULL;

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   NULL;
            char const* rhs =   "";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "abc";
            char const* rhs =   "abc";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            char const* lhs =   "abc";
            char const* rhs =   "def";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            char const* lhs =   "ABC";
            char const* rhs =   "abc";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            char const* lhs =   "";
            char const* rhs =   "def";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            char const* lhs =   NULL;
            char const* rhs =   "def";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            char const* lhs =   "def";
            char const* rhs =   "abc";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            char const* lhs =   "abc";
            char const* rhs =   "ABC";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            char const* lhs =   "def";
            char const* rhs =   "";

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            char const* lhs =   "def";
            char const* rhs =   NULL;

            int const   r   =   collect_c_fn_cmp_ccs(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}

static void TEST_collect_c_fn_cmp_ccs_ignore_case(void)
{
    /* == */
    {
        {
            char const* lhs =   NULL;
            char const* rhs =   NULL;

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "";
            char const* rhs =   "";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "";
            char const* rhs =   NULL;

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   NULL;
            char const* rhs =   "";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "abc";
            char const* rhs =   "abc";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }
    }

    /* < */
    {
        {
            char const* lhs =   "abc";
            char const* rhs =   "def";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            char const* lhs =   "ABC";
            char const* rhs =   "abc";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "";
            char const* rhs =   "def";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }

        {
            char const* lhs =   NULL;
            char const* rhs =   "def";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_LT(0, r);
        }
    }

    /* > */
    {
        {
            char const* lhs =   "def";
            char const* rhs =   "abc";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            char const* lhs =   "abc";
            char const* rhs =   "ABC";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_EQ(0, r);
        }

        {
            char const* lhs =   "def";
            char const* rhs =   "";

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }

        {
            char const* lhs =   "def";
            char const* rhs =   NULL;

            int const   r   =   collect_c_fn_cmp_ccs_ignore_case(&lhs, &rhs, sizeof(lhs));

            TEST_INT_GT(0, r);
        }
    }
}


 /* ///////////////////////////// end of file //////////////////////////// */

