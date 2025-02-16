/* /////////////////////////////////////////////////////////////////////////
 * File:    src/vec.c
 *
 * Purpose: Vector container.
 *
 * Created: 5th February 2025
 * Updated: 14th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/vec.h>

#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

    #include <stdio.h>


/* /////////////////////////////////////////////////////////////////////////
 * helper functions and macros
 */

 #define COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, ix)      ((void*)(((char*)(v)->storage) + ((ix) * (v)->el_size)))


 /* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

uint32_t
collect_c_vec_version(void)
{
    return COLLECT_C_VEC_VER;
}

int
collect_c_vec_allocate_storage(
    collect_c_vec_t*    v
,   size_t              initial_capacity
)
{
    assert(NULL != v);
    assert(NULL == v->storage);

    {
        bool const uses_stack_array =   0 != (COLLECT_C_VEC_F_USE_STACK_ARRAY & v->flags);
        bool const uses_boo         =   0 == (COLLECT_C_VEC_F_NO_BOO & v->flags);

        if (uses_stack_array)
        {
            return ENODEV;
        }
        else
        {
            size_t const    offset  =   uses_boo ? initial_capacity / 4 : 0;
            size_t const    cap_all =   initial_capacity + offset;

            size_t const    cb      =   cap_all * v->el_size;

            if (NULL == (v->storage = malloc(cb)))
            {
                return ENOMEM;
            }
            else
            {
                v->capacity =   cap_all;
                v->offset   =   offset;

                return 0;
            }
        }
    }
}


void
collect_c_vec_free_storage(
    collect_c_vec_t*    v
)
{
    assert(NULL != v);
    assert(NULL != v->storage);

    {
        if (NULL != v->pfn_element_free)
        {
            for (size_t i = 0; v->size != i; ++i)
            {
                size_t const    ix  =   i + v->offset;
                void* const     pe  =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, ix);

                (*v->pfn_element_free)(v->el_size, ix, pe, v->param_element_free);
            }
        }

        if (0 == (COLLECT_C_VEC_F_USE_STACK_ARRAY & v->flags))
        {
            free(v->storage);

            v->storage = NULL;
        }

        v->capacity = 0;
        v->offset = 0;
        v->size = 0;
    }
}

int
collect_c_vec_clear(
    collect_c_vec_t*    v
,   void*               reserved0
,   void*               reserved1
,   size_t*             num_dropped
)
{
    ((void)&reserved0);
    ((void)&reserved1);

    assert(NULL != v);
    assert(NULL != v->storage);
    assert(NULL == reserved0);
    assert(NULL == reserved1);

    {
        size_t dummy;

        if (NULL == num_dropped)
        {
            num_dropped = &dummy;
        }

        *num_dropped = 0;

        if (NULL != v->pfn_element_free)
        {
            for (size_t i = 0; v->size != i; ++i)
            {
                size_t const    ix  =   i + v->offset;
                void* const     pe  =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, ix);

                (*v->pfn_element_free)(v->el_size, ix, pe, v->param_element_free);
            }
        }
        else
        {
            *num_dropped = v->size;
        }

        v->offset = 0;
        v->size = 0;

        return 0;
    }
}

int
collect_c_vec_shrink_to_fit(
    collect_c_vec_t*    v
)
{
    assert(NULL != v);
    assert(NULL != v->storage);

    {
        bool const uses_stack_array =   0 != (COLLECT_C_VEC_F_USE_STACK_ARRAY & v->flags);
        bool const uses_boo         =   0 == (COLLECT_C_VEC_F_NO_BOO & v->flags);

        if (uses_stack_array)
        {
            return ENODEV;
        }
        else
        {
            size_t const    cb_boo  =   uses_boo ? v->size / 4 : 0;
            size_t const    cb_els  =   v->size * v->el_size;

            size_t const    cb      =   cb_boo + cb_els;
            void* const     pv_new  =   realloc(v->storage, cb);

            if (NULL == pv_new)
            {
                return ENOMEM;
            }
            else
            {
                v->storage = pv_new;

                return 0;
            }
        }
    }
}

int
collect_c_v_push_back_by_ref(
    collect_c_vec_t*    v
,   void const*         ptr_new_el
)
{
    assert(NULL != v);
    assert(NULL != v->storage);

    {
        bool const uses_stack_array =   0 != (COLLECT_C_VEC_F_USE_STACK_ARRAY & v->flags);
        bool const uses_boo         =   0 == (COLLECT_C_VEC_F_NO_BOO & v->flags);

        assert(!uses_stack_array || 0 == v->offset);

        if (!COLLECT_C_VEC_spare_back(*v))
        {
            if (uses_stack_array)
            {
                return ENOSPC;
            }
            else
            {
                /* at this point, we have two possibilities to create space:
                 *
                 * (1) if we have front-spare, then we can shuffle all down; or
                 * (2) we need to realloc;
                 */

                if (0 != v->offset)
                {
                    size_t const    ix_src  =   0 + v->offset;
                    size_t const    ix_dst  =   0 + --v->offset;
                    void* const     pe_src  =   ((char*)v->storage) + (ix_src * v->el_size);
                    void* const     pe_dst  =   ((char*)v->storage) + (ix_dst * v->el_size);

                    memmove(pe_dst, pe_src, v->el_size * v->size);
                }
                else
                {
                    size_t const    cap_new =   (v->capacity * 3) / 2;
                    size_t const    off_new =   uses_boo ? cap_new / 4 : 0;
                    size_t const    cb_new  =   (cap_new + off_new) * v->el_size;
                    void* const     pv_new  =   realloc(v->storage, cb_new);

                    if (NULL == pv_new)
                    {
                        return ENOMEM;
                    }
                    else
                    {
                        v->capacity =   cb_new / v->el_size;
                        v->storage  =   pv_new;

                        if (0 != off_new)
                        {
                            size_t const    ix_src  =   v->offset;
                            size_t const    ix_dst  =   off_new;
                            void* const     pe_src  =   ((char*)v->storage) + (ix_src * v->el_size);
                            void* const     pe_dst  =   ((char*)v->storage) + (ix_dst * v->el_size);

                            memmove(pe_dst, pe_src, v->el_size * v->size);

                            v->offset = off_new;
                        }
                    }
                }
            }
        }

        {
            size_t const    ix  =   v->offset + v->size;
            void* const     pe  =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, ix);

            memcpy(pe, ptr_new_el, v->el_size);

            ++v->size;

            assert(v->offset + v->size <= v->capacity);

            return 0;
        }
    }
}

int
collect_c_v_push_front_by_ref(
    collect_c_vec_t*    v
,   void const*         ptr_new_el
)
{
    assert(NULL != v);
    assert(NULL != v->storage);

    {
        bool const uses_stack_array =   0 != (COLLECT_C_VEC_F_USE_STACK_ARRAY & v->flags);
        bool const uses_boo         =   0 == (COLLECT_C_VEC_F_NO_BOO & v->flags);

        assert(!uses_stack_array || 0 == v->offset);

        if (0 == v->offset)
        {
            if (v->size == v->capacity)
            {
                if (uses_stack_array)
                {
                    return ENOSPC;
                }
                else
                {
                    /* reallocate */

                    // TODO: recalculate this, as leaves nothing at end

                    size_t const    cap_new =   (v->capacity * 3) / 2;
                    size_t const    off_new =   uses_boo ? cap_new / 4 : 0;
                    size_t const    cb_new  =   (cap_new + off_new) * v->el_size;
                    void* const     pv_new  =   realloc(v->storage, cb_new);

                    if (NULL == pv_new)
                    {
                        return ENOMEM;
                    }
                    else
                    {
                        v->capacity =   cb_new / v->el_size;
                        v->offset   =   off_new;
                        v->storage  =   pv_new;

                        void* const     pe_src          =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, 0);
                        void* const     pe_dst          =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, off_new);

                        memmove(pe_dst, pe_src, v->size * v->el_size);

                        --v->offset;
                    }
                }
            }
            else
            {
                /* slide-up - but the question is by how much */

                size_t const    num_spare       =   v->capacity - v->size;
                size_t const    new_offset      =   num_spare < 4 ? 1 : num_spare / 4;

                void* const     pe_src          =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, 0);
                void* const     pe_dst          =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, new_offset);

                memmove(pe_dst, pe_src, v->size * v->el_size);

                v->offset = new_offset;

                --v->offset;
            }

#if 0

            else
            {
                /* at this point, we have two possibilities to create space:
                 *
                 * (1) if uses_boo AND we have front-spare, then we can shuffle all down; or
                 * (2) we need to realloc;
                 */

                 if (0 != v->offset)
                 {
                    size_t const    ix_src  =   0 + v->offset--;
                     size_t const    ix_dst  =   0 + v->offset;
                     void* const     pe_src   =   ((char*)v->storage) + (ix_src * v->el_size);
                     void* const     pe_dst   =   ((char*)v->storage) + (ix_dst * v->el_size);

                     memmove(pe_dst, pe_src, v->el_size * v->size);

                     --v->offset;
                 }
                 else
                 {
                    size_t const    cap_new =   (v->capacity * 3) / 2;
                     size_t const    off_new =   uses_boo ? cap_new / 4 : 0;
                     size_t const    cb_new  =   (cap_new + off_new) * v->el_size;
                     void* const     pv_new  =   realloc(v->storage, cb_new);

                     if (NULL == pv_new)
                     {
                        return ENOMEM;
                     }
                     else
                     {
                        v->capacity =   cb_new;
                        v->storage  =   pv_new;
                     }
                 }
             }
#endif
        }
        else
        {
            --v->offset;
        }

        {
            size_t const    ix  =   v->offset + 0;
            void* const     pe  =   COLLECT_C_VEC_INTERNAL_el_ptr_from_ix_(v, ix);

            memcpy(pe, ptr_new_el, v->el_size);

            ++v->size;

            return 0;
        }
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

