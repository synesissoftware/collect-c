/* /////////////////////////////////////////////////////////////////////////
 * File:    src/tmap.c
 *
 * Purpose: Tree-map container.
 *
 * Created: 14th February 2025
 * Updated: 19th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/tmap.h>

#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

    #include <stdio.h>


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#define CLC_TMAP_INTERNAL_MAX_SPARES_                       64


/* /////////////////////////////////////////////////////////////////////////
 * local types
 */

typedef collect_c_tmap_node_t                               node_t;


/* /////////////////////////////////////////////////////////////////////////
 * helper functions and macros
 */

#define CLC_TMAP_node_key_ptr_(n)                           (&(n)->data->data[0])
// #define CLC_TMAP_node_val_ptr_(n)                           ((n)->value)
#define CLC_TMAP_node_val_ptr_(n, key_size)                 ((void*)(CLC_TMAP_node_key_ptr_(n) + (CLC_TMAP_INTERNAL_NUM_nd_FOR_key_(key_size) * sizeof(collect_c_common_node_data_t))))

#define CLC_TMAP_INTERNAL_NUM_nd_FOR_key_(key_size)         (((key_size) + (sizeof(sizeof(collect_c_common_node_data_t)) - 1)) / sizeof(collect_c_common_node_data_t))
#define CLC_TMAP_INTERNAL_NUM_nd_FOR_val_(val_size)         (((val_size) + (sizeof(sizeof(collect_c_common_node_data_t)) - 1)) / sizeof(collect_c_common_node_data_t))

#define CLC_TMAP_INTERNAL_sizeof_node_(key_size, val_size)  (offsetof(node_t, data) + (CLC_TMAP_INTERNAL_NUM_nd_FOR_key_((key_size)) * sizeof(collect_c_common_node_data_t)) + val_size)

static
node_t*
clc_c_tm_alloc_node_(
    collect_c_mem_api_t*    mem_api
,   size_t                  key_size
,   size_t                  val_size
)
{
    size_t const    cb  =   CLC_TMAP_INTERNAL_sizeof_node_(key_size, val_size);
    node_t* const   nd  =   (*mem_api->pfn_alloc)(mem_api->param, cb);

    // _Static_assert(8 == sizeof(collect_c_common_node_data_t), "VIOLATION: node data unexpected size");

    return nd;
}

static
void
clc_c_tm_free_node_(
    collect_c_mem_api_t*    mem_api
,   node_t*                 node
,   size_t                  key_size
,   size_t                  val_size
)
{
    size_t const cb = CLC_TMAP_INTERNAL_sizeof_node_(key_size, val_size);

    (*mem_api->pfn_free)(mem_api->param, node, cb);
}


static
void
clc_c_tm_destroy_child_nodes_4_(
    collect_c_mem_api_t*        mem_api
,   node_t*                     n
,   size_t                      key_size
,   size_t                      val_size
)
{
    assert(NULL != n);

    if (NULL != n->left)
    {
        node_t* const child = n->left;

        clc_c_tm_destroy_child_nodes_4_(mem_api, child, key_size, val_size);

        clc_c_tm_free_node_(mem_api, child, key_size, val_size);

        n->left = NULL;
    }

    if (NULL != n->right)
    {
        node_t* const child = n->right;

        clc_c_tm_destroy_child_nodes_4_(mem_api, child, key_size, val_size);

        clc_c_tm_free_node_(mem_api, child, key_size, val_size);

        n->right = NULL;
    }
}

static
void
clc_c_tm_destroy_child_nodes_6_(
    collect_c_mem_api_t*        mem_api
,   node_t*                     n
,   collect_c_tmap_pfn_free     pfn_element_free
,   void*                       param_element_free
,   size_t                      key_size
,   size_t                      val_size
)
{
    assert(NULL != n);
    assert(NULL != pfn_element_free);

    if (NULL != n->left)
    {
        node_t* const   child   =   n->left;
        void* const     p_key   =   CLC_TMAP_node_key_ptr_(child);
        void* const     p_val   =   CLC_TMAP_node_val_ptr_(child, key_size);

        clc_c_tm_destroy_child_nodes_6_(mem_api, child, pfn_element_free, param_element_free, key_size, val_size);

        (*pfn_element_free)(key_size, val_size, -1, p_key, p_val, param_element_free);

        clc_c_tm_free_node_(mem_api, child, key_size, val_size);

        n->left = NULL;
    }

    if (NULL != n->right)
    {
        node_t* const   child   =   n->right;
        void* const     p_key   =   CLC_TMAP_node_key_ptr_(child);
        void* const     p_val   =   CLC_TMAP_node_val_ptr_(child, key_size);

        clc_c_tm_destroy_child_nodes_6_(mem_api, child, pfn_element_free, param_element_free, key_size, val_size);

        (*pfn_element_free)(key_size, val_size, -1, p_key, p_val, param_element_free);

        clc_c_tm_free_node_(mem_api, child, key_size, val_size);

        n->right = NULL;
    }
}

static
void
add_node_to_(
    collect_c_tmap_t const* m
,   node_t*                 node
,   node_t*                 new_node
)
{
    int const cr = (*m->pfn_key_cmp)(CLC_TMAP_node_key_ptr_(new_node), CLC_TMAP_node_key_ptr_(node), m->key_size);

    assert(0 != cr);

    if (cr < 0)
    {
        if (NULL == node->left)
        {
            node->left = new_node;
        }
        else
        {
            add_node_to_(m, node->left, new_node);
        }
    }
    else
    {
        if (NULL == node->right)
        {
            node->right = new_node;
        }
        else
        {
            add_node_to_(m, node->right, new_node);
        }
    }
}

static
node_t const*
clc_c_tm_find_(
    collect_c_tmap_t const* m
,   void const*             p_key
,   node_t**                last_visited
,   size_t*                 num_visited
)
{
    assert(NULL != m);
    assert(NULL != p_key);
    assert(NULL != last_visited);

    {
        size_t dummy;

        if (NULL == num_visited)
        {
            num_visited = &dummy;
        }

        *num_visited = 0;

        *last_visited = NULL;

        /* start at root */
        for (collect_c_tmap_node_t* n = m->root; NULL != n; )
        {
            int const cr = (*m->pfn_key_cmp)(p_key, CLC_TMAP_node_key_ptr_(n), m->key_size);

            *last_visited = n;

            if (cr < 0)
            {
                n = n->left;

                continue;
            }

            if (cr > 0)
            {
                n = n->right;

                continue;
            }

            return n;
        }

        return NULL;
    }
}

bool
node_walk_backward_(
    collect_c_tmap_t*           m
,   node_t const*               node
,   collect_c_tmap_pfn_walk     pfn_walk
,   void*                       param_walk
,   size_t                      depth
)
{
    assert(NULL != m);
    assert(NULL != node);
    assert(NULL != pfn_walk);

    {
        void const* const   p_key   =   CLC_TMAP_node_key_ptr_(node);
        void* const         p_val   =   CLC_TMAP_node_val_ptr_(node, m->key_size);

        if (NULL != node->right)
        {
            if (!node_walk_backward_(m, node->right, pfn_walk, param_walk, depth + 1))
            {
                return false;
            }
        }

        if (0 == (*pfn_walk)(m->key_size, m->val_size, -1, depth, p_key, p_val, param_walk))
        {
            return false;
        }

        if (NULL != node->left)
        {
            if (!node_walk_backward_(m, node->left, pfn_walk, param_walk, depth + 1))
            {
                return false;
            }
        }

        return true;
    }
}

bool
node_walk_downward_(
    collect_c_tmap_t*           m
,   node_t const*               node
,   collect_c_tmap_pfn_walk     pfn_walk
,   void*                       param_walk
,   size_t                      depth
)
{
    assert(NULL != m);
    assert(NULL != node);
    assert(NULL != pfn_walk);

    {
        void const* const   p_key   =   CLC_TMAP_node_key_ptr_(node);
        void* const         p_val   =   CLC_TMAP_node_val_ptr_(node, m->key_size);

        if (0 == (*pfn_walk)(m->key_size, m->val_size, -1, depth, p_key, p_val, param_walk))
        {
            return false;
        }

        if (NULL != node->right)
        {
            if (!node_walk_downward_(m, node->right, pfn_walk, param_walk, depth + 1))
            {
                return false;
            }
        }

        if (NULL != node->left)
        {
            if (!node_walk_downward_(m, node->left, pfn_walk, param_walk, depth + 1))
            {
                return false;
            }
        }

        return true;
    }
}

bool
node_walk_forward_(
    collect_c_tmap_t*           m
,   node_t const*               node
,   collect_c_tmap_pfn_walk     pfn_walk
,   void*                       param_walk
,   size_t                      depth
)
{
    assert(NULL != m);
    assert(NULL != node);
    assert(NULL != pfn_walk);

    {
        void const* const   p_key   =   CLC_TMAP_node_key_ptr_(node);
        void* const         p_val   =   CLC_TMAP_node_val_ptr_(node, m->key_size);

        if (NULL != node->left)
        {
            if (!node_walk_forward_(m, node->left, pfn_walk, param_walk, depth + 1))
            {
                return false;
            }
        }

        if (0 == (*pfn_walk)(m->key_size, m->val_size, -1, depth, p_key, p_val, param_walk))
        {
            return false;
        }

        if (NULL != node->right)
        {
            if (!node_walk_forward_(m, node->right, pfn_walk, param_walk, depth + 1))
            {
                return false;
            }
        }

        return true;
    }
}


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

uint32_t
collect_c_tmap_version(void)
{
    return COLLECT_C_TMAP_VER;
}

void
clc_treemap_free_storage(
    collect_c_tmap_t*   m
)
{
    if (NULL != m->pfn_element_free)
    {
        clc_c_tm_destroy_child_nodes_6_(&m->mem_api, m->root, m->pfn_element_free, m->param_element_free, m->key_size, m->val_size);
    }
    else
    {
        clc_c_tm_destroy_child_nodes_4_(&m->mem_api, m->root, m->key_size, m->val_size);
    }

    if (NULL != m->root)
    {
        if (NULL != m->pfn_element_free)
        {
            void* const p_key   =   CLC_TMAP_node_key_ptr_(m->root);
            void* const p_val   =   CLC_TMAP_node_val_ptr_(m->root, m->key_size);

            (*m->pfn_element_free)(m->key_size, m->val_size, -1, p_key, p_val, m->param_element_free);
        }

        clc_c_tm_free_node_(&m->mem_api, m->root, m->key_size, m->val_size);
    }

    m->size = 0;
    m->root = NULL;
}

collect_c_tmap_node_t const*
collect_c_tmap_find_node(
    collect_c_tmap_t const* m
,   void const*             ptr_lhs_key
)
{
    ((void)&m);
    ((void)&ptr_lhs_key);

    {
        collect_c_tmap_node_t* last_visited;

        return clc_c_tm_find_(m, ptr_lhs_key, &last_visited, NULL);
    }
}

int
collect_c_tmap_insert(
    collect_c_tmap_t*           m
,   void const*                 ptr_new_key
,   void const*                 ptr_new_val
,   collect_c_tmap_node_t**     new_node
,   int*                        p_was_replaced
)
{
    ((void)&m);
    ((void)&ptr_new_key);
    ((void)&ptr_new_val);
    ((void)&new_node);
    ((void)&p_was_replaced);

    {
        int dummy;

        if (NULL == p_was_replaced)
        {
            p_was_replaced = &dummy;
        }

        if (0 == m->size)
        {
            node_t* const new_root = clc_c_tm_alloc_node_(&m->mem_api, m->key_size, m->val_size);

            if (NULL == new_root)
            {
                return ENOMEM;
            }
            else
            {
                *p_was_replaced = 0;

                memcpy(CLC_TMAP_node_key_ptr_(new_root), ptr_new_key, m->key_size);
                memcpy(CLC_TMAP_node_val_ptr_(new_root, m->key_size), ptr_new_val, m->val_size);

                m->root = new_root;

                m->size = 1;

                return 0;
            }
        }
        else
        {
            node_t*             last_visited;
            node_t const* const n_existing = clc_c_tm_find_(m, ptr_new_key, &last_visited, NULL);

            if (NULL != n_existing)
            {
                *p_was_replaced = 1;

                if (NULL != m->pfn_element_free)
                {
                    (*m->pfn_element_free)(m->key_size, m->val_size, -1, NULL, CLC_TMAP_node_val_ptr_(n_existing, m->key_size), m->param_element_free);
                }

                memcpy(CLC_TMAP_node_val_ptr_(n_existing, m->key_size), ptr_new_val, m->val_size);
            }
            else
            {
                *p_was_replaced = 0;

                assert(NULL != last_visited);

                /* NOTE: might be a way to have `clc_c_tm_find_()` return whether should go left or right */

                {
                    node_t* const new_node = clc_c_tm_alloc_node_(&m->mem_api, m->key_size, m->val_size);

                    if (NULL == new_node)
                    {
                        return ENOMEM;
                    }
                    else
                    {
                        memcpy(CLC_TMAP_node_key_ptr_(new_node), ptr_new_key, m->key_size);
                        memcpy(CLC_TMAP_node_val_ptr_(new_node, m->key_size), ptr_new_val, m->val_size);

                        add_node_to_(m, m->root, new_node);

                        ++m->size;

                        return 0;
                    }
                }

                ;
            }

            return 0;
        }
    }
}

int
collect_c_tmap_walk(
    collect_c_tmap_t*           m
,   collect_c_tmap_pfn_walk     pfn_walk
,   void*                       param_walk
,   collect_c_tmap_walkdir_t    direction
)
{
    assert(NULL != m);
    assert(NULL != pfn_walk);

    {
        switch (direction)
        {
        case COLLECT_C_TMAP_WALK_FORWARD:

            node_walk_forward_(m, m->root, pfn_walk, param_walk, 0);
            break;
        case COLLECT_C_TMAP_WALK_BACKWARD:

            node_walk_backward_(m, m->root, pfn_walk, param_walk, 0);
            break;

        default:
            assert(!"invalid walk direction");

            /* fallthrough */
        case COLLECT_C_TMAP_WALK_DEFAULT:
        case COLLECT_C_TMAP_WALK_DOWNWARD:

            node_walk_downward_(m, m->root, pfn_walk, param_walk, 0);
            break;
        }

        return 0;
    }
}



/* ///////////////////////////// end of file //////////////////////////// */

