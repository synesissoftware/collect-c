/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/common/mem_api.h
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

#include <stdlib.h>


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */


/* /////////////////////////////////////////////////////////////////////////
 * API types
 */

typedef void* (*collect_c_mem_pfn_alloc)(void* param, size_t cb_new);
typedef void* (*collect_c_mem_pfn_realloc)(void* param, void* pv_curr, size_t cb_curr, size_t cb_new);
typedef void (*collect_c_mem_pfn_free)(void* param, void* pv_curr, size_t cb_curr);

struct collect_c_mem_api_t
{
    collect_c_mem_pfn_alloc     pfn_alloc;
    collect_c_mem_pfn_realloc   pfn_realloc;
    collect_c_mem_pfn_free      pfn_free;
    void*                       param;
};
#ifndef __cplusplus
typedef struct collect_c_mem_api_t                          collect_c_mem_api_t;
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

void*
collect_c_mem_std_alloc(
    void*   param
,   size_t  cb_new
);

void*
collect_c_mem_std_realloc(
    void*   param
,   void*   pv_curr
,   size_t  cb_curr
,   size_t  cb_new
);

void
collect_c_mem_std_free(
    void*   param
,   void*   pv_curr
,   size_t  cb_curr
);


#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

