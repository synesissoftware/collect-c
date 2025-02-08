/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/dlist.h
 *
 * Purpose: Doubly-linked list container.
 *
 * Created: 7th February 2025
 * Updated: 8th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifdef __cplusplus
# ifndef COLLECT_C_DLIST_SUPPRESS_CXX_WARNING
#  error This file not currently compatible with C++ compilation
# endif
#endif


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#define COLLECT_C_DLIST_VER_MAJOR       0
#define COLLECT_C_DLIST_VER_MINOR       0
#define COLLECT_C_DLIST_VER_PATCH       0
#define COLLECT_C_DLIST_VER_ALPHABETA   1

#define COLLECT_C_DLIST_VER \
    (0\
        |   (   COLLECT_C_DLIST_VER_MAJOR       << 24   ) \
        |   (   COLLECT_C_DLIST_VER_MINOR       << 16   ) \
        |   (   COLLECT_C_DLIST_VER_PATCH       <<  8   ) \
        |   (   COLLECT_C_DLIST_VER_ALPHABETA   <<  0   ) \
    )


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * API types
 */

typedef union
{
    char            data[1];
    int             _i;
    long            _l;
    long long       _ll;
    double          _d;
    long double     _ld;
    void*           _pv;
} collect_c_dlist_node_data_t;

struct collect_c_dlist_node_t;
#ifndef __cplusplus
typedef struct collect_c_dlist_node_t   collect_c_dlist_node_t;
#endif

struct collect_c_dlist_node_t
{
    collect_c_dlist_node_t*     prev;
    collect_c_dlist_node_t*     next;
    collect_c_dlist_node_data_t data[1];
};


struct collect_c_dlist_block_t;
#ifndef __cplusplus
typedef struct collect_c_dlist_block_t  collect_c_dlist_block_t;
#endif
struct collect_c_dlist_block_t
{
    size_t                      el_size;
    size_t                      num_nodes;
    size_t                      num_used;
    collect_c_dlist_block_t*    next_block;
    collect_c_dlist_node_t      nodes[1];
};


struct collect_c_dlist_t
{
    size_t                      el_size;            /*! The element size. */
    size_t                      num_spares;         /*! */
    size_t                      size;               /*! */
    int32_t                     flags;              /*! Control flags. */
    int32_t                     reserved0;          /*! Reserved field. */
    collect_c_dlist_node_t*     head;               /*! */
    collect_c_dlist_node_t*     tail;               /*! */
    collect_c_dlist_node_t*     spares;             /*! */
    collect_c_dlist_block_t*    blocks;             /*! */
    void*                       param_element_free; /*! Custom parameter to be passed to invocations of pfn_element_free */
    void                      (*pfn_element_free)(
        size_t  el_size
    ,   size_t  el_index
    ,   void*   el_ptr
    ,   void*   param_element_free
    );                                              /*! Custom function to be invoked when */
};
#ifndef __cplusplus
typedef struct collect_c_dlist_t        collect_c_dlist_t;
#endif

/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros
 */

/** @def COLLECT_C_DLIST_define_empty(l_el_type, l_name)
 *
 * Declares and defines an empty list instance. The instance will need to
 * be further set-up via collect_c_dlist_allocate_storage().
 *
 * @param l_el_type The type of the elements to be stored;
 * @param l_name The name of the instance;
 */
#define COLLECT_C_DLIST_define_empty(l_el_type, l_name)                     \
                                                                            \
    collect_c_dlist_t l_name = COLLECT_C_DLIST_EMPTY_INITIALIZER_(l_el_type, 0, NULL, NULL, 0)


#define COLLECT_C_DLIST_is_empty(l_name)                    (0 == (l_name).size)
#define COLLECT_C_DLIST_len(l_name)                         ((l_name).size)
#define COLLECT_C_DLIST_spare(l_name)                       ((l_name).num_spares)

#define COLLECT_DLIST_push_back_by_val(l_name, t_el, new_el)    (assert(sizeof(t_el) == (l_name).el_size), collect_c_dlist_push_back_by_ref(&(l_name), &((t_el){(new_el)})))
#define COLLECT_DLIST_push_front_by_val(l_name, t_el, new_el)   (assert(sizeof(t_el) == (l_name).el_size), collect_c_dlist_push_front_by_ref(&(l_name), &((t_el){(new_el)})))

#define COLLECT_C_DLIST_clear_1_(l_name)                    collect_c_dlist_clear(&(l_name), NULL, NULL, NULL)
#define COLLECT_C_DLIST_clear_2_(l_name, p)                 collect_c_dlist_clear(&(l_name), NULL, NULL, (p))

#define COLLECT_C_DLIST_clear_GET_MACRO_(_1, _2, macro, ...)    macro

#define COLLECT_C_DLIST_clear(...)                          COLLECT_C_DLIST_clear_GET_MACRO_(__VA_ARGS__, COLLECT_C_DLIST_clear_2_, COLLECT_C_DLIST_clear_1_, NULL)(__VA_ARGS__)




/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Obtains the value of COLLECT_C_DLIST_VER at the time of compilation of
 * the library.
 */
uint32_t
collect_c_dlist_version(void);

/** Frees storage associated with the instance.
 *
 * @param l Pointer to the list. Must not be NULL;
 *
 * @pre (NULL != l);
 */
int
clc_dlist_free_storage(
    collect_c_dlist_t*  l
);

/** Clears all elements from the list.
 *
 * @param l Pointer to the list. Must not be NULL;
 * @param reserved0 Reserved. Must be 0;
 * @param reserved1 Reserved. Must be 0;
 * @param num_dropped Optional pointer to variable to retrieve number of
 *  entries dropped;
 *
 * @pre (NULL != l);
 * @pre (NULL != l->storage);
 * @pre (0 == reserved0);
 * @pre (0 == reserved1);
 */
int
collect_c_dlist_clear(
    collect_c_dlist_t*  l
,   void*               reserved0
,   void*               reserved1
,   size_t*             num_dropped
);

int
collect_c_dlist_push_back_by_ref(
    collect_c_dlist_t*  l
,   void const*         ptr_new_el
);

int
collect_c_dlist_push_front_by_ref(
    collect_c_dlist_t*  l
,   void const*         ptr_new_el
);

#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * helper macros
 */

#define COLLECT_C_DLIST_EMPTY_INITIALIZER_(l_el_type, l_flags, l_storage, elf_fn, elf_param) \
                                                                            \
    {                                                                       \
        .el_size = sizeof(l_el_type),                                       \
        .num_spares = 0,                                                    \
        .size = 0,                                                          \
        .flags = (l_flags),                                                 \
        .reserved0 = 0,                                                     \
        .head = NULL,                                                       \
        .tail = NULL,                                                       \
        .spares = NULL,                                                     \
        .param_element_free = (elf_param),                                  \
        .pfn_element_free = (elf_fn),                                       \
    }


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

