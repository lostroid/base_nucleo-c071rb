/******************************************************************************
* File:    mortor_type.h
* Author:  LOSTROID
* Created: 2026-03-07
*
* Description:
* This is the motor Type.
*
* Revision History:
*   2026-03-07  New.
-------------------------------------------------------------------------------
01. a = array variable                      e.g. a_Data[]
02. b = bit fields                          e.g. b_Data
03. c = const                               e.g, c_Data
04. d = define                              e.g. d_Data
05. e = enum Type                           e.g. e_Data
06. f = Function                            e.g. f_Data
07. g = Source file static variable         e.g. gv_Data
08. i = Inline                              e.g. i_Data
09. l = Static variables inside functions   e.g. lv_Data
10. m = enum member                         e.g. m_Data
11. p = Pointer                             e.g. p_Data 
12. s = struct                              e.g. s_Data 
13. t = typedef                             e.g. t_Data 
14. u = Union                               e.g. u_Data 
15. v = variable                            e.g. v_Data
16. x = extern                              e.g. xv_data
******************************************************************************/
#ifndef H_MOTOR_TYPE_H
#define H_MOTOR_TYPE_H

#include "../Base_LL/base_ll_config.h"

typedef enum{
    m_MOTOR_LAST_STATE_OPEN,
    m_MOTOR_LAST_STATE_CLOSE,
    m_MOTOR_LAST_STATE_UNKNOWN
}te_Motor_State;

typedef struct {
    te_Motor_State e_state;
    tu32 v_time;
}ts_Motor_Context;

#endif
