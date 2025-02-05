/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/circq.h
 *
 * Purpose: Circular-queue container.
 *
 * Created: 4th February 2025
 * Updated: 6th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifdef __cplusplus
# error This file not currently compatible with C++ compilation
#endif


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

#define COLLECT_C_CIRCQ_VER_MAJOR       0
#define COLLECT_C_CIRCQ_VER_MINOR       0
#define COLLECT_C_CIRCQ_VER_PATCH       0
#define COLLECT_C_CIRCQ_VER_ALPHABETA   1

#define COLLECT_C_CIRCQ_VER \
    (0\
        |   (   COLLECT_C_CIRCQ_VER_MAJOR       << 24   ) \
        |   (   COLLECT_C_CIRCQ_VER_MINOR       << 16   ) \
        |   (   COLLECT_C_CIRCQ_VER_PATCH       <<  8   ) \
        |   (   COLLECT_C_CIRCQ_VER_ALPHABETA   <<  0   ) \
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

/** Causes allocation to be on the heap. */
#define COLLECT_C_CIRCQ_F_USE_STACK_ARRAY                   (0x00000001)


/* /////////////////////////////////////////////////////////////////////////
 * API types
 */

struct collect_c_cq_t
{
    size_t  el_size;            /*! The element size. */
    size_t  capacity;           /*! The capacity. */
    size_t  b;                  /*! The pseudo-index of el[0]. */
    size_t  e;                  /*! The pseudo-index of el[size]. */
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
typedef struct collect_c_cq_t   collect_c_cq_t;
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions & macros
 */

/** @def COLLECT_C_CIRCQ_DEFINE_EMPTY(cq_el_type, cq_name, cq_cap)
 *
 * Declares and defines an empty queue instance. The instance will need to
 * be further set-up via collect_c_cq_allocate_storage().
 *
 * @param cq_el_type The type of the elements to be stored;
 * @param cq_name The name of the instance;
 * @param cq_cap The capacity that the instance should have;
 */
#define COLLECT_C_CIRCQ_DEFINE_EMPTY(cq_el_type, cq_name, cq_cap)           \
                                                                            \
    collect_c_cq_t cq_name = COLLECT_C_CIRCQ_EMPTY_INITIALIZER_(cq_el_type, cq_cap, 0, NULL, NULL, 0)


/** @def COLLECT_C_CIRCQ_DEFINE_ON_STACK(cq_name, cq_array)
 *
 * Declares and defines a queue instance that uses for its memory the given
 * array instance.
 *
 * @param cq_name The name of the instance;
 * @param cq_array The name of the array instance that will serve as the
 *  memory of the queue instance;
 */
#define COLLECT_C_CIRCQ_DEFINE_ON_STACK(cq_name, cq_array)                  \
                                                                            \
    collect_c_cq_t cq_name = COLLECT_C_CIRCQ_EMPTY_INITIALIZER_((cq_array)[0], sizeof((cq_array)) / sizeof((cq_array)[0]), COLLECT_C_CIRCQ_F_USE_STACK_ARRAY, cq_array, NULL, 0)


/** @def COLLECT_C_CIRCQ_DEFINE_ON_STACK_WITH_CALLBACK(cq_name, cq_array, elf_fn, elf_param)
 *
 * Declares and defines a queue instance that uses for its memory the given
 * array instance, and specifies a callback function to be invoked each time
 * an API function causes an element to be erased/removed/overwritte.
 *
 * @param cq_name The name of the instance;
 * @param cq_array The name of the array instance that will serve as the
 *  memory of the queue instance;
 * @param elf_fn Callback function to be invoked when element is
 *  erased/removed/overwritte;
 * @param elf_param Parameter to be given to the callback function;
 */
#define COLLECT_C_CIRCQ_DEFINE_ON_STACK_WITH_CALLBACK(cq_name, cq_array, elf_fn, elf_param) \
                                                                                            \
    collect_c_cq_t cq_name = COLLECT_C_CIRCQ_EMPTY_INITIALIZER_((cq_array)[0], sizeof((cq_array)) / sizeof((cq_array)[0]), COLLECT_C_CIRCQ_F_USE_STACK_ARRAY, cq_array, elf_fn, elf_param)


#define COLLECT_C_CIRCQ_is_empty(cq_name)                   ((cq_name).e == (cq_name).b)
#define COLLECT_C_CIRCQ_len(cq_name)                        ((cq_name).e - (cq_name).b)
#define COLLECT_C_CIRCQ_spare(cq_name)                      ((cq_name).capacity - COLLECT_C_CIRCQ_len(cq_name))

#define COLLECT_C_CIRCQ_at(cq_name, el_ix)                  (((char*)(cq_name).storage) + ((((cq_name).b + (el_ix)) % (cq_name).capacity) * (cq_name).el_size))
#define COLLECT_C_CIRCQ_element_index(cq_name, el_ix)       (((cq_name).b + (el_ix)) % (cq_name).capacity)

#define COLLECT_C_CIRCQ_add_by_ref(cq_name, ptr_new_el)     collect_c_cq_add_by_ref(&(cq_name), ptr_new_el)
#define COLLECT_C_CIRCQ_add_by_value(cq_name, t_el, new_el) (assert(sizeof(t_el) == (cq_name).el_size), collect_c_cq_add_by_ref(&(cq_name), &((t_el){(new_el)})))

#define COLLECT_C_CIRCQ_pop_back(cq_name)                   collect_c_cq_pop_from_back_n(&(cq_name), 1, NULL)
#define COLLECT_C_CIRCQ_pop_front(cq_name)                  collect_c_cq_pop_from_front_n(&(cq_name), 1, NULL)

#define COLLECT_C_CIRCQ_clear_1_(cq_name)                   collect_c_cq_clear(&(cq_name), NULL, NULL, NULL)
#define COLLECT_C_CIRCQ_clear_2_(cq_name, p)                collect_c_cq_clear(&(cq_name), NULL, NULL, (p))

#define COLLECT_C_CIRCQ_clear_GET_MACRO_(_1, _2, macro, ...)    macro

#define COLLECT_C_CIRCQ_clear(...)                          COLLECT_C_CIRCQ_clear_GET_MACRO_(__VA_ARGS__, COLLECT_C_CIRCQ_clear_2_, COLLECT_C_CIRCQ_clear_1_, NULL)(__VA_ARGS__)


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Obtains the value of COLLECT_C_CIRCQ_VER at the time of compilation of
 * the library.
 */
uint32_t
collect_c_cq_version(void);

/** Allocates storage for an instance from the heap.
 *
 * @param q Pointer to the queue. Must not be NULL. Must not point to an
 *  instance that has already been successfully allocated;
 *
 * @return Indicates whether operation succeeded.
 * @retval 0 Operation succeed;
 * @retval ENOMEM Sufficient memory not available;
 *
 * @pre (NULL != q);
 * @pre (NULL == q->storage);
 */
int
collect_c_cq_allocate_storage(
    collect_c_cq_t* q
);

/** Frees storage associated with the instance. Must be called if the
 * instance's allocation was obtained via collect_c_cq_allocate_storage().
 *
 * @param q Pointer to the circular queue. Must not be NULL;
 *
 * @pre (NULL != q);
 * @pre (NULL != q->storage);
 */
void
collect_c_cq_free_storage(
    collect_c_cq_t* q
);

/** Attempts to add an item to the (back of) the queue.
 *
 * @param q Pointer to the circular queue. Must not be NULL;
 *
 * @retval 0 The item was added to the queue;
 * @retval ENOSPC No space left in queue;
 *
 * @pre (NULL != q);
 * @pre (NULL != q->storage);
 */
int
collect_c_cq_add_by_ref(
    collect_c_cq_t* q
,   void const*     ptr_new_el
);

/** Clears all elements from the front of the queue.
 *
 * @param q Pointer to the circular queue. Must not be NULL;
 * @param reserved0 Reserved. Must be 0;
 * @param reserved1 Reserved. Must be 0;
 * @param num_dropped Optional pointer to variable to retrieve number of
 *  entries dropped;
 *
 * @pre (NULL != q);
 * @pre (NULL != q->storage);
 * @pre (0 == reserved0);
 * @pre (0 == reserved1);
 */
int
collect_c_cq_clear(
    collect_c_cq_t* q
,   void*           reserved0
,   void*           reserved1
,   size_t*         num_dropped
);

/** Attempts to drop a number of elements from the front of the queue.
 *
 * @param q Pointer to the circular queue. Must not be NULL;
 * @param num_to_drop Maximum number of elements to drop working from the
 *  back of the queue;
 * @param num_dropped Optional pointer to variable to retrieve number of
 *  entries dropped;
 *
 * @pre (NULL != q);
 * @pre (NULL != q->storage);
 */
int
collect_c_cq_pop_from_back_n(
    collect_c_cq_t* q
,   size_t          num_to_drop
,   size_t*         num_dropped
);

/** Attempts to drop a number of elements from the front of the queue.
 *
 * @param q Pointer to the circular queue. Must not be NULL;
 * @param num_to_drop Maximum number of elements to drop working from the
 *  back of the queue;
 * @param num_dropped Optional pointer to variable to retrieve number of
 *  entries dropped;
 *
 * @pre (NULL != q);
 * @pre (NULL != q->storage);
 */
int
collect_c_cq_pop_from_front_n(
    collect_c_cq_t* q
,   size_t          num_to_drop
,   size_t*         num_dropped
);

#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * helper macros
 */

#define COLLECT_C_CIRCQ_EMPTY_INITIALIZER_(cq_el_type, cq_cap, cq_flags, cq_storage, elf_fn, elf_param) \
                                                                            \
    {                                                                       \
        .el_size = sizeof(cq_el_type),                                      \
        .capacity = (cq_cap),                                               \
        .b = 0,                                                             \
        .e = 0,                                                             \
        .flags = (cq_flags),                                                \
        .reserved0 = 0,                                                     \
        .storage = (cq_storage),                                            \
        .param_element_free = (elf_param),                                  \
        .pfn_element_free = (elf_fn),                                       \
    }


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

