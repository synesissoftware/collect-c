/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.dlist/entry.c
 *
 * Purpose: Unit-test for doubly-linked list.
 *
 * Created: 7th February 2025
 * Updated: 8th February 2025
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
static void TEST_push_back_1_ELEMENT(void);
static void TEST_push_back_9_ELEMENTS(void);
static void TEST_push_front_1_ELEMENT(void);
static void TEST_push_front_9_ELEMENTS(void);
static void TEST_push_front_1_ELEMENT_THEN_clear(void);
static void TEST_push_front_9_ELEMENTS_THEN_clear(void);
static void TEST_push_back_9_ELEMENTS_THEN_find(void);
static void TEST_push_back_9_ELEMENTS_THEN_rfind(void);
static void TEST_push_back_9_ELEMENTS_THEN_find_THEN_erase(void);
static void TEST_push_back_9_ELEMENTS_THEN_find_THEN_erase_NO_SPARES(void);


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
        XTESTS_RUN_CASE(TEST_push_back_1_ELEMENT);
        XTESTS_RUN_CASE(TEST_push_back_9_ELEMENTS);
        XTESTS_RUN_CASE(TEST_push_front_1_ELEMENT);
        XTESTS_RUN_CASE(TEST_push_front_9_ELEMENTS);
        XTESTS_RUN_CASE(TEST_push_front_1_ELEMENT_THEN_clear);
        XTESTS_RUN_CASE(TEST_push_front_9_ELEMENTS_THEN_clear);
        XTESTS_RUN_CASE(TEST_push_back_9_ELEMENTS_THEN_find);
        XTESTS_RUN_CASE(TEST_push_back_9_ELEMENTS_THEN_rfind);
        XTESTS_RUN_CASE(TEST_push_back_9_ELEMENTS_THEN_find_THEN_erase);
        XTESTS_RUN_CASE(TEST_push_back_9_ELEMENTS_THEN_find_THEN_erase_NO_SPARES);

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

int compare_matching_int(
    collect_c_dlist_t const*    l
,   void const*                 p_lhs
,   void const*                 p_rhs
)
{
    int const* const    pi_lhs  =   (int const*)p_lhs;
    int const* const    pi_rhs  =   (int const*)p_rhs;
    int const           d       =   *pi_lhs - *pi_rhs;

    ((void)&l);

    if (d < 0)
    {
        return -1;
    }

    if (d > 0)
    {
        return +1;
    }

    return 0;
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

static void TEST_push_back_1_ELEMENT(void)
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

static void TEST_push_back_9_ELEMENTS(void)
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

static void TEST_push_front_1_ELEMENT(void)
{
    {
        CLC_DL_define_empty(int, l);

        int const r = CLC_DL_push_front_by_val(l, int, 101);

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

static void TEST_push_front_9_ELEMENTS(void)
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
            int const r = CLC_DL_push_front_by_val(l, int, values[i]);

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

static void TEST_push_front_1_ELEMENT_THEN_clear(void)
{
    {
        CLC_DL_define_empty(int, l);

        int const r = CLC_DL_push_front_by_val(l, int, 101);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            TEST_BOOLEAN_FALSE(CLC_DL_is_empty(l));
            TEST_INT_EQ(1, CLC_DL_len(l));
            TEST_INT_EQ(0, CLC_DL_spare(l));

            TEST_INT_EQ(101, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(101, accumulate_l2_backward(&l, 0));

            {
                int const r2 = CLC_DL_clear(l);

                TEST_INT_EQ(0, r2);

                TEST_BOOLEAN_TRUE(CLC_DL_is_empty(l));
                TEST_INT_EQ(0, CLC_DL_len(l));
                TEST_INT_EQ(1, CLC_DL_spare(l));
            }

            clc_dlist_free_storage(&l);
        }
    }

    {
        CLC_DL_define_empty(int, l);

        int const r = CLC_DL_push_front_by_val(l, int, 101);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            size_t num_dropped;

            TEST_BOOLEAN_FALSE(CLC_DL_is_empty(l));
            TEST_INT_EQ(1, CLC_DL_len(l));
            TEST_INT_EQ(0, CLC_DL_spare(l));

            TEST_INT_EQ(101, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(101, accumulate_l2_backward(&l, 0));

            {
                int const r2 = CLC_DL_clear(l, &num_dropped);

                TEST_INT_EQ(0, r2);

                TEST_INT_EQ(1, num_dropped);

                TEST_BOOLEAN_TRUE(CLC_DL_is_empty(l));
                TEST_INT_EQ(0, CLC_DL_len(l));
                TEST_INT_EQ(1, CLC_DL_spare(l));
            }

            clc_dlist_free_storage(&l);
        }
    }
}

static void TEST_push_front_9_ELEMENTS_THEN_clear(void)
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
            int const r = CLC_DL_push_front_by_val(l, int, values[i]);

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

            {
                size_t num_dropped;

                int const r2 = CLC_DL_clear(l, &num_dropped);

                TEST_INT_EQ(0, r2);

                TEST_INT_EQ(9, num_dropped);

                TEST_BOOLEAN_TRUE(CLC_DL_is_empty(l));
                TEST_INT_EQ(0, CLC_DL_len(l));
                TEST_INT_EQ(9, CLC_DL_spare(l));
            }
        }

        clc_dlist_free_storage(&l);
    }
}

static void TEST_push_back_9_ELEMENTS_THEN_find(void)
{
    {
        int const values[] =
        {
            1, 2, 3, 4, 5, 4, 3, 2, 1,
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
            TEST_INT_EQ(25, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(25, accumulate_l2_backward(&l, 0));

            /* find one that exists */
            {
                int const               v_1 =   1;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_find_node(&l, compare_matching_int, &v_1, 0, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(1, num_searched);
            }

            /* find another that exists */
            {
                int const               v_3 =   3;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_find_node(&l, compare_matching_int, &v_3, 0, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(3, num_searched);
            }

            /* find one that does not exist */
            {
                int const               v_6 =   6;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_find_node(&l, compare_matching_int, &v_6, 0, &node, &num_searched);

                TEST_INT_NE(0, r);

                TEST_PTR_EQ(NULL, node);
                TEST_INT_EQ(9, num_searched);
            }

            /* find one that exists twice, getting the second one */
            {
                int const               v_4 =   4;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_find_node(&l, compare_matching_int, &v_4, 1, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(6, num_searched);
            }
        }

        clc_dlist_free_storage(&l);
    }
}

static void TEST_push_back_9_ELEMENTS_THEN_rfind(void)
{
    {
        int const values[] =
        {
            1, 2, 3, 4, 5, 1, 2, 3, 4,
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
            TEST_INT_EQ(25, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(25, accumulate_l2_backward(&l, 0));

            /* find one that exists */
            {
                int const               v_4 =   4;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_rfind_node(&l, compare_matching_int, &v_4, 0, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(1, num_searched);
            }

            /* find another that exists */
            {
                int const               v_3 =   3;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_rfind_node(&l, compare_matching_int, &v_3, 0, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(2, num_searched);
            }

            /* find one that does not exist */
            {
                int const               v_6 =   6;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_rfind_node(&l, compare_matching_int, &v_6, 0, &node, &num_searched);

                TEST_INT_NE(0, r);

                TEST_PTR_EQ(NULL, node);
                TEST_INT_EQ(9, num_searched);
            }

            /* find one that exists twice, getting the second one */
            {
                int const               v_3 =   3;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_rfind_node(&l, compare_matching_int, &v_3, 1, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(7, num_searched);
            }
        }

        clc_dlist_free_storage(&l);
    }
}

static void TEST_push_back_9_ELEMENTS_THEN_find_THEN_erase(void)
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
            TEST_INT_EQ(9, CLC_DL_len(l));
            TEST_INT_EQ(45, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(45, accumulate_l2_backward(&l, 0));

            /* find and erase one that exists */
            {
                int const               v_6 = 6;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_find_node(&l, compare_matching_int, &v_6, 0, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(6, num_searched);

                {
                    int const r2 = CLC_DL_erase_node(&l, node);

                    TEST_INT_EQ(0, r2);

                    TEST_INT_EQ(8, CLC_DL_len(l));
                    TEST_INT_EQ(1, CLC_DL_spare(l));

                    TEST_INT_EQ(39, accumulate_l2_forward(&l, 0));
                    TEST_INT_EQ(39, accumulate_l2_backward(&l, 0));
                }
            }
        }

        clc_dlist_free_storage(&l);
    }
}

static void TEST_push_back_9_ELEMENTS_THEN_find_THEN_erase_NO_SPARES(void)
{
    {
        int const values[] =
        {
            1, 2, 3, 4, 5, 6, 7, 8, 9,
        };

        CLC_DL_define_empty(int, l);

        l.flags |= CLC_DL_F_NO_SPARES;

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
            TEST_INT_EQ(9, CLC_DL_len(l));
            TEST_INT_EQ(45, accumulate_l2_forward(&l, 0));
            TEST_INT_EQ(45, accumulate_l2_backward(&l, 0));

            /* find and erase one that exists */
            {
                int const               v_6 = 6;

                collect_c_dlist_node_t* node;
                size_t                  num_searched;
                int const               r = collect_c_dlist_find_node(&l, compare_matching_int, &v_6, 0, &node, &num_searched);

                TEST_INT_EQ(0, r);

                TEST_PTR_NE(NULL, node);
                TEST_INT_EQ(6, num_searched);

                {
                    int const r2 = CLC_DL_erase_node(&l, node);

                    TEST_INT_EQ(0, r2);

                    TEST_INT_EQ(8, CLC_DL_len(l));
                    TEST_INT_EQ(0, CLC_DL_spare(l));

                    TEST_INT_EQ(39, accumulate_l2_forward(&l, 0));
                    TEST_INT_EQ(39, accumulate_l2_backward(&l, 0));
                }
            }
        }

        clc_dlist_free_storage(&l);
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

