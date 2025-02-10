/* /////////////////////////////////////////////////////////////////////////
 * File:    test/performance/test.performance.cq/main.cpp
 *
 * Purpose: Performance-test for circular queue.
 *
 * Created: 6th February 2025
 * Updated: 10th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#define COLLECT_C_CIRCQ_SUPPRESS_CXX_WARNING
#include <collect-c/terse/circq.h>
#undef COLLECT_C_CIRCQ_SUPPRESS_CXX_WARNING

#include <xtests/terse-api.h>

#include <stlsoft/diagnostics/doomgram.hpp>
#include <stlsoft/diagnostics/std_chrono_hrc_stopwatch.hpp>
#include <stlsoft/conversion/number/grouping_functions.hpp>

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * types
 */

typedef stlsoft::std_chrono_hrc_stopwatch                   stopwatch_t;
typedef stopwatch_t::interval_type                          interval_t;


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

namespace {

    const std::size_t NUM_ITERATIONS    =   1000000;
    const std::size_t NUM_WARM_LOOPS    =   2;
} // anonymous namespace


/* /////////////////////////////////////////////////////////////////////////
 * test function declarations
 */

namespace {

    std::uint64_t
    create_on_stack(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_stack_then_clear(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_stack_16_and_push_by_ref_4_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_stack_16_and_push_by_ref_16_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_heap_16_and_push_by_ref_16_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_stack_256_and_push_by_ref_64_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_heap_256_and_push_by_ref_64_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_stack_256_and_push_by_ref_64_elements_cb(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_heap_256_and_push_by_ref_64_elements_cb(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    create_on_stack_256_and_push_by_ref_4092_elements_cb_overwrite(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
} // anonymous namespace


/* /////////////////////////////////////////////////////////////////////////
 * helper functions
 */

namespace {

template <ss_typename_param_k T_integer>
std::string
thousands(
    T_integer const& v
)
{
    char    dest[41];
    size_t  n = stlsoft::format_thousands(dest, std::size(dest), "3;0", v);

    // NOTE: unfortunately `stlsoft::format_thousands()` return includes 1 for NUL terminator

    return std::string(dest, n - 1);
}

void
display_results_title(
)
{
    std::cout
        << '\t'
        << std::setw(64) << std::left << "function name"
        << '\t'
        << std::setw(16) << std::right << "#iterations"
        << '\t'
        << std::setw(12) << std::right << "#actions"
        << '\t'
        << std::setw(16) << std::right << "tm (ns)"
        << '\t'
        << std::setw(12) << std::right << "tm/iter (ns)"
        << '\t'
        << std::setw(16) << std::right << "anchor (⚓)"
        << std::endl
        ;
}

void
display_results(
    char const*     function_name
,   std::size_t     num_iterations
,   std::size_t     num_actions
,   interval_t      tm_ns
,   std::uint64_t   anchor_value
)
{
    ((void)&num_iterations);
    ((void)&tm_ns);

    std::cout
        << '\t'
        << std::setw(64) << std::left << function_name
        << '\t'
        << std::setw(16) << std::right << num_iterations
        << '\t'
        << std::setw(12) << std::right << num_actions
        << '\t'
        << std::setw(16) << std::right << thousands(tm_ns)
        << '\t'
        << std::setw(12) << std::right << thousands(tm_ns / (num_iterations * num_actions))
        << '\t'
        << std::setw(16) << std::right << anchor_value
        << std::endl
        ;
}
} // anonymous namespace


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* /*argv*/[])
{
    std::uint64_t anchor_value = 0;

    display_results_title();

    anchor_value += create_on_stack(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_stack_then_clear(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_stack_16_and_push_by_ref_4_elements(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_stack_16_and_push_by_ref_16_elements(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_heap_16_and_push_by_ref_16_elements(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_stack_256_and_push_by_ref_64_elements(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_heap_256_and_push_by_ref_64_elements(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_stack_256_and_push_by_ref_64_elements_cb(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_heap_256_and_push_by_ref_64_elements_cb(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += create_on_stack_256_and_push_by_ref_4092_elements_cb_overwrite(NUM_ITERATIONS, NUM_WARM_LOOPS);

    return (0 == argc && 0 == anchor_value) ? EXIT_FAILURE : EXIT_SUCCESS;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function implementations
 */

namespace {

    void int_callback(
        size_t      el_size
    ,   intptr_t    el_index
    ,   void*       el_ptr
    ,   void*       param_element_free
    )
    {
        ((void)&el_size);

        uint64_t&   anchor_value    =   *(uint64_t*)param_element_free;
        int const&  element_value   =   *(int const*)el_ptr;

        anchor_value += element_value;
        anchor_value += el_index;
    }

    std::uint64_t
    create_on_stack(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    int ar[16];

                    CLC_CQ_define_on_stack(q, ar);

                    anchor_value += CLC_CQ_spare(q);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 1, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_stack_then_clear(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    int ar[16];

                    CLC_CQ_define_on_stack(q, ar);

                    anchor_value += CLC_CQ_spare(q);

                    CLC_CQ_clear(q);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 1, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_stack_16_and_push_by_ref_4_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        int const values[] =
        {
            1,
            2,
            3,
            4,
        };

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    int ar[16];

                    CLC_CQ_define_on_stack(q, ar);

                    CLC_CQ_push_back_by_ref(q, &values[0]);
                    CLC_CQ_push_back_by_ref(q, &values[1]);
                    CLC_CQ_push_back_by_ref(q, &values[2]);
                    CLC_CQ_push_back_by_ref(q, &values[3]);

                    anchor_value += CLC_CQ_spare(q);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 4, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_stack_16_and_push_by_ref_16_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        int const values[] =
        {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        };

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    int ar[16];

                    CLC_CQ_define_on_stack(q, ar);

                    for (std::size_t j = 0; std::size(values) != j; ++j)
                    {
                        CLC_CQ_push_back_by_ref(q, &values[j]);
                    }
                    anchor_value += CLC_CQ_spare(q);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, std::size(values), tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_heap_16_and_push_by_ref_16_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        int const values[] =
        {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        };

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    COLLECT_C_CIRCQ_define_empty(int, q, 16);

                    int const r = clc_cq_allocate_storage(&q);

                    if (0 == r)
                    {
                        for (std::size_t j = 0; std::size(values) != j; ++j)
                        {
                            CLC_CQ_push_back_by_ref(q, &values[j]);
                        }
                        anchor_value += CLC_CQ_spare(q);

                        clc_cq_free_storage(&q);
                    }
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, std::size(values), tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_stack_256_and_push_by_ref_64_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        int const values[] =
        {
             1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            11, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            11, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            11, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            11, 52, 53, 54, 55, 56, 57, 58, 59, 60,
            11, 62, 63, 64,
        };

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    int ar[256];

                    CLC_CQ_define_on_stack(q, ar);

                    for (std::size_t j = 0; std::size(values) != j; ++j)
                    {
                        CLC_CQ_push_back_by_ref(q, &values[j]);
                    }
                    anchor_value += CLC_CQ_spare(q);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, std::size(values), tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_heap_256_and_push_by_ref_64_elements(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        int const values[] =
        {
             1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            11, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            11, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            11, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            11, 52, 53, 54, 55, 56, 57, 58, 59, 60,
            11, 62, 63, 64,
        };

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    COLLECT_C_CIRCQ_define_empty(int, q, 256);

                    int const r = clc_cq_allocate_storage(&q);

                    if (0 == r)
                    {
                        for (std::size_t j = 0; std::size(values) != j; ++j)
                        {
                            CLC_CQ_push_back_by_ref(q, &values[j]);
                        }
                        anchor_value += CLC_CQ_spare(q);

                        clc_cq_free_storage(&q);
                    }
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, std::size(values), tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_stack_256_and_push_by_ref_64_elements_cb(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        int const values[] =
        {
             1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            11, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            11, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            11, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            11, 52, 53, 54, 55, 56, 57, 58, 59, 60,
            11, 62, 63, 64,
        };

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    int ar[256];

                    CLC_CQ_define_on_stack_with_cb(q, ar, int_callback, &anchor_value);

                    for (std::size_t j = 0; std::size(values) != j; ++j)
                    {
                        CLC_CQ_push_back_by_ref(q, &values[j]);
                    }
                    anchor_value += CLC_CQ_spare(q);

                    CLC_CQ_clear(q);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, std::size(values), tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    create_on_heap_256_and_push_by_ref_64_elements_cb(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        std::uint64_t anchor_value = 0;

        int const values[] =
        {
             1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            11, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            11, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            11, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            11, 52, 53, 54, 55, 56, 57, 58, 59, 60,
            11, 62, 63, 64,
        };

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    COLLECT_C_CIRCQ_define_empty(int, q, 256);

                    q.pfn_element_free      =   int_callback;
                    q.param_element_free    =   &anchor_value;

                    int const r = clc_cq_allocate_storage(&q);

                    if (0 == r)
                    {
                        for (std::size_t j = 0; std::size(values) != j; ++j)
                        {
                            CLC_CQ_push_back_by_ref(q, &values[j]);
                        }
                        anchor_value += CLC_CQ_spare(q);

                        clc_cq_free_storage(&q);
                    }
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, std::size(values), tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

#if 0

    static void fn_stub(
        size_t   /* el_size */
    ,   size_t   /* el_index */
    ,   void*    /* el_ptr */
    ,   void*    /* param_element_free */
    )
    {}
#endif

    std::uint64_t
    create_on_stack_256_and_push_by_ref_4092_elements_cb_overwrite(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    )
    {
        const std::size_t NUM_VALUES = 4092;

        num_iterations /= 10;

        std::uint64_t anchor_value = 0;

        stopwatch_t sw;

        for (std::size_t w = num_warm_loops; 0 != w; --w)
        {
            interval_t tm_ns = 0;

            anchor_value = 0;

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    CLC_CQ_define_empty_with_cb(int, q, 256, int_callback, &anchor_value);

                    int const r = clc_cq_allocate_storage(&q);

                    if (0 == r)
                    {
                        q.flags |= CLC_CQ_F_OVERWRITE_FRONT_WHEN_FULL;

                        for (std::size_t j = 0; NUM_VALUES != j; ++j)
                        {
                            int const value = static_cast<int>(j);

                            CLC_CQ_push_back_by_ref(q, &value);
                        }
                        anchor_value += CLC_CQ_spare(q);

                        clc_cq_free_storage(&q);
                    }
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, NUM_VALUES, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }
} // anonymous namespace


/* ///////////////////////////// end of file //////////////////////////// */

