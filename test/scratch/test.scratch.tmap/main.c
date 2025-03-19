/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/test.scratch.tmap/main.c
 *
 * Purpose: Scratch-test for tree-map.
 *
 * Created: 19th March 2025
 * Updated: 19th March 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/terse/tmap.h>

#include <cstring/cstring.h>
#include <platformstl/filesystem/path_functions.h>
#include <stlsoft/stlsoft.h>

#include <stdio.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * helper functions
 */

void free_element(
    size_t      key_size
,   size_t      val_size
,   intptr_t    el_index    /* always -1 */
,   void*       p_el_key    /* may be NULL */
,   void*       p_el_val
,   void*       param_element_free
)
{
    int* const          el_key  =   p_el_key;
    cstring_t* const    el_val  =   p_el_val;

    ((void)&key_size);
    ((void)&val_size);
    ((void)&el_index);
    ((void)&param_element_free);

    ((void)&el_key);

    cstring_destroy(el_val);
}

int element_walk_function(
    size_t      key_size
,   size_t      val_size
,   intptr_t    el_index    /* always -1 */
,   size_t      depth
,   void const* p_el_key
,   void*       p_el_val
,   void*       param_walk
)
{
    int const* const        el_key  =   p_el_key;
    cstring_t const* const  el_val  =   p_el_val;
    size_t* const           p_max_d =   param_walk;

    ((void)&key_size);
    ((void)&val_size);
    ((void)&el_index);

    if (*p_max_d < depth)
    {
        *p_max_d = depth;
    }

    printf("\t[d: %zu], %d => %.*s\n"
    ,   depth
    ,   *el_key
    ,   (int)el_val->len
    ,   el_val->ptr
    );

    return 1;
}


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    stlsoft_C_string_slice_m_t const program_name = platformstl_C_get_executable_name_from_path(argv[0]);

    CLC_TM_define_empty(int, cstring_t, m);

    collect_c_tmap_walkdir_t    direction   =   COLLECT_C_TMAP_WALK_DEFAULT;

    m.pfn_element_free = free_element;

    if (1 == argc)
    {
        fprintf(stderr, "%.*s: no values; use --help for usage\n"
        ,   (int)program_name.len, program_name.ptr
        );

        goto free_storage;
    }

    for (int i = 1; argc != i; ++i)
    {
        char const* const arg = argv[i];

        if (0 == strcmp("--help", arg))
        {
            fprintf(stdout, "USAGE: %.*s [ { --forward | --backward | --downward } ] <value-1> [ ... <value-2> ]\n"
            ,   (int)program_name.len, program_name.ptr
            );

            goto free_storage;
        }
        else if (0 == strcmp("--forward", arg))
        {
            direction = COLLECT_C_TMAP_WALK_FORWARD;
        }
        else if (0 == strcmp("--backward", arg))
        {
            direction = COLLECT_C_TMAP_WALK_BACKWARD;
        }
        else if (0 == strcmp("--downward", arg))
        {
            direction = COLLECT_C_TMAP_WALK_DOWNWARD;
        }
        else
        {
            char*       endptr;
            int const   value = strtod(arg, &endptr);

            if (0 == value &&
                '\0' != *endptr)
            {
                fprintf(stderr, "%.*s: argument-%d '%s' could not be converted into an integer\n"
                ,   (int)program_name.len, program_name.ptr
                ,   argc
                ,   arg
                );

                goto free_storage;
            }
            else
            {
                cstring_t cs;

                cstring_init(&cs);

                CSTRING_RC const rcs = cstring_create(&cs, arg);

                if (CSTRING_RC_SUCCESS != rcs)
                {
                    fprintf(stderr, "%.*s: could not prepare value to insert: %.*s\n"
                    ,   (int)program_name.len, program_name.ptr
                    ,   (int)cstring_getStatusCodeStringLength(rcs)
                    ,   cstring_getStatusCodeString(rcs)
                    );

                    goto free_storage;
                }
                else
                {
                    int         was_replaced;
                    int const   rm = collect_c_tmap_insert(&m, &value, &cs, NULL, &was_replaced);

                    if (0 != rm)
                    {
                        fprintf(stderr, "%.*s: could not prepare value to insert: %s\n"
                        ,   (int)program_name.len, program_name.ptr
                        ,   strerror(rm)
                        );

                        cstring_destroy(&cs);

                        goto free_storage;
                    }
                }

            }
        }
    }

    printf("map contains %zu inserted element(s)\n"
    ,   CLC_TM_len(m)
    );

    size_t max_depth = 0;

    collect_c_tmap_walk(
        &m
    ,   element_walk_function
    ,   &max_depth
    ,   direction
    );

    printf("maximum depth: %zu\n"
    ,   max_depth
    );


free_storage:
    clc_treemap_free_storage(&m);

    return 0;
}


/* ///////////////////////////// end of file //////////////////////////// */

