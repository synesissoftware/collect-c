/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c//terse/circq.h
 *
 * Purpose: Circular-queue container terse api.
 *
 * Created: 5th February 2025
 * Updated: 5th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/circq.h>


/* /////////////////////////////////////////////////////////////////////////
 * terse-form macros
 */

#define CLC_CQ_F_USE_STACK_ARRAY                            COLLECT_C_CIRCQ_F_USE_STACK_ARRAY

#define CLC_CQ_DEFINE_EMPTY                                 COLLECT_C_CIRCQ_DEFINE_EMPTY
#define CLC_CQ_DEFINE_ON_STACK                              COLLECT_C_CIRCQ_DEFINE_ON_STACK
#define CLC_CQ_DEFINE_ON_STACK_WITH_CB                      COLLECT_C_CIRCQ_DEFINE_ON_STACK_WITH_CALLBACK

#define CLC_CQ_is_empty                                     COLLECT_C_CIRCQ_is_empty
#define CLC_CQ_len                                          COLLECT_C_CIRCQ_len
#define CLC_CQ_spare                                        COLLECT_C_CIRCQ_spare
#define CLC_CQ_at                                           COLLECT_C_CIRCQ_at
#define CLC_CQ_element_index                                COLLECT_C_CIRCQ_element_index
#define CLC_CQ_add_by_ref                                   COLLECT_C_CIRCQ_add_by_ref
#define CLC_CQ_add_by_value                                 COLLECT_C_CIRCQ_add_by_value
#define CLC_CQ_pop_back                                     COLLECT_C_CIRCQ_pop_back
#define CLC_CQ_pop_front                                    COLLECT_C_CIRCQ_pop_front
#define CLC_CQ_clear                                        COLLECT_C_CIRCQ_clear


#define clc_cq_allocate_storage                             collect_c_cq_allocate_storage
#define clc_cq_free_storage                                 collect_c_cq_free_storage
#define clc_cq_add_by_ref                                   collect_c_cq_add_by_ref
#define clc_cq_clear                                        collect_c_cq_clear
#define clc_cq_pop_from_back_n                              collect_c_cq_pop_from_back_n
#define clc_cq_pop_from_front_n                             collect_c_cq_pop_from_front_n



/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#pragma once


/* ///////////////////////////// end of file //////////////////////////// */

