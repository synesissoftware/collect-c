/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.cq/entry.c
 *
 * Purpose: Unit-test for circular queue.
 *
 * Created: 5th February 2025
 * Updated: 10th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/terse/circq.h>

#include <xtests/terse-api.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void TEST_define_empty(void);
static void TEST_define_empty_AND_allocate(void);

static void TEST_define_on_stack(void);
static void TEST_define_on_stack_with_cb(void);

static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_FAIL_TO_push_by_ref(void);
static void TEST_STACK_AND_push_by_value_UNTIL_FULL_THEN_pop_front_TWO_THEN_push_by_value(void);
static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_pop_back_THEN_push_by_ref(void);
static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_clear(void);
static void TEST_STACK_AND_push_by_value_AND_clear_WITH_CB(void);
static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_F_OVERWRITE_FRONT_WHEN_FULL(void);

static void TEST_STACK_AND_collect_c_cq_push_back_n_by_ref_WITHOUT_OVERWRITE(void);
static void TEST_STACK_AND_collect_c_cq_push_back_n_by_ref_WITH_OVERWRITE(void);

static void TEST_HEAP_AND_push_by_ref_WITHOUT_WRAP(void);
static void TEST_HEAP_AND_CALLBACK_INDEXES_1(void);
static void TEST_HEAP_AND_CALLBACK_INDEXES_2(void);


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity = 2;

    XTESTS_COMMANDLINE_PARSE_HELP_OR_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("test.unit.cq", verbosity))
    {
        XTESTS_RUN_CASE(TEST_define_empty);
        XTESTS_RUN_CASE(TEST_define_empty_AND_allocate);

        XTESTS_RUN_CASE(TEST_define_on_stack);
        XTESTS_RUN_CASE(TEST_define_on_stack_with_cb);

        XTESTS_RUN_CASE(TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_FAIL_TO_push_by_ref);
        XTESTS_RUN_CASE(TEST_STACK_AND_push_by_value_UNTIL_FULL_THEN_pop_front_TWO_THEN_push_by_value);
        XTESTS_RUN_CASE(TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_pop_back_THEN_push_by_ref);
        XTESTS_RUN_CASE(TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_clear);
        XTESTS_RUN_CASE(TEST_STACK_AND_push_by_value_AND_clear_WITH_CB);
        XTESTS_RUN_CASE(TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_F_OVERWRITE_FRONT_WHEN_FULL);

        XTESTS_RUN_CASE(TEST_STACK_AND_collect_c_cq_push_back_n_by_ref_WITHOUT_OVERWRITE);
        XTESTS_RUN_CASE(TEST_STACK_AND_collect_c_cq_push_back_n_by_ref_WITH_OVERWRITE);

        XTESTS_RUN_CASE(TEST_HEAP_AND_push_by_ref_WITHOUT_WRAP);
        XTESTS_RUN_CASE(TEST_HEAP_AND_CALLBACK_INDEXES_1);
        XTESTS_RUN_CASE(TEST_HEAP_AND_CALLBACK_INDEXES_2);

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


static void fn_element_free_accumulate_on_free(
    size_t      el_size
,   intptr_t    el_index
,   void*       el_ptr
,   void*       param_element_free
)
{
    int* const  p_el    =   (int*)el_ptr;
    long* const p_sum   =   (long*)param_element_free;

    ((void)&el_size);
    ((void)&el_index);

    *p_sum += *p_el;
}

static void fn_element_free_record_most_recent_overwrite(
    size_t      el_size
,   intptr_t    el_index
,   void*       el_ptr
,   void*       param_element_free
)
{
    int* const  p_el    =   (int*)el_ptr;
    int* const  p_rec   =   (int*)param_element_free;

    ((void)&el_size);
    ((void)&el_index);

    *p_rec = *p_el;
}

static void fn_element_free_store_in_array(
    size_t      el_size
,   intptr_t    el_index
,   void*       el_ptr
,   void*       param_element_free
)
{
    int* const          ar_int  =   (int*)param_element_free;
    int const* const    p_el    =   (int const*)el_ptr;

    ((void)&el_size);

    ar_int[el_index] = *p_el;
}

static void fn_element_free_stub(
    size_t      el_size
,   intptr_t    el_index
,   void*       el_ptr
,   void*       param_element_free
)
{
    ((void)&el_size);
    ((void)&el_index);
    ((void)&el_ptr);
    ((void)&param_element_free);
}


/* /////////////////////////////////////////////////////////////////////////
 * test function definitions
 */

static void TEST_define_empty(void)
{
    {
        CLC_CQ_define_empty(int, q, 32);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(32, CLC_CQ_spare(q));
    }

    {
        CLC_CQ_define_empty(bool, q, 64);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(64, CLC_CQ_spare(q));
    }

    {
        CLC_CQ_define_empty(custom_t, q, 16);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(16, CLC_CQ_spare(q));
    }
}

static void TEST_define_empty_AND_allocate(void)
{
    {
        CLC_CQ_define_empty(int, q, 32);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            clc_cq_free_storage(&q);
        }
    }
}

static void TEST_define_on_stack(void)
{
    {
        int array[32];

        CLC_CQ_define_on_stack(q, array);

        TEST_INT_EQ(32, CLC_CQ_spare(q));
    }
}

static void TEST_define_on_stack_with_cb(void)
{
    {
        int array[32];

        CLC_CQ_define_on_stack_with_cb(q, array, fn_element_free_stub, &array[0]);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(32, CLC_CQ_spare(q));

        TEST_POINTER_EQUAL(&array[0], q.param_element_free);
        TEST_FUNCTION_POINTER_EQUAL(fn_element_free_stub, q.pfn_element_free);
    }
}

static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_FAIL_TO_push_by_ref(void)
{
    {
        int array[8];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        /* add el[0] */
        {
            int const   el  =   101;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(7, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(6, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(5, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(4, CLC_CQ_spare(q));
        }

        /* add el[4] */
        {
            int const   el  =   505;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(5, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[5] */
        {
            int const   el  =   606;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(6, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[6] */
        {
            int const   el  =   707;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(7, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[7] */
        {
            int const   el  =   808;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   el  =   911;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(ENOSPC, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }
    }
}

static void TEST_STACK_AND_push_by_value_UNTIL_FULL_THEN_pop_front_TWO_THEN_push_by_value(void)
{
    {
        int array[4];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(4, CLC_CQ_spare(q));

        /* add el[0] */
        {
            int const   r   =   CLC_CQ_push_back_by_value(q, int, 101);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   r   =   CLC_CQ_push_back_by_value(q, int, 202);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   r   =   CLC_CQ_push_back_by_value(q, int, 303);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   r   =   CLC_CQ_push_back_by_value(q, int, 404);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   r   =   CLC_CQ_push_back_by_value(q, int, 911);

            TEST_INT_EQ(ENOSPC, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 3));
        }

        /* pop front */
        {
            int r = CLC_CQ_pop_front(q);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 2));
        }

        /* pop front */
        {
            int r = CLC_CQ_pop_front(q);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 1));
        }

        /* add el[2]' */
        {
            int const   r   =   CLC_CQ_push_back_by_value(q, int, 502);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(502, *(int const*)CLC_CQ_at(q, 2));
        }

        /* add el[3]' */
        {
            int const   r   =   CLC_CQ_push_back_by_value(q, int, 603);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(502, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(603, *(int const*)CLC_CQ_at(q, 3));
        }
    }
}

static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_pop_back_THEN_push_by_ref(void)
{
    {
        int array[4];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(4, CLC_CQ_spare(q));

        /* add el[0] */
        {
            int const   el  =   101;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   el  =   911;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(ENOSPC, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 3));
        }

        /* pop back */
        {
            int r = CLC_CQ_pop_back(q);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 2));
        }

        /* pop back */
        {
            int r = CLC_CQ_pop_back(q);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
        }

        /* add el[2]' */
        {
            int const   el  =   503;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(503, *(int const*)CLC_CQ_at(q, 2));
        }

        /* add el[3]' */
        {
            int const   el  =   604;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(503, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(604, *(int const*)CLC_CQ_at(q, 3));
        }
    }
}

static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_clear(void)
{
    {
        int array[4];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(4, CLC_CQ_spare(q));

        /* add el[0] */
        {
            int const   el  =   101;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   el  =   911;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(ENOSPC, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 3));
        }

        /* pop back */
        {
            int r = CLC_CQ_pop_back(q);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 2));
        }

        /* pop back */
        {
            int r = CLC_CQ_pop_back(q);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
        }

        /* add el[2]' */
        {
            int const   el  =   503;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(503, *(int const*)CLC_CQ_at(q, 2));
        }

        /* add el[3]' */
        {
            int const   el  =   604;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* verify contents */
        {
            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(503, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(604, *(int const*)CLC_CQ_at(q, 3));
        }

        /* clear all elements */
        {
            CLC_CQ_clear(q);

            TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(0, CLC_CQ_len(q));
            TEST_INT_EQ(4, CLC_CQ_spare(q));
        }
    }
}

static void TEST_STACK_AND_push_by_value_AND_clear_WITH_CB(void)
{
    {
        long    sum = 0;
        size_t  num_removed;

        int     array[8];
        CLC_CQ_define_on_stack_with_cb(q, array, fn_element_free_accumulate_on_free, &sum);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        TEST_INT_EQ(0, sum);


        /* add five elements */

        CLC_CQ_push_back_by_value(q, int, 1);
        CLC_CQ_push_back_by_value(q, int, 2);
        CLC_CQ_push_back_by_value(q, int, 3);
        CLC_CQ_push_back_by_value(q, int, 4);
        CLC_CQ_push_back_by_value(q, int, 5);

        TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(5, CLC_CQ_len(q));
        TEST_INT_EQ(3, CLC_CQ_spare(q));

        TEST_INT_EQ(0, sum);


        /* remove first element */

        CLC_CQ_pop_front(q);

        TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(4, CLC_CQ_len(q));
        TEST_INT_EQ(4, CLC_CQ_spare(q));

        TEST_INT_EQ(1, sum);


        /* remove last element */

        CLC_CQ_pop_back(q);

        TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(3, CLC_CQ_len(q));
        TEST_INT_EQ(5, CLC_CQ_spare(q));

        TEST_INT_EQ(6, sum);


        /* clear all elements */

        CLC_CQ_clear(q, &num_removed);

        TEST_INT_EQ(3, num_removed);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        TEST_INT_EQ(15, sum);
    }
}

static void TEST_STACK_AND_push_by_ref_UNTIL_FULL_THEN_F_OVERWRITE_FRONT_WHEN_FULL(void)
{
    {
        int array[8];
        int previous = -1;

        COLLECT_C_CIRCQ_define_on_stack_with_callback(q, array, fn_element_free_record_most_recent_overwrite, &previous);

        q.flags |= CLC_CQ_F_OVERWRITE_FRONT_WHEN_FULL;

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        /* add el[0] */
        {
            int const   el  =   101;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(7, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(6, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(5, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(4, CLC_CQ_spare(q));
        }

        /* add el[4] */
        {
            int const   el  =   505;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(5, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[5] */
        {
            int const   el  =   606;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(6, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[6] */
        {
            int const   el  =   707;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(7, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[7] */
        {
            int const   el  =   808;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        {
            TEST_INT_EQ(-1, previous);

            TEST_INT_EQ(101, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 3));
            TEST_INT_EQ(505, *(int const*)CLC_CQ_at(q, 4));
            TEST_INT_EQ(606, *(int const*)CLC_CQ_at(q, 5));
            TEST_INT_EQ(707, *(int const*)CLC_CQ_at(q, 6));
            TEST_INT_EQ(808, *(int const*)CLC_CQ_at(q, 7));
        }

        /* add "el[8]" -> overwrite el[0] */
        {
            int const   el  =   909;
            int const   r   =   CLC_CQ_push_back_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        {
            TEST_INT_EQ(101, previous);

            TEST_INT_EQ(202, *(int const*)CLC_CQ_at(q, 0));
            TEST_INT_EQ(303, *(int const*)CLC_CQ_at(q, 1));
            TEST_INT_EQ(404, *(int const*)CLC_CQ_at(q, 2));
            TEST_INT_EQ(505, *(int const*)CLC_CQ_at(q, 3));
            TEST_INT_EQ(606, *(int const*)CLC_CQ_at(q, 4));
            TEST_INT_EQ(707, *(int const*)CLC_CQ_at(q, 5));
            TEST_INT_EQ(808, *(int const*)CLC_CQ_at(q, 6));
            TEST_INT_EQ(909, *(int const*)CLC_CQ_at(q, 7));
        }
    }
}

static void TEST_STACK_AND_collect_c_cq_push_back_n_by_ref_WITHOUT_OVERWRITE(void)
{
    /* push back 0 of 8 */
    {
        int array[8];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, 0, values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(0, num_inserted);

            TEST_INT_EQ(0, CLC_CQ_len(q));
            TEST_INT_EQ(8, CLC_CQ_spare(q));
        }
    }

    /* push back 1 of 8 */
    {
        int array[8];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, 1, values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(1, num_inserted);

            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(7, CLC_CQ_spare(q));

            TEST_INT_EQ(1, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(1, *CC_CQ_cback_t(q, int));
        }
    }

    /* push back 8 of 8 */
    {
        int array[8];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(8, num_inserted);

            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));

            TEST_INT_EQ(1, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(8, *CC_CQ_cback_t(q, int));
        }
    }

    /* push back 9 of 8 */
    {
        int array[8];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

            TEST_INT_EQ(ENOSPC, r1);
            TEST_INT_EQ(8, num_inserted);

            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));

            TEST_INT_EQ(1, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(8, *CC_CQ_cback_t(q, int));
        }
    }

    /* push back 16 of 8 */
    {
        int array[8];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

            TEST_INT_EQ(ENOSPC, r1);
            TEST_INT_EQ(8, num_inserted);

            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));

            TEST_INT_EQ(1, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(8, *CC_CQ_cback_t(q, int));
        }
    }
}

static void TEST_STACK_AND_collect_c_cq_push_back_n_by_ref_WITH_OVERWRITE(void)
{
    /* push back 0 of 8 */
    {
        int array[8];
        int previous = -1;

        COLLECT_C_CIRCQ_define_on_stack_with_callback(q, array, fn_element_free_record_most_recent_overwrite, &previous);

        q.flags |= CLC_CQ_F_OVERWRITE_FRONT_WHEN_FULL;

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, 0, values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(0, num_inserted);

            TEST_INT_EQ(-1, previous);

            TEST_INT_EQ(0, CLC_CQ_len(q));
            TEST_INT_EQ(8, CLC_CQ_spare(q));
        }
    }

    /* push back 1 of 8 */
    {
        int array[8];
        int previous = -1;

        COLLECT_C_CIRCQ_define_on_stack_with_callback(q, array, fn_element_free_record_most_recent_overwrite, &previous);

        q.flags |= CLC_CQ_F_OVERWRITE_FRONT_WHEN_FULL;

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, 1, values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(1, num_inserted);

            TEST_INT_EQ(-1, previous);

            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(7, CLC_CQ_spare(q));

            TEST_INT_EQ(1, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(1, *CC_CQ_cback_t(q, int));
        }
    }

    /* push back 8 of 8 */
    {
        int array[8];
        int previous = -1;

        COLLECT_C_CIRCQ_define_on_stack_with_callback(q, array, fn_element_free_record_most_recent_overwrite, &previous);

        q.flags |= CLC_CQ_F_OVERWRITE_FRONT_WHEN_FULL;

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(8, num_inserted);

            TEST_INT_EQ(-1, previous);

            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));

            TEST_INT_EQ(1, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(8, *CC_CQ_cback_t(q, int));
        }
    }

    /* push back 9 of 8 */
    {
        int array[8];
        int previous = -1;

        COLLECT_C_CIRCQ_define_on_stack_with_callback(q, array, fn_element_free_record_most_recent_overwrite, &previous);

        q.flags |= CLC_CQ_F_OVERWRITE_FRONT_WHEN_FULL;

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(9, num_inserted);

            TEST_INT_EQ(1, previous);

            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));

            TEST_INT_EQ(2, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(9, *CC_CQ_cback_t(q, int));
        }
    }

    /* push back 16 of 8 */
    {
        int array[8];
        int previous = -1;

        COLLECT_C_CIRCQ_define_on_stack_with_callback(q, array, fn_element_free_record_most_recent_overwrite, &previous);

        q.flags |= CLC_CQ_F_OVERWRITE_FRONT_WHEN_FULL;

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        {
            int const   values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, };

            size_t      num_inserted;
            int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

            TEST_INT_EQ(0, r1);
            TEST_INT_EQ(16, num_inserted);

            TEST_INT_EQ(8, previous);

            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));

            TEST_INT_EQ(9, *CC_CQ_cfront_t(q, int));
            TEST_INT_EQ(16, *CC_CQ_cback_t(q, int));
        }
    }
}

static void TEST_HEAP_AND_push_by_ref_WITHOUT_WRAP(void)
{
    {
        CLC_CQ_define_empty(int, q, 32);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            for (int i = 0; 10 != i; ++i)
            {
                int const   r2  =   CLC_CQ_push_back_by_ref(q, &i);
                size_t      ix  =   CLC_CQ_element_index(q, i);

                TEST_INT_EQ(0, r2);

                TEST_INT_EQ(i, (int)ix);
            }

            TEST_INT_EQ(10, CLC_CQ_len(q));

            clc_cq_free_storage(&q);
        }
    }
}

static void TEST_HEAP_AND_CALLBACK_INDEXES_1(void)
{
    /* clc_cq_free_storage(): run straight through allocated spaces and then deallocate */
    {
        CLC_CQ_define_empty(int, q, 16);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            int const   values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, };
            int         checks[16];

            q.pfn_element_free      =   fn_element_free_store_in_array;
            q.param_element_free    =   &checks[0];

            /* load values */

            { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(values) != i; ++i)
            {
                int const r2 = CLC_CQ_push_back_by_ref(q, &values[i]);

                TEST_INT_EQ(0, r2); // there is space, so should always work

                TEST_INT_EQ(1 + i, CLC_CQ_len(q));

                checks[i] = -1;
            }}


            /* check values in place */

            { for (size_t i = 0; CLC_CQ_len(q) != i; ++i)
            {
                int const v_at_i = *CLC_CQ_cat_t(q, int, i);

                TEST_INT_EQ((int)(1 + i), v_at_i);
            }}


            /* clear queue */
            clc_cq_free_storage(&q);


            /* check cleared values that were recorded */

            { for (size_t i = 0; CLC_CQ_len(q) != i; ++i)
            {
                TEST_INT_EQ((int)(1 + i), checks[i]);
            }}
        }
    }

    /* clc_cq_free_storage(): run straight through allocated spaces, pop front, push back one more, and then deallocate */
    {
        CLC_CQ_define_empty(int, q, 16);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            int const   values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, };
            int         checks[16];

            /* load values */
            {
                { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(values) != i; ++i)
                {
                    int const r2 = CLC_CQ_push_back_by_ref(q, &values[i]);

                    TEST_INT_EQ(0, r2); // there is space, so should always work

                    TEST_INT_EQ(1 + i, CLC_CQ_len(q));

                    checks[i] = -1;
                }}

                {
                    int const r3 = CLC_CQ_pop_front(q);

                    TEST_INT_EQ(0, r3);
                }

                {
                    int const r4 = CLC_CQ_push_back_by_value(q, int, 17);

                    TEST_INT_EQ(0, r4);
                }
            }


            /* check values in place */
            { for (size_t i = 0; CLC_CQ_len(q) != i; ++i)
            {
                int const v_at_i = *CLC_CQ_cat_t(q, int, i);

                TEST_INT_EQ((int)(2 + i), v_at_i);

                TEST_INT_EQ(-1, checks[i]);
            }}


            /* clear queue */
            {
                q.pfn_element_free      =   fn_element_free_store_in_array;
                q.param_element_free    =   &checks[0];

                clc_cq_free_storage(&q);

                q.pfn_element_free      =   NULL;
                q.param_element_free    =   NULL;
            }


            /* check cleared values that were recorded */
            { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(checks) != i; ++i)
            {
                TEST_INT_EQ((int)(2 + i), checks[i]);
            }}
        }
    }

    /* clear(): run straight through allocated spaces and then clear */
    {
        CLC_CQ_define_empty(int, q, 16);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            int const   values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, };
            int         checks[16];

            q.pfn_element_free      =   fn_element_free_store_in_array;
            q.param_element_free    =   &checks[0];

            /* load values */

            { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(values) != i; ++i)
            {
                int const r2 = CLC_CQ_push_back_by_ref(q, &values[i]);

                TEST_INT_EQ(0, r2); // there is space, so should always work

                TEST_INT_EQ(1 + i, CLC_CQ_len(q));

                checks[i] = -1;
            }}


            /* check values in place */

            { for (size_t i = 0; CLC_CQ_len(q) != i; ++i)
            {
                int const v_at_i = *CLC_CQ_cat_t(q, int, i);

                TEST_INT_EQ((int)(1 + i), v_at_i);
            }}


            /* clear queue */
            {
                size_t      num_removed;
                int const   r3 = CLC_CQ_clear(q, &num_removed);

                TEST_INT_EQ(0, r3);
                TEST_INT_EQ(16, num_removed);

                TEST_INT_EQ(0, CLC_CQ_len(q));
            }


            /* check cleared values that were recorded */

            { for (size_t i = 0; CLC_CQ_len(q) != i; ++i)
            {
                TEST_INT_EQ((int)(1 + i), checks[i]);
            }}


            clc_cq_free_storage(&q);
        }
    }

    /* clear(): run straight through allocated spaces, pop front, push back one more, and then clear */
    {
        CLC_CQ_define_empty(int, q, 16);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            int const   values[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, };
            int         checks[16];

            /* load values */

            {
                { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(values) != i; ++i)
                {
                    int const r2 = CLC_CQ_push_back_by_ref(q, &values[i]);

                    TEST_INT_EQ(0, r2); // there is space, so should always work

                    TEST_INT_EQ(1 + i, CLC_CQ_len(q));

                    checks[i] = -1;
                }}

                {
                    int const r3 = CLC_CQ_pop_front(q);

                    TEST_INT_EQ(0, r3);
                }

                {
                    int const r4 = CLC_CQ_push_back_by_value(q, int, 17);

                    TEST_INT_EQ(0, r4);
                }
            }


            /* check values in place */

            { for (size_t i = 0; CLC_CQ_len(q) != i; ++i)
            {
                int const v_at_i = *CLC_CQ_cat_t(q, int, i);

                TEST_INT_EQ((int)(2 + i), v_at_i);
            }}


            /* clear queue */
            {
                q.pfn_element_free      =   fn_element_free_store_in_array;
                q.param_element_free    =   &checks[0];

                {
                    size_t      num_removed;
                    int const   r3 = CLC_CQ_clear(q, &num_removed);

                    TEST_INT_EQ(0, r3);
                    TEST_INT_EQ(16, num_removed);

                    TEST_INT_EQ(0, CLC_CQ_len(q));
                }

                q.pfn_element_free      =   NULL;
                q.param_element_free    =   NULL;
            }


            /* check cleared values that were recorded */

            { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(checks) != i; ++i)
            {
                TEST_INT_EQ((int)(2 + i), checks[i]);
            }}


            clc_cq_free_storage(&q);
        }
    }
}

static void TEST_HEAP_AND_CALLBACK_INDEXES_2(void)
{
    /* pop_front(): run straight through allocated spaces, pop front, push back one more, and then deallocate */
    {
        CLC_CQ_define_empty(int, q, 8);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };
            int         checks[1];


            /* load values */
            {
                {
                    size_t      num_inserted;
                    int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

                    TEST_INT_EQ(0, r1);

                    TEST_INT_EQ(8, CLC_CQ_len(q));
                    TEST_INT_EQ(1, *CLC_CQ_cat_t(q, int, 0));
                    TEST_INT_EQ(8, *CLC_CQ_cat_t(q, int, 7));
                }

                {
                    int const r2 = CLC_CQ_pop_front(q);

                    TEST_INT_EQ(0, r2);

                    TEST_INT_EQ(7, CLC_CQ_len(q));
                    TEST_INT_EQ(2, *CLC_CQ_cat_t(q, int, 0));
                    TEST_INT_EQ(8, *CLC_CQ_cat_t(q, int, 6));
                }

                {
                    int const r3 = CLC_CQ_push_back_by_value(q, int, 9);

                    TEST_INT_EQ(0, r3);

                    TEST_INT_EQ(8, CLC_CQ_len(q));
                    TEST_INT_EQ(2, *CLC_CQ_cat_t(q, int, 0));
                    TEST_INT_EQ(9, *CLC_CQ_cat_t(q, int, 7));
                }
            }


            /* pop_front() */
            {
                q.pfn_element_free      =   fn_element_free_store_in_array;
                q.param_element_free    =   &checks[0];

                CLC_CQ_pop_front(q);

                q.pfn_element_free      =   NULL;
                q.param_element_free    =   NULL;
            }


            /* check cleared values that were recorded */

            { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(checks) != i; ++i)
            {
                TEST_INT_EQ(2, checks[i]);
            }}


            clc_cq_free_storage(&q);
        }
    }

    /* pop_back(): run straight through allocated spaces, pop front, push back one more, and then deallocate */
    {
        CLC_CQ_define_empty(int, q, 8);

        int const r = clc_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            int const   values[8] = { 1, 2, 3, 4, 5, 6, 7, 8, };
            int         checks[1];


            /* load values */
            {
                {
                    size_t      num_inserted;
                    int const   r1 = collect_c_cq_push_back_n_by_ref(&q, STLSOFT_NUM_ELEMENTS(values), values, &num_inserted);

                    TEST_INT_EQ(0, r1);
                    TEST_INT_EQ(8, CLC_CQ_len(q));
                }

                {
                    int const r2 = CLC_CQ_pop_front(q);

                    TEST_INT_EQ(0, r2);
                }

                {
                    int const r3 = CLC_CQ_push_back_by_value(q, int, 9);

                    TEST_INT_EQ(0, r3);
                }
            }


            /* pop_back() */
            {
                q.pfn_element_free      =   fn_element_free_store_in_array;
                q.param_element_free    =   &checks[0];

                CLC_CQ_pop_back(q);

                q.pfn_element_free      =   NULL;
                q.param_element_free    =   NULL;
            }


            /* check cleared values that were recorded */

            { for (size_t i = 0; STLSOFT_NUM_ELEMENTS(checks) != i; ++i)
            {
                TEST_INT_EQ(9, checks[i]);
            }}


            clc_cq_free_storage(&q);
        }
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

