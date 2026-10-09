/* /////////////////////////////////////////////////////////////////////////
 * File:    src/mem_api.c
 *
 * Purpose: Memory API.
 *
 * Created: 14th February 2025
 * Updated: 15th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common/mem_api.h>

#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

void*
collect_c_mem_std_alloc(
    void*   param
,   size_t  cb_new
)
{
    ((void)&param);

/*
    if (0 != cb_new)
    {
        return NULL;
    }
*/

    return malloc(cb_new);
}

void*
collect_c_mem_std_realloc(
    void*   param
,   void*   pv_curr
,   size_t  cb_curr
,   size_t  cb_new
)
{
    ((void)&param);
    ((void)&cb_curr);

    return realloc(pv_curr, cb_new);
}

void
collect_c_mem_std_free(
    void*   param
,   void*   pv_curr
,   size_t  cb_curr
)
{
    ((void)&param);
    ((void)&cb_curr);

    free(pv_curr);
}


/* ///////////////////////////// end of file //////////////////////////// */

