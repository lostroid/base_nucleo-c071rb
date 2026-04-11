/**********************************************************
* File:    base_ll_Tick_type.h
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the ll Tick type (타임 정보타입)
*
* Revision History:
*   2026-02-19  New.
-----------------------------------------------------------
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
**********************************************************/
#ifndef H_LL_TICK_TYPE_H
#define H_LL_TICK_TYPE_H

#include "typedef.h"
#define d_LL_TICK_1MS              1000u           //+ us
#define d_LL_TICK_10MS             10000u          //+ us
#define d_LL_TICK_100MS            100000u         //+ us
#define d_LL_TICK_500MS            500000u         //+ us
#define d_LL_TICK_1S               1000000u        //+ us

#define d_LL_TICK_HCLK_CLK             48000000ul           //+ HCLK 48Mhz
#define d_LL_TICK_DWT_CLK              d_LL_TICK_HCLK_CLK
#define d_LL_TICK_DWT_GET_TICK(us)     ((us) * (d_LL_TICK_DWT_CLK/1000000ul))       //+ us -> Tick count
#define d_LL_TICK_DWT_GET_US(tick)     ((tick) / (d_LL_TICK_DWT_CLK/1000000ul))     //+ Tick count -> us

#define d_LL_TICK_SYS_CLK               d_LL_TICK_HCLK_CLK  //+ HCLK 48Mhz
#define d_LL_TICK_SYS_GET_TICK(us)      ((us) * (d_LL_TICK_SYS_CLK/1000000ul))      //+ us -> Tick count
#define d_LL_TICK_SYS_GET_US(tick)      ((tick) / (d_LL_TICK_SYS_CLK/1000000ul))    //+ Tick count -> us

typedef struct
{
    tu32 v_pt;    //+ previous tick
    tu32 v_ct;    //+ Current tick
    tu32 v_st;    //+ Setting tick
    tu32 v_tt;    //+ Total tick
}ts_LL_Tick_Context;

#endif
