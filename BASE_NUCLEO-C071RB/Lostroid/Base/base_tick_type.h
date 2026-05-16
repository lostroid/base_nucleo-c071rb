/********************************************************************
* File:    base_Tick_type.h
* Author:  Lostroid
* Created: 2026-02-19
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
#ifndef H_BASE_TICK_TYPE_H
#define H_BASE_TICK_TYPE_H

#include "types.h"
#define d_BASE_TICK_1MS                 1000u           //+ Value 1ms
#define d_BASE_TICK_10MS                10000u          //+ Value 10ms
#define d_BASE_TICK_100MS               100000u         //+ Value 100ms
#define d_BASE_TICK_500MS               500000u         //+ Value 500ms
#define d_BASE_TICK_1S                  1000000u        //+ Value 1s

#define d_BASE_TICK_HCLK                    48000000ul      //+ HCLK 48Mhz
#define d_BASE_TICK_DWT_CLK                 d_BASE_TICK_HCLK
#define d_BASE_TICK_DWT_GET_TICK(D_US)      (D_US   * (d_BASE_TICK_HCLK / 1000000ul))   //+ us -> Tick count
#define d_BASE_TICK_DWT_GET_US(D_TICK)      (D_TICK / (d_BASE_TICK_HCLK / 1000000ul))   //+ Tick count -> us

#define d_BASE_TICK_SYS_CLK                 (d_BASE_TICK_HCLK / 1)                      //+ HCLK / 1 ro 8
#define d_BASE_TICK_SYS_GET_TICK(D_US)      ((D_US)   * (d_BASE_TICK_HCLK / 1000000ul))   //+ us -> Tick count
#define d_BASE_TICK_SYS_GET_US(D_TICK)      ((D_TICK) / (d_BASE_TICK_HCLK / 1000000ul))   //+ Tick count -> us

//+ Time Tick measurement structure
//"타임 Tick 측정 구조체"
typedef struct
{
    tu32 v_pt;    //+ previous tick "이전 틱 값"
    tu32 v_ct;    //+ Current tick  "현재 틱 값"
    tu32 v_st;    //+ Setting tick  "설정 틱 값"
    tu32 v_tt;    //+ Total tick    "총 틱 값"
}ts_base_tick_context;

#endif
