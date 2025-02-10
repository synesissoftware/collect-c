/* /////////////////////////////////////////////////////////////////////////
 * File:    src/circq.c
 *
 * Purpose: Circular-queue container.
 *
 * Created: 4th February 2025
 * Updated: 10th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/circq.h>

#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * helper functions and macros
 */

#define COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix)     ((void*)(((char*)(q)->storage) + ((ix) * (q)->el_size)))


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

    {
        if (NULL != q->pfn_element_free)
        {
            for (size_t lix = 0; q->e != q->b; ++lix)
            {
                size_t const    ix  =   q->b % q->capacity;
                void* const     pe  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);

                (*q->pfn_element_free)(q->el_size, lix, pe, q->param_element_free);

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
}

int
collect_c_cq_push_back_by_ref(
    collect_c_cq_t* q
,   void const*     ptr_new_el
)
{
    assert(NULL != q);
    assert(NULL != q->storage);

    {
        if (q->capacity == q->e - q->b)
        {
            bool const overwrite_front_when_full = (0 != (COLLECT_C_CIRCQ_F_OVERWRITE_FRONT_WHEN_FULL & q->flags)) && (NULL != q->pfn_element_free);

            if (!overwrite_front_when_full)
            {
                return ENOSPC;
            }
            else
            {
                size_t const    ix  =   q->b % q->capacity;
                void* const     pe  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);

                (*q->pfn_element_free)(q->el_size, ix, pe, q->param_element_free);

                ++q->b;
            }
        }

        {
            size_t const    ix  =   q->e % q->capacity;
            void* const     pe  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);

            memcpy(pe, ptr_new_el, q->el_size);

            ++q->e;

            return 0;
        }
    }
}

int
collect_c_cq_push_back_n_by_ref(
    collect_c_cq_t* q
,   size_t          num_els
,   void const*     ptr_new_els
,   size_t*         num_inserted
)
{
    assert(NULL != q);
    assert(NULL != q->storage);
    assert(0 == num_els || NULL != ptr_new_els);

    {
        bool const overwrite_front_when_full = (0 != (COLLECT_C_CIRCQ_F_OVERWRITE_FRONT_WHEN_FULL & q->flags)) && (NULL != q->pfn_element_free);

        size_t dummy;

        if (NULL == num_inserted)
        {
            num_inserted = &dummy;
        }

        *num_inserted = 0;

        for (size_t i = 0; num_els != i; ++i)
        {
            if (q->capacity == q->e - q->b)
            {
                if (!overwrite_front_when_full)
                {
                    return ENOSPC;
                }
                else
                {
                    /* drop current front */

                    size_t const    ix  =   q->b % q->capacity;
                    void* const     pe  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);

                    (*q->pfn_element_free)(q->el_size, ix, pe, q->param_element_free);

                    ++q->b;
                };
            }

            {
                size_t const    ix      =   q->e % q->capacity;
                void* const     pe_dst  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);
                void const*     pe_src  =   (char*)ptr_new_els + (i * q->el_size);

                memcpy(pe_dst, pe_src, q->el_size);

                ++q->e;

                *num_inserted += 1;
            }
        }

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
    ((void)&reserved0);
    ((void)&reserved1);

    assert(NULL != q);
    assert(NULL != q->storage);
    assert(NULL == reserved0);
    assert(NULL == reserved1);

    {
        size_t dummy;

        if (NULL == num_dropped)
        {
            num_dropped = &dummy;
        }

        *num_dropped = 0;

        if (NULL != q->pfn_element_free)
        {
            for (size_t lix = 0; q->e != q->b; ++lix, ++*num_dropped)
            {
                size_t const    ix  =   q->b % q->capacity;
                void* const     pe  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);

                (*q->pfn_element_free)(q->el_size, lix, pe, q->param_element_free);

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
}

int
collect_c_cq_pop_from_back_n(
    collect_c_cq_t* q
,   size_t          num_to_drop
,   size_t*         num_dropped
)
{
    assert(NULL != q);
    assert(NULL != q->storage);

    {
        size_t dummy;

        if (NULL == num_dropped)
        {
            num_dropped = &dummy;
        }

        *num_dropped = 0;

        for (size_t lix = 0; q->e != q->b && 0 != num_to_drop; --num_to_drop, ++lix)
        {
            if (NULL != q->pfn_element_free)
            {
                size_t const    ix  =   (q->e - 1) % q->capacity;
                void* const     pe  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);

                (*q->pfn_element_free)(q->el_size, lix, pe, q->param_element_free);
            }

            --q->e;
        }

        return 0;
    }
}

int
collect_c_cq_pop_from_front_n(
    collect_c_cq_t* q
,   size_t          num_to_drop
,   size_t*         num_dropped
)
{
    assert(NULL != q);
    assert(NULL != q->storage);

    {
        size_t dummy;

        if (NULL == num_dropped)
        {
            num_dropped = &dummy;
        }

        *num_dropped = 0;

        for (size_t lix = 0; q->e != q->b && 0 != num_to_drop; --num_to_drop, ++lix)
        {
            if (NULL != q->pfn_element_free)
            {
                size_t const    ix  =   q->b % q->capacity;
                void* const     pe  =   COLLECT_C_CIRCQ_INTERNAL_el_ptr_from_ix_(q, ix);

                (*q->pfn_element_free)(q->el_size, lix, pe, q->param_element_free);
            }

            ++q->b;
        }

        return 0;
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

