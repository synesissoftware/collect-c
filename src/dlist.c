/* /////////////////////////////////////////////////////////////////////////
 * File:    src/dlist.c
 *
 * Purpose: Doubly-linked list container.
 *
 * Created: 7th February 2025
 * Updated: 7th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/dlist.h>

#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


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
clc_c_dl_make_node_(
    node_t*     prev
,   node_t*     next
,   size_t      el_size
,   void const* ptr_new_el
)
{
    size_t const    cb  =   COLLECT_C_DLIST_INTERNAL_sizeof_node_(el_size);
    node_t* const   nd  =   malloc(cb);

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

    for (collect_c_dlist_node_t* n = l->head; NULL != n; )
    {
        collect_c_dlist_node_t* const n2 = n;

        n = n->next;

        free(n2);
    }

    l->head = l->tail = NULL;
    l->size = 0;

    return 0;
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
            collect_c_dlist_node_t* const nd = clc_c_dl_make_node_(NULL, NULL, l->el_size, ptr_new_el);

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
            collect_c_dlist_node_t* const   nd      =   clc_c_dl_make_node_(prev, next, l->el_size, ptr_new_el);

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


/* ///////////////////////////// end of file //////////////////////////// */

