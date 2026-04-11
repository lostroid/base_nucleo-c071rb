/********************************************************************
* File:    base_ll_timer.c
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the time control.
*
* Revision History:
*   2025-08-06  New.
********************************************************************/

#include "../DBG/dbg.h"
#include "stm32c071xx.h"
#include "base_ll_tick.h"

#define d_TIME_SYSTICK  (SysTick->VAL)
//===================================================================
/*### LL initialize
-------------------------------------------------------------------*/
void f_LL_Tick_Init(void)
{


}
//===================================================================
/*### LL Time Module
-------------------------------------------------------------------*/
void f_LL_Tick_Module(void)
{


}
//===================================================================
/*### Base LL time systick capture
---------------------------------------------------------------------
Warning: MAX 2.7s = (16,777,215(24bit) / 6,000,000 Hz (48Mhz /8)
+ ps_time: Time Context point "시간 정보 포인터"
+ v_time_us: Setting time "설정 타임 us"
-------------------------------------------------------------------*/
void f_LL_Tick_Systick_Base32_Capture(ts_LL_Tick_Context *ps_time, tu32 v_time_us)
{
    v_time_us = d_LL_TICK_SYS_GET_TICK(v_time_us);
    ps_time->v_pt = f_LL_Tick_Systick_Base32();
    ps_time->v_ct = f_LL_Tick_Systick_Base32();
    ps_time->v_st = v_time_us;
    ps_time->v_tt = 0;
}
//===================================================================
/*### LL time systick elapsed
---------------------------------------------------------------------
Warning: MAX 2.7s = (16,777,215(24bit) / 6,000,000 Hz (48Mhz /8)
+ ps_time: Time Context point "시간 정보 포인터"
+ return: tu32 1(wait), 0(ok)
-------------------------------------------------------------------*/
tu32 f_LL_Tick_Systick_Base32_Elapsed(ts_LL_Tick_Context* ps_time)
{
    ps_time->v_ct = f_LL_Tick_Systick_Base32();
    ps_time->v_tt += (ps_time->v_ct - ps_time->v_pt);
    ps_time->v_pt = ps_time->v_ct;
    if(ps_time->v_tt >= ps_time->v_st)
    {
        ps_time->v_tt -= ps_time->v_st;
        return 0;
    }
    return 1;
}
//===================================================================
/*### LL time systick32 start
---------------------------------------------------------------------
Warning: Start ~ finish  MAX 89.47s = 4,294,967,295 / 48,000,000 Hz
+ ps_time: Time Context point "시간 정보 포인터"
-------------------------------------------------------------------*/
void f_LL_Tick_Systick32_Start(tu32 *p_tick_buff)
{
    *p_tick_buff = f_LL_Tick_Systick_Base32();
}
//===================================================================
/*### LL time systick32 finish
---------------------------------------------------------------------
Warning: Start ~ finish  MAX 89.47s = 4,294,967,295 / 48,000,000 Hz
+ ps_time: Time Context point "시간 정보 포인터"
+ return: tu32 Time US
-------------------------------------------------------------------*/
tu32 f_LL_Tick_Systick32_Finish(tu32 *p_pave_tick_buff)
{
    tu32 v_current_tick = f_LL_Tick_Systick_Base32();
    v_current_tick = (v_current_tick - *p_pave_tick_buff);
    return d_LL_TICK_SYS_GET_US(v_current_tick);
}
//===================================================================
/*### LL time systick64 start
---------------------------------------------------------------------
Warning: Start ~ finish 
MAX 384,307,168,202s(12 years) = 18,446,744,073,709,551,615 / 48,000,000 Hz
+ ps_time: Time Context point "시간 정보 포인터"
-------------------------------------------------------------------*/
void f_LL_Tick_Systick64_Start(tu64 *p_tick_buff)
{
    *p_tick_buff = f_LL_Tick_Systick_Base64();
}
//===================================================================
/*### LL time systick64 finish
---------------------------------------------------------------------
Warning: Start ~ finish  
MAX 384,307,168,202s(12 years) = 18,446,744,073,709,551,615 / 48,000,000 Hz
+ ps_time: Time Context point "시간 정보 포인터"
+ return: tu64 Time US
-------------------------------------------------------------------*/
tu64 f_LL_Tick_Systick64_Finish(tu64 *p_pave_tick_buff)
{
    tu64 v_current_tick = f_LL_Tick_Systick_Base64();
    v_current_tick = (v_current_tick - *p_pave_tick_buff);
    return d_LL_TICK_SYS_GET_US(v_current_tick);
}
//===================================================================
/*### SysTick Base Time Stamp 32bit count "32bit 타임 카운터"
---------------------------------------------------------------------
Max 349ms overflow "최대 349ms 넘지 않습니다"
Must be called at least every 100 ms " 최소 100ms 마다 호출 해야 합니다"
+ return 32bit microceconds "리턴값 32비트 마이크로초"
-------------------------------------------------------------------*/
tu32 f_LL_Tick_Systick_Base32(void)
{
    static tu32 v_time_stamp = 0;
    static tu32 v_pave_time = 0xFFFFFF; /* 24bit */
    tu32 v_now_time = d_TIME_SYSTICK;
    v_time_stamp += ((v_pave_time - v_now_time) & 0xFFFFFF);
    v_pave_time = v_now_time;
    return v_time_stamp;
}
//===================================================================
/*### SysTick Base Time Stamp 64bit count "64bit 타임 카운터"
---------------------------------------------------------------------
Max 349ms overflow "최대 349ms 넘지 않습니다"
Must be called at least every 100 ms " 최소 100ms 마다 호출 해야 합니다"
+ return 64bit microceconds "리턴값 64비트 마이크로초"
-------------------------------------------------------------------*/
tu64 f_LL_Tick_Systick_Base64(void)
{
    static tu64 v_time_stamp = 0;
    static tu32 v_pave_time = 0xFFFFFF; /* 24bit */
    tu32 v_now_time = d_TIME_SYSTICK;
    v_time_stamp += ((v_pave_time - v_now_time) & 0xFFFFFF);
    v_pave_time = v_now_time;
    return v_time_stamp;
}
//===================================================================
/*### Log time stemp "로그 시스템 스템프"
---------------------------------------------------------------------
+ Time stamp [1d 00:00:00.000000]us
-------------------------------------------------------------------*/
void f_LL_Tick_Systick_Run_Stamp_Print(void)
{
    tu64 v_time = d_LL_TICK_SYS_GET_US(f_LL_Tick_Systick_Base64());
    f_DBG_Print_String("\r\n[");      
    f_DBG_Print_Dec32((tu32)(v_time / 86400000000));
    f_DBG_Print_String("d");
    v_time %= 86400000000;
    f_DBG_Print_Dec32_Digit(2, (tu32)(v_time / 3600000000));
    f_DBG_Print_String(":");
    v_time %= 3600000000;
    f_DBG_Print_Dec32_Digit(2, (tu32)(v_time / 60000000));
    f_DBG_Print_String(":");
    v_time %= 60000000;
    f_DBG_Print_Dec32_Digit(2, (tu32)(v_time / 1000000));
    f_DBG_Print_String(".");
    v_time %= 1000000;
    f_DBG_Print_Dec32_Digit(6, v_time);
    f_DBG_Print_String("]");
}
