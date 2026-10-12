/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/versions/main.c
 *
 * Purpose: Prints collect-c version strings via Diagnosticism.
 *
 * Created: 17th September 2026
 * Updated: 14th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <collect-c/circq.h>
#include <collect-c/common/version.h>
#include <collect-c/dlist.h>
#include <collect-c/htab.h>
#include <collect-c/tmap.h>
#include <collect-c/vec.h>

#include <diagnosticism/version_string.h>

#include <stdio.h>
#include <stdlib.h>


#define PROGRAM_NAME                                        "test.scratch.versions"


static int
version(
    FILE*       stm
,   char const* prefix
,   char const* libname
,   char const* macroname
,   unsigned    libver
)
{
    char        buffer[64];
    size_t      n;
    int const   r = diagnosticism_calc_version_string(
        buffer
    ,   sizeof(buffer) / sizeof(buffer[0])
    ,   (int)((libver >> 24) & 0xff)
    ,   (int)((libver >> 16) & 0xff)
    ,   (int)((libver >> 8) & 0xff)
    ,   (int)((libver >> 0) & 0xff)
    ,   &n
    );

    if (0 != r)
    {
        fprintf(stderr, "%s: failed to format version string for %s\n", PROGRAM_NAME, libname);

        return EXIT_FAILURE;
    }

    buffer[n] = '\0';

    fprintf(
        stm
    ,   "%s%s: %s (%s = 0x%08x)\n"
    ,   prefix
    ,   libname
    ,   buffer
    ,   macroname
    ,   libver
    );

    return EXIT_SUCCESS;
}


int main(int argc, char* argv[])
{
    ((void)&argc);
    ((void)&argv);

    if (EXIT_SUCCESS != version(
            stdout
        ,   ""
        ,   "collect-c"
        ,   "COLLECT_C_VER"
        ,   (unsigned)COLLECT_C_VER
        )
    )
    {
        return EXIT_FAILURE;
    }

    fputs("\ncomponents:\n", stdout);

    if (EXIT_SUCCESS != version(
            stdout
        ,   "\t"
        ,   "circq"
        ,   "COLLECT_C_CIRCQ_VER"
        ,   (unsigned)COLLECT_C_CIRCQ_VER
        )
    )
    {
        return EXIT_FAILURE;
    }

    if (EXIT_SUCCESS != version(
            stdout
        ,   "\t"
        ,   "dlist"
        ,   "COLLECT_C_DLIST_VER"
        ,   (unsigned)COLLECT_C_DLIST_VER
        )
    )
    {
        return EXIT_FAILURE;
    }

    if (EXIT_SUCCESS != version(
            stdout
        ,   "\t"
        ,   "htab"
        ,   "COLLECT_C_HTAB_VER"
        ,   (unsigned)COLLECT_C_HTAB_VER
        )
    )
    {
        return EXIT_FAILURE;
    }

    if (EXIT_SUCCESS != version(
            stdout
        ,   "\t"
        ,   "tmap"
        ,   "COLLECT_C_TMAP_VER"
        ,   (unsigned)COLLECT_C_TMAP_VER
        )
    )
    {
        return EXIT_FAILURE;
    }

    if (EXIT_SUCCESS != version(
            stdout
        ,   "\t"
        ,   "vec"
        ,   "COLLECT_C_VEC_VER"
        ,   (unsigned)COLLECT_C_VEC_VER
        )
    )
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
