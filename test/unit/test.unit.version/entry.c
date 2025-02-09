/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.version/entry.c
 *
 * Purpose: Unit-test for version(s).
 *
 * Created: 7th February 2025
 * Updated: 9th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common/version.h>
#include <collect-c/circq.h>

#include <xtests/terse-api.h>

#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_LIBRARY_version(void);
static void TEST_CIRCQ_version(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.version", verbosity))
    {
        XTESTS_RUN_CASE(TEST_LIBRARY_version);
        XTESTS_RUN_CASE(TEST_CIRCQ_version);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */


static void TEST_LIBRARY_version(void)
{
    {
        TEST_INT_EQ(COLLECT_C_VER, collect_c_api_version());
    }
}

static void TEST_CIRCQ_version(void)
{
    {
        TEST_INT_EQ(COLLECT_C_CIRCQ_VER, collect_c_cq_version());
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

