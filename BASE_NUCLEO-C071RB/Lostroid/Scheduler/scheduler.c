/********************************************************************
* File:    scheduler.c
* Author:  LOSTROID
* Created: 2025-06-23
* Encoding: UTF-8
********************************************************************/
#include <string.h>
#include "../DBG/dbg.h"
#include "../Base/base_main.h"
#include "../Base/base_uart.h"
#include "../Base/base_tick.h"
#include "scheduler.h"

#define d_SCH_MSG_LIST_LEN          17
//static char ga_sch_string_info[]  = "\r\nI-SCH: ";  //+ Scheduler info
static char ga_sch_string_error[] = "\r\nE-SCH: ";  //+ Scheduler error
// Mapping te_SCHEDULER_ERR "te_SCHEDULER_ERR와 매핑 필요"
static char *gpa_sch_string_msg_list[d_SCH_MSG_LIST_LEN] = {
    "NONE",                 //+ 00  m_SCH_ERROR_NONE
    "JOB_TABLE_NULL",       //+ 01  m_SCH_ERROR_JOB_TABLE_NULL
    "JOB_RUN_NULL",         //+ 02  m_SCH_ERROR_JOB_RUN_NULL
    "JOB_NEXT_NULL",        //+ 03  m_SCH_ERROR_JOB_NEXT_NULL
    "JOB_JUMP_NULL",        //+ 04  m_SCH_ERROR_JOB_JUMP_NULL
    "ACTION_TABLE_NULL",    //+ 05  m_SCH_ERROR_ACTION_TABLE_NULL
    "ACTION_RUN_NULL",      //+ 06  m_SCH_ERROR_ACTION_RUN_NULL
    "ACTION_NEXT_NULL",     //+ 07  m_SCH_ERROR_ACTION_NEXT_NULL
    "ACTION_JUMP_NULL",     //+ 08  m_SCH_ERROR_ACTION_JUMP_NULL
    "ACTION_JUMP_OVER",     //+ 09  m_SCH_ERROR_ACTION_JUMP_OVER
    "DONE",                 //+ 10  m_SCH_ERROR_DONE
    "LEN",                  //+ 11  m_SCH_ERROR_LEN
    "VALUE",                //+ 12  m_SCH_ERROR_VALUE
    "EMPTY",                //+ 13  m_SCH_ERROR_EMPTY
    "RETRY",                //+ 14  m_SCH_ERROR_RETRY
    "SWITCH_CASE",          //+ 15  m_SCH_ERROR_SWITCH_CASE
    "TIMEOUT"               //+ 16  m_SCH_ERROR_DRDY_TIMEOUT
};

//===================================================================
/*#### Setting scheduler config "스케쥴러 설정"
---------------------------------------------------------------------
+ *ps_sch_control           : variable pointer                  "제어 포인터"
+ v_penalty_set_value       : Penalty-based delay               "패널티에 따른 지연"
+ v_retry_set_value         : Retry count for response failure  "응답실패 재시도 횟수"
+ v_timeout_set_value_us    : Response timeout configuration    "응답 타임아웃 설정"
+ v_cycle_run_time_us       : Cycle time setting                "주기 타임 설정"
+ *p_user_struct            : User structure registration       "사용자 구조체 등록"
-------------------------------------------------------------------*/
void f_Scheduler_Config(
    ts_Scheduler_Control *ps_sch_control    //+ variable pointer                  "제어 포인터"
    , tu32 v_penalty_set_value              //+ Penalty-based delay               "패널티에 따른 지연"
    , tu32 v_retry_set_value                //+ Retry count for response failure  "응답실패 재시도 횟수"
    , tu32 v_timeout_set_value_us           //+ Response timeout configuration    "응답 타임아웃 설정"
    , tu32 v_cycle_run_time_us              //+ Cycle time setting                "주기 타임 설정"
    , void *p_user_struct)                  //+ User structure registration       "사용자 구조체 등록"
{
    ps_sch_control->v_penalty_set_value     = v_penalty_set_value;   
    ps_sch_control->v_retry_set_value       = v_retry_set_value;
    ps_sch_control->v_timeout_set_value_us  = v_timeout_set_value_us;
    ps_sch_control->ps_user_struct          = p_user_struct;
    f_Base_Tick_Systick32_Stopwatch_Start(&ps_sch_control->s_time_interval, v_cycle_run_time_us);
}

//===================================================================
/*#### Scheduler control Initialize "스케쥴러 테이블 제어 초기화"
---------------------------------------------------------------------
+ *ps_sch_control   : Control pointer       "제어 포인터"
+ *ps_table[]       : Pointer to table list "테이블 리스트 포인터"
+ *p_table_name     : Table name string     "테이블 이름 string"
+ v_table_size      : Table size            "테이블 사이즈"
-------------------------------------------------------------------*/
void f_Scheduler_Init( 
    ts_Scheduler_Control *ps_sch_control
    , void(*ps_table[])(ts_Scheduler_Control *)
    , char* p_table_name
    , tu32 v_table_size)
{
    if(ps_table == d_NULL)
    { 
        f_Scheduler_Error(ps_sch_control, m_SCH_ERROR_JOB_TABLE_NULL); 
        return; 
    }
    ps_sch_control->e_error                 = m_SCH_ERROR_JOB_TABLE_NULL;
    ps_sch_control->v_penalty_down_count    = 0;
    ps_sch_control->v_retry_down_count      = 0;
    ps_sch_control->v_time_run_now_us       = 0; 
    ps_sch_control->v_time_run_max_us       = 0;
    ps_sch_control->ps_table_list           = ps_table;
    ps_sch_control->p_table_name            = p_table_name;
    ps_sch_control->v_table_size            = v_table_size;
    ps_sch_control->v_table_pos             = 0;
    ps_sch_control->pf_func                 = ps_sch_control->ps_table_list[ps_sch_control->v_table_pos];
    ps_sch_control->v_case_step             = 0;
}

//===================================================================
/*#### Execute schedule "스케쥴러 함수 수행"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Run(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    //+ Check interval
    if(ps_sch_control-> v_penalty_down_count != 0)
    {
        ps_sch_control->v_penalty_down_count--;
        return;
    }
    f_Scheduler_RunTime_Start(ps_sch_control);
    ps_sch_control->pf_func(ps_sch_control);
    f_Scheduler_RunTime_Finish(ps_sch_control);
}

//===================================================================
/*#### Move to the next workflow "다음 테이블 위치 변경"
---------------------------------------------------------------------
Start of the next overflow position 0           "다음 위치가 넘는 경우 자동 0으로 동작"
+ *ps_sch_control : control variable pointer    "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Next(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    if(ps_sch_control->ps_table_list == d_NULL)
    { 
        f_Scheduler_Error(ps_sch_control, m_SCH_ERROR_JOB_RUN_NULL); 
        return;
    }
    ps_sch_control->v_table_pos++;
    if(ps_sch_control->v_table_size <= ps_sch_control->v_table_pos)
        { ps_sch_control->v_table_pos = 0;}
    ps_sch_control->pf_func = ps_sch_control->ps_table_list[ps_sch_control->v_table_pos];
    ps_sch_control->v_case_step = 0;
    ps_sch_control->v_retry_down_count = ps_sch_control->v_retry_set_value;
}

//===================================================================
/*#### Scheduler job Jump "사용자 테이블 위치 변경"
---------------------------------------------------------------------
Jump number 0 ~ (MAX table size -1).
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Jump(ts_Scheduler_Control *ps_sch_control, tu32 v_jump_num)
{
    if(ps_sch_control == d_NULL)
        { return; }
    if(ps_sch_control->v_table_size < v_jump_num)
    {
        f_Scheduler_Error(ps_sch_control, m_SCH_ERROR_JOB_RUN_NULL);
        return;
    }
    ps_sch_control->v_table_pos = v_jump_num;
    ps_sch_control->pf_func = ps_sch_control->ps_table_list[ps_sch_control->v_table_pos];               
    ps_sch_control->v_case_step = 0;
    ps_sch_control->v_retry_down_count = ps_sch_control->v_retry_set_value;
}

//===================================================================
/*#### Scheduler job retry "오류 재시도 횟수 측정"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
tu32 f_Scheduler_Retry(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return 1; }

    if(ps_sch_control->v_retry_down_count != 0)
    {
        ps_sch_control->v_retry_down_count--;
        if(ps_sch_control->v_retry_down_count == 0)
            { return 1; }
    }
    return 0;
}

//===================================================================
/*#### Error indication print "에러 정보 출력"
---------------------------------------------------------------------
+ *ps_sch_control   : variable pointer  "제어 포인터"
+ e_error           : Error type        "에러 종류"
-------------------------------------------------------------------*/
void f_Scheduler_Error(ts_Scheduler_Control *ps_sch_control, te_SCHEDULER_ERR e_error)
{
    if(ps_sch_control == d_NULL)
        { return; }
    //+ 처음 발생한 에러에 대해서만 한번 표시 이후 에러 표시는 무시되는 조건.
    if(ps_sch_control->e_error != m_SCH_ERROR_NONE)
        { return; }
    ps_sch_control->e_error = e_error; 
    f_DBG_Print_String(ga_sch_string_error); 
    f_DBG_Print_String(gpa_sch_string_msg_list[(tu32)e_error]); 
    f_DBG_Print_String(" ");
    f_DBG_Print_String(ps_sch_control->p_table_name);
    f_DBG_Print_String(":");
    f_DBG_Print_Dec32(ps_sch_control->v_case_step);
}

//===================================================================
/*#### Scheduler setting start time "시작 타임 설정"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_RunTime_Start(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    f_Base_Tick_Systick32_Start(&ps_sch_control->v_time_run_count);
}

//===================================================================
/*#### Scheduler check finish time : 완료시간 측정
---------------------------------------------------------------------
update save time: now, max "현재,최대 시간 정보 저장"
Warning: run_Time MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ *ps_sch_control  : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_RunTime_Finish(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    tu32 v_time_us = f_Base_Tick_Systick32_Finish(&ps_sch_control->v_time_run_count);

    if(ps_sch_control->v_time_run_now_us == 0)
    {  
        ps_sch_control->v_time_run_now_us++;               //+ init Start
        ps_sch_control->v_time_run_max_us = v_time_us;     //+ Set initial value
        ps_sch_control->v_time_run_tick_us = v_time_us;  //+ Set initial value
        return;
    }
    ps_sch_control->v_time_run_now_us = v_time_us;

    //+ Check maximum value
    if(ps_sch_control->v_time_run_max_us < v_time_us)
        { ps_sch_control->v_time_run_max_us = v_time_us; }

    //+ Calculate average
    if(ps_sch_control->v_time_run_tick_us < v_time_us)
    { 
        v_time_us = ((v_time_us - ps_sch_control->v_time_run_tick_us) / 100); 
        if(v_time_us == 0)
            { v_time_us++; }
        ps_sch_control->v_time_run_tick_us += v_time_us; 
    }
    else
    {
        v_time_us = (ps_sch_control->v_time_run_tick_us - v_time_us);
        if(v_time_us == 0)
            { v_time_us++; }
        if(ps_sch_control->v_time_run_tick_us > v_time_us)
            { ps_sch_control->v_time_run_tick_us -= v_time_us; }
        else
            { ps_sch_control->v_time_run_tick_us = 0; }
    }
}

//===================================================================
/*#### Scheduler Cycle time "주기 타임"
---------------------------------------------------------------------
update save time: now, max "현재,최대 시간 정보 저장"
Warning: loop_Time MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ *ps_sch_control  : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_LoopTime_Update(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    tu32 v_time_us = f_Base_Tick_Systick32_Finish(&ps_sch_control->v_time_loop_count);
    f_Base_Tick_Systick32_Start(&ps_sch_control->v_time_loop_count);

    if(ps_sch_control->v_time_loop_now_us == 0)
    {  
        ps_sch_control->v_time_loop_now_us++;               //+ init Start
        ps_sch_control->v_time_loop_max_us = v_time_us;     //+ Set initial value
        ps_sch_control->v_time_loop_tick_us = v_time_us;  //+ Set initial value
        return;
    }
    ps_sch_control->v_time_loop_now_us = v_time_us;
    
    //+ Check maximum value
    if(ps_sch_control->v_time_loop_max_us < v_time_us)
        { ps_sch_control->v_time_loop_max_us = v_time_us; }

    //+ Calculate average
    if(ps_sch_control->v_time_loop_tick_us < v_time_us)
    { 
        v_time_us = ((v_time_us - ps_sch_control->v_time_loop_tick_us) / 100); 
        if(v_time_us == 0)
            { v_time_us++; }
        ps_sch_control->v_time_loop_tick_us += v_time_us; 
    }
    else
    {
        v_time_us = (ps_sch_control->v_time_loop_tick_us - v_time_us);
        if(v_time_us == 0)
            { v_time_us++; }
        if(ps_sch_control->v_time_loop_tick_us > v_time_us)
            { ps_sch_control->v_time_loop_tick_us -= v_time_us; }
        else
            { ps_sch_control->v_time_loop_tick_us = 0; }
    }
}

//===================================================================
/*#### Reset run time info "초기화 함수 수행 시간"
---------------------------------------------------------------------
+ *ps_sch_control  : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_RunTime_Info_Reset(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
   ps_sch_control->v_time_run_now_us = 0;
   ps_sch_control->v_time_run_max_us = 0;
   ps_sch_control->v_time_run_tick_us = 0;
}

//===================================================================
/*#### Reset Loop time info "초기화 루프 수행 시간"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_LoopTime_Info_Reset(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
   ps_sch_control->v_time_loop_now_us = 0;
   ps_sch_control->v_time_loop_max_us = 0;
   ps_sch_control->v_time_loop_tick_us = 0;
}

//===================================================================
/*#### JOB to JOB delay "JOB 간격 딜레이" (통신 딜레이용)
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Job_To_Job_Set_Delay(ts_Scheduler_Control *ps_sch_control, tu32 v_time_us)
{
    if(ps_sch_control == d_NULL)
        { return; }
    f_Base_Tick_Systick32_Start(&ps_sch_control->v_delay_tick_count);
    ps_sch_control->v_delay_set_us = v_time_us;
    f_Scheduler_Next(ps_sch_control);
}

//===================================================================
/*#### JOB to JOB delay "JOB 간격 딜레이" (통신 딜레이용)
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Job_To_Job_Delay(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    if( f_Base_Tick_Systick32_Finish(&ps_sch_control->v_delay_tick_count)
        >=  ps_sch_control->v_delay_set_us)
        { f_Scheduler_Next(ps_sch_control); }
}

//===================================================================
/*#### JOB to JOB delay "JOB 대기 딜레이 (기능동작 대기 용)"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Job_To_Job_Set_Wait(ts_Scheduler_Control *ps_sch_control, tu32 v_time_us)
{
    if(ps_sch_control == d_NULL)
        { return; }
    f_Base_Tick_Systick32_Start(&ps_sch_control->v_wait_tick_count);
    ps_sch_control->v_wait_set_us = v_time_us;
    f_Scheduler_Next(ps_sch_control);

}

//===================================================================
/*#### JOB to JOB wait "JOB  대기 딜레 (기능동작 대기 용)"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Job_To_Job_Wait(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    if( f_Base_Tick_Systick32_Finish(&ps_sch_control->v_wait_tick_count)
        >=  ps_sch_control->v_wait_set_us)
        { f_Scheduler_Next(ps_sch_control); }
}

//===================================================================
/*#### print time(NOW,MAX) title name "타임정보 제목 출력"
---------------------------------------------------------------------
+ e_level : Debug print level "디버깅 프린터 레벨"
-------------------------------------------------------------------*/
void f_Scheduler_RunTime_Title_print(void)
{
    if(f_DBG_Get_Mode(m_DBG_MODE_SCHEDLUE) == m_YESNO_YES)
    { 
        f_DBG_Print_String("\r\nSystem infomation \r\n[Schedlue name] [Run now/tick/max(us)] [Loop now/tick/max(us)]");
    }
}

//===================================================================
/*#### Scheduler run time print "스케쥴러 동작 시간 표시"
---------------------------------------------------------------------
+ *ps_sch_control   : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_RunTime_Info_print(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    if(f_DBG_Get_Mode(m_DBG_MODE_SCHEDLUE) == m_YESNO_YES)
    {
        f_DBG_Print_String("\r\nI: ");
        f_DBG_Print_String(ps_sch_control->p_table_name);
        f_DBG_Print_String("[");
        f_DBG_Print_Dec32(ps_sch_control->v_time_run_now_us);
        f_DBG_Print_String("/");
        f_DBG_Print_Dec32(ps_sch_control->v_time_run_tick_us);
        f_DBG_Print_String("/");
        f_DBG_Print_Dec32(ps_sch_control->v_time_run_max_us);
        f_DBG_Print_String("] [");
        f_DBG_Print_Dec32(ps_sch_control->v_time_loop_now_us);
        f_DBG_Print_String("/");
        f_DBG_Print_Dec32(ps_sch_control->v_time_loop_tick_us);
        f_DBG_Print_String("/");
        f_DBG_Print_Dec32(ps_sch_control->v_time_loop_max_us);
        f_DBG_Print_String("]");
    }
}

//===================================================================
/*#### Error Print Table Infomation "스케쥴러 에러 정보 위치 표시"
---------------------------------------------------------------------
+ *ps_sch_control   : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_Scheduler_Error_Table_print(ts_Scheduler_Control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
        { return; }
    if(f_DBG_Get_Mode(m_DBG_MODE_SCHEDLUE) == m_YESNO_YES)
    {
        f_Base_Tick_Systick_Run_Time_Print();
        f_DBG_Print_String("\r\nE: Invalid");
        f_DBG_Print_String(ps_sch_control->p_table_name);
        f_DBG_Print_String(" Table=");
        f_DBG_Print_Dec32(ps_sch_control->v_table_pos);
        f_DBG_Print_String(", Case=");
        f_DBG_Print_Dec32(ps_sch_control->v_case_step);
        f_DBG_Print_String(" ");
    }
}
