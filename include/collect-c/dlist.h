/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/dlist.h
 *
 * Purpose: Doubly-linked list container.
 *
 * Created: 7th February 2025
 * Updated: 11th February 2025
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
#define COLLECT_C_DLIST_VER_ALPHABETA   3

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
 * API constants
 */

/** Suppresses collection of erased nodes as spares. */
#define COLLECT_C_DLIST_F_NO_SPARES                         (0x00000001)


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
    ,   size_t  el_index    /* always 0 */
    ,   void*   el_ptr
    ,   void*   param_element_free
    );                                              /*! Custom function to be invoked when */
};
#ifndef __cplusplus
typedef struct collect_c_dlist_t        collect_c_dlist_t;
#endif

/** T.B.C.
 *
 * @param l Pointer to the list. Will not be NULL;
 * @param p_lhs Pointer to the lhs element. Will not be NULL;
 * @param p_rhs Pointer to the lhs element. Will not be NULL;
 *
 * @retval <0 The element referred to by p_lhs is "less than" the element
 *  referred to by p_rhs;
 * @retval 0 A matching element is found;
 * @retval >0 The element referred to by p_lhs is "greater than" the element
 *  referred to by p_rhs;
 *
 * @note No modifications to l are permitted.
 */
typedef int (*collect_c_dlist_pfn_compare_t)(
    collect_c_dlist_t const*    l
,   void const*                 p_lhs
,   void const*                 p_rhs
);


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros (internal)
 */

#define COLLECT_C_DLIST_get_l_ptr_(l)                       _Generic((l),   \
                                                                            \
                             collect_c_dlist_t* :  (l),                     \
                       collect_c_dlist_t const* :  (l),                     \
                                        default : &(l)                      \
)

#define COLLECT_C_DLIST_assert_el_size_(l_name, t_el)       assert(sizeof(t_el) == COLLECT_C_DLIST_get_l_ptr_(l_name)->el_size)
#define COLLECT_C_DLIST_assert_ix_(l_name, ix)              assert((ix) < COLLECT_C_DLIST_get_l_ptr_(l_name)->size)
#define COLLECT_C_DLIST_assert_not_empty_(l_name)           assert(0 != COLLECT_C_DLIST_get_l_ptr_(l_name)->size)
#define COLLECT_C_DLIST_assert_not_null_(l_name)            assert(NULL != (l_name))

#define COLLECT_C_DLIST_head_(l_name)                       (COLLECT_C_DLIST_assert_not_empty_(l_name), (COLLECT_C_DLIST_get_l_ptr_(l_name)->head))
#define COLLECT_C_DLIST_tail_(l_name)                       (COLLECT_C_DLIST_assert_not_empty_(l_name), (COLLECT_C_DLIST_get_l_ptr_(l_name)->tail))

#define COLLECT_C_DLIST_clear_1_(l_name)                    collect_c_dlist_clear(COLLECT_C_DLIST_get_l_ptr_(l_name), NULL, NULL, NULL)
#define COLLECT_C_DLIST_clear_2_(l_name, p)                 collect_c_dlist_clear(COLLECT_C_DLIST_get_l_ptr_(l_name), NULL, NULL,  (p))

#define COLLECT_C_DLIST_node_data_(n)                       ((void      *)(&(n)->data[0].data[0]))
#define COLLECT_C_DLIST_node_cdata_(n)                      ((void const*)(&(n)->data[0].data[0]))

#define COLLECT_C_DLIST_insert_after_3_(l_name, ref_node, new_el)                collect_c_dlist_insert_after(COLLECT_C_DLIST_get_l_ptr_(l_name), (ref_node), (new_el), NULL)
#define COLLECT_C_DLIST_insert_after_4_(l_name, ref_node, new_el, p_new_node)    collect_c_dlist_insert_after(COLLECT_C_DLIST_get_l_ptr_(l_name), (ref_node), (new_el), (p_new_node))

#define COLLECT_C_DLIST_insert_before_3_(l_name, ref_node, new_el)               collect_c_dlist_insert_before(COLLECT_C_DLIST_get_l_ptr_(l_name), (ref_node), (new_el), NULL)
#define COLLECT_C_DLIST_insert_before_4_(l_name, ref_node, new_el, p_new_node)   collect_c_dlist_insert_before(COLLECT_C_DLIST_get_l_ptr_(l_name), (ref_node), (new_el), (p_new_node))


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


/* modifiers */

#define COLLECT_C_DLIST_clear(...)                          COLLECT_C_UTIL_GET_MACRO_1_or_2_(__VA_ARGS__, COLLECT_C_DLIST_clear_2_, COLLECT_C_DLIST_clear_1_, NULL)(__VA_ARGS__)

#define COLLECT_C_DLIST_erase_node(l_name, node)            collect_c_dlist_erase_node((l_name), (node))

#define COLLECT_C_DLIST_insert_after(...)                   COLLECT_C_UTIL_GET_MACRO_3_or_4_(__VA_ARGS__, COLLECT_C_DLIST_insert_after_4_, COLLECT_C_DLIST_insert_after_3_, NULL)(__VA_ARGS__)

#define COLLECT_C_DLIST_insert_before(...)                  COLLECT_C_UTIL_GET_MACRO_3_or_4_(__VA_ARGS__, COLLECT_C_DLIST_insert_before_4_, COLLECT_C_DLIST_insert_before_3_, NULL)(__VA_ARGS__)

#define COLLECT_DLIST_push_back_by_val(l_name, t_el, new_el)    \
                                                            (COLLECT_C_DLIST_assert_el_size_(l_name, t_el),  collect_c_dlist_push_back_by_ref(COLLECT_C_DLIST_get_l_ptr_(l_name), &((t_el){(new_el)})))
#define COLLECT_DLIST_push_front_by_val(l_name, t_el, new_el)   \
                                                            (COLLECT_C_DLIST_assert_el_size_(l_name, t_el), collect_c_dlist_push_front_by_ref(COLLECT_C_DLIST_get_l_ptr_(l_name), &((t_el){(new_el)})))

/* attributes */

#define COLLECT_C_DLIST_is_empty(l_name)                    (0 == COLLECT_C_DLIST_get_l_ptr_(l_name)->size      )
#define COLLECT_C_DLIST_len(l_name)                         (     COLLECT_C_DLIST_get_l_ptr_(l_name)->size      )
#define COLLECT_C_DLIST_spare(l_name)                       (     COLLECT_C_DLIST_get_l_ptr_(l_name)->num_spares)

/* accessors */

#define COLLECT_C_DLIST_front_v(l_name)                     COLLECT_C_DLIST_node_data_( COLLECT_C_DLIST_head_((l_name)))
#define COLLECT_C_DLIST_back_v(l_name)                      COLLECT_C_DLIST_node_data_( COLLECT_C_DLIST_tail_((l_name)))

#define COLLECT_C_DLIST_cfront_v(l_name)                    COLLECT_C_DLIST_node_cdata_(COLLECT_C_DLIST_head_((l_name)))
#define COLLECT_C_DLIST_cback_v(l_name)                     COLLECT_C_DLIST_node_cdata_(COLLECT_C_DLIST_tail_((l_name)))

#define COLLECT_C_DLIST_front_t(l_name, t_el)               ((t_el      *)(COLLECT_C_DLIST_assert_el_size_(l_name, t_el),  COLLECT_C_DLIST_front_v((l_name))))
#define COLLECT_C_DLIST_back_t(l_name, t_el)                ((t_el      *)(COLLECT_C_DLIST_assert_el_size_(l_name, t_el),   COLLECT_C_DLIST_back_v((l_name))))

#define COLLECT_C_DLIST_cfront_t(l_name, t_el)              ((t_el const*)(COLLECT_C_DLIST_assert_el_size_(l_name, t_el), COLLECT_C_DLIST_cfront_v((l_name))))
#define COLLECT_C_DLIST_cback_t(l_name, t_el)               ((t_el const*)(COLLECT_C_DLIST_assert_el_size_(l_name, t_el),  COLLECT_C_DLIST_cback_v((l_name))))


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

/** T.B.C.
 *
 * @param l T.B.C.
 * @param node T.B.C.
 */
int
collect_c_dlist_erase_node(
    collect_c_dlist_t*      l
,   collect_c_dlist_node_t* node
);

/** Performs a forward search of the list for a node matching the search
 * criteria.
 *
 * @param l Pointer to the list. May not be NULL;
 * @param pfn Comparison function that will be used to identify a node. The
 *  comparison function's lhs is provided by p_lhs, and its rhs values will
 *  be provided during this API function's execution. May not be NULL;
 * @param p_lhs Caller-supplied parameter to be passed back to pfn for
 *  identifying the node(s) to be found. May not be NULL, and must point to
 *  a value suitable to act as the "lhs" for pfn;
 * @param skip_found Number of matched nodes to ignore before a node is
 *  designated as being found;
 * @param node Pointer to a variable to receive the found node. May not be
 *  NULL;
 * @param num_searched Optional pointer to obtain the number of elements
 *  traversed in the search including, if found, the matching element;
 *
 * @retval 0 An element was found matching the given criteria;
 * @retval ENOENT No element was found matching the given criteria;
 */
int
collect_c_dlist_find_node(
    collect_c_dlist_t const*        l
,   collect_c_dlist_pfn_compare_t   pfn
,   void const*                     p_lhs
,   size_t                          skip_count
,   collect_c_dlist_node_t**        node
,   size_t*                         num_searched
);

/** Performs a reverse search of the list for a node matching the search
 * criteria.
 *
 * @param l Pointer to the list. May not be NULL;
 * @param pfn Comparison function that will be used to identify a node. The
 *  comparison function's lhs is provided by p_lhs, and its rhs values will
 *  be provided during this API function's execution. May not be NULL;
 * @param p_lhs Caller-supplied parameter to be passed back to pfn for
 *  identifying the node(s) to be found. May not be NULL, and must point to
 *  a value suitable to act as the "lhs" for pfn;
 * @param skip_found Number of matched nodes to ignore before a node is
 *  designated as being found;
 * @param node Pointer to a variable to receive the found node. May not be
 *  NULL;
 * @param num_searched Optional pointer to obtain the number of elements
 *  traversed in the search including, if found, the matching element;
 *
 * @retval 0 An element was found matching the given criteria;
 * @retval ENOENT No element was found matching the given criteria;
 */
int
collect_c_dlist_rfind_node(
    collect_c_dlist_t const*        l
,   collect_c_dlist_pfn_compare_t   pfn
,   void const*                     p_lhs
,   size_t                          skip_count
,   collect_c_dlist_node_t**        node
,   size_t*                         num_searched
);

/** T.B.C.
 *
 * @param l T.B.C.
 * @param reference_node T.B.C.
 * @param ptr_new_el T.B.C.
 * @param new_node Optional pointer to the created node;
 *
 * @pre (NULL != l);
 * @pre (NULL != l->storage);
 * @pre (NULL != reference_node);
 * @pre (NULL != ptr_new_el);
 */
int
collect_c_dlist_insert_after(
    collect_c_dlist_t*          l
,   collect_c_dlist_node_t*     reference_node
,   void const*                 ptr_new_el
,   collect_c_dlist_node_t**    new_node
);

/** T.B.C.
 *
 * @param l T.B.C.
 * @param reference_node T.B.C.
 * @param ptr_new_el T.B.C.
 * @param new_node Optional pointer to the created node;
 *
 * @pre (NULL != l);
 * @pre (NULL != l->storage);
 * @pre (NULL != reference_node);
 * @pre (NULL != ptr_new_el);
 */
int
collect_c_dlist_insert_before(
    collect_c_dlist_t*          l
,   collect_c_dlist_node_t*     reference_node
,   void const*                 ptr_new_el
,   collect_c_dlist_node_t**    new_node
);

/** T.B.C.
 *
 * @param l T.B.C.
 * @param ptr_new_el T.B.C.
 *
 * @pre (NULL != l);
 * @pre (NULL != l->storage);
 * @pre (NULL != ptr_new_el);
 */
int
collect_c_dlist_push_back_by_ref(
    collect_c_dlist_t*  l
,   void const*         ptr_new_el
);

/** T.B.C.
 *
 * @param l T.B.C.
 * @param ptr_new_el T.B.C.
 *
 * @pre (NULL != l);
 * @pre (NULL != l->storage);
 * @pre (NULL != ptr_new_el);
 */
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

