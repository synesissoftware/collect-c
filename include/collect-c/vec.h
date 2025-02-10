/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/vec.h
 *
 * Purpose: Vector container.
 *
 * Created: 5th February 2025
 * Updated: 11th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifdef __cplusplus
# ifndef COLLECT_C_VEC_SUPPRESS_CXX_WARNING
#  error This file not currently compatible with C++ compilation
# endif
#endif


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#define COLLECT_C_VEC_VER_MAJOR     0
#define COLLECT_C_VEC_VER_MINOR     0
#define COLLECT_C_VEC_VER_PATCH     0
#define COLLECT_C_VEC_VER_ALPHABETA 2

#define COLLECT_C_VEC_VER \
    (0\
        |   (   COLLECT_C_VEC_VER_MAJOR     << 24   ) \
        |   (   COLLECT_C_VEC_VER_MINOR     << 16   ) \
        |   (   COLLECT_C_VEC_VER_PATCH     <<  8   ) \
        |   (   COLLECT_C_VEC_VER_ALPHABETA <<  0   ) \
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

/** Causes allocation to be on the heap.
 *
 * @note In the current implementation, using stack allocation implies the
 *  effects of COLLECT_C_VEC_F_NO_BOO.
 */
#define COLLECT_C_VEC_F_USE_STACK_ARRAY                     (0x00000001)

/** Prevents the use of base-offset optimisation. */
#define COLLECT_C_VEC_F_NO_BOO                              (0x00000002)


/* /////////////////////////////////////////////////////////////////////////
 * API types
 */

/** T.B.C.
 *
 * @note This type supports what we refer to as "base-offset optimisation",
 *  which means that the front of the vector is managed such that inserting
 *  at/pushing to the front may be achieved without reallocation. This is
 *  enabled by default, and means that erasing at/popping from the front may
 *  be done without moving all of the elements, and is often done in that
 *  way. Naturally, this results in faster popping _and_ pushing behaviour,
 *  but at the cost of using more memory. The implementation makes a
 *  determination as to what level of "spare" memory is acceptable, and will
 *  do a shrink when it is deemded appropriate. To suspend this behaviour,
 *  and thereby obtain a classic vector implementation, the flag
 *  COLLECT_C_VEC_F_NO_BOO may be applied.
 */
struct collect_c_vec_t
{
    size_t  el_size;            /*! The element size. */
    size_t  capacity;           /*! The capacity. */
    size_t  offset;             /*! The base offset. */
    size_t  size;               /*! The size. */
    int32_t flags;              /*! Control flags. */
    int32_t reserved0;          /*! Reserved field. */
    void*   storage;            /*! Pointer to the storage. */
    void*   param_element_free; /*! Custom parameter to be passed to invocations of pfn_element_free */
    void  (*pfn_element_free)(
        size_t  el_size
    ,   size_t  el_index
    ,   void*   el_ptr
    ,   void*   param_element_free
    );                          /*! Custom function to be invoked when */
};
#ifndef __cplusplus
typedef struct collect_c_vec_t  collect_c_vec_t;
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros (internal)
 */

#define COLLECT_C_VEC_get_l_ptr_(v)                         _Generic((v),   \
                                                                            \
              collect_c_vec_t* :  (v),                                      \
        collect_c_vec_t const* :  (v),                                      \
                       default : &(v)                                       \
)

#define COLLECT_C_VEC_assert_el_size_(v_name, t_el)         assert(sizeof(t_el) == COLLECT_C_VEC_get_l_ptr_(v_name)->el_size)
#define COLLECT_C_VEC_assert_ix_(v_name, ix)                assert((ix) < COLLECT_C_VEC_get_l_ptr_(v_name)->size)
#define COLLECT_C_VEC_assert_not_empty_(v_name)             assert(0 != COLLECT_C_VEC_get_l_ptr_(v_name)->size)
#define COLLECT_C_VEC_assert_not_null_(v_name)              assert(NULL != (v_name))

#define COLLECT_C_VEC_clear_1_(l_name)                      collect_c_vec_clear(COLLECT_C_VEC_get_l_ptr_(l_name), NULL, NULL, NULL)
#define COLLECT_C_VEC_clear_2_(l_name, p)                   collect_c_vec_clear(COLLECT_C_VEC_get_l_ptr_(l_name), NULL, NULL,  (p))

#define COLLECT_C_VEC_at_v_(v_name, ix)                     ((void      *)(((char      *)COLLECT_C_VEC_get_l_ptr_(v_name)->storage) + ((COLLECT_C_VEC_get_l_ptr_(v_name)->offset + (ix)) * COLLECT_C_VEC_get_l_ptr_(v_name)->el_size)))
#define COLLECT_C_VEC_cat_v_(v_name, ix)                    ((void const*)(((char const*)COLLECT_C_VEC_get_l_ptr_(v_name)->storage) + ((COLLECT_C_VEC_get_l_ptr_(v_name)->offset + (ix)) * COLLECT_C_VEC_get_l_ptr_(v_name)->el_size)))

#define COLLECT_C_VEC_cbegin_(v_name)                       COLLECT_C_VEC_cat_v_(v_name,                                      0)
#define COLLECT_C_VEC_cend_(v_name)                         COLLECT_C_VEC_cat_v_(v_name, COLLECT_C_VEC_get_l_ptr_(v_name)->size)

#define COLLECT_C_VEC_begin_(v_name)                        COLLECT_C_VEC_at_v_( v_name,                                      0)
#define COLLECT_C_VEC_end_(v_name)                          COLLECT_C_VEC_at_v_( v_name, COLLECT_C_VEC_get_l_ptr_(v_name)->size)


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros
 */

/** @def COLLECT_C_VEC_define_empty(el_type, v_name)
 *
 * Declares and defines an empty vector instance. The instance will need to
 * be further set-up via collect_c_vec_allocate_storage().
 *
 * @param el_type The type of the elements to be stored;
 * @param v_name The name of the instance;
 */
#define COLLECT_C_VEC_define_empty(el_type, v_name)         \
                                                            \
    collect_c_vec_t v_name = { .el_size = sizeof(el_type), }


/** @def COLLECT_C_VEC_define_empty_with_callback(el_type, v_name, elf_fn, elf_param)
 *
 * Declares and defines an vector instance. The instance will need to
 * be further set-up via collect_c_vec_allocate_storage().
 *
 * @param el_type The type of the elements to be stored;
 * @param v_name The name of the instance;
 * @param elf_fn Callback function to be invoked when element is
 *  erased/removed/overwritte;
 * @param elf_param Parameter to be given to the callback function;
 */
#define COLLECT_C_VEC_define_empty_with_callback(el_type, v_name, elf_fn, elf_param)    \
                                                                                        \
                                                            collect_c_vec_t v_name = { .el_size = sizeof(el_type), .pfn_element_free = elf_fn, .param_element_free = elf_param, }


/** @def COLLECT_C_VEC_define_on_stack(v_name, ar_name)
 *
 * Declares and defines a vector instance that uses for its memory the given
 * array instance.
 *
 * @param v_name The name of the instance;
 * @param ar_name The name of the array instance that will serve as the
 *  memory of the vector instance;
 */
#define COLLECT_C_VEC_define_on_stack(v_name, ar_name)      \
                                                            \
    collect_c_vec_t v_name = COLLECT_C_VEC_EMPTY_INITIALIZER_(((ar_name)[0]), (sizeof((ar_name)) / sizeof((ar_name)[0])), COLLECT_C_VEC_F_USE_STACK_ARRAY, &(ar_name)[0], NULL, 0)


/* modifiers */

#define COLLECT_C_VEC_clear(...)                            COLLECT_C_UTIL_GET_MACRO_1_or_2_(__VA_ARGS__, COLLECT_C_VEC_clear_2_, COLLECT_C_VEC_clear_1_, NULL)(__VA_ARGS__)

#define COLLECT_C_VEC_push_back_by_ref(v_name, ptr_new_el)  collect_c_v_push_back_by_ref(COLLECT_C_VEC_get_l_ptr_(v_name), (ptr_new_el))
#define COLLECT_C_VEC_push_back_by_value(v_name, t_el, new_el)  \
                                                                \
                                                            (COLLECT_C_VEC_assert_el_size_(v_name, t_el), collect_c_v_push_back_by_ref(COLLECT_C_VEC_get_l_ptr_(v_name), &((t_el){(new_el)})))

#define COLLECT_C_VEC_push_front_by_ref(v_name, ptr_new_el)  collect_c_v_push_front_by_ref(COLLECT_C_VEC_get_l_ptr_(v_name), (ptr_new_el))
#define COLLECT_C_VEC_push_front_by_value(v_name, t_el, new_el) \
                                                                \
                                                            (COLLECT_C_VEC_assert_el_size_(v_name, t_el), collect_c_v_push_front_by_ref(COLLECT_C_VEC_get_l_ptr_(v_name)->, &((t_el){(new_el)})))

#define COLLECT_C_VEC_shrink_to_fit(v_name)                 collect_c_vec_shrink_to_fit(COLLECT_C_VEC_get_l_ptr_(v_name))

/* attributes */

#define COLLECT_C_VEC_is_empty(v_name)                      (0 == COLLECT_C_VEC_len((v_name)))
#define COLLECT_C_VEC_len(v_name)                           (COLLECT_C_VEC_get_l_ptr_(v_name)->size)
#define COLLECT_C_VEC_spare(v_name)                         (COLLECT_C_VEC_get_l_ptr_(v_name)->capacity - COLLECT_C_VEC_get_l_ptr_(v_name)->size)
#define COLLECT_C_VEC_spare_back(v_name)                    (COLLECT_C_VEC_get_l_ptr_(v_name)->capacity - (COLLECT_C_VEC_get_l_ptr_(v_name)->offset + COLLECT_C_VEC_get_l_ptr_(v_name)->size))
#define COLLECT_C_VEC_spare_front(v_name)                   (COLLECT_C_VEC_get_l_ptr_(v_name)->offset)

/* accessors */

#define COLLECT_C_VEC_at_v(v_name, ix)                      (COLLECT_C_VEC_assert_ix_(v_name, ix),  COLLECT_C_VEC_at_v_(v_name, ix))
#define COLLECT_C_VEC_cat_v(v_name, ix)                     (COLLECT_C_VEC_assert_ix_(v_name, ix), COLLECT_C_VEC_cat_v_(v_name, ix))

#define COLLECT_C_VEC_at_t(v_name, t_el, ix)                ((t_el      *)(COLLECT_C_VEC_assert_el_size_(v_name, t_el), COLLECT_C_VEC_assert_ix_(v_name, ix),  COLLECT_C_VEC_at_v_(v_name, ix)))
#define COLLECT_C_VEC_cat_t(v_name, t_el, ix)               ((t_el const*)(COLLECT_C_VEC_assert_el_size_(v_name, t_el), COLLECT_C_VEC_assert_ix_(v_name, ix), COLLECT_C_VEC_cat_v_(v_name, ix)))

#define COLLECT_C_VEC_front_t(v_name, t_el)                 (COLLECT_C_VEC_assert_not_empty_(v_name), COLLECT_C_VEC_at_t( v_name, t_el,                 0))
#define COLLECT_C_VEC_back_t(v_name, t_el)                  (COLLECT_C_VEC_assert_not_empty_(v_name), COLLECT_C_VEC_at_t( v_name, t_el, COLLECT_C_VEC_get_l_ptr_(v_name)->size - 1))

#define COLLECT_C_VEC_cfront_t(v_name, t_el)                (COLLECT_C_VEC_assert_not_empty_(v_name), COLLECT_C_VEC_cat_t(v_name, t_el,                 0))
#define COLLECT_C_VEC_cback_t(v_name, t_el)                 (COLLECT_C_VEC_assert_not_empty_(v_name), COLLECT_C_VEC_cat_t(v_name, t_el, COLLECT_C_VEC_get_l_ptr_(v_name)->size - 1))

/* iteration */

#define COLLECT_C_VEC_begin_v(v_name)                       COLLECT_C_VEC_begin_(v_name)
#define COLLECT_C_VEC_end_v(v_name)                           COLLECT_C_VEC_end_(v_name)

#define COLLECT_C_VEC_cbegin_v(v_name)                      COLLECT_C_VEC_cbegin_(v_name)
#define COLLECT_C_VEC_cend_v(v_name)                          COLLECT_C_VEC_cend_(v_name)

#define COLLECT_C_VEC_cbegin_t(v_name, t_el)                ((t_el const*)(COLLECT_C_VEC_assert_el_size_(v_name, t_el), COLLECT_C_VEC_cbegin_(v_name)))
#define COLLECT_C_VEC_cend_t(v_name, t_el)                  ((t_el const*)(COLLECT_C_VEC_assert_el_size_(v_name, t_el),   COLLECT_C_VEC_cend_(v_name)))

#define COLLECT_C_VEC_begin_t(v_name, t_el)                 ((t_el      *)(COLLECT_C_VEC_assert_el_size_(v_name, t_el), COLLECT_C_VEC_begin_(v_name)))
#define COLLECT_C_VEC_end_t(v_name, t_el)                   ((t_el      *)(COLLECT_C_VEC_assert_el_size_(v_name, t_el),   COLLECT_C_VEC_end_(v_name)))


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Obtains the value of COLLECT_C_VEC_VER at the time of compilation of
 * the library.
 */
uint32_t
collect_c_vec_version(void);

/** Allocates storage for an instance from the heap.
 *
 * @param v Pointer to the vector. May not be NULL. May not point to an
 *  instance that has already been successfully allocated;
 * @param initial_capacity The initial capacity to be allocated;
 *
 * @return Indicates whether operation succeeded.
 * @retval 0 Operation succeed;
 * @retval ENOMEM Sufficient memory not available;
 *
 * @pre (NULL != v)
 * @pre (NULL == v->storage)
 */
int
collect_c_vec_allocate_storage(
    collect_c_vec_t*    v
,   size_t              initial_capacity
);

/** Frees storage associated with the instance. Must be called if the
 * instance's allocation was obtained via collect_c_vec_allocate_storage().
 *
 * @param v Pointer to the vector. May not be NULL;
 *
 * @pre (NULL != v)
 * @pre (NULL != v->storage)
 */
void
collect_c_vec_free_storage(
    collect_c_vec_t*    v
);

/** Clears all elements from the vector.
 *
 * @param v Pointer to the vector. May not be NULL;
 * @param reserved0 Reserved. Must be 0;
 * @param reserved1 Reserved. Must be 0;
 * @param num_dropped Optional pointer to variable to retrieve number of
 *  entries dropped;
 *
 * @note This function does not change the allocated memory underlying the
 *  instance. To do this, call collect_c_vec_shrink_to_fit().
 *
 * @pre (NULL != v)
 * @pre (NULL != v->storage)
 * @pre (0 == reserved0)
 * @pre (0 == reserved1)
 */
int
collect_c_vec_clear(
    collect_c_vec_t*    v
,   void*               reserved0
,   void*               reserved1
,   size_t*             num_dropped
);

/** T.B.C.
 *
 * @param v T.B.C.
 */
int
collect_c_vec_shrink_to_fit(
    collect_c_vec_t*    v
);

/** T.B.C.
 *
 * @param v T.B.C.
 * @param ptr_new_el T.B.C.
 */
int
collect_c_v_push_back_by_ref(
    collect_c_vec_t*    v
,   void const*         ptr_new_el
);

/** T.B.C.
 *
 * @param v T.B.C.
 * @param ptr_new_el T.B.C.
 */
int
collect_c_v_push_front_by_ref(
    collect_c_vec_t*    v
,   void const*         ptr_new_el
);

#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * helper macros
 */

#define COLLECT_C_VEC_EMPTY_INITIALIZER_(vec_el_type, vec_cap, vec_flags, vec_storage, elf_fn, elf_param) \
                                                                            \
    {                                                                       \
        .el_size = sizeof(vec_el_type),                                     \
        .capacity = (vec_cap),                                              \
        .offset = 0,                                                        \
        .size = 0,                                                          \
        .flags = (vec_flags),                                               \
        .reserved0 = 0,                                                     \
        .storage = (vec_storage),                                           \
        .param_element_free = (elf_param),                                  \
        .pfn_element_free = (elf_fn),                                       \
    }


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

