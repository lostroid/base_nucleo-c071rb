/******************************************************************************
* File:    scheduler.c
* Author:  LOSTROID
* Created: 2025-06-23
*
* Description:
* This is the Scheduler Control.
*
* Revision History:
*   2026-02-23  v1.01  UPDATE
*   2025-06-23  v1.00  NEW
******************************************************************************/
#include <string.h>
#include "../DBG/dbg.h"
#include "../Base_LL/base_ll_main.h"
#include "../Base_LL/base_ll_uart.h"
#include "../Base_LL/base_ll_tick.h"
#include "scheduler.h"

static char ga_sch_string_info[]  = "\r\nI-SCH: ";  // Scheduler info
static char ga_sch_string_error[] = "\r\nE-SCH: ";  // Scheduler error
static char *gpa_sch_string_msg_list[16] = {       	// Duble check array num [숫자표시 더블 체크를 위한]
    "NONE",                 // 00  m_SCH_ERROR_NONE
    "JOB_TABLE_NULL",       // 01  m_SCH_ERROR_JOB_TABLE_NULL
    "JOB_RUN_NULL",         // 02  m_SCH_ERROR_JOB_RUN_NULL
    "JOB_NEXT_NULL",        // 03  m_SCH_ERROR_JOB_NEXT_NULL
    "JOB_JUMP_NULL",        // 04  m_SCH_ERROR_JOB_JUMP_NULL
    "ACTION_TABLE_NULL",    // 05  m_SCH_ERROR_ACTION_TABLE_NULL
    "ACTION_RUN_NULL",      // 06  m_SCH_ERROR_ACTION_RUN_NULL
    "ACTION_NEXT_NULL",     // 07  m_SCH_ERROR_ACTION_NEXT_NULL
    "ACTION_JUMP_NULL",     // 08  m_SCH_ERROR_ACTION_JUMP_NULL
    "ACTION_JUMP_OVER",     // 09  m_SCH_ERROR_ACTION_JUMP_OVER
    "DONE",                 // 10  m_SCH_ERROR_DONE
    "LEN",                  // 11  m_SCH_ERROR_LEN
    "VALUE",                // 12  m_SCH_ERROR_VALUE
    "EMPTY",                // 13  m_SCH_ERROR_EMPTY
    "RETRY",                // 14  m_SCH_ERROR_RETRY
    "SWITCH_CASE"           // 15  m_SCH_ERROR_SWITCH_CASE
    "TIMEOUT"               // 16  m_SCH_ERROR_DRDY_TIMEOUT
};
static te_SCHEDULER_PRIORITY gv_priority = m_SCH_PRIORITY_LV0;
//============================================================================
/* Setting scheduler config (스케쥴러 설정) 
-------------------------------------------------------------------------------
+ *ps_sch_ctrl                  : variable pointer (제어 포인터)
+ e_priority_cfg            : 우선순위 설정
+ v_retry_max_count         : 응답실패 재시도 횟수
+ v_timeout_us_cfg          : 응답 타임아웃 설정
+ v_cycle_run_time_us       : 주기 타임 설정
+ *p_user_struct            : 사용자 구조체 등록 
---------------------------------------------------------------------------- */
void f_Scheduler_Config(
    ts_Scheduler_Ctrl *ps_sch_ctrl
    , te_SCHEDULER_PRIORITY e_priority_cfg
    , tu32 v_retry_max_count
    , tu32 v_timeout_us_cfg
    , tu32 v_cycle_run_time_us
    , void *p_user_struct)
{
    ps_sch_ctrl->e_priority_cfg     = e_priority_cfg;       /// 우선 순위 설정
    ps_sch_ctrl->v_retry_max_count  = v_retry_max_count;    /// 재시도 횟수
    ps_sch_ctrl->v_timeout_us_cfg   = v_timeout_us_cfg;     /// 응답 타임아웃
    ps_sch_ctrl->ps_user_struct     = p_user_struct;        /// 유저 구조체
    f_LL_Tick_Systick_Base32_Capture(&ps_sch_ctrl->s_cycle_run_time, v_cycle_run_time_us);
}
//===================================================================
/* Scheduler control Initialize (스케쥴러 테이블 제어 초기화)
-------------------------------------------------------------------------------
+ *ps_sch_ctrl      : variable pointer (제어 포인터)
+ *ps_table[]   : 테이블 리스트 포인터
+ *p_table_name : 테이블 이름 "string"
+ v_table_size  : 테이블 사이즈
---------------------------------------------------------------------------- */
void f_Scheduler_Init( 
    ts_Scheduler_Ctrl *ps_sch_ctrl
    , void(*ps_table[])(ts_Scheduler_Ctrl *)
    , char* p_table_name
    , tu32 v_table_size)
{
    if(ps_table == d_NULL)
    { 
        f_Scheduler_Error(ps_sch_ctrl, m_SCH_ERROR_JOB_TABLE_NULL); 
        return; 
    }
    ps_sch_ctrl->e_error            = m_SCH_ERROR_JOB_TABLE_NULL;
    ps_sch_ctrl->v_interval_uncount = 0;
    ps_sch_ctrl->v_retry_countdown    = 0;
    ps_sch_ctrl->v_time_run_now_us  = 0; 
    ps_sch_ctrl->v_time_run_max_us  = 0;
    ps_sch_ctrl->ps_table_list      = ps_table;
    ps_sch_ctrl->p_table_name       = p_table_name;
    ps_sch_ctrl->v_table_size       = v_table_size;
    ps_sch_ctrl->v_table_pos        = 0;
    ps_sch_ctrl->pf_func            = ps_sch_ctrl->ps_table_list[ps_sch_ctrl->v_table_pos];
    ps_sch_ctrl->v_case_step        = 0;
}
//===================================================================
/*### Execute schedule (스케쥴러 함수 수행)
-------------------------------------------------------------------------------
+ *ps_sch_ctrl   : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_Run(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
    // Check pariority
    if((tu32)ps_sch_ctrl->e_priority_cfg < (tu32)gv_priority)
        { return; }
    // Check interval
    if(ps_sch_ctrl-> v_interval_uncount != 0)
    {
        ps_sch_ctrl->v_interval_uncount--;
        return;
    }
    f_Scheduler_RunTime_Start(ps_sch_ctrl);
    ps_sch_ctrl->pf_func(ps_sch_ctrl);
    f_Scheduler_RunTime_Finish(ps_sch_ctrl);
}
//===================================================================
/* Move to the next workflow (다음 테이블 위치 변경)
-------------------------------------------------------------------------------
Start of the next overflow position (0).
+ *ps_sch_ctrl   : control variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_Next(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
    if(ps_sch_ctrl->ps_table_list == d_NULL)
    { 
        f_Scheduler_Error(ps_sch_ctrl, m_SCH_ERROR_JOB_RUN_NULL); 
        return;
    }
    ps_sch_ctrl->v_table_pos++;
    if(ps_sch_ctrl->v_table_size <= ps_sch_ctrl->v_table_pos)
        { ps_sch_ctrl->v_table_pos = 0;}
    ps_sch_ctrl->pf_func = ps_sch_ctrl->ps_table_list[ps_sch_ctrl->v_table_pos];
    ps_sch_ctrl->v_case_step = 0;
    ps_sch_ctrl->v_retry_countdown = ps_sch_ctrl->v_retry_max_count;
}
//===================================================================
/* Scheduler job Jump (사용자 테이블 위치 변경)
-------------------------------------------------------------------------------
Jump number 0 ~ MAX table size -1.
+ *ps_sch_ctrl   : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_Jump(ts_Scheduler_Ctrl *ps_sch_ctrl, tu32 v_jump_num)
{
    if(ps_sch_ctrl->v_table_size < v_jump_num)
    {
        f_Scheduler_Error(ps_sch_ctrl, m_SCH_ERROR_JOB_RUN_NULL);
        return;
    }
    ps_sch_ctrl->v_table_pos = v_jump_num;
    ps_sch_ctrl->pf_func = ps_sch_ctrl->ps_table_list[ps_sch_ctrl->v_table_pos];               
    ps_sch_ctrl->v_case_step = 0;
    ps_sch_ctrl->v_retry_countdown = ps_sch_ctrl->v_retry_max_count;
}
//===================================================================
/* Scheduler job retry (오류 재시도 횟수 측정)
-------------------------------------------------------------------------------
+ *ps_sch_ctrl   : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
tu32 f_Scheduler_Retry(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
    tu32 v_error = 0;
    if(ps_sch_ctrl->v_retry_countdown != 0)
    {
        ps_sch_ctrl->v_retry_countdown--;
        if(ps_sch_ctrl->v_retry_countdown == 0)
            { v_error++; }
    }
    return v_error;
}
//===================================================================
/* Error indication print (에러 정보 출력)
-------------------------------------------------------------------------------
+ *ps_sch_ctrl  : variable pointer (제어 포인터)
+ e_error   : Error type (에러 종류)
---------------------------------------------------------------------------- */
void f_Scheduler_Error(ts_Scheduler_Ctrl *ps_sch_ctrl, te_SCHEDULER_ERR e_error)
{
    // 처음 발생한 에러에 대해서만 한번 표시 이후 에러 표시는 무시되는 조건.
    if(ps_sch_ctrl->e_error != m_SCH_ERROR_NONE)
        { return; }
    ps_sch_ctrl->e_error = e_error; 
    f_DBG_Print_String(ga_sch_string_error); 
    f_DBG_Print_String(gpa_sch_string_msg_list[(tu32)e_error]); 
    f_DBG_Print_String(" ");
    f_DBG_Print_String(ps_sch_ctrl->p_table_name);
    f_DBG_Print_String(":");
    f_DBG_Print_Dec32(ps_sch_ctrl->v_case_step);
}
//===================================================================
/* Scheduler setting start time (시작 타임 설정)
-------------------------------------------------------------------------------
+ *ps_sch_ctrl  : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_RunTime_Start(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
    f_LL_Tick_Systick32_Start(&ps_sch_ctrl->v_time_run_count);
}
//===================================================================
/*### Scheduler check finish time (완료시간 측정)
-------------------------------------------------------------------------------
update save time: now, max (현재,최대 시간 정보 저장)
Warning: run_Time MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ *ps_sch_ctrl  : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_RunTime_Finish(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
    tu32 v_time_us = f_LL_Tick_Systick32_Finish(&ps_sch_ctrl->v_time_run_count);
    if(ps_sch_ctrl->v_time_run_now_us == 0)
    {  
        ps_sch_ctrl->v_time_run_now_us++;           // 0은 초기값으로 인지 하기 때문에 1증가
        ps_sch_ctrl->v_time_run_max_us = v_time_us; // 처음 값이라 MAX로 바로 저장
        ps_sch_ctrl->v_time_run_avg100_us = v_time_us;
        return;                                 // 초기 값이므로 조기 종료
    }
    ps_sch_ctrl->v_time_run_now_us = v_time_us;
    
    if(ps_sch_ctrl->v_time_run_max_us < v_time_us)
        { ps_sch_ctrl->v_time_run_max_us = v_time_us; }

    // avg 평균 추이
    if(v_time_us > ps_sch_ctrl->v_time_run_avg100_us)
        { ps_sch_ctrl->v_time_run_avg100_us += ((v_time_us - ps_sch_ctrl->v_time_run_avg100_us) / 100); }
    else
    {
        v_time_us = (ps_sch_ctrl->v_time_run_avg100_us - v_time_us);
        if(ps_sch_ctrl->v_time_run_avg100_us > v_time_us)
            { ps_sch_ctrl->v_time_run_avg100_us -= v_time_us; }
        else
            { ps_sch_ctrl->v_time_run_avg100_us = 0; }
    }
}
//===================================================================
/*### Scheduler Cycle time (주기 타임)
-------------------------------------------------------------------------------
update save time: now, max (현재,최대 시간 정보 저장)
Warning: run_Time MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ *ps_sch_ctrl  : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_CycleTime_Update(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
    tu32 v_time_us = f_LL_Tick_Systick32_Finish(&ps_sch_ctrl->v_time_run_count);
    f_LL_Tick_Systick32_Start(&ps_sch_ctrl->v_time_run_count);

    if(ps_sch_ctrl->v_time_cycle_now_us == 0)
    {  
        ps_sch_ctrl->v_time_cycle_now_us++;           // 0은 초기값으로 인지 하기 때문에 1증가
        ps_sch_ctrl->v_time_cycle_max_us = v_time_us; // 처음 값이라 MAX로 바로 저장
        ps_sch_ctrl->v_time_cycle_avg100_us = v_time_us;
        return;                                 // 초기 값이므로 조기 종료
    }
    ps_sch_ctrl->v_time_cycle_now_us = v_time_us;
    
    if(ps_sch_ctrl->v_time_cycle_max_us < v_time_us)
        { ps_sch_ctrl->v_time_cycle_max_us = v_time_us; }

    // avg 평균 추이
    if(v_time_us > ps_sch_ctrl->v_time_cycle_avg100_us)
        { ps_sch_ctrl->v_time_cycle_avg100_us += ((v_time_us - ps_sch_ctrl->v_time_cycle_avg100_us) / 100); }
    else
    {
        v_time_us = (ps_sch_ctrl->v_time_cycle_avg100_us - v_time_us);
        if(ps_sch_ctrl->v_time_cycle_avg100_us > v_time_us)
            { ps_sch_ctrl->v_time_cycle_avg100_us -= v_time_us; }
        else
            { ps_sch_ctrl->v_time_cycle_avg100_us = 0; }
    }
}
//===================================================================
/* Reset run time info.
-------------------------------------------------------------------------------
+ *ps_sch_ctrl  : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_RunTime_Info_Reset(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
   ps_sch_ctrl->v_time_run_now_us = 0;
   ps_sch_ctrl->v_time_run_max_us = 0;
   ps_sch_ctrl->v_time_run_avg100_us = 0;
}
//===================================================================
/* Reset Cycle time info.
-------------------------------------------------------------------------------
+ *ps_sch_ctrl  : variable pointer (제어 포인터)
---------------------------------------------------------------------------- */
void f_Scheduler_CycleTime_Info_Reset(ts_Scheduler_Ctrl *ps_sch_ctrl)
{
   ps_sch_ctrl->v_time_cycle_now_us = 0;
   ps_sch_ctrl->v_time_cycle_max_us = 0;
   ps_sch_ctrl->v_time_cycle_avg100_us = 0;
}
//===================================================================
/* print time(NOW,MAX) title name (타임정보 제목 출력)
-------------------------------------------------------------------------------
+ e_level  : Debug print level (디버깅 프린터 레벨)
---------------------------------------------------------------------------- */
void f_Scheduler_RunTime_Title_print(te_DBG_Mode e_mode)
{
    if(f_DBG_Get_Mode(e_mode) != 0)
    	{ f_DBG_Print_String(" System infomation Schedlue \r\n[name] [Now/avg100/max(us)]"); }
}
//===================================================================
/* print time(NOW,MAX) value (타임 정보 출력)
-------------------------------------------------------------------------------
+ *ps_sch_ctrl          : variable pointer (제어 포인터)
+ e_level           : Debug print level (디버깅 프린터 레벨)
+ *p_name_String    : time name (시간 정보 이름)
---------------------------------------------------------------------------- */
void f_Scheduler_RunTime_Info_print(ts_Scheduler_Ctrl *ps_sch_ctrl, te_DBG_Mode e_mode, char *p_name_String)
{
    if(f_DBG_Get_Mode(e_mode) != 0)
    {
        f_DBG_Print_String("\r\nI: ");
        f_DBG_Print_String(p_name_String);
        f_DBG_Print_String(" ");
        f_DBG_Print_Dec32(ps_sch_ctrl->v_time_run_now_us);
        f_DBG_Print_String("/");
        f_DBG_Print_Dec32(ps_sch_ctrl->v_time_run_avg100_us);
        f_DBG_Print_String("/");
        f_DBG_Print_Dec32(ps_sch_ctrl->v_time_run_max_us);
    }
}
