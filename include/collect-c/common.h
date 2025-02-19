/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/common.h
 *
 * Purpose: Common elements.
 *
 * Created: 5th February 2025
 * Updated: 19th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common/macros.h>
#include <collect-c/common/mem_api.h>
#include <collect-c/common/string.h>
#ifndef __cplusplus
# include <collect-c/common/version.h>
#endif


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */


/* /////////////////////////////////////////////////////////////////////////
 * types
 */

typedef union
{
    char            data[1];
    int             _i;
    long            _l;
    long long       _ll;
    double          _d;
#if 0

    long double     _ld;
#endif
    void*           _pv;
} collect_c_common_node_data_t;


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

int
collect_c_fn_cmp_int16(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_int32(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_int64(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_uint16(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_uint32(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_uint64(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);


int
collect_c_fn_cmp_short(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_int(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_long(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_llong(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_ushort(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_uint(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_ulong(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_ullong(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);


int
collect_c_fn_cmp_ccs(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);

int
collect_c_fn_cmp_ccs_ignore_case(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          el_size
);



#ifdef __cplusplus
} /* extern "C" */
#endif




/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

