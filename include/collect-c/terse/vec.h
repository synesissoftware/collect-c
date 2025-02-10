/* /////////////////////////////////////////////////////////////////////////
 * File:    collect-c/terse/vec.h
 *
 * Purpose: Vector container terse api.
 *
 * Created: 5th February 2025
 * Updated: 10th February 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <collect-c/vec.h>


/* /////////////////////////////////////////////////////////////////////////
 * terse-form macros
 */

#define CLC_V_F_USE_STACK_ARRAY                             COLLECT_C_VEC_F_USE_STACK_ARRAY
#define CLC_V_F_NO_BOO                                      COLLECT_C_VEC_F_NO_BOO


#define CLC_V_define_empty                                  COLLECT_C_VEC_define_empty
#define CLC_V_define_empty_with_cb                          COLLECT_C_VEC_define_empty_with_callback
#define CLC_V_define_on_stack                               COLLECT_C_VEC_define_on_stack

#define CLC_V_push_back_by_ref                              COLLECT_C_VEC_push_back_by_ref
#define CLC_V_push_back_by_value                            COLLECT_C_VEC_push_back_by_value

#define CLC_V_push_front_by_ref                             COLLECT_C_VEC_push_front_by_ref
#define CLC_V_push_front_by_value                           COLLECT_C_VEC_push_front_by_value

#define CLC_V_shrink_to_fit                                 COLLECT_C_VEC_shrink_to_fit

#define CLC_V_is_empty                                      COLLECT_C_VEC_is_empty
#define CLC_V_len                                           COLLECT_C_VEC_len
#define CLC_V_spare                                         COLLECT_C_VEC_spare
#define CLC_V_spare_back                                    COLLECT_C_VEC_spare_back
#define CLC_V_spare_front                                   COLLECT_C_VEC_spare_front

#define CLC_V_at_v                                          COLLECT_C_VEC_at_v
#define CLC_V_cat_v                                         COLLECT_C_VEC_cat_v

#define CLC_V_at_t                                          COLLECT_C_VEC_at_t
#define CLC_V_cat_t                                         COLLECT_C_VEC_cat_t

#define CLC_V_front_t                                       COLLECT_C_VEC_front_t
#define CLC_V_back_t                                        COLLECT_C_VEC_back_t

#define CLC_V_cfront_t                                      COLLECT_C_VEC_cfront_t
#define CLC_V_cback_t                                       COLLECT_C_VEC_cback_t

#define CLC_V_begin_v                                       COLLECT_C_VEC_begin_v
#define CLC_V_end_v                                         COLLECT_C_VEC_end_v

#define CLC_V_cbegin_v                                      COLLECT_C_VEC_cbegin_v
#define CLC_V_cend_v                                        COLLECT_C_VEC_cend_v

#define CLC_V_cbegin_t                                      COLLECT_C_VEC_cbegin_t
#define CLC_V_cend_t                                        COLLECT_C_VEC_cend_t

#define CLC_V_begin_t                                       COLLECT_C_VEC_begin_t
#define CLC_V_end_t                                         COLLECT_C_VEC_end_t


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

 #pragma once


 /* ///////////////////////////// end of file //////////////////////////// */

