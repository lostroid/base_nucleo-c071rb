/********************************************************************
* File:    base_uart_type.h
* Author:  Lostroid
* Created: 2025-08-06
* Encoding: UTF-8
---------------------------------------------------------------------
01. a_ = array                         e.g. a_data[]
02. b_ = bit field                     e.g. b_Data
03. c_ = constant                      e.g, c_Data
04. d_ = #define macro                 e.g. d_Data
05. e_ = enum type                     e.g. e_Data
06. f_ = function                      e.g. f_Data
07. g_ = global (source file static)   e.g. gv_Data
08. i_ = inline function               e.g. i_Data
09. l_ = local static variable         e.g. lv_Data
10. m_ = enum member                   e.g. m_Data
11. p_ = pointer                       e.g. p_Data 
12. s_ = struct                        e.g. s_Data 
13. t_ = typedef                       e.g. t_Data 
14. u_ = union                         e.g. u_Data 
15. v_ = variable                      e.g. v_Data
16. x_ = extern variable               e.g. xv_data
********************************************************************/
#ifndef H_BASE_UART_TYPE_H
#define H_BASE_UART_TYPE_H

#include "types.h"

typedef struct
{
    const tu32 cv_max_size;     //+ Const Buff size "버퍼 사이즈"
    tu32 v_read_pos;            //+ Read Postion    "읽기 위치"
    tu32 v_write_pos;           //+ Write Postion   "쓰기 위치"
    tu32 v_Error;               //+ Error           "에러"
}ts_Base_Uart_Buff_Ctrol;

#endif
