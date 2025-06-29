/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.util.limits/entry.c
 *
 * Purpose: Unit-test for Type-detecting limits functions/macros.
 *
 * Created: 7th June 2025
 * Updated: 7th June 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/util/limits.h>

#include <xtests/terse-api.h>

#include <limits.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_FOR_BUILT_IN_INTEGRAL_TYPES(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.util.limits", verbosity))
    {
        XTESTS_RUN_CASE(TEST_FOR_BUILT_IN_INTEGRAL_TYPES);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

static void TEST_FOR_BUILT_IN_INTEGRAL_TYPES(void)
{
    // signed char
    {
        // by value
        {
            signed char const   v       =   123; /* NOTE: the value is irrelevant */

            signed char const   maximum =   COLLECT_C_LIMITS_maximum(v);
            signed char const   minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(CHAR_MAX, maximum);
            TEST_INT_EQ(CHAR_MIN, minimum);
        }

        // by pionter
        {
            signed char const   v       =   123; /* NOTE: the value is irrelevant */

            signed char const   maximum =   COLLECT_C_LIMITS_maximum(&v);
            signed char const   minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(CHAR_MAX, maximum);
            TEST_INT_EQ(CHAR_MIN, minimum);
        }
    }

    // unsigned char
    {
        // by value
        {
            unsigned char const  v       =   123; /* NOTE: the value is irrelevant */

            unsigned char const  maximum =   COLLECT_C_LIMITS_maximum(v);
            unsigned char const  minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(UCHAR_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }

        // by pionter
        {
            unsigned char const v       =   123; /* NOTE: the value is irrelevant */

            unsigned char const maximum =   COLLECT_C_LIMITS_maximum(&v);
            unsigned char const minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(UCHAR_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }
    }

    // signed short
    {
        // by value
        {
            signed short const  v       =   123; /* NOTE: the value is irrelevant */

            signed short const  maximum =   COLLECT_C_LIMITS_maximum(v);
            signed short const  minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(SHRT_MAX, maximum);
            TEST_INT_EQ(SHRT_MIN, minimum);
        }

        // by pointer
        {
            signed short const  v       =   123; /* NOTE: the value is irrelevant */

            signed short const  maximum =   COLLECT_C_LIMITS_maximum(&v);
            signed short const  minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(SHRT_MAX, maximum);
            TEST_INT_EQ(SHRT_MIN, minimum);
        }
    }

    // unsigned short
    {
        // by value
        {
            unsigned short const    v       =   123; /* NOTE: the value is irrelevant */

            unsigned short const    maximum =   COLLECT_C_LIMITS_maximum(v);
            unsigned short const    minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(USHRT_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }

        // by pointer
        {
            unsigned short const    v       =   123; /* NOTE: the value is irrelevant */

            unsigned short const    maximum =   COLLECT_C_LIMITS_maximum(&v);
            unsigned short const    minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(USHRT_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }
    }

    // signed int
    {
        // by value
        {
            signed int const    v       =   123; /* NOTE: the value is irrelevant */

            signed int const    maximum =   COLLECT_C_LIMITS_maximum(v);
            signed int const    minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(INT_MAX, maximum);
            TEST_INT_EQ(INT_MIN, minimum);
        }

        // by pointer
        {
            signed int const    v       =   123; /* NOTE: the value is irrelevant */

            signed int const    maximum =   COLLECT_C_LIMITS_maximum(&v);
            signed int const    minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(INT_MAX, maximum);
            TEST_INT_EQ(INT_MIN, minimum);
        }
    }

    // unsigned int
    {
        // by value
        {
            unsigned int const  v       =   123; /* NOTE: the value is irrelevant */

            unsigned int const  maximum =   COLLECT_C_LIMITS_maximum(v);
            unsigned int const  minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(UINT_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }

        // by pointer
        {
            unsigned int const  v       =   123; /* NOTE: the value is irrelevant */

            unsigned int const  maximum =   COLLECT_C_LIMITS_maximum(&v);
            unsigned int const  minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(UINT_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }
    }

    // unsigned int
    {
        // by value
        {
            unsigned int const  v       =   123; /* NOTE: the value is irrelevant */

            unsigned int const  maximum =   COLLECT_C_LIMITS_maximum(v);
            unsigned int const  minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(UINT_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }

        // by pointer
        {
            unsigned int const  v       =   123; /* NOTE: the value is irrelevant */

            unsigned int const  maximum =   COLLECT_C_LIMITS_maximum(&v);
            unsigned int const  minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(UINT_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }
    }

    // signed long
    {
        // by value
        {
            signed long const   v       =   123; /* NOTE: the value is irrelevant */

            signed long const   maximum =   COLLECT_C_LIMITS_maximum(v);
            signed long const   minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(LONG_MAX, maximum);
            TEST_INT_EQ(LONG_MIN, minimum);
        }

        // by pionter
        {
            signed long const   v       =   123; /* NOTE: the value is irrelevant */

            signed long const   maximum =   COLLECT_C_LIMITS_maximum(&v);
            signed long const   minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(LONG_MAX, maximum);
            TEST_INT_EQ(LONG_MIN, minimum);
        }
    }

    // unsigned long
    {
        // by value
        {
            unsigned long const  v       =   123; /* NOTE: the value is irrelevant */

            unsigned long const  maximum =   COLLECT_C_LIMITS_maximum(v);
            unsigned long const  minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(ULONG_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }

        // by pionter
        {
            unsigned long const v       =   123; /* NOTE: the value is irrelevant */

            unsigned long const maximum =   COLLECT_C_LIMITS_maximum(&v);
            unsigned long const minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(ULONG_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }
    }

    // signed long long
    {
        // by value
        {
            signed long long const  v       =   123; /* NOTE: the value is irrelevant */

            signed long long const  maximum =   COLLECT_C_LIMITS_maximum(v);
            signed long long const  minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(LLONG_MAX, maximum);
            TEST_INT_EQ(LLONG_MIN, minimum);
        }

        // by pionter
        {
            signed long long const  v       =   123; /* NOTE: the value is irrelevant */

            signed long long const  maximum =   COLLECT_C_LIMITS_maximum(&v);
            signed long long const  minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(LLONG_MAX, maximum);
            TEST_INT_EQ(LLONG_MIN, minimum);
        }
    }

    // unsigned long long
    {
        // by value
        {
            unsigned long long const    v       =   123; /* NOTE: the value is irrelevant */

            unsigned long long const    maximum =   COLLECT_C_LIMITS_maximum(v);
            unsigned long long const    minimum =   COLLECT_C_LIMITS_minimum(v);

            TEST_INT_EQ(ULLONG_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }

        // by pionter
        {
            unsigned long long const    v       =   123; /* NOTE: the value is irrelevant */

            unsigned long long const    maximum =   COLLECT_C_LIMITS_maximum(&v);
            unsigned long long const    minimum =   COLLECT_C_LIMITS_minimum(&v);

            TEST_INT_EQ(ULLONG_MAX, maximum);
            TEST_INT_EQ(0, minimum);
        }
    }
}


 /* ///////////////////////////// end of file //////////////////////////// */

