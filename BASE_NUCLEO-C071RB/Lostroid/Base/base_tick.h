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

void f_base_tick_init(void);
void f_base_tick_module(void);
void f_base_tick_systick_init(void);
tu32 f_base_tick_systick_time32_get(void);
tu64 f_base_tick_systick_time64_get(void);
void      f_base_tick_systick32_stopwatch_start(ts_base_tick_context *ps_time, tu32 v_time_us);
te_return f_base_tick_systick32_stopwatch_check(ts_base_tick_context *ps_time);
te_return f_base_tick_systick32_elapsed_flag(ts_base_tick_context *ps_time);
void f_base_tick_systick32_blocking_delay(tu32 v_time_us);
void f_base_tick_systick32_start(tu32 *p_tick_buff);
tu32 f_base_tick_systick32_finish(tu32 *p_pave_tick_buff);
void f_base_tick_systick64_start(tu64 *p_tick_buff);
tu64 f_base_tick_systick64_finish(tu64 *p_pave_tick_buff);
void f_base_tick_systick_start_time_print(void);
void f_base_tick_systick_end_time_print(void);
void f_base_tick_systick_run_time_print(void);

    #ifdef d_DWT_ENABLE
    void f_base_tick_dwt_init(void);
    tu64 f_base_tick_dwt_time64_get(void);
    void      f_base_tick_dwt_stopwatch_start(ts_base_tick_context *ps_time, tu32 v_time_us);
    te_return f_base_tick_dwt_stopwatch_check(ts_base_tick_context *ps_time);
    te_return f_base_tick_dwt_elapsed_flag(ts_base_tick_context *ps_time);
    void f_base_tick_dwt_blocking_delay(tu32 v_time_us);
    void f_base_tick_dwt_check_start(tu32 *p_tick);
    tu32 f_base_tick_dwt_check_finish(tu32 *p_tick);
    void f_base_tick_dwt_start_time_print(void);
    void f_base_tick_dwt_end_time_print(void);
    void f_base_tick_dwt_run_time_print(void);
    #endif

#endif
