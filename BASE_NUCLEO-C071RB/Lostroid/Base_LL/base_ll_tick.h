/********************************************************************
* File:    base_ll_tick.h
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the times header.
*
* Revision History:
*   2026-02-19  New.
********************************************************************/
#ifndef H_LL_TICK_H
#define H_LL_TICK_H

#include "base_ll_tick_type.h"

void f_LL_Tick_Init(void);
void f_LL_Tick_Module(void);
void f_LL_Tick_Systick_Base32_Capture(ts_LL_Tick_Context *ps_time, tu32 v_time_us);
tu32 f_LL_Tick_Systick_Base32_Elapsed(ts_LL_Tick_Context* ps_time);
void f_LL_Tick_Systick32_Start(tu32 *p_tick_buff);
tu32 f_LL_Tick_Systick32_Finish(tu32 *p_pave_tick_buff);
void f_LL_Tick_Systick64_Start(tu64 *p_tick_buff);
tu64 f_LL_Tick_Systick64_Finish(tu64 *p_pave_tick_buff);
tu32 f_LL_Tick_Systick_Base32(void);
tu64 f_LL_Tick_Systick_Base64(void);
void f_LL_Tick_Systick_Run_Stamp_Print(void);

#endif
