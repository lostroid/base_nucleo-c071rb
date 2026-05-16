/********************************************************************
* File:    base_tick.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#include "stm32c071xx.h"
#include "../DBG/dbg.h"
#include "base_tick.h"

#define d_TICK_SYSTICK_CNT      (SysTick->VAL)  //+ 24bit SYSTICK counte

static tu32 gv_base_tick_strart = 0;
static tu32 gv_base_tick_end = 0;
//===================================================================
/*### Tick Initialize 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tick_init(void)
{
    f_base_tick_systick_init();
    f_base_tick_systick_time32_get();
    f_base_tick_systick_time64_get();
}
//===================================================================
/*### Tick Module (Polling Type) 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tick_module(void)
{

}

//===================================================================
/*### Tick DWT Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tick_systick_init(void)
{
    SysTick->LOAD   = 0x00FFFFFF;                   //+ Reload Max 24bit
    SysTick->VAL    = 0UL;                          //+ Count reset
    SCB->SHP[1]    |= 0xC0000000;                   //+ Systick Priority 03
    //SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;    //+ Enable Clock /8, Systick (6Mhz = 48Mhz / 8)
    SysTick->CTRL  |= ( SysTick_CTRL_CLKSOURCE_Msk 
                      | SysTick_CTRL_ENABLE_Msk);   //+ Enable Clock /1, Systick
}

//===================================================================
/*### SysTick Base Time Stamp 32bit up count "32bit 틱 업 카운터"
---------------------------------------------------------------------
Max 349ms overflow "최대 349ms 넘지 않습니다"
Call at least once per 100ms " 최소 100ms 마다 호출 권장"
+ return : Convert 32-bit tick  "리턴값 32비트 틱 카운터 변환 마이크로초"
-------------------------------------------------------------------*/
tu32 f_base_tick_systick_time32_get(void)
{
    static tu32 v_tick_total = 0;
    static tu32 v_tick_pave = 0;
    tu32 v_tick_now = d_TICK_SYSTICK_CNT;   //+ 24bit down counter
    v_tick_total += (tu32)((v_tick_pave - v_tick_now) & 0xFFFFFF);
    v_tick_pave = v_tick_now;
    return v_tick_total;
}

//===================================================================
/*### SysTick Base Time Stamp 64bit up count "64bit 틱 업 카운터"
---------------------------------------------------------------------
Max 349ms overflow "최대 349ms 넘지 않습니다"
Call at least once per 100ms " 최소 100ms 마다 호출 권장"
+ return : Convert 64-bit tick "리턴값 64비트 틱 카운터 변환 마이크로초"
-------------------------------------------------------------------*/
tu64 f_base_tick_systick_time64_get(void)
{
    static tu64 v_tick_total = 0;
    static tu32 v_tick_pave = 0;
    tu32 v_tick_now = d_TICK_SYSTICK_CNT;   //+ 24bit down counter
    v_tick_total += (tu64)((v_tick_pave - v_tick_now) & 0xFFFFFF);
    v_tick_pave = v_tick_now;
    return v_tick_total;
}

//===================================================================
/*### Systick32 Tick base capture "Systic32 틱 기반으로 시작"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ ps_timeTaget : struct variable pointer
+ v_time_us : convert microsecond to tick count "us 단위를 tick 값으로 변경저장"
-------------------------------------------------------------------*/
void f_base_tick_time32_start_lap(ts_base_tick_context *ps_time, tu32 v_time_us)
{
    if(ps_time == d_NULL)
        { return; }
    ps_time->v_pt = f_base_tick_systick_time32_get();
    ps_time->v_ct = f_base_tick_systick_time32_get();
    ps_time->v_st = d_BASE_TICK_SYS_GET_TICK(v_time_us);;
    ps_time->v_tt = 0;
}

//===================================================================
/*### Systick32 Elapsed Check "Systick기반으로 경과 시간 여부 확인"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ ps_time : struct variable pointer
+  Return : m_RETURN_OK, m_RETURN_WAIT
-------------------------------------------------------------------*/
te_return f_base_tick_time32_check_lap(ts_base_tick_context *ps_time)
{
    if(ps_time == d_NULL)
        { return m_RETURN_ERR_NULL; }
    ps_time->v_ct = f_base_tick_systick_time32_get();
    ps_time->v_tt += (ps_time->v_ct - ps_time->v_pt);
    ps_time->v_pt = ps_time->v_ct;
    if(ps_time->v_tt >= ps_time->v_st)
    {
        ps_time->v_tt -= ps_time->v_st;
        return m_RETURN_OK;
    }
    else
    {
        return m_RETURN_WAIT;
    }
}

//===================================================================
/*### Tick DWT Elapsed Flag: DWT 틱 경과 여부 플래그
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ ps_time : struct variable pointer
+  Return : m_RETURN_OK, m_RETURN_WAIT
-------------------------------------------------------------------*/
te_return f_base_tick_time32_flag_lap(ts_base_tick_context *ps_time)
{
    if(ps_time == d_NULL)
        { return m_RETURN_ERR_NULL; }
    ps_time->v_ct = f_base_tick_systick_time32_get();
    ps_time->v_tt += (ps_time->v_ct - ps_time->v_pt);
    ps_time->v_pt = ps_time->v_ct;
    if(ps_time->v_tt >= ps_time->v_st)
        { return m_RETURN_OK; }
    else
        { return m_RETURN_WAIT; }
}

//===================================================================
/*### DWT Blocking Delay "DWT를 사용하여 코드 실행을 블로킹하면서 지연"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
Timeout : 10s
+ v_time_us : microseconds Max 17,000,000 "최대 17,000,000us 를 넘지 않습니다."
-------------------------------------------------------------------*/
void f_base_tick_time32_blocking_delay(tu32 v_time_us)
{
    ts_base_tick_context s_time;
    s_time.v_pt = f_base_tick_systick_time32_get();
    s_time.v_st = d_BASE_TICK_SYS_GET_TICK(v_time_us);
    s_time.v_tt = 0;
    while(1)
    {
        s_time.v_ct = f_base_tick_systick_time32_get();
        s_time.v_tt += (s_time.v_ct - s_time.v_pt);
        s_time.v_pt = s_time.v_ct;
        if(s_time.v_tt >= s_time.v_st)
            { break; }
        // Timeout 10s
        if(s_time.v_tt > d_BASE_TICK_SYS_GET_TICK(10000000))
            { break; }
    }
}

//===================================================================
/*### SysTick-based 32-bit tick count start "Systick 기반 32비트 틱 카운터 시작"
---------------------------------------------------------------------
Warning: Start ~ finish 
Overflow MAX 89s = 4,294,967,296 / 48,000,000 Hz 
"최대 측정 시간은 89초를 넘기지 않습니다."
+ p_tick_buff: save tick count point "시간 정보 포인터"
-------------------------------------------------------------------*/
void f_base_tick_time32_start_timer(tu32 *p_tick_buff)
{
    if(p_tick_buff == d_NULL)
        { return; }
    *p_tick_buff = f_base_tick_systick_time32_get();
}

//===================================================================
/*### SysTick-based 32-bit tick count finish "Systick 기반 32비트 틱 카운터 완료"
---------------------------------------------------------------------
Warning: Start ~ finish
Overflow MAX 89s = 4,294,967,296 / 48,000,000 Hz 
"최대 측정 시간은 89초를 넘기지 않습니다."
+ p_pave_tick_buff: Start tick count point "시작 tick정보 포인터"
+ return: Convert 32-bit tick to us
-------------------------------------------------------------------*/
tu32 f_base_tick_time32_check_timer(tu32 *p_pave_tick_buff)
{
    if(p_pave_tick_buff == d_NULL)
        { return 0; }
    tu32 v_current_tick = f_base_tick_systick_time32_get();
    v_current_tick = (v_current_tick - *p_pave_tick_buff);
    return d_BASE_TICK_SYS_GET_US(v_current_tick);
}
 
//===================================================================
/*### SysTick-based 64-bit tick count start 
  ### "Systick 기반 64비트 틱 카운터 시작"
---------------------------------------------------------------------
+ ps_time: Time Context point "시간 정보 포인터"
-------------------------------------------------------------------*/
void f_base_tick_time64_start_timer(tu64 *p_tick_buff)
{
    if(p_tick_buff == d_NULL)
        { return; }
    *p_tick_buff = f_base_tick_systick_time64_get();
}

//===================================================================
/*### Base time systick64 finish
---------------------------------------------------------------------
Warning: Start ~ finish  
+ ps_time: Time Context point "시간 정보 포인터"
+ return: Convert 64-bit tick to us
-------------------------------------------------------------------*/
tu64 f_base_tick_time64_check_timer(tu64 *p_pave_tick_buff)
{
    if(p_pave_tick_buff == d_NULL)
        { return 0; }
    tu64 v_current_tick = f_base_tick_systick_time64_get();
    v_current_tick = (v_current_tick - *p_pave_tick_buff);
    return d_BASE_TICK_SYS_GET_US(v_current_tick);
}

//===================================================================
/*### Print SysTick start counter
  ### "Systick 카운터 시작 정보 출력"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ void
-------------------------------------------------------------------*/
void f_base_tick_time32_start_time_print(void)
{
    if(f_dbg_mode_get(m_DBG_MODE_SCHEDULE) == m_YESNO_YES)
    {
        gv_base_tick_strart = f_base_tick_systick_time32_get();
        f_dbg_print_string("\r\nI: Start Tick count: ");
        f_dbg_print_dec32(gv_base_tick_strart);
    }
}

//===================================================================
/*### Print SysTick elapsed time
  ### "Systick 경과시간 정보 출력"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ void
-------------------------------------------------------------------*/
void f_base_tick_time32_end_time_print(void)
{
    if(f_dbg_mode_get(m_DBG_MODE_SCHEDULE) == m_YESNO_YES)
    {
        gv_base_tick_end = f_base_tick_systick_time32_get();
        f_dbg_print_string("\r\nI: End Tick Count: ");
        f_dbg_print_dec32(gv_base_tick_end);
        f_dbg_print_string("\r\nI: Run Time: ");
        tu32 v_Time_US;
        v_Time_US = gv_base_tick_end - gv_base_tick_strart;
        f_dbg_print_dec32(d_BASE_TICK_SYS_GET_US(v_Time_US));
    }
}

//===================================================================
/*### Systeick Base Log time stemp "Systick 로그 시스템 스템프"
---------------------------------------------------------------------
+ Print [0d-00:00:00.000000]
-------------------------------------------------------------------*/
void f_base_tick_time64_run_time_print(void)
{
    tu64 v_time = d_BASE_TICK_SYS_GET_US(f_base_tick_systick_time64_get());
    f_dbg_print_string("\r\n[");      
    f_dbg_print_dec32((tu32)(v_time / 86400000000));
    f_dbg_print_string("d-");
    v_time %= 86400000000;
    f_dbg_print_dec32_digit(2, (tu32)(v_time / 3600000000));
    f_dbg_print_string(":");
    v_time %= 3600000000;
    f_dbg_print_dec32_digit(2, (tu32)(v_time / 60000000));
    f_dbg_print_string(":");
    v_time %= 60000000;
    f_dbg_print_dec32_digit(2, (tu32)(v_time / 1000000));
    f_dbg_print_string(".");
    v_time %= 1000000;
    f_dbg_print_dec32_digit(6, v_time);
    f_dbg_print_string("]");
}



//===================================================================
// DWT Function "DWT 전용 함수"
//===================================================================
#ifdef d_DWT_ENABLE
#define d_TICK_DWT_CNT          (DWT->CYCCNT)   //+ 32bit Tick counter

//===================================================================
/*### Tick DWT Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tick_dwt_init(void)
{
    #ifdef d_TICK_DWT_CNT
    //+ DWT Counter Setting
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    ITM->LAR = 0xC5ACCE55;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
    #endif
}
//===================================================================
/*### DWT Base Time Stamp 64bit count "64bit 타임 카운터"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
Call at least once per second " 최소 1s 마다 호출 권장"
+ return 64bit Tick to microseconds "리턴값 64비트 틱 카운터 변환 마이크로초"
-------------------------------------------------------------------*/
tu64 f_base_tick_dwt_time64_get(void)
{
    static tu64 v_tick_total = 0;
    static tu32 v_tick_pave = 0;                //+ Init value
    tu32 v_tick_now = d_TICK_DWT_CNT;           //+ 32bit up counter
    v_tick_total += (tu64)(v_tick_now - v_tick_pave); //+ Order depends on counter direction "카운터 방향에 따라 순서를 변경"
    v_tick_pave = v_tick_now;
    return v_tick_total;
}

//===================================================================
/*### DWT Time us
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ ps_timeTaget : struct variable pointer
+ v_time_us : setting microsecond
-------------------------------------------------------------------*/
void f_base_tick_dwt_start_lap(ts_base_tick_context *ps_time, tu32 v_time_us)
{
    if(ps_time == d_NULL)
        { return; }
    ps_time->v_pt = d_TICK_DWT_CNT;
    ps_time->v_ct = d_TICK_DWT_CNT;
    ps_time->v_st = d_BASE_TICK_DWT_GET_TICK(v_time_us);
    ps_time->v_tt = 0;
}

//===================================================================
/*### Tick DWT Elapsed Check 1us: 1µs 단위로 DWT 틱 경과 여부를 확인
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ ps_time : struct variable pointer
+  Return
m_RETURN_OK
m_RETURN_WAIT
-------------------------------------------------------------------*/
te_return f_base_tick_dwt_check_lap(ts_base_tick_context* ps_time)
{
    if(ps_time == d_NULL)
        { return m_RETURN_ERR_NULL; }
    ps_time->v_ct = d_TICK_DWT_CNT;
    ps_time->v_tt += (ps_time->v_ct - ps_time->v_pt);
    ps_time->v_pt = ps_time->v_ct;
    if(ps_time->v_tt >= ps_time->v_st)
    {
        ps_time->v_tt -= ps_time->v_st;
        return m_RETURN_OK;
    }
    else
    {
        return m_RETURN_WAIT;
    }
}

//===================================================================
/*### Tick DWT Elapsed Flag: DWT 틱 경과 여부 플래그
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ ps_time : struct variable pointer
+  Return
m_RETURN_OK
m_RETURN_WAIT
-------------------------------------------------------------------*/
te_return f_base_tick_dwt_flag_lap(ts_base_tick_context* ps_time)
{
    if(ps_time == d_NULL)
        { return m_RETURN_ERR_NULL; }
    ps_time->v_ct = d_TICK_DWT_CNT;
    ps_time->v_tt += (ps_time->v_ct - ps_time->v_pt);
    ps_time->v_pt = ps_time->v_ct;
    if(ps_time->v_tt >= ps_time->v_st)
        { return m_RETURN_OK; }
    else
        { return m_RETURN_WAIT; }
}

//===================================================================
/*### DWT Blocking Delay: DWT를 사용하여 코드 실행을 블로킹하면서 지연
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
Timeout : 10s
+ v_time_us : tu32
-------------------------------------------------------------------*/
void f_base_tick_dwt_blocking_delay(tu32 v_time_us)
{
    ts_base_tick_context s_time;
    s_time.v_pt = d_TICK_DWT_CNT;
    s_time.v_st = d_BASE_TICK_DWT_GET_TICK(v_time_us);
    s_time.v_tt = 0;
    while(1)
    {
        s_time.v_ct = d_TICK_DWT_CNT;
        s_time.v_tt += (s_time.v_ct - s_time.v_pt);
        s_time.v_pt = s_time.v_ct;
        if(s_time.v_tt >= s_time.v_st)
            { break; }
        // Timeout 10s
        if(s_time.v_tt > d_BASE_TICK_DWT_GET_TICK(10000000))
            { break; }
    }
}

//===================================================================
/*### Save the current tick count via pointer
### 현재 Tick Count 정보를 포인터를 통해 저장
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ *v_time_us : tu32 pointor
-------------------------------------------------------------------*/
void f_base_tick_dwt_start_timer(tu32 *p_tick)
{
    if(p_tick == d_NULL)
        { return; }
    *p_tick = d_TICK_DWT_CNT;
}

//===================================================================
/*### Difference (in microseconds) between the Tick stored via pointer 
    ### "기존에 포인터를 통해 저장한 Tick 과 현재 Tick 차이 US시간"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ p_count : Tick count vaule
+ Return : us(microsecond)
-------------------------------------------------------------------*/
tu32 f_base_tick_dwt_check_timer(tu32 *p_tick)
{
    if(p_tick == d_NULL)
        { return 0; }
    tu32 v_dwt_tick = d_TICK_DWT_CNT;
    v_dwt_tick = (v_dwt_tick - *p_tick);
    return d_BASE_TICK_DWT_GET_US(v_dwt_tick);
}

//===================================================================
/*### DBG pirnt DWT start tick counter.
    ### "디버깅 포트를 통한 Tick 카운터 시작 정보 출력"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ void
-------------------------------------------------------------------*/
void f_base_tick_dwt_start_time_print(void)
{
    if(f_dbg_mode_get(m_DBG_MODE_SCHEDULE) == m_YESNO_YES)
    {
        gv_base_tick_strart = d_TICK_DWT_CNT;
        f_dbg_print_string("\r\nI: Start DWT Count: ");
        f_dbg_print_dec32(gv_base_tick_strart);
    }
}

//===================================================================
/*### DBG Print DWT end counter, run time 
    ### "디버깅 포트를 통한 측정시간 정보 출력"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ void
-------------------------------------------------------------------*/
void f_base_tick_dwt_end_time_print(void)
{
    if(f_dbg_mode_get(m_DBG_MODE_SCHEDULE) == m_YESNO_YES)
    {
        gv_base_tick_end = d_TICK_DWT_CNT;
        f_dbg_print_string("\r\nI: End DWT Count: ");
        f_dbg_print_dec32(gv_base_tick_end);
        f_dbg_print_string("\r\nI: Run Time: ");
        tu32 v_Time_Tick;
        v_Time_Tick = gv_base_tick_end - gv_base_tick_strart;
        f_dbg_print_dec32(d_BASE_TICK_DWT_GET_US(v_Time_Tick));
    }
}

//===================================================================
/*### DWT Base Log time stemp "DWT 로그 시스템 스템프"
---------------------------------------------------------------------
+ Print [0d-00:00:00.000000]
-------------------------------------------------------------------*/
void f_base_tick_dwt_time64_run_time_print(void)
{
    tu64 v_time_us = d_BASE_TICK_DWT_GET_US(f_base_tick_dwt_time64_get());
    f_dbg_print_string("\r\n[");      
    f_dbg_print_dec32((tu32)(v_time_us / 86400000000));
    f_dbg_print_string("d ");
    v_time_us %= 86400000000;
    f_dbg_print_dec32_digit(2, (tu32)(v_time_us / 3600000000));
    f_dbg_print_string(":");
    v_time_us %= 3600000000;
    f_dbg_print_dec32_digit(2, (tu32)(v_time_us / 60000000));
    f_dbg_print_string(":");
    v_time_us %= 60000000;
    f_dbg_print_dec32_digit(2, (tu32)(v_time_us / 1000000));
    f_dbg_print_string(".");
    v_time_us %= 1000000;
    f_dbg_print_dec32_digit(6, v_time_us);
    f_dbg_print_string("]");
}

#endif
