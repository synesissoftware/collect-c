/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.vec/entry.c
 *
 * Purpose: Unit-test for vector.
 *
 * Created: 5th February 2025
 * Updated: 11th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/terse/vec.h>

#include <xtests/terse-api.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_V_define_empty(void);
static void TEST_V_define_empty_with_cb(void);
static void TEST_V_define_empty_THEN_allocate_storage(void);
static void TEST_V_define_on_stack(void);
static void TEST_V_define_on_stack_AND_push_back_UNTIL_FULL_THEN_FAIL(void);
static void TEST_V_define_empty_AND_push_back_UNTIL_FULL_THEN_clear_THEN_push_front_UNTIL_FULL_AND_VERIFY_NO_REALLOCATION(void);
static void TEST_V_define_empty_AND_allocate_storage_VERY_LARGE_THEN_push_back_1_ELEMENT_THEN_shrink_to_fit(void);
static void TEST_V_define_empty_THEN_allocate_storage_THEN_push_back_UNTIL_FULL_THEN_reallocate(void);
static void TEST_V_define_empty_THEN_allocate_storage_THEN_push_front_UNTIL_FULL_THEN_reallocate(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.vec", verbosity))
    {
        XTESTS_RUN_CASE(TEST_V_define_empty);
        XTESTS_RUN_CASE(TEST_V_define_empty_with_cb);
        XTESTS_RUN_CASE(TEST_V_define_empty_THEN_allocate_storage);
        XTESTS_RUN_CASE(TEST_V_define_on_stack);
        XTESTS_RUN_CASE(TEST_V_define_on_stack_AND_push_back_UNTIL_FULL_THEN_FAIL);
        XTESTS_RUN_CASE(TEST_V_define_empty_AND_push_back_UNTIL_FULL_THEN_clear_THEN_push_front_UNTIL_FULL_AND_VERIFY_NO_REALLOCATION);
        XTESTS_RUN_CASE(TEST_V_define_empty_AND_allocate_storage_VERY_LARGE_THEN_push_back_1_ELEMENT_THEN_shrink_to_fit);
        XTESTS_RUN_CASE(TEST_V_define_empty_THEN_allocate_storage_THEN_push_back_UNTIL_FULL_THEN_reallocate);
        XTESTS_RUN_CASE(TEST_V_define_empty_THEN_allocate_storage_THEN_push_front_UNTIL_FULL_THEN_reallocate);

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

void fn_int_stub(
    size_t  el_size
,   size_t  el_index
,   void*   el_ptr
,   void*   param_element_free
)
{
    ((void)&el_size);
    ((void)&el_index);
    ((void)&el_ptr);
    ((void)&param_element_free);
}

int
accumulate_v2(
    collect_c_vec_t*    v
,   int                 init
)
{
    int r = init;

    for (size_t i = 0; v->size != i; ++i)
    {
        int const value = *COLLECT_C_VEC_cat_t(*v, int, i);

        r += value;
    }

    return r;
}
int
accumulate_v3(
    int const*          b
,   int const* const    e
,   int                 init
)
{
    int r = init;

    for (; e != b; ++b)
    {
        r += *b;
    }

    return r;
}

static void TEST_V_define_empty(void)
{
    {
        CLC_V_define_empty(int, v);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(0, CLC_V_spare_front(v));
        TEST_INT_EQ(0, CLC_V_spare_back(v));
        TEST_INT_EQ(0, CLC_V_spare(v));
    }

    {
        CLC_V_define_empty(bool, v);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(0, CLC_V_spare_front(v));
        TEST_INT_EQ(0, CLC_V_spare_back(v));
        TEST_INT_EQ(0, CLC_V_spare(v));
    }

    {
        CLC_V_define_empty(custom_t, v);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(0, CLC_V_spare_front(v));
        TEST_INT_EQ(0, CLC_V_spare_back(v));
        TEST_INT_EQ(0, CLC_V_spare(v));
    }
}

static void TEST_V_define_empty_with_cb(void)
{
    {
        CLC_V_define_empty_with_cb(int, v, fn_int_stub, NULL);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(0, CLC_V_spare_front(v));
        TEST_INT_EQ(0, CLC_V_spare_back(v));
        TEST_INT_EQ(0, CLC_V_spare(v));
    }

    {
        CLC_V_define_empty_with_cb(bool, v, fn_int_stub, NULL);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(0, CLC_V_spare_front(v));
        TEST_INT_EQ(0, CLC_V_spare_back(v));
        TEST_INT_EQ(0, CLC_V_spare(v));
    }

    {
        CLC_V_define_empty_with_cb(custom_t, v, fn_int_stub, NULL);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(0, CLC_V_spare_front(v));
        TEST_INT_EQ(0, CLC_V_spare_back(v));
        TEST_INT_EQ(0, CLC_V_spare(v));
    }
}

static void TEST_V_define_empty_THEN_allocate_storage(void)
{
    {
        CLC_V_define_empty(int, v);

        int const r = collect_c_vec_allocate_storage(&v, 32);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
            TEST_INT_EQ( 0, CLC_V_len(v));
            TEST_INT_EQ( 8, CLC_V_spare_front(v));
            TEST_INT_EQ(32, CLC_V_spare_back(v));
            TEST_INT_EQ(40, CLC_V_spare(v));

            collect_c_vec_free_storage(&v);
        }
    }
}

static void TEST_V_define_on_stack(void)
{
    {
        int array[32];

        CLC_V_define_on_stack(v, array);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(0, CLC_V_spare_front(v));
        TEST_INT_EQ(32, CLC_V_spare_back(v));
        TEST_INT_EQ(32, CLC_V_spare(v));
    }
}

static void TEST_V_define_on_stack_AND_push_back_UNTIL_FULL_THEN_FAIL(void)
{
    {
        int array[8];

        CLC_V_define_on_stack(v, array);

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ(0, CLC_V_len(v));
        TEST_INT_EQ(8, CLC_V_spare(v));

        /* add el[0] */
        {
            int const   el  =   101;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(1, CLC_V_len(v));
            TEST_INT_EQ(7, CLC_V_spare(v));

            TEST_INT_EQ(101, *CLC_V_cat_t(v, int, 0));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(2, CLC_V_len(v));
            TEST_INT_EQ(6, CLC_V_spare(v));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(3, CLC_V_len(v));
            TEST_INT_EQ(5, CLC_V_spare(v));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(4, CLC_V_len(v));
            TEST_INT_EQ(4, CLC_V_spare(v));
        }

        /* add el[4] */
        {
            int const   el  =   505;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(5, CLC_V_len(v));
            TEST_INT_EQ(3, CLC_V_spare(v));
        }

        /* add el[5] */
        {
            int const   el  =   606;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(6, CLC_V_len(v));
            TEST_INT_EQ(2, CLC_V_spare(v));
        }

        /* add el[6] */
        {
            int const   el  =   707;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(7, CLC_V_len(v));
            TEST_INT_EQ(1, CLC_V_spare(v));
        }

        /* add el[7] */
        {
            int const   el  =   808;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(8, CLC_V_len(v));
            TEST_INT_EQ(0, CLC_V_spare(v));
        }

        /* fail to add another add when no space */
        {
            int const   el  =   911;
            int const   r   =   CLC_V_push_back_by_ref(v, &el);

            TEST_INT_EQ(ENOSPC, r);

            TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
            TEST_INT_EQ(8, CLC_V_len(v));
            TEST_INT_EQ(0, CLC_V_spare(v));
        }
    }
}

static void TEST_V_define_empty_AND_push_back_UNTIL_FULL_THEN_clear_THEN_push_front_UNTIL_FULL_AND_VERIFY_NO_REALLOCATION(void)
{
    CLC_V_define_empty(int, v);

    v.flags |= CLC_V_F_NO_BOO;

    int const r = collect_c_vec_allocate_storage(&v, 32);

    TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

    if (0 == r)
    {
        void* const start_storage = v.storage;

        TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
        TEST_INT_EQ( 0, CLC_V_len(v));
        TEST_INT_EQ( 0, CLC_V_spare_front(v));
        TEST_INT_EQ(32, CLC_V_spare_back(v));
        TEST_INT_EQ(32, CLC_V_spare(v));

        /* add 32 elements, then assert that no reallocation occured */
        {
            for (int i = 0; 32 != i; ++i)
            {
                int const r2 = CLC_V_push_back_by_ref(v, &i);

                TEST_INT_EQ(0, r2);

                TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                TEST_INT_EQ(i, *CLC_V_cat_t(v, int, CLC_V_len(v) - 1));
            }

            TEST_INT_EQ(32, CLC_V_len(v));
            TEST_INT_EQ( 0, CLC_V_spare_front(v));
            TEST_INT_EQ( 0, CLC_V_spare_back(v));
            TEST_INT_EQ( 0, CLC_V_spare(v));

            TEST_INT_EQ(496, accumulate_v2(&v, 0));
            TEST_INT_EQ(496, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

            void* const curr_storage = v.storage;

            TEST_PTR_EQ(start_storage, curr_storage);
        }

        /* clear */
        {
            collect_c_vec_clear(&v, NULL, NULL, NULL);

            TEST_INT_EQ( 0, CLC_V_len(v));
            TEST_INT_EQ( 0, CLC_V_spare_front(v));
            TEST_INT_EQ(32, CLC_V_spare_back(v));
            TEST_INT_EQ(32, CLC_V_spare(v));

            void* const curr_storage = v.storage;

            TEST_PTR_EQ(start_storage, curr_storage);
        }

        /* add 32 elements, then assert that no reallocation occured */
        {
            for (int i = 0; 32 != i; ++i)
            {
                int const r2 = CLC_V_push_back_by_ref(v, &i);

                TEST_INT_EQ(0, r2);

                TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                TEST_INT_EQ(i, *CLC_V_cat_t(v, int, CLC_V_len(v) - 1));
            }

            TEST_INT_EQ(32, CLC_V_len(v));
            TEST_INT_EQ( 0, CLC_V_spare_front(v));
            TEST_INT_EQ( 0, CLC_V_spare_back(v));
            TEST_INT_EQ( 0, CLC_V_spare(v));

            TEST_INT_EQ(496, accumulate_v2(&v, 0));
            TEST_INT_EQ(496, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

            void* const curr_storage = v.storage;

            TEST_PTR_EQ(start_storage, curr_storage);
        }

        collect_c_vec_free_storage(&v);
    }
}

static void TEST_V_define_empty_AND_allocate_storage_VERY_LARGE_THEN_push_back_1_ELEMENT_THEN_shrink_to_fit(void)
{
    {
        CLC_V_define_empty(int, v);

        int const r = collect_c_vec_allocate_storage(&v, 100000);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            void* const     start_storage   =   v.storage;
            // size_t const    start_capacity  =   v.capacity;

            TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
            TEST_INT_EQ( 0, CLC_V_len(v));
            TEST_INT_GE(100000, CLC_V_spare(v));


            /* add 1 element */
            {
                int const r2 = CLC_V_push_back_by_value(v, int, 101);

                TEST_INT_EQ(0, r2);

                TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));
                TEST_INT_EQ( 1, CLC_V_len(v));
                TEST_INT_GE(99999, CLC_V_spare(v));
                // TEST_INT_NE(start_capacity, CLC_V_spare(v));

                void* const curr_storage = v.storage;

                TEST_PTR_EQ(start_storage, curr_storage);
            }


            /* shrink to fit */
            {
                int const r3 = CLC_V_shrink_to_fit(v);

                TEST_INT_EQ(0, r3);
            }


            collect_c_vec_free_storage(&v);
        }
    }
}

static void TEST_V_define_empty_THEN_allocate_storage_THEN_push_back_UNTIL_FULL_THEN_reallocate(void)
{
    {
        CLC_V_define_empty(int, v);

        int const r = collect_c_vec_allocate_storage(&v, 32);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
            TEST_INT_EQ( 0, CLC_V_len(v));
            TEST_INT_EQ( 8, CLC_V_spare_front(v));
            TEST_INT_EQ(32, CLC_V_spare_back(v));
            TEST_INT_EQ(40, CLC_V_spare(v));

            /* add 32 elements, then assert that no reallocation occured */
            {
                void* const     start_storage   =   v.storage;

                for (int i = 0; 32 != i; ++i)
                {
                    CLC_V_push_back_by_ref(v, &i);

                    TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                    TEST_INT_EQ(i, *CLC_V_cat_t(v, int, CLC_V_len(v) - 1));
                }

                TEST_INT_EQ(32, CLC_V_len(v));
                TEST_INT_NE( 0, CLC_V_spare_front(v));
                TEST_INT_EQ( 0, CLC_V_spare_back(v));
                TEST_INT_NE( 0, CLC_V_spare(v));

                TEST_INT_EQ(496, accumulate_v2(&v, 0));
                TEST_INT_EQ(496, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

                TEST_PTR_EQ(start_storage, curr_storage);
            }

            /* add 1 more element, then assert that reallocation occured */
            {
                int const       v32             =   32;

                void* const     start_storage   =   v.storage;

                CLC_V_push_back_by_ref(v, &v32);

                TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                TEST_INT_EQ(v32, *CLC_V_cat_t(v, int, CLC_V_len(v) - 1));

                TEST_INT_EQ(33, CLC_V_len(v));
                TEST_INT_NE( 0, CLC_V_spare_front(v));
                TEST_INT_EQ( 0, CLC_V_spare_back(v));
                TEST_INT_NE( 0, CLC_V_spare(v));

                TEST_INT_EQ(528, accumulate_v2(&v, 0));
                TEST_INT_EQ(528, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

#if 0 /* This test might fail if reallocate gives back same (albeit extended) block */

                TEST_PTR_NE(start_storage, curr_storage);
#else

                ((void)&start_storage);
                ((void)&curr_storage);
#endif
            }

            /* add 7 more elements, then assert that no reallocation occured, and that there's no spare front or back */
            {
                void* const     start_storage   =   v.storage;

                for (int i = 0; 7 != i; ++i)
                {
                    int const value = 33 + i;

                    CLC_V_push_back_by_ref(v, &value);

                    TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                    TEST_INT_EQ(value, *CLC_V_back_t(v, int));
                }

                TEST_INT_EQ(40, CLC_V_len(v));
                TEST_INT_EQ( 0, CLC_V_spare_front(v));
                TEST_INT_EQ( 0, CLC_V_spare_back(v));
                TEST_INT_EQ( 0, CLC_V_spare(v));

                TEST_INT_EQ(780, accumulate_v2(&v, 0));
                TEST_INT_EQ(780, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

                TEST_PTR_EQ(start_storage, curr_storage);
            }

            /* add 1 more element, then assert that reallocation occured */
            {
                int const       v40             =   40;

                void* const     start_storage   =   v.storage;

                CLC_V_push_back_by_ref(v, &v40);

                TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                TEST_INT_EQ(v40, *CLC_V_cat_t(v, int, CLC_V_len(v) - 1));

                TEST_INT_EQ(41, CLC_V_len(v));
                TEST_INT_NE( 0, CLC_V_spare_front(v));
                TEST_INT_NE( 0, CLC_V_spare_back(v));
                TEST_INT_GE(20, CLC_V_spare(v));

                TEST_INT_EQ(820, accumulate_v2(&v, 0));
                TEST_INT_EQ(820, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

#if 0 /* This test might fail if reallocate gives back same (albeit extended) block */

                TEST_PTR_NE(start_storage, curr_storage);
#else

                ((void)&start_storage);
                ((void)&curr_storage);
#endif
            }

            collect_c_vec_free_storage(&v);
        }
    }
}

static void TEST_V_define_empty_THEN_allocate_storage_THEN_push_front_UNTIL_FULL_THEN_reallocate(void)
{
    {
        CLC_V_define_empty(int, v);

        int const r = collect_c_vec_allocate_storage(&v, 32);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            TEST_BOOLEAN_TRUE(CLC_V_is_empty(v));
            TEST_INT_EQ( 0, CLC_V_len(v));
            TEST_INT_EQ( 8, CLC_V_spare_front(v));
            TEST_INT_EQ(32, CLC_V_spare_back(v));
            TEST_INT_EQ(40, CLC_V_spare(v));

            /* add 32 elements, then assert that no reallocation occured */
            {
                void* const     start_storage   =   v.storage;

                for (int i = 0; 32 != i; ++i)
                {
                    int const r2 = CLC_V_push_front_by_ref(v, &i);

                    TEST_INT_EQ(0, r2);

                    TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                    TEST_INT_EQ(i, *CLC_V_cfront_t(v, int));
                    TEST_INT_EQ(0, *CLC_V_cback_t(v, int));

                    TEST_INT_NE(-1, accumulate_v2(&v, 0));
                }

                TEST_INT_EQ(32, CLC_V_len(v));
                TEST_INT_EQ( 1, CLC_V_spare_front(v));
                TEST_INT_EQ( 7, CLC_V_spare_back(v));
                TEST_INT_EQ( 8, CLC_V_spare(v));

                TEST_INT_EQ(496, accumulate_v2(&v, 0));
                TEST_INT_EQ(496, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

                TEST_PTR_EQ(start_storage, curr_storage);
            }

            /* add 1 more element, then assert that reallocation occured */
            {
                int const       v32             =   32;

                void* const     start_storage   =   v.storage;

                CLC_V_push_front_by_ref(v, &v32);

                TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                TEST_INT_EQ(v32, *CLC_V_cat_t(v, int, 0));

                TEST_INT_EQ(33, CLC_V_len(v));
                TEST_INT_EQ( 0, CLC_V_spare_front(v));
                TEST_INT_EQ( 7, CLC_V_spare_back(v));
                TEST_INT_EQ( 7, CLC_V_spare(v));

                TEST_INT_EQ(528, accumulate_v2(&v, 0));
                TEST_INT_EQ(528, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

#if 0 /* This test might fail if reallocate gives back same (albeit extended) block */

                TEST_PTR_NE(start_storage, curr_storage);
#else

                ((void)&start_storage);
                ((void)&curr_storage);
#endif
            }

            /* add 7 more elements, then assert that reallocation occured */
            {
                void* const     start_storage   =   v.storage;

                for (int i = 0; 7 != i; ++i)
                {
                    int const   v3x =   33 + i;
                    int const   r2  =   CLC_V_push_front_by_ref(v, &v3x);

                    TEST_INT_EQ(0, r2);

                    TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                    TEST_INT_EQ(v3x, *CLC_V_cfront_t(v, int));

                    TEST_INT_EQ((size_t)(33 + (i + 1)), CLC_V_len(v));
                    TEST_INT_EQ( 0, CLC_V_spare_front(v));
                    TEST_INT_EQ((size_t)(7 - (i + 1)), CLC_V_spare_back(v));
                    TEST_INT_EQ((size_t)(7 - (i + 1)), CLC_V_spare(v));
                }

                TEST_INT_EQ(780, accumulate_v2(&v, 0));
                TEST_INT_EQ(780, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

#if 0 /* This test might fail if reallocate gives back same (albeit extended) block */

                TEST_PTR_NE(start_storage, curr_storage);
#else

                ((void)&start_storage);
                ((void)&curr_storage);
#endif
            }

            /* add 1 more element, then assert that reallocation occured */
            {
                int const       v40             =   40;

                void* const     start_storage   =   v.storage;

                CLC_V_push_front_by_ref(v, &v40);

                TEST_BOOLEAN_FALSE(CLC_V_is_empty(v));

                TEST_INT_EQ(v40, *CLC_V_cfront_t(v, int));

                TEST_INT_EQ(41, CLC_V_len(v));
                TEST_INT_EQ(14, CLC_V_spare_front(v));
                TEST_INT_NE( 0, CLC_V_spare_back(v));
                TEST_INT_NE( 0, CLC_V_spare(v));

                TEST_INT_EQ(820, accumulate_v2(&v, 0));
                TEST_INT_EQ(820, accumulate_v3(COLLECT_C_VEC_cbegin_t(v, int), COLLECT_C_VEC_cend_t(v, int), 0));

                void* const     curr_storage    =   v.storage;

#if 0 /* This test might fail if reallocate gives back same (albeit extended) block */

                TEST_PTR_NE(start_storage, curr_storage);
#else

                ((void)&start_storage);
                ((void)&curr_storage);
#endif
            }

            collect_c_vec_free_storage(&v);
        }
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

