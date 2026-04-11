/********************************************************************
* File:    base_ll_uart_type.h
* Author:  Lostroid
* Created: 2025-08-06
*
* Description:
* This is the uart Type.
*
* Revision History:
*   2025-08-06  New.
---------------------------------------------------------------------
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
********************************************************************/
#ifndef H_LL_UART_TYPE_H
#define H_LL_UART_TYPE_H

#include "typedef.h"

typedef struct
{
    const tu32 cv_max_size;     //+ Buff size: 버퍼 사이즈
    tu32 v_read_pos;            //+ Read Postion: 읽기 위치
    tu32 v_write_pos;           //+ Write Postion: 쓰기 위치
    tu32 v_Error;               //+ Error: 에러
}ts_LL_Uart_Ctrol;

#endif
