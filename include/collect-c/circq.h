
#include <collect-c/common.h>

#include <stddef.h>
#include <stdint.h>


struct clc_circq_t
{
    size_t  el_size;
    size_t  capacity;
    size_t  b;
    size_t  e;
    void*   storage;
};
#ifndef __cplusplus
typedef struct clc_circq_t clc_circq_t;
#endif



#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

