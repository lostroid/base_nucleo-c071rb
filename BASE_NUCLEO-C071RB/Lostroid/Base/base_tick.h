/********************************************************************
* File:    base_tick.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_TICK_H
#define H_BASE_TICK_H

#include "base_tick_type.h"

//#define d_DWT_ENABLE  //+ Active DWT Function

void f_Base_Tick_Init(void);
void f_Base_Tick_Module(void);
void f_Base_Tick_Systick_Init(void);
tu32 f_Base_Tick_Systick_Get_Time32_Tick(void);
tu64 f_Base_Tick_Systick_Get_Time64_Tick(void);
void      f_Base_Tick_Systick32_Stopwatch_Start(ts_Base_Tick_Context *ps_time, tu32 v_time_us);
te_Return f_Base_Tick_Systick32_Stopwatch_Check(ts_Base_Tick_Context *ps_time);
te_Return f_Base_Tick_Systick32_Elapsed_Flag(ts_Base_Tick_Context *ps_time);
void f_Base_Tick_Systick32_Blocking_Delay(tu32 v_time_us);
void f_Base_Tick_Systick32_Start(tu32 *p_tick_buff);
tu32 f_Base_Tick_Systick32_Finish(tu32 *p_pave_tick_buff);
void f_Base_Tick_Systick64_Start(tu64 *p_tick_buff);
tu64 f_Base_Tick_Systick64_Finish(tu64 *p_pave_tick_buff);
void f_Base_Tick_Systick_StartTime_Print(void);
void f_Base_Tick_Systick_EndTime_Print(void);
void f_Base_Tick_Systick_Run_Time_Print(void);

    #ifdef d_DWT_ENABLE
    void f_Base_Tick_DWT_Init(void);
    tu64 f_Base_Tick_DWT_Get_Time64_Tick(void);
    void      f_Base_Tick_DWT_Stopwatch_Start(ts_Base_Tick_Context *ps_time, tu32 v_time_us);
    te_Return f_Base_Tick_DWT_Stopwatch_Check(ts_Base_Tick_Context *ps_time);
    te_Return f_Base_Tick_DWT_Elapsed_Flag(ts_Base_Tick_Context *ps_time);
    void f_Base_Tick_DWT_Blocking_Delay(tu32 v_time_us);
    void f_Base_Tick_DWT_Check_Start(tu32 *p_tick);
    tu32 f_Base_Tick_DWT_Check_Finish(tu32 *p_tick);
    void f_Base_Tick_DWT_StartTime_Print(void);
    void f_Base_Tick_DWT_EndTime_Print(void);
    void f_Base_Tick_DWT_Run_Time_Print(void);
    #endif

#endif
