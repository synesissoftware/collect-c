/* /////////////////////////////////////////////////////////////////////////
 * File:    src/containers/circular_queue.c
 *
 * Purpose: Circular-queue container.
 *
 * Created: 4th February 2025
 * Updated: 7th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/circq.h>

#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

uint32_t
collect_c_cq_version(void)
{
    return COLLECT_C_CIRCQ_VER;
}

int
collect_c_cq_allocate_storage(
    collect_c_cq_t* q
)
{
    assert(NULL != q);
    assert(NULL == q->storage);

    {
        size_t const cb = q->el_size * q->capacity;

        if (NULL == (q->storage = malloc(cb)))
        {
            return errno;
        }
        else
        {
            /* TODO: should initialize? (I don't think so ...) */

            return 0;
        }
    }
}


void
collect_c_cq_free_storage(
    collect_c_cq_t* q
)
{
    assert(NULL != q);
    assert(NULL != q->storage);

    if (NULL != q->pfn_element_free)
    {
        for (; q->e != q->b; )
        {
            size_t const    ix  =   q->b % q->capacity;
            void* const     p   =   ((char*)q->storage) + (ix * q->el_size);

            (*q->pfn_element_free)(q->el_size, ix, p, q->param_element_free);

            ++q->b;
        }
    }

    if (0 == (COLLECT_C_CIRCQ_F_USE_STACK_ARRAY & q->flags))
    {
        free(q->storage);

        q->storage = NULL;
    }

    q->b = q->e = 0;
}


int
collect_c_cq_push_by_ref(
    collect_c_cq_t* q
,   void const*     ptr_new_el
)
{
    assert(NULL != q);
    assert(NULL != q->storage);
    assert(COLLECT_C_CIRCQ_spare(*q) != 0);

    if (q->capacity == q->e - q->b)
    {
        return ENOSPC;
    }
    else
    {
        size_t const    ix  =   q->e % q->capacity;
        void* const     p   =   ((char*)q->storage) + (ix * q->el_size);

        memcpy(p, ptr_new_el, q->el_size);

        ++q->e;

        return 0;
    }
}

int
collect_c_cq_clear(
    collect_c_cq_t* q
,   void*           reserved0
,   void*           reserved1
,   size_t*         num_dropped
)
{
    size_t dummy;

    ((void)&reserved0);
    ((void)&reserved1);

    assert(NULL != q);
    assert(NULL != q->storage);
    assert(NULL != reserved0);
    assert(NULL != reserved1);

    if (NULL == num_dropped)
    {
        num_dropped = &dummy;
    }

    *num_dropped = 0;

    if (NULL != q->pfn_element_free)
    {
        for (; q->e != q->b; ++*num_dropped)
        {
            size_t const    ix  =   q->b % q->capacity;
            void* const     p   =   ((char*)q->storage) + (ix * q->el_size);

            (*q->pfn_element_free)(q->el_size, ix, p, q->param_element_free);

            ++q->b;
        }
    }
    else
    {
        for (; q->e != q->b; ++*num_dropped)
        {
            ++q->b;
        }
    }

    q->b = q->e = 0;

    return 0;
}

int
collect_c_cq_pop_from_back_n(
    collect_c_cq_t* q
,   size_t          num_to_drop
,   size_t*         num_dropped
)
{
    size_t dummy;

    assert(NULL != q);
    assert(NULL != q->storage);

    if (NULL == num_dropped)
    {
        num_dropped = &dummy;
    }

    *num_dropped = 0;

    for (; q->e != q->b && 0 != num_to_drop; --num_to_drop)
    {
        if (NULL != q->pfn_element_free)
        {
            size_t const    ix  =   (q->e - 1) % q->capacity;
            void* const     p   =   ((char*)q->storage) + (ix * q->el_size);

            (*q->pfn_element_free)(q->el_size, ix, p, q->param_element_free);
        }

        --q->e;
    }

    return 0;
}

int
collect_c_cq_pop_from_front_n(
    collect_c_cq_t* q
,   size_t          num_to_drop
,   size_t*         num_dropped
)
{
    size_t dummy;

    assert(NULL != q);
    assert(NULL != q->storage);

    if (NULL == num_dropped)
    {
        num_dropped = &dummy;
    }

    *num_dropped = 0;

    for (size_t i = q->b; q->e != i && 0 != num_to_drop; --num_to_drop)
    {
        if (NULL != q->pfn_element_free)
        {
            size_t const    ix  =   i % q->capacity;
            void* const     p   =   ((char*)q->storage) + (ix * q->el_size);

            (*q->pfn_element_free)(q->el_size, ix, p, q->param_element_free);
        }

        ++q->b;
    }

    return 0;
}


/* ///////////////////////////// end of file //////////////////////////// */

