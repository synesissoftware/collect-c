/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.cq/entry.c
 *
 * Purpose: Unit-test for circular queue.
 *
 * Created: 5th February 2025
 * Updated: 7th February 2025
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
static void TEST_define_AND_allocate(void);

static void TEST_CQ_define_on_stack(void);
static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_FAIL_TO_ADD(void);
static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_POP_FRONT_TWO_THEN_ADD(void);
static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_POP_BACK_TWO_THEN_ADD(void);
static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_CLEAR(void);
static void TEST_ADD_REMOVE_CLEAR_WITH_CALLBACKS(void);
static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_F_OVERWRITE_FRONT_WHEN_FULL(void);

static void TEST_HEAP_ADD_WITHOUT_WRAP_1(void);


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
        XTESTS_RUN_CASE(TEST_define_AND_allocate);

        XTESTS_RUN_CASE(TEST_CQ_define_on_stack);
        XTESTS_RUN_CASE(TEST_STACK_AND_ADD_UNTIL_FULL_THEN_FAIL_TO_ADD);
        XTESTS_RUN_CASE(TEST_STACK_AND_ADD_UNTIL_FULL_THEN_POP_FRONT_TWO_THEN_ADD);
        XTESTS_RUN_CASE(TEST_STACK_AND_ADD_UNTIL_FULL_THEN_POP_BACK_TWO_THEN_ADD);
        XTESTS_RUN_CASE(TEST_STACK_AND_ADD_UNTIL_FULL_THEN_CLEAR);
        XTESTS_RUN_CASE(TEST_ADD_REMOVE_CLEAR_WITH_CALLBACKS);
        XTESTS_RUN_CASE(TEST_STACK_AND_ADD_UNTIL_FULL_THEN_F_OVERWRITE_FRONT_WHEN_FULL);

        XTESTS_RUN_CASE(TEST_HEAP_ADD_WITHOUT_WRAP_1);

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

static void TEST_define_AND_allocate(void)
{
    {
        CLC_CQ_define_empty(int, q, 32);

        int const r = collect_c_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            collect_c_cq_free_storage(&q);
        }
    }
}

static void TEST_CQ_define_on_stack(void)
{
    {
        int array[32];

        CLC_CQ_define_on_stack(q, array);

        TEST_INT_EQ(32, CLC_CQ_spare(q));
    }
}

static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_FAIL_TO_ADD(void)
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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(7, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(6, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(5, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(4, CLC_CQ_spare(q));
        }

        /* add el[4] */
        {
            int const   el  =   505;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(5, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[5] */
        {
            int const   el  =   606;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(6, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[6] */
        {
            int const   el  =   707;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(7, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[7] */
        {
            int const   el  =   808;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   el  =   911;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(ENOSPC, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(8, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }
    }
}

static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_POP_FRONT_TWO_THEN_ADD(void)
{
    {
        int array[4];

        CLC_CQ_define_on_stack(q, array);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(4, CLC_CQ_spare(q));

        /* add el[0] */
        {
            int const   r   =   CLC_CQ_add_by_value(q, int, 101);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   r   =   CLC_CQ_add_by_value(q, int, 202);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   r   =   CLC_CQ_add_by_value(q, int, 303);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   r   =   CLC_CQ_add_by_value(q, int, 404);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   r   =   CLC_CQ_add_by_value(q, int, 911);

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
            int const   r   =   CLC_CQ_add_by_value(q, int, 502);

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
            int const   r   =   CLC_CQ_add_by_value(q, int, 603);

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

static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_POP_BACK_TWO_THEN_ADD(void)
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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   el  =   911;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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

static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_CLEAR(void)
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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(0, CLC_CQ_spare(q));
        }

        /* fail to add another add when no space */
        {
            int const   el  =   911;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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


static void fn_accumulate_on_free(
    size_t  el_size
,   size_t  el_index
,   void*   el_ptr
,   void*   param_element_free
)
{
    int* const  p_el    =   (int*)el_ptr;
    long* const p_sum   =   (long*)param_element_free;

    ((void)&el_size);
    ((void)&el_index);

    *p_sum += *p_el;
}

static void fn_record_most_recent_overwrite(
    size_t  el_size
,   size_t  el_index
,   void*   el_ptr
,   void*   param_element_free
)
{
    int* const  p_el    =   (int*)el_ptr;
    int* const  p_rec   =   (int*)param_element_free;

    ((void)&el_size);
    ((void)&el_index);

    *p_rec = *p_el;
}

static void TEST_ADD_REMOVE_CLEAR_WITH_CALLBACKS(void)
{
    {
        long    sum = 0;
        size_t  num_removed;

        int     array[8];
        CLC_CQ_define_on_stack_with_cb(q, array, fn_accumulate_on_free, &sum);

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        TEST_INT_EQ(0, sum);


        /* add five elements */

        CLC_CQ_add_by_value(q, int, 1);
        CLC_CQ_add_by_value(q, int, 2);
        CLC_CQ_add_by_value(q, int, 3);
        CLC_CQ_add_by_value(q, int, 4);
        CLC_CQ_add_by_value(q, int, 5);

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

static void TEST_HEAP_ADD_WITHOUT_WRAP_1(void)
{
    {
        CLC_CQ_define_empty(int, q, 32);

        int const r = collect_c_cq_allocate_storage(&q);

        TEST_INTEGER_EQUAL_ANY_OF2(0, ENOMEM, r);

        if (0 == r)
        {
            for (int i = 0; 10 != i; ++i)
            {
                int const   r2  =   CLC_CQ_add_by_ref(q, &i);
                size_t      ix  =   CLC_CQ_element_index(q, i);

                TEST_INT_EQ(0, r2);

                TEST_INT_EQ(i, (int)ix);
            }

            TEST_INT_EQ(10, CLC_CQ_len(q));

            collect_c_cq_free_storage(&q);
        }
    }
}

static void TEST_STACK_AND_ADD_UNTIL_FULL_THEN_F_OVERWRITE_FRONT_WHEN_FULL(void)
{
    {
        int array[8];
        int previous = -1;

        COLLECT_C_CIRCQ_define_on_stack_with_callback(q, array, fn_record_most_recent_overwrite, &previous);

        q.flags |= COLLECT_C_CIRCQ_F_OVERWRITE_FRONT_WHEN_FULL;

        TEST_BOOLEAN_TRUE(CLC_CQ_is_empty(q));
        TEST_INT_EQ(0, CLC_CQ_len(q));
        TEST_INT_EQ(8, CLC_CQ_spare(q));

        /* add el[0] */
        {
            int const   el  =   101;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(1, CLC_CQ_len(q));
            TEST_INT_EQ(7, CLC_CQ_spare(q));
        }

        /* add el[1] */
        {
            int const   el  =   202;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(2, CLC_CQ_len(q));
            TEST_INT_EQ(6, CLC_CQ_spare(q));
        }

        /* add el[2] */
        {
            int const   el  =   303;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(3, CLC_CQ_len(q));
            TEST_INT_EQ(5, CLC_CQ_spare(q));
        }

        /* add el[3] */
        {
            int const   el  =   404;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(4, CLC_CQ_len(q));
            TEST_INT_EQ(4, CLC_CQ_spare(q));
        }

        /* add el[4] */
        {
            int const   el  =   505;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(5, CLC_CQ_len(q));
            TEST_INT_EQ(3, CLC_CQ_spare(q));
        }

        /* add el[5] */
        {
            int const   el  =   606;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(6, CLC_CQ_len(q));
            TEST_INT_EQ(2, CLC_CQ_spare(q));
        }

        /* add el[6] */
        {
            int const   el  =   707;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

            TEST_INT_EQ(0, r);

            TEST_BOOLEAN_FALSE(CLC_CQ_is_empty(q));
            TEST_INT_EQ(7, CLC_CQ_len(q));
            TEST_INT_EQ(1, CLC_CQ_spare(q));
        }

        /* add el[7] */
        {
            int const   el  =   808;
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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
            int const   r   =   CLC_CQ_add_by_ref(q, &el);

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


/* ///////////////////////////// end of file //////////////////////////// */

