/* /////////////////////////////////////////////////////////////////////////
 * File:    test/performance/test.performance.dlist/main.cpp
 *
 * Purpose: Performance-test for doubly-linked list.
 *
 * Created: 16th February 2025
 * Updated: 17th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#define COLLECT_C_DLIST_SUPPRESS_CXX_WARNING
#include <collect-c/terse/dlist.h>
#undef COLLECT_C_DLIST_SUPPRESS_CXX_WARNING

#include <stlsoft/diagnostics/doomgram.hpp>
#include <stlsoft/diagnostics/std_chrono_hrc_stopwatch.hpp>
#include <stlsoft/conversion/number/grouping_functions.hpp>

#include <cmath>
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
    define_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    push_back_10_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_front_10_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_back_100_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_front_100_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_back_1000_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_front_1000_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_back_10000_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_front_10000_int_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    push_back_10_int_then_clear_then_push_back_10_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_back_100_int_then_clear_then_push_back_100_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    push_back_1000_int_then_clear_then_push_back_1000_then_free_storage(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );

    std::uint64_t
    accumulate_over_10(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    accumulate_over_100(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    accumulate_over_1000(
        std::size_t num_iterations
    ,   std::size_t num_warm_loops
    );
    std::uint64_t
    accumulate_over_10000(
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
        << std::setw(16) << std::right << "tm/iter (ns)"
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
    if (function_name == strstr(function_name, "`anonymous-namespace'::"))
    {
        function_name += 23;
    }

    auto divisor = num_iterations * num_actions;
    auto d = std::div(tm_ns, divisor);
    auto rem_3dp = static_cast<interval_t>(std::round((1000.0 * d.rem) / divisor));

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
        << std::setw(12) << std::right << thousands(d.quot) << '.' << std::setw(3) << std::setfill('0') << rem_3dp << std::setfill(' ')
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

    anchor_value += define_then_free_storage(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += push_back_10_int_then_free_storage(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += push_front_10_int_then_free_storage(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += push_back_100_int_then_free_storage(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += push_front_100_int_then_free_storage(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += push_back_1000_int_then_free_storage(NUM_ITERATIONS / 10, NUM_WARM_LOOPS);

    anchor_value += push_front_1000_int_then_free_storage(NUM_ITERATIONS / 10, NUM_WARM_LOOPS);

    anchor_value += push_back_10000_int_then_free_storage(NUM_ITERATIONS / 100, NUM_WARM_LOOPS);

    anchor_value += push_front_10000_int_then_free_storage(NUM_ITERATIONS / 100, NUM_WARM_LOOPS);

    anchor_value += push_back_10_int_then_clear_then_push_back_10_then_free_storage(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += push_back_100_int_then_clear_then_push_back_100_then_free_storage(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += push_back_1000_int_then_clear_then_push_back_1000_then_free_storage(NUM_ITERATIONS / 10, NUM_WARM_LOOPS);

    anchor_value += accumulate_over_10(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += accumulate_over_100(NUM_ITERATIONS, NUM_WARM_LOOPS);

    anchor_value += accumulate_over_1000(NUM_ITERATIONS / 10, NUM_WARM_LOOPS);

    anchor_value += accumulate_over_10000(NUM_ITERATIONS / 100, NUM_WARM_LOOPS);

    return (0 == argc && 0 == anchor_value) ? EXIT_FAILURE : EXIT_SUCCESS;
}


/* /////////////////////////////////////////////////////////////////////////
 * test function implementations
 */

namespace {

    std::uint64_t
    define_then_free_storage(
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
                    COLLECT_C_DLIST_define_empty(int, l);

                    anchor_value += CLC_DL_spare(l);

                    CLC_DL_free(l);
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
    push_back_10_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 10 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 10, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_front_10_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 10 != j; ++j)
                    {
                        int r = CLC_DL_push_front_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cfront_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 10, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_back_100_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 100 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 100, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_front_100_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 100 != j; ++j)
                    {
                        int r = CLC_DL_push_front_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cfront_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 100, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_back_1000_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 1000 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 1000, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_front_1000_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 1000 != j; ++j)
                    {
                        int r = CLC_DL_push_front_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cfront_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 1000, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_back_10000_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 10000 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 10000, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_front_10000_int_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    for (int j = 0; 10000 != j; ++j)
                    {
                        int r = CLC_DL_push_front_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }

                    anchor_value += *CLC_DL_cfront_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 10000, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_back_10_int_then_clear_then_push_back_10_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    { for (int j = 0; 10 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }}

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_clear(l);

                    { for (int j = 0; 10 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }}

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 10, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_back_100_int_then_clear_then_push_back_100_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    { for (int j = 0; 100 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }}

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_clear(l);

                    { for (int j = 0; 100 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }}

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 100, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }

    std::uint64_t
    push_back_1000_int_then_clear_then_push_back_1000_then_free_storage(
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
                    CLC_DL_define_empty(int, l);

                    { for (int j = 0; 1000 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }}

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_clear(l);

                    { for (int j = 0; 1000 != j; ++j)
                    {
                        int r = CLC_DL_push_back_by_val(l, int, j);

                        if (0 != r)
                        {
                            break;
                        }
                    }}

                    anchor_value += *CLC_DL_cback_t(l, int);

                    CLC_DL_free(l);
                }
            }
            sw.stop();

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 1000, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }


    std::uint64_t
    accumulate_over_10(
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

            CLC_DL_define_empty(int, l);

            for (int j = 0; 10 != j; ++j)
            {
                int r = CLC_DL_push_back_by_val(l, int, j);

                if (0 != r)
                {
                    break;
                }
            }

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    for (collect_c_dlist_node_t* n = l.head; NULL != n; n = n->next)
                    {
                        anchor_value += *(int const*)&n->data->data[0];
                    }
                }
            }
            sw.stop();


            CLC_DL_free(l);

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 10, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }
    std::uint64_t
    accumulate_over_100(
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

            CLC_DL_define_empty(int, l);

            for (int j = 0; 100 != j; ++j)
            {
                int r = CLC_DL_push_back_by_val(l, int, j);

                if (0 != r)
                {
                    break;
                }
            }

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    for (collect_c_dlist_node_t* n = l.head; NULL != n; n = n->next)
                    {
                        anchor_value += *(int const*)&n->data->data[0];
                    }
                }
            }
            sw.stop();


            CLC_DL_free(l);

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 100, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }
    std::uint64_t
    accumulate_over_1000(
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

            CLC_DL_define_empty(int, l);

            for (int j = 0; 1000 != j; ++j)
            {
                int r = CLC_DL_push_back_by_val(l, int, j);

                if (0 != r)
                {
                    break;
                }
            }

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    for (collect_c_dlist_node_t* n = l.head; NULL != n; n = n->next)
                    {
                        anchor_value += *(int const*)&n->data->data[0];
                    }
                }
            }
            sw.stop();


            CLC_DL_free(l);

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 1000, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }
    std::uint64_t
    accumulate_over_10000(
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

            CLC_DL_define_empty(int, l);

            for (int j = 0; 10000 != j; ++j)
            {
                int r = CLC_DL_push_back_by_val(l, int, j);

                if (0 != r)
                {
                    break;
                }
            }

            sw.start();
            for (std::size_t i = 0; num_iterations != i; ++i)
            {
                {
                    for (collect_c_dlist_node_t* n = l.head; NULL != n; n = n->next)
                    {
                        anchor_value += *(int const*)&n->data->data[0];
                    }
                }
            }
            sw.stop();


            CLC_DL_free(l);

            tm_ns = sw.get_nanoseconds();

            if (1 == w)
            {
                display_results(__FUNCTION__, num_iterations, 10000, tm_ns, anchor_value);
            }
        }

        return anchor_value;
    }
} // anonymous namespace


/* ///////////////////////////// end of file //////////////////////////// */

