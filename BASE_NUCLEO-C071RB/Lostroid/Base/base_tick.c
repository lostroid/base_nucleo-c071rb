/********************************************************************
* File:    base_timer.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#include "stm32c071xx.h"
#include "../DBG/dbg.h"
#include "base_tick.h"

#define d_TICK_SYSTICK_CNT      (SysTick->VAL)  //+ 24bit SYSTICK counte

static tu32 gv_base_time_strart = 0;
static tu32 gv_base_time_end = 0;
//===================================================================
/*### Tick Initialize 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_Base_Tick_Init(void)
{
    f_Base_Tick_Systick_Init();
    f_Base_Tick_Systick_Get_Time32_us();
    f_Base_Tick_Systick_Get_Time64_us();
}
//===================================================================
/*### Tick Module (Polling Type) 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_Base_Tick_Module(void)
{

}

//===================================================================
/*### Tick DWT Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_Base_Tick_Systick_Init(void)
{
    SysTick->LOAD   = 0x00FFFFFF;                   //+ Reload Max 24bit
    SysTick->VAL    = 0UL;                          //+ Count reset
    SCB->SHP[1]    |= 0xC0000000;                   //+ Systick Priority 03
    //SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk;    //+ Enable Clock /8, Systick (6Mhz = 48Mhz / 8)
    SysTick->CTRL  |= ( SysTick_CTRL_CLKSOURCE_Msk 
                      | SysTick_CTRL_ENABLE_Msk);   //+ Enable Clock /1, Systick
}

//===================================================================
/*### SysTick Base Time Stamp 32bit count "32bit 틱 카운터"
---------------------------------------------------------------------
Max 349ms overflow "최대 349ms 넘지 않습니다"
Call at least once per 100ms " 최소 100ms 마다 호출 권장"
+ return : Convert 32-bit tick to us "리턴값 32비트 틱 카운터 변환 마이크로초"
-------------------------------------------------------------------*/
tu32 f_Base_Tick_Systick_Get_Time32_us(void)
{
    static tu32 v_tick_total = 0;
    static tu32 v_tick_pave = 0xFFFFFF;     //+ Init value
    tu32 v_tick_now = d_TICK_SYSTICK_CNT;   //+ 24bit down counter
    v_tick_total += ((v_tick_pave - v_tick_now) & 0xFFFFFF);
    v_tick_pave = v_tick_now;
    return v_tick_total;
}

//===================================================================
/*### SysTick Base Time Stamp 64bit count "64bit 틱 카운터"
---------------------------------------------------------------------
Max 349ms overflow "최대 349ms 넘지 않습니다"
Call at least once per 100ms " 최소 100ms 마다 호출 권장"
+ return : Convert 64-bit tick to us "리턴값 64비트 틱 카운터 변환 마이크로초"
-------------------------------------------------------------------*/
tu64 f_Base_Tick_Systick_Get_Time64_us(void)
{
    static tu64 v_tick_total = 0;
    static tu32 v_tick_pave = 0xFFFFFF;     //+ Init value
    tu32 v_tick_now = d_TICK_SYSTICK_CNT;   //+ 24bit down counter
    v_tick_total += ((v_tick_pave - v_tick_now) & 0xFFFFFF);
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
void f_Base_Tick_Systick32_Stopwatch_Start(ts_Base_Tick_Context *ps_time, tu32 v_time_us)
{
    v_time_us     = d_BASE_TICK_SYS_GET_TICK(v_time_us);
    ps_time->v_pt = f_Base_Tick_Systick_Get_Time32_us();
    ps_time->v_ct = f_Base_Tick_Systick_Get_Time32_us();
    ps_time->v_st = v_time_us;
    ps_time->v_tt = 0;
}

//===================================================================
/*### Systick32 Elapsed Check "Systick기반으로 경과 시간 여부 확인"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ ps_time : struct variable pointer
+  Return : m_RETURN_OK, m_RETURN_WAIT
-------------------------------------------------------------------*/
te_Return f_Base_Tick_Systick32_Stopwatch_Check(ts_Base_Tick_Context* ps_time)
{
    ps_time->v_ct = f_Base_Tick_Systick_Get_Time32_us();
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
tu32 f_Base_Tick_Systick32_Elapsed_Flag(ts_Base_Tick_Context* ps_time)
{
    ps_time->v_ct = f_Base_Tick_Systick_Get_Time32_us();
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
+ v_time_us : microseconds Max 17,000,000 "최대 17,000,000us 를 넘지 않습니다."
-------------------------------------------------------------------*/
void f_Base_Tick_Systick32_Blocking_Delay(tu32 v_time_us)
{
    ts_Base_Tick_Context s_time;
    s_time.v_pt = f_Base_Tick_Systick_Get_Time32_us();
    s_time.v_st = d_BASE_TICK_SYS_GET_TICK(v_time_us);
    s_time.v_tt = 0;
    while(1)
    {
        s_time.v_ct = f_Base_Tick_Systick_Get_Time32_us();
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
void f_Base_Tick_Systick32_Start(tu32 *p_tick_buff)
{
    *p_tick_buff = f_Base_Tick_Systick_Get_Time32_us();
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
tu32 f_Base_Tick_Systick32_Finish(tu32 *p_pave_tick_buff)
{
    tu32 v_current_tick = f_Base_Tick_Systick_Get_Time32_us();
    v_current_tick = (v_current_tick - *p_pave_tick_buff);
    return d_BASE_TICK_SYS_GET_US(v_current_tick);
}
 
//===================================================================
/*### SysTick-based 64-bit tick count start 
  ### "Systick 기반 64비트 틱 카운터 시작"
---------------------------------------------------------------------
+ ps_time: Time Context point "시간 정보 포인터"
-------------------------------------------------------------------*/
void f_Base_Tick_Systick64_Start(tu64 *p_tick_buff)
{
    *p_tick_buff = f_Base_Tick_Systick_Get_Time64_us();
}

//===================================================================
/*### LL time systick64 finish
---------------------------------------------------------------------
Warning: Start ~ finish  
+ ps_time: Time Context point "시간 정보 포인터"
+ return: Convert 64-bit tick to us
-------------------------------------------------------------------*/
tu64 f_Base_Tick_Systick64_Finish(tu64 *p_pave_tick_buff)
{
    tu64 v_current_tick = f_Base_Tick_Systick_Get_Time64_us();
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
void f_Base_Tick_Systick_StartTime_Print(void)
{
    if(f_DBG_Get_Mode(m_DBG_MODE_SCHEDLUE) == m_YESNO_YES)
    {
        gv_base_time_strart = f_Base_Tick_Systick_Get_Time32_us();
        f_DBG_Print_String("\r\nI: Start Tick count: ");
        f_DBG_Print_Dec32(gv_base_time_strart);
    }
}

//===================================================================
/*### Print SysTick elapsed time
  ### "Systick 경과시간 정보 출력"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ void
-------------------------------------------------------------------*/
void f_Base_Tick_Systick_EndTime_Print(void)
{
    if(f_DBG_Get_Mode(m_DBG_MODE_SCHEDLUE) == m_YESNO_YES)
    {
        gv_base_time_end = f_Base_Tick_Systick_Get_Time32_us();
        f_DBG_Print_String("\r\nI: End Tick Count: ");
        f_DBG_Print_Dec32(gv_base_time_end);
        f_DBG_Print_String("\r\nI: Run Time: ");
        tu32 v_Time_US;
        v_Time_US = gv_base_time_end - gv_base_time_strart;
        f_DBG_Print_Dec32(d_BASE_TICK_SYS_GET_US(v_Time_US));
    }
}

//===================================================================
/*### Systeick Base Log time stemp "Systick 로그 시스템 스템프"
---------------------------------------------------------------------
+ Print [0d-00:00:00.000000]
-------------------------------------------------------------------*/
void f_Base_Tick_Systick_Run_Time_Print(void)
{
    tu64 v_time = d_BASE_TICK_SYS_GET_US(f_Base_Tick_Systick_Get_Time64_us());
    f_DBG_Print_String("\r\n[");      
    f_DBG_Print_Dec32((tu32)(v_time / 86400000000));
    f_DBG_Print_String("d-");
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

#ifdef d_DWT_ENABLE
#define d_TICK_DWT_CNT          (DWT->CYCCNT)   //+ 32bit Tick counter
//===================================================================
// DWT Function "DWT 전용 함수"
//===================================================================
//===================================================================
/*### Tick DWT Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_Base_Tick_DWT_Init(void)
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
tu64 f_Base_Tick_DWT_Get_Time64_us(void)
{
    static tu64 v_tick_total = 0;
    static tu32 v_tick_pave = 0;        //+ Init value
    tu32 v_tick_now = d_TICK_DWT_CNT;   //+ 32bit up counter
    v_tick_total += (v_tick_pave - v_tick_now);
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
void f_Base_Tick_DWT_Stopwatch_Start(ts_Base_Tick_Context *ps_time, tu32 v_time_us)
{
    v_time_us     = d_BASE_TICK_DWT_GET_TICK(v_time_us);
    ps_time->v_pt = d_TICK_DWT_CNT;
    ps_time->v_ct = d_TICK_DWT_CNT;
    ps_time->v_st = v_time_us;
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
te_Return f_Base_Tick_DWT_Stopwatch_Check(ts_Base_Tick_Context* ps_time)
{
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
tu32 f_Base_Tick_DWT_Elapsed_Flag(ts_Base_Tick_Context* ps_time)
{
    tu32 v_wait = 1u;
    ps_time->v_ct = d_TICK_DWT_CNT;
    ps_time->v_tt += (ps_time->v_ct - ps_time->v_pt);
    ps_time->v_pt = ps_time->v_ct;
    if(ps_time->v_tt >= ps_time->v_st)
        { v_wait = 0u; }
    return(v_wait);
}

//===================================================================
/*### DWT Blocking Delay: DWT를 사용하여 코드 실행을 블로킹하면서 지연
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
Timeout : 10s
+ v_time_us : tu32
-------------------------------------------------------------------*/
void f_Base_Tick_DWT_Blocking_Delay(tu32 v_time_us)
{
    ts_Base_Tick_Context s_time;
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
void f_Base_Tick_DWT_Check_Start(tu32 *p_count)
{
    *p_count = d_TICK_DWT_CNT;
}

//===================================================================
/*### Difference (in microseconds) between the Tick stored via pointer 
    ### "기존에 포인터를 통해 저장한 Tick 과 현재 Tick 차이 US시간"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ p_count : Tick count vaule
+ Return : us(microsecond)
-------------------------------------------------------------------*/
tu32 f_Base_Tick_DWT_Check_Finish(tu32 *p_count)
{
    tu32 v_dwt_time = d_TICK_DWT_CNT;
    v_dwt_time = (v_dwt_time - *p_count);
    return d_BASE_TICK_DWT_GET_US(v_dwt_time);
}

//===================================================================
/*### DBG pirnt DWT start tick counter.
    ### "디버깅 포트를 통한 Tick 카운터 시작 정보 출력"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ void
-------------------------------------------------------------------*/
void f_Base_Tick_DWT_StartTime_Print(void)
{
    if(f_DBG_Get_Mode(m_DBG_MODE_SCHEDLUE) == m_YESNO_YES)
    {
        gv_base_time_strart = d_TICK_DWT_CNT;
        f_DBG_Print_String("\r\nI: Start DWT Count: ");
        f_DBG_Print_Dec32(gv_base_time_strart);
    }
}

//===================================================================
/*### DBG Print DWT end counter, run time 
    ### "디버깅 포트를 통한 측정시간 정보 출력"
---------------------------------------------------------------------
Warning: MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ void
-------------------------------------------------------------------*/
void f_Base_Tick_DWT_EndTime_Print(void)
{
    if(f_DBG_Get_Mode(m_DBG_MODE_SCHEDLUE) == m_YESNO_YES)
    {
        gv_base_time_end = d_TICK_DWT_CNT;
        f_DBG_Print_String("\r\nI: End DWT Count: ");
        f_DBG_Print_Dec32(gv_base_time_end);
        f_DBG_Print_String("\r\nI: Run Time: ");
        tu32 v_Time_US;
        v_Time_US = gv_base_time_end - gv_base_time_strart;
        f_DBG_Print_Dec32(d_BASE_TICK_DWT_GET_US(v_Time_US));
    }
}

//===================================================================
/*### DWT Base Log time stemp "DWT 로그 시스템 스템프"
---------------------------------------------------------------------
+ Print [0d-00:00:00.000000]
-------------------------------------------------------------------*/
void f_Base_Tick_DWT_Run_Time_Print(void)
{
    tu64 v_time = d_BASE_TICK_DWT_GET_US(f_Base_Tick_DWT_Get_Time64_us());
    f_DBG_Print_String("\r\n[");      
    f_DBG_Print_Dec32((tu32)(v_time / 86400000000));
    f_DBG_Print_String("d-");
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
#endif