
#include <collect-c/circq.h>

#include <xtests/terse-api.h>

#include <assert.h>
#include <stdlib.h>


static void TEST_COUNTERS(void);

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.cq", verbosity))
    {
        XTESTS_RUN_CASE(TEST_COUNTERS);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


struct clc_cq_nix_test_t
{
    size_t  ix;
    size_t  n;
};
#ifndef __cplusplus
typedef struct clc_cq_nix_test_t clc_cq_nix_test_t;
#endif

static
clc_cq_nix_test_t
calc_nix(
    size_t b
,   size_t e
,   size_t capacity
)
{
    assert(b <= e);

    clc_cq_nix_test_t r;

    r.ix    =   b % capacity;
    r.n     =   e - b;

    return r;
}


static void TEST_COUNTERS(void)
{
    {
        size_t const            b   =   0;
        size_t const            e   =   0;

        clc_cq_nix_test_t const nix =   calc_nix(b, e, 8);

        TEST_INT_EQ(0, nix.ix);
        TEST_INT_EQ(0, nix.n);
    }

    {
        size_t const            b   =   0;
        size_t const            e   =   1;

        clc_cq_nix_test_t const nix =   calc_nix(b, e, 8);

        TEST_INT_EQ(0, nix.ix);
        TEST_INT_EQ(1, nix.n);
    }

    {
        size_t const            b   =   0;
        size_t const            e   =   5;

        clc_cq_nix_test_t const nix =   calc_nix(b, e, 8);

        TEST_INT_EQ(0, nix.ix);
        TEST_INT_EQ(5, nix.n);
    }

    {
        size_t const            b   =   6;
        size_t const            e   =   13;

        clc_cq_nix_test_t const nix =   calc_nix(b, e, 8);

        TEST_INT_EQ(6, nix.ix);
        TEST_INT_EQ(7, nix.n);
    }

    {
        size_t const            b   =   106;
        size_t const            e   =   113;

        clc_cq_nix_test_t const nix =   calc_nix(b, e, 8);

        TEST_INT_EQ(2, nix.ix);
        TEST_INT_EQ(7, nix.n);
    }

    {
        size_t const            b   =   111;
        size_t const            e   =   113;

        clc_cq_nix_test_t const nix =   calc_nix(b, e, 8);

        TEST_INT_EQ(7, nix.ix);
        TEST_INT_EQ(2, nix.n);
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

