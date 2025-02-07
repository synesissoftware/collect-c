/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.dlist/entry.c
 *
 * Purpose: Unit-test for doubly-linked list.
 *
 * Created: 7th February 2025
 * Updated: 7th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/terse/dlist.h>

#include <xtests/terse-api.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_define_empty(void);
static void TEST_ADD_1_ELEMENT(void);
static void TEST_ADD_9_ELEMENTS(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.dlist", verbosity))
    {
        XTESTS_RUN_CASE(TEST_define_empty);
        XTESTS_RUN_CASE(TEST_ADD_1_ELEMENT);
        XTESTS_RUN_CASE(TEST_ADD_9_ELEMENTS);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

struct custom_t
{
    uint32_t    x;
    uint32_t    y;
    uint64_t    z;
};
typedef struct custom_t custom_t;

int
accumulate_l2_forward(
    collect_c_dlist_t*  l
,   int                 init
)
{
    int r = init;

    for (collect_c_dlist_node_t* n = l->head; NULL != n; n = n->next)
    {
        int const value = *(int const*)&n->data->data[0];

        r += value;
    }

    return r;
}

int
accumulate_l2_backward(
    collect_c_dlist_t*  l
,   int                 init
)
{
    int r = init;

    for (collect_c_dlist_node_t* n = l->tail; NULL != n; n = n->prev)
    {
        int const value = *(int const*)&n->data->data[0];

        r += value;
    }

    return r;
}



static void TEST_define_empty(void)
{
    {
        CLC_DL_define_empty(int, l);

        TEST_BOOLEAN_TRUE(CLC_DL_is_empty(l));
        TEST_INT_EQ(0, CLC_DL_len(l));
        TEST_INT_EQ(0, CLC_DL_spare(l));

        TEST_INT_EQ(0, accumulate_l2_forward(&l, 0));
        TEST_INT_EQ(0, accumulate_l2_backward(&l, 0));
    }

    {
        CLC_DL_define_empty(bool, l);

        TEST_BOOLEAN_TRUE(CLC_DL_is_empty(l));
        TEST_INT_EQ(0, CLC_DL_len(l));
        TEST_INT_EQ(0, CLC_DL_spare(l));
    }

    {
        CLC_DL_define_empty(custom_t, l);

        TEST_BOOLEAN_TRUE(CLC_DL_is_empty(l));
        TEST_INT_EQ(0, CLC_DL_len(l));
        TEST_INT_EQ(0, CLC_DL_spare(l));
    }
}

static void TEST_ADD_1_ELEMENT(void)
{
    {
        CLC_DL_define_empty(int, l);

        int const r = CLC_DL_push_back_by_val(l, int, 101);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            TEST_BOOLEAN_FALSE(CLC_DL_is_empty(l));
            TEST_INT_EQ(1, CLC_DL_len(l));
            TEST_INT_EQ(0, CLC_DL_spare(l));

            TEST_INT_EQ(101, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(101, accumulate_l2_backward(&l, 0));

            clc_dlist_free_storage(&l);
        }
    }
}

static void TEST_ADD_9_ELEMENTS(void)
{
    {
        int const values[] =
        {
            1, 2, 3, 4, 5, 6, 7, 8, 9,
        };

        CLC_DL_define_empty(int, l);

        size_t num_succeeded = 0;

        for (size_t i = 0; STLSOFT_NUM_ELEMENTS(values) != i; ++i)
        {
            int const r = CLC_DL_push_back_by_val(l, int, values[i]);

            TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

            if (0 == r)
            {
                ++num_succeeded;

                TEST_BOOLEAN_FALSE(CLC_DL_is_empty(l));
                TEST_INT_EQ(num_succeeded, CLC_DL_len(l));
                TEST_INT_EQ(0, CLC_DL_spare(l));
            }
        }

        if (STLSOFT_NUM_ELEMENTS(values) == num_succeeded)
        {
            TEST_INT_EQ(45, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(45, accumulate_l2_backward(&l, 0));
        }

        clc_dlist_free_storage(&l);
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

