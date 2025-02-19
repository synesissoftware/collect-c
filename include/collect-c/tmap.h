/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/tmap.h
 *
 * Purpose: Tree-map container.
 *
 * Created: 14th February 2025
 * Updated: 19th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifdef __cplusplus
# ifndef COLLECT_C_TMAP_SUPPRESS_CXX_WARNING
#  error This file not currently compatible with C++ compilation
# endif
#endif


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#define COLLECT_C_TMAP_VER_MAJOR        0
#define COLLECT_C_TMAP_VER_MINOR        0
#define COLLECT_C_TMAP_VER_PATCH        0
#define COLLECT_C_TMAP_VER_ALPHABETA    1

#define COLLECT_C_TMAP_VER \
    (0\
        |   (   COLLECT_C_TMAP_VER_MAJOR        << 24   ) \
        |   (   COLLECT_C_TMAP_VER_MINOR        << 16   ) \
        |   (   COLLECT_C_TMAP_VER_PATCH        <<  8   ) \
        |   (   COLLECT_C_TMAP_VER_ALPHABETA    <<  0   ) \
    )


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/common.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * API constants
 */

/** Suppresses collection of erased nodes as spares. */
#define COLLECT_C_TMAP_F_NO_SPARES                          (0x00000001)


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros (internal)
 */

#ifdef __cplusplus

inline collect_c_tmap_t      * COLLECT_C_TMAP_get_l_ptr_(collect_c_tmap_t      & l) { return &l; }
inline collect_c_tmap_t const* COLLECT_C_TMAP_get_l_ptr_(collect_c_tmap_t const& l) { return &l; }
inline collect_c_tmap_t      * COLLECT_C_TMAP_get_l_ptr_(collect_c_tmap_t      * p) { return  p; }
inline collect_c_tmap_t const* COLLECT_C_TMAP_get_l_ptr_(collect_c_tmap_t const* p) { return  p; }
#else

# define COLLECT_C_TMAP_get_l_ptr_(l)                       _Generic((l),   \
                                                                            \
                               collect_c_tmap_t* :  (l),                    \
                         collect_c_tmap_t const* :  (l),                    \
                                         default : &(l)                     \
)
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API types
 */

struct collect_c_tmap_node_t;
#ifndef __cplusplus
typedef struct collect_c_tmap_node_t                        collect_c_tmap_node_t;
#endif
struct collect_c_tmap_node_t
{
    collect_c_tmap_node_t*          left;
    collect_c_tmap_node_t*          right;
    void*                           value;
    collect_c_common_node_data_t    data[1];
};

/** Callback function that performs key comparison.
 *
 * @param pe_lhs Pointer to the left-hand side key;
 * @param pe_rhs Pointer to the right-hand side key;
 * @param key_size Sizze of the key type;
 */
typedef int (*collect_c_tmap_pfn_cmp)(
    void const*     pe_lhs
,   void const*     pe_rhs
,   size_t          key_size
);

/** Callback function that, if attached to instance, will be called back for
 * each element upon its erasure or replacement by any of the API functions.
 *
 * @param key_size T.B.C.;
 * @param val_size T.B.C.;
 * @param el_index T.B.C.;
 * @param p_el_key Pointer to element key. May be NULL in case that a value
 *  replacement is being done;
 * @param p_el_val T.B.C.;
 * @param param_element_free T.B.C.;
 */
typedef void (*collect_c_tmap_pfn_free)(
    size_t      key_size
,   size_t      val_size
,   intptr_t    el_index    /* always -1 */
,   void*       p_el_key    /* may be NULL */
,   void*       p_el_val
,   void*       param_element_free
);

/** T.B.C.
 *
 */
typedef int (*collect_c_tmap_pfn_walk)(
    size_t      key_size
,   size_t      val_size
,   intptr_t    el_index    /* always -1 */
,   size_t      depth
,   void const* p_el_key
,   void*       p_el_val
,   void*       param_walk
);

enum collect_c_tmap_walkdir_t
{
    COLLECT_C_TMAP_WALK_DEFAULT     =   0
,   COLLECT_C_TMAP_WALK_FORWARD     =   1
,   COLLECT_C_TMAP_WALK_BACKWARD    =   2
,   COLLECT_C_TMAP_WALK_DOWNWARD    =   3
};
#ifndef __cplusplus
typedef enum collect_c_tmap_walkdir_t                       collect_c_tmap_walkdir_t;
#endif



struct collect_c_tmap_t
{
    collect_c_mem_api_t         mem_api;
    size_t                      key_size;           /*! The element key size. */
    size_t                      val_size;           /*! The element value size. */
    size_t                      size;               /*! The number of elements currently stored */
    int32_t                     flags;              /*! Control flags. */
    int32_t                     reserved0;          /*! Reserved field. */
    collect_c_tmap_node_t*      root;               /*! Root element. */
    collect_c_tmap_pfn_cmp      pfn_key_cmp;        /*! T.B.C. */
    void*                       param_element_free; /*! Custom parameter to be passed to invocations of pfn_element_free. */
    collect_c_tmap_pfn_free     pfn_element_free;   /*! Custom function to be invoked when element erased/replaced. */
};
#ifndef __cplusplus
typedef struct collect_c_tmap_t                             collect_c_tmap_t;
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros
 */

/** @def COLLECT_C_TMAP_define_empty_with_comparison_function(t_key, t_val, fn_key_cmp, m_name)
 *
 * Declares and defines an empty tree-map instance with the given key type,
 * value type, key comparison function. The instance will need to be further
 * set-up via collect_c_tmap_allocate_storage().
 *
 * @param t_key T.B.C.;
 * @param t_val T.B.C.;
 * @param fn_key_cmp T.B.C.;
 * @param m_name T.B.C.;
 */

#define COLLECT_C_TMAP_define_empty_with_comparison_function(t_key, t_val, fn_key_cmp, m_name)\
                                                                            \
    collect_c_tmap_t m_name = COLLECT_C_TMAP_EMPTY_INITIALIZER_(t_key, t_val, 0, fn_key_cmp, NULL, NULL)



/** @def COLLECT_C_TMAP_define_empty(m_key_type, m_val_type, m_name)
 *
 * Declares and defines an empty tree-map instance. The instance will need
 * to be further set-up via collect_c_tmap_allocate_storage().
 *
 * @param m_key_type The type of the keys to be stored;
 * @param m_val_type The type of the values to be stored;
 * @param m_name The name of the instance;
 */
#define COLLECT_C_TMAP_define_empty(t_key, t_val, m_name)   \
                                                            \
    collect_c_tmap_t m_name = COLLECT_C_TMAP_EMPTY_INITIALIZER_(t_key, t_val, 0, collect_c_fn_cmp_ ## t_key, NULL, NULL)


/* modifiers */

#define COLLECT_C_TMAP_insert_by_val(m_name, t_key, v_key, t_val, v_val, p_was_replaced)    \
                                                            collect_c_tmap_insert(COLLECT_C_TMAP_get_l_ptr_(m_name), &((t_key){(v_key)}), &((t_val){(v_val)}), NULL, p_was_replaced)


/* attributes */

#define COLLECT_C_TMAP_is_empty(m_name)                     (0 == COLLECT_C_TMAP_get_l_ptr_(m_name)->size      )
#define COLLECT_C_TMAP_len(m_name)                          (     COLLECT_C_TMAP_get_l_ptr_(m_name)->size      )



/* accessors */




/* /////////////////////////////////////////////////////////////////////////
 * API macros
 */


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Obtains the value of COLLECT_C_TMAP_VER at the time of compilation of
 * the library.
 */
uint32_t
collect_c_tmap_version(void);


void
clc_treemap_free_storage(
    collect_c_tmap_t*   m
);


collect_c_tmap_node_t const*
collect_c_tmap_find_node(
    collect_c_tmap_t const*     m
,   void const*                 ptr_lhs_key
);

int
collect_c_tmap_insert(
    collect_c_tmap_t*           m
,   void const*                 ptr_new_key
,   void const*                 ptr_new_val
,   collect_c_tmap_node_t**     new_node
,   int*                        p_was_replaced
);

int
collect_c_tmap_walk(
    collect_c_tmap_t*           m
,   collect_c_tmap_pfn_walk     pfn_walk
,   void*                       param_walk
,   collect_c_tmap_walkdir_t    direction
);


#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * helper macros
 */

/*
 * @param t_key T.B.C.;
 * @param t_val T.B.C.;
 * @param m_flags T.B.C.;
 * @param fn_key_cmp T.B.C.;
 * @param fn_entry_free T.B.C.;
 * @param param_entry_free T.B.C.;
 */

#define COLLECT_C_TMAP_EMPTY_INITIALIZER_(t_key, t_val, m_flags, fn_key_cmp, fn_entry_free, param_entry_free)   \
                                                            \
    {                                                       \
        .mem_api =                                          \
        {                                                   \
            .pfn_alloc = collect_c_mem_std_alloc,           \
            .pfn_realloc = collect_c_mem_std_realloc,       \
            .pfn_free = collect_c_mem_std_free,             \
            .param = NULL,                                  \
        },                                                  \
        .key_size = sizeof(t_key),                          \
        .val_size = sizeof(t_val),                          \
        .size = 0,                                          \
        .flags = (m_flags),                                 \
        .reserved0 = 0,                                     \
        .root = NULL,                                       \
        .pfn_key_cmp = (fn_key_cmp),                        \
        .param_element_free = (param_entry_free),           \
        .pfn_element_free = (fn_entry_free),                \
    }


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

