/********************************************************************
* File:    base_Tick_type.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
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
#ifndef H_BASE_TIME_TYPE_H
#define H_BASE_TIME_TYPE_H

#include "typedef.h"
#define d_BASE_TICK_1MS                 1000u           //+ Value 1ms
#define d_BASE_TICK_10MS                10000u          //+ Value 10ms
#define d_BASE_TICK_100MS               100000u         //+ Value 100ms
#define d_BASE_TICK_500MS               500000u         //+ Value 500ms
#define d_BASE_TICK_1S                  1000000u        //+ Value 1s

#define d_BASE_TICK_HCLK                48000000ul      //+ HCLK 48Mhz
#define d_BASE_TICK_DWT_CLK             d_BASE_TICK_HCLK
#define d_BASE_TICK_DWT_GET_TICK(us)    ((us) * (d_BASE_TICK_HCLK / 1000000ul))     //+ us -> Tick count
#define d_BASE_TICK_DWT_GET_US(tick)    ((tick) / (d_BASE_TICK_HCLK / 1000000ul))   //+ Tick count -> us

#define d_BASE_TICK_SYS_CLK             (d_BASE_TICK_HCLK / 1)                      //+ HCLK / 1 ro 8
#define d_BASE_TICK_SYS_GET_TICK(us)    ((us) * (d_BASE_TICK_HCLK / 1000000ul))     //+ us -> Tick count
#define d_BASE_TICK_SYS_GET_US(tick)    ((tick) / (d_BASE_TICK_HCLK / 1000000ul))   //+ Tick count -> us

//+ Time Tick measurement structure
//"타임 Tick 측정 구조체"
typedef struct
{
    tu32 v_pt;    //+ previous tick "이전 틱 값"
    tu32 v_ct;    //+ Current tick  "현재 틱 값"
    tu32 v_st;    //+ Setting tick  "설정 틱 값"
    tu32 v_tt;    //+ Total tick    "총 틱 값"
}ts_Base_Tick_Context;

#endif
