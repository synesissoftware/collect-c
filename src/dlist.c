/* /////////////////////////////////////////////////////////////////////////
 * File:    src/dlist.c
 *
 * Purpose: Doubly-linked list container.
 *
 * Created: 7th February 2025
 * Updated: 15th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/dlist.h>

#include <errno.h>
#include <assert.h>
#include <stddef.h>


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#define COLLECT_C_DLIST_INTERNAL_MAX_SPARES_                64


/* /////////////////////////////////////////////////////////////////////////
 * local types
 */

typedef collect_c_dlist_node_t                              node_t;


/* /////////////////////////////////////////////////////////////////////////
 * helper functions and macros
 */

#define COLLECT_C_DLIST_INTERNAL_sizeof_node_(el_size)      (offsetof(collect_c_dlist_node_t, data) + el_size)

static
node_t*
clc_c_dl_alloc_node_(
    size_t      el_size
)
{
    size_t const    cb  =   COLLECT_C_DLIST_INTERNAL_sizeof_node_(el_size);
    node_t* const   nd  =   malloc(cb);

    return nd;
}

static
node_t*
clc_c_dl_obtain_node_(
    collect_c_dlist_t*  l
,   node_t*             prev
,   node_t*             next
,   size_t              el_size
,   void const*         ptr_new_el
)
{
    node_t* nd;

    if (0 != l->num_spares)
    {
        nd = l->spares;

        l->spares = nd->next;

        --l->num_spares;
    }
    else
    {
        nd = clc_c_dl_alloc_node_(el_size);

        if (NULL == nd)
        {
            return NULL;
        }
    }

    nd->prev    =   prev;
    nd->next    =   next;

    memcpy(&nd->data->data[0], ptr_new_el, el_size);

    return nd;
}


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

uint32_t
collect_c_dlist_version(void)
{
    return COLLECT_C_DLIST_VER;
}

int
clc_dlist_free_storage(
    collect_c_dlist_t*  l
)
{
    assert(NULL != l);

    {
        for (collect_c_dlist_node_t* n = l->head; NULL != n; )
        {
            collect_c_dlist_node_t* const n2 = n;

            n = n->next;

            free(n2);
        }

        l->head = l->tail = NULL;
        l->size = 0;

        for (collect_c_dlist_node_t* n = l->spares; NULL != n; )
        {
            collect_c_dlist_node_t* const n2 = n;

            n = n->next;

            free(n2);

            --l->num_spares;
        }

        l->spares = NULL;

        return 0;
    }
}

int
collect_c_dlist_clear(
    collect_c_dlist_t*  l
,   void*               reserved0
,   void*               reserved1
,   size_t*             num_dropped
)
{
    ((void)&reserved0);
    ((void)&reserved1);

    assert(NULL != l);
    assert(NULL == reserved0);
    assert(NULL == reserved1);

    {
        size_t dummy;

        if (NULL == num_dropped)
        {
            num_dropped = &dummy;
        }

        *num_dropped = 0;

        for (collect_c_dlist_node_t* n = l->head; NULL != n; )
        {
            collect_c_dlist_node_t* const n2 = n;

            n = n->next;

            if (0 == (COLLECT_C_DLIST_F_NO_SPARES & l->flags) &&
                l->num_spares < COLLECT_C_DLIST_INTERNAL_MAX_SPARES_)
            {
                n2->next = n2->prev = l->spares;

                l->spares = n2;

                ++l->num_spares;
            }
            else
            {
                free(n2);
            }

            ++*num_dropped;
        }

        l->head = l->tail = NULL;
        l->size = 0;

        return 0;
    }
}

int
collect_c_dlist_erase_node(
    collect_c_dlist_t*      l
,   collect_c_dlist_node_t* node
)
{
    assert(NULL != l);
    assert(NULL != node);

    {
        /* simple set of actions:
         *
         * 1. "destruct" element;
         * 2. unhook;
         * 3. destroy (or make spare);
         */

        if (NULL != l->pfn_element_free)
        {
            (*l->pfn_element_free)(l->el_size, -1, &node->data->data[0], l->param_element_free);
        }

        {
            assert(NULL != node->prev || l->head == node);
            assert(NULL != node->next || l->tail == node);

            /* four aspects:
             *
             * 1. if node is head, then replace head with node->next; else
             * 2.
             *
             * 3.
             * 4.
             */

            if (NULL != node->prev)
            {
                node->prev->next = node->next;
            }
            else
            {
                l->head = node->next;
            }

            if (NULL != node->next)
            {
                node->next->prev = node->prev;
            }
            else
            {
                l->tail = node->prev;
            }

            --l->size;

            if (0 == (COLLECT_C_DLIST_F_NO_SPARES & l->flags) &&
                l->num_spares < COLLECT_C_DLIST_INTERNAL_MAX_SPARES_)
            {
                node->next = node->prev = l->spares;

                l->spares = node;

                ++l->num_spares;
            }
            else
            {
                free(node);
            }
        }

        return 0;
    }
}

int
collect_c_dlist_find_node(
    collect_c_dlist_t const*        l
,   collect_c_dlist_pfn_compare_t   pfn
,   void const*                     p_lhs
,   size_t                          skip_count
,   collect_c_dlist_node_t**        node
,   size_t*                         num_searched
)
{
    assert(NULL != l);
    assert(NULL != pfn);
    assert(NULL != p_lhs);
    assert(NULL != node);

    *node = NULL;

    {
        size_t dummy;

        if (NULL == num_searched)
        {
            num_searched = &dummy;
        }

        *num_searched = 0;

        for (collect_c_dlist_node_t* n = l->head; NULL != n; n = n->next)
        {
            int const r = (*pfn)(l, p_lhs, &n->data->data[0]);

            ++*num_searched;

            if (0 == r)
            {
                if (0 == skip_count--)
                {
                    *node = n;

                    return 0;
                }
            }
        }

        return ENOENT;
    }
}

int
collect_c_dlist_rfind_node(
    collect_c_dlist_t const*        l
,   collect_c_dlist_pfn_compare_t   pfn
,   void const*                     p_lhs
,   size_t                          skip_count
,   collect_c_dlist_node_t**        node
,   size_t*                         num_searched
)
{
    assert(NULL != l);
    assert(NULL != pfn);
    assert(NULL != p_lhs);
    assert(NULL != node);

    *node = NULL;

    {
        size_t dummy;

        if (NULL == num_searched)
        {
            num_searched = &dummy;
        }

        *num_searched = 0;

        for (collect_c_dlist_node_t* n = l->tail; NULL != n; n = n->prev)
        {
            int const r = (*pfn)(l, p_lhs, &n->data->data[0]);

            ++*num_searched;

            if (0 == r)
            {
                if (0 == skip_count--)
                {
                    *node = n;

                    return 0;
                }
            }
        }

        return ENOENT;
    }
}

int
collect_c_dlist_insert_after(
    collect_c_dlist_t*          l
,   collect_c_dlist_node_t*     reference_node
,   void const*                 ptr_new_el
,   collect_c_dlist_node_t**    new_node
)
{
    assert(NULL != l);
    assert(NULL != reference_node);
    assert(NULL != ptr_new_el);

    {
        collect_c_dlist_node_t* dummy;

        if (NULL == new_node)
        {
            new_node = &dummy;
        }

        *new_node = clc_c_dl_obtain_node_(l, reference_node, reference_node->next, l->el_size, ptr_new_el);

        if (NULL == *new_node)
        {
            return ENOMEM;
        }
        else
        {
            if (NULL != reference_node->next)
            {
                reference_node->next->prev = *new_node;
            }
            else
            {
                l->tail = *new_node;
            }

            reference_node->next = *new_node;

            ++l->size;

            return 0;
        }
    }
}


int
collect_c_dlist_insert_before(
    collect_c_dlist_t*          l
,   collect_c_dlist_node_t*     reference_node
,   void const*                 ptr_new_el
,   collect_c_dlist_node_t**    new_node
)
{
    assert(NULL != l);
    assert(NULL != reference_node);
    assert(NULL != ptr_new_el);

    {
        collect_c_dlist_node_t* dummy;

        if (NULL == new_node)
        {
            new_node = &dummy;
        }

        *new_node = clc_c_dl_obtain_node_(l, reference_node->prev, reference_node, l->el_size, ptr_new_el);

        if (NULL == *new_node)
        {
            return ENOMEM;
        }
        else
        {
            if (NULL != reference_node->prev)
            {
                reference_node->prev->next = *new_node;
            }
            else
            {
                l->head = *new_node;
            }

            reference_node->prev = *new_node;

            ++l->size;

            return 0;
        }
    }
}

int
collect_c_dlist_push_back_by_ref(
    collect_c_dlist_t*  l
,   void const*         ptr_new_el
)
{
    assert(NULL != l);
    assert(NULL != ptr_new_el);

    assert((NULL == l->head) == (NULL == l->tail));

    {
        if (NULL == l->head)
        {
            collect_c_dlist_node_t* const nd = clc_c_dl_obtain_node_(l, NULL, NULL, l->el_size, ptr_new_el);

            if (NULL == nd)
            {
                return ENOMEM;
            }

            l->head = l->tail = nd;

            assert(0 == l->size);

            l->size = 1;
        }
        else
        {
            collect_c_dlist_node_t* const   prev    =   l->tail;
            collect_c_dlist_node_t* const   next    =   NULL;
            collect_c_dlist_node_t* const   nd      =   clc_c_dl_obtain_node_(l, prev, next, l->el_size, ptr_new_el);

            if (NULL == nd)
            {
                return ENOMEM;
            }

            l->tail->next = nd;
            l->tail = nd;

            ++l->size;
        }

        return 0;
    }
}

int
collect_c_dlist_push_front_by_ref(
    collect_c_dlist_t*  l
,   void const*         ptr_new_el
)
{
    assert(NULL != l);
    assert(NULL != ptr_new_el);

    assert((NULL == l->head) == (NULL == l->tail));

    {
        if (NULL == l->head)
        {
            collect_c_dlist_node_t* const nd = clc_c_dl_obtain_node_(l, NULL, NULL, l->el_size, ptr_new_el);

            if (NULL == nd)
            {
                return ENOMEM;
            }

            l->head = l->tail = nd;

            assert(0 == l->size);

            l->size = 1;
        }
        else
        {
            collect_c_dlist_node_t* const   prev    =   NULL;
            collect_c_dlist_node_t* const   next    =   l->head;
            collect_c_dlist_node_t* const   nd      =   clc_c_dl_obtain_node_(l, prev, next, l->el_size, ptr_new_el);

            if (NULL == nd)
            {
                return ENOMEM;
            }

            l->head->prev = nd;
            l->head = nd;

            ++l->size;
        }

        return 0;
    }
}


/* ///////////////////////////// end of file //////////////////////////// */

