/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.tmap/entry.c
 *
 * Purpose: Unit-test for Tree-map container.
 *
 * Created: 14th February 2025
 * Updated: 19th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/terse/tmap.h>

#include <xtests/terse-api.h>

#include <errno.h>
#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_define_empty(void);
static void TEST_insert_1_ELEMENT(void);
static void TEST_insert_2_ELEMENTS(void);
static void TEST_insert_1000_ELEMENTS(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.tmap", verbosity))
    {
        XTESTS_RUN_CASE(TEST_define_empty);
        XTESTS_RUN_CASE(TEST_insert_1_ELEMENT);
        XTESTS_RUN_CASE(TEST_insert_2_ELEMENTS);
        XTESTS_RUN_CASE(TEST_insert_1000_ELEMENTS);

        XTESTS_PRINT_RESULTS();

        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* /////////////////////////////////////////////////////////////////////////
 * test helpers
 */

struct custom_t
{
    uint32_t    x;
    uint32_t    y;
    uint64_t    z;
};
typedef struct custom_t custom_t;

int cmp_int(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int cmp_bool(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int cmp_custom_t(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
)
{
    assert(NULL != pe_lhs);
    assert(NULL != pe_rhs);
    assert(sizeof(custom_t) != el_size);

    ((void)&el_size);

    {
        custom_t const* const   lhs =   pe_lhs;
        custom_t const* const   rhs =   pe_rhs;

        if (lhs->x < rhs->x)
        {
            return -1;
        }

        if (lhs->x > rhs->x)
        {
            return +1;
        }

        if (lhs->y < rhs->y)
        {
            return -1;
        }

        if (lhs->y > rhs->y)
        {
            return +1;
        }

        if (lhs->z < rhs->z)
        {
            return -1;
        }

        if (lhs->z > rhs->z)
        {
            return +1;
        }

        return 0;
    }
}

int walk_int_int(
    size_t      key_size
,   size_t      val_size
,   intptr_t    el_index    /* always -1 */
,   size_t      depth
,   void const* p_el_key
,   void*       p_el_val
,   void*       param_walk
)
{
    assert(sizeof(int) == key_size);
    assert(sizeof(int) == val_size);

    ((void)&el_index);
    ((void)&param_walk);

    fprintf(stderr, "[%zu] %d => %d\n"
    ,   depth
    ,   *(int const*)p_el_key
    ,   *(int const*)p_el_val
    );

    return 1;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

static void TEST_define_empty(void)
{
    {
        CLC_TM_define_empty(int, int, m);

        TEST_BOOLEAN_TRUE(CLC_TM_is_empty(m));
        TEST_INT_EQ(0, CLC_TM_len(m));
    }

    {
        CLC_TM_define_empty(int, bool, m);

        TEST_BOOLEAN_TRUE(CLC_TM_is_empty(m));
        TEST_INT_EQ(0, CLC_TM_len(m));
    }

    {
        CLC_TM_define_empty(int, custom_t, m);

        TEST_BOOLEAN_TRUE(CLC_TM_is_empty(m));
        TEST_INT_EQ(0, CLC_TM_len(m));
    }

    {
        CLC_TM_define_empty_with_cmp(custom_t, custom_t, cmp_custom_t, m);

        TEST_BOOLEAN_TRUE(CLC_TM_is_empty(m));
        TEST_INT_EQ(0, CLC_TM_len(m));
    }
}

static void TEST_insert_1_ELEMENT(void)
{
    {
        CLC_TM_define_empty(int, int, m);

        int         was_replaced;
        int const   r = CLC_TM_insert_by_val(m, int, 101, int, -101, &was_replaced);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            TEST_INT_EQ(0, was_replaced);

            TEST_BOOLEAN_FALSE(CLC_TM_is_empty(m));
            TEST_INT_EQ(1, CLC_TM_len(m));

            {
                int const                       value   =   101;
                collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                TEST_PTR_NE(NULL, node);
                TEST_PTR_EQ(NULL, node->left);
                TEST_PTR_EQ(NULL, node->right);

                TEST_INT_EQ(-101, *(int const*)&node->data[1].data[0]);
            }

            {
                int const                       value   =   100;
                collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                TEST_PTR_EQ(NULL, node);
            }

            {
                int const                       value   =   102;
                collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                TEST_PTR_EQ(NULL, node);
            }

            clc_treemap_free_storage(&m);
        }
    }
}

static void TEST_insert_2_ELEMENTS(void)
{
    {
        CLC_TM_define_empty(int, int, m);

        int         was_replaced;
        int const   r = CLC_TM_insert_by_val(m, int, 101, int, -101, &was_replaced);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            TEST_INT_EQ(0, was_replaced);

            TEST_BOOLEAN_FALSE(CLC_TM_is_empty(m));
            TEST_INT_EQ(1, CLC_TM_len(m));

            {
                int const                       value   =   101;
                collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                TEST_PTR_NE(NULL, node);
                TEST_PTR_EQ(NULL, node->left);
                TEST_PTR_EQ(NULL, node->right);

                TEST_INT_EQ(-101, *(int const*)&node->data[1].data[0]);
            }

            {
                int const r2 = CLC_TM_insert_by_val(m, int, 202, int, -202, &was_replaced);

                TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r2);

                if (0 == r2)
                {
                    TEST_INT_EQ(0, was_replaced);

                    TEST_BOOLEAN_FALSE(CLC_TM_is_empty(m));
                    TEST_INT_EQ(2, CLC_TM_len(m));
                }

                {
                    int const                       value   =   101;
                    collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                    TEST_PTR_NE(NULL, node);
                    TEST_PTR_EQ(NULL, node->left);
                    TEST_PTR_NE(NULL, node->right);

                    TEST_INT_EQ(-101, *(int const*)&node->data[1].data[0]);
                }

                {
                    int const                       value   =   202;
                    collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                    TEST_PTR_NE(NULL, node);
                    TEST_PTR_EQ(NULL, node->left);
                    TEST_PTR_EQ(NULL, node->right);

                    TEST_INT_EQ(-202, *(int const*)&node->data[1].data[0]);
                }

                {
                    int const r3 = CLC_TM_insert_by_val(m, int, 201, int, -201, &was_replaced);

                    TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r3);

                    if (0 == r3)
                    {
                        TEST_INT_EQ(0, was_replaced);

                        TEST_BOOLEAN_FALSE(CLC_TM_is_empty(m));
                        TEST_INT_EQ(3, CLC_TM_len(m));
                    }

                    {
                        int const                       value   =   101;
                        collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                        TEST_PTR_NE(NULL, node);
                        TEST_PTR_EQ(NULL, node->left);
                        TEST_PTR_NE(NULL, node->right);

                        TEST_INT_EQ(-101, *(int const*)&node->data[1].data[0]);
                    }

                    {
                        int const                       value   =   201;
                        collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                        TEST_PTR_NE(NULL, node);
                        TEST_PTR_EQ(NULL, node->left);
                        TEST_PTR_EQ(NULL, node->right);

                        TEST_INT_EQ(-201, *(int const*)&node->data[1].data[0]);
                    }

                    {
                        int const                       value   =   202;
                        collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                        TEST_PTR_NE(NULL, node);
                        TEST_PTR_NE(NULL, node->left);
                        TEST_PTR_EQ(NULL, node->right);

                        TEST_INT_EQ(-202, *(int const*)&node->data[1].data[0]);
                    }

                    {
                        int const r4 = CLC_TM_insert_by_val(m, int, 303, int, -303, &was_replaced);

                        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r4);

                        if (0 == r4)
                        {
                            TEST_INT_EQ(0, was_replaced);

                            TEST_BOOLEAN_FALSE(CLC_TM_is_empty(m));
                            TEST_INT_EQ(4, CLC_TM_len(m));
                        }

                        {
                            int const                       value   =   101;
                            collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                            TEST_PTR_NE(NULL, node);
                            TEST_PTR_EQ(NULL, node->left);
                            TEST_PTR_NE(NULL, node->right);

                            TEST_INT_EQ(-101, *(int const*)&node->data[1].data[0]);
                        }

                        {
                            int const                       value   =   201;
                            collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                            TEST_PTR_NE(NULL, node);
                            TEST_PTR_EQ(NULL, node->left);
                            TEST_PTR_EQ(NULL, node->right);

                            TEST_INT_EQ(-201, *(int const*)&node->data[1].data[0]);
                        }

                        {
                            int const                       value   =   202;
                            collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                            TEST_PTR_NE(NULL, node);
                            TEST_PTR_NE(NULL, node->left);
                            TEST_PTR_NE(NULL, node->right);

                            TEST_INT_EQ(-202, *(int const*)&node->data[1].data[0]);
                        }

                        {
                            int const                       value   =   303;
                            collect_c_tmap_node_t const*    node    =   collect_c_tmap_find_node(&m, &value);

                            TEST_PTR_NE(NULL, node);
                            TEST_PTR_EQ(NULL, node->left);
                            TEST_PTR_EQ(NULL, node->right);

                            TEST_INT_EQ(-303, *(int const*)&node->data[1].data[0]);
                        }
                    }
                }
            }

            clc_treemap_free_storage(&m);
        }
    }
}

static void TEST_insert_1000_ELEMENTS(void)
{
    {
        CLC_TM_define_empty(int, int, m);

        for (int i = -10, n = 1; 10 != i; ++i, ++n)
        {
            int         was_replaced;
            int const   r = CLC_TM_insert_by_val(m, int, i, int, -i, &was_replaced);

            TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

            if (0 != r)
            {
                break;
            }
            else
            {
                TEST_INT_EQ(0, was_replaced);

                TEST_BOOLEAN_FALSE(CLC_TM_is_empty(m));
                TEST_INT_EQ((size_t)n, CLC_TM_len(m));
            }
        }

        {
            int const r = collect_c_tmap_walk(&m, walk_int_int, NULL, CLC_TM_WALK_DEFAULT);

            TEST_INT_EQ(0, r);
        }

        clc_treemap_free_storage(&m);
    }

    {
        int const values[20] =
        {
            0,
            -5, +5,
            -2, +2,
            -7, +7,
            -1, +1,
            -3, +3,
            -4, +4,
            -6, +6,
            -8, +8,
            -9, +9,
            -10
        };

        CLC_TM_define_empty(int, int, m);

        for (int i = 0, n = 1; STLSOFT_NUM_ELEMENTS(values) != i; ++i, ++n)
        {
            int         was_replaced;
            int const   v = values[i];
            int const   r = CLC_TM_insert_by_val(m, int, v, int, -i, &was_replaced);

            TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

            if (0 != r)
            {
                break;
            }
            else
            {
                TEST_INT_EQ(0, was_replaced);

                TEST_BOOLEAN_FALSE(CLC_TM_is_empty(m));
                TEST_INT_EQ((size_t)n, CLC_TM_len(m));
            }
        }

        {
            int const r = collect_c_tmap_walk(&m, walk_int_int, NULL, CLC_TM_WALK_DEFAULT);

            TEST_INT_EQ(0, r);
        }

        {
            int const r = collect_c_tmap_walk(&m, walk_int_int, NULL, CLC_TM_WALK_DOWNWARD);

            TEST_INT_EQ(0, r);
        }

        {
            int const r = collect_c_tmap_walk(&m, walk_int_int, NULL, CLC_TM_WALK_FORWARD);

            TEST_INT_EQ(0, r);
        }

        {
            int const r = collect_c_tmap_walk(&m, walk_int_int, NULL, CLC_TM_WALK_BACKWARD);

            TEST_INT_EQ(0, r);
        }

        clc_treemap_free_storage(&m);
    }
}


 /* ///////////////////////////// end of file //////////////////////////// */

