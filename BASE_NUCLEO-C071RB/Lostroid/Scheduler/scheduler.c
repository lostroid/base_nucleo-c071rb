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

enum {M_SCH_MSG_LIST_LEN = 30};
//static char ga_sch_string_info[]  = "\r\nI-SCH: ";  //+ Scheduler info
static char ga_sch_string_error[] = "\r\nE-SCH: ";  //+ Scheduler error
// Mapping te_schedule_error "te_schedule_error와 매핑 필요"
static const char *gpa_sch_string_msg_list[M_SCH_MSG_LIST_LEN] = {
/* 00 */   "SCH_OK",                 
/* 01 */   "SCH_WAIT",               
/* 02 */   "SCH_ERR_ARG",            
/* 03 */   "SCH_ERR_BUG",            
/* 04 */   "SCH_ERR_BUSY",           
/* 05 */   "SCH_ERR_CRC",            
/* 06 */   "SCH_ERR_DONE",           
/* 07 */   "SCH_ERR_EMPTY",          
/* 08 */   "SCH_ERR_FRAME",          
/* 09 */   "SCH_ERR_HW",             
/* 10 */   "SCH_ERR_JOB_CTRL_NULL",  
/* 11 */   "SCH_ERR_JOB_JUMP_NULL",  
/* 12 */   "SCH_ERR_JOB_JUMP_OVER",  
/* 13 */   "SCH_ERR_JOB_NEXT_NULL",  
/* 14 */   "SCH_ERR_JOB_RUN_NULL",   
/* 15 */   "SCH_ERR_JOB_TABLE_NULL", 
/* 16 */   "SCH_ERR_JOB_TABLE_ZERO", 
/* 17 */   "SCH_ERR_LEN",            
/* 18 */   "SCH_ERR_LOCK",         
/* 19 */   "SCH_ERR_LIST_OVERFLOW",  
/* 20 */   "SCH_ERR_NOT_INIT",       
/* 21 */   "SCH_ERR_NULL",           
/* 22 */   "SCH_ERR_OVERFLOW",       
/* 23 */   "SCH_ERR_RETRY",          
/* 24 */   "SCH_ERR_SETTING",        
/* 25 */   "SCH_ERR_SWITCH_CASE",    
/* 26 */   "SCH_ERR_TIMEOUT",        
/* 27 */   "SCH_ERR_UNDERFLOW",      
/* 28 */   "SCH_ERR_UNKNOWN",        
/* 29 */   "SCH_ERR_VALUE"           
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
void f_scheduler_config(
    ts_scheduler_control *ps_sch_control    //+ variable pointer                  "제어 포인터"
    , tu32 v_penalty_set_value              //+ Penalty-based delay               "패널티에 따른 지연"
    , tu32 v_retry_set_value                //+ Retry count for response failure  "응답실패 재시도 횟수"
    , tu32 v_timeout_set_value_us           //+ Response timeout configuration    "응답 타임아웃 설정"
    , tu32 v_cycle_run_time_us              //+ Cycle time setting                "주기 타임 설정"
    , void *p_user_struct)                  //+ User structure registration       "사용자 구조체 등록"
{
    if(ps_sch_control == NULL)
    {
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    
    ps_sch_control->v_penalty_set_value     = v_penalty_set_value;   
    ps_sch_control->v_retry_set_value       = v_retry_set_value;
    ps_sch_control->v_timeout_set_value_us  = v_timeout_set_value_us;
    ps_sch_control->ps_user_struct          = p_user_struct;
    f_base_tick_systick32_stopwatch_start(&ps_sch_control->s_time_interval, v_cycle_run_time_us);
}

//===================================================================
/*#### Scheduler control Initialize "스케쥴러 테이블 제어 초기화"
---------------------------------------------------------------------
+ *ps_sch_control   : Control pointer       "제어 포인터"
+ *ps_table[]       : Pointer to table list "테이블 리스트 포인터"
+ *p_table_name     : Table name string     "테이블 이름 string"
+ v_table_size      : Table size            "테이블 사이즈"
-------------------------------------------------------------------*/
void f_scheduler_init( 
    ts_scheduler_control *ps_sch_control
    , void(*ps_table[])(ts_scheduler_control *ps_sch_control)
    , const char* p_table_name
    , tu32 v_table_size)
{
    if(ps_sch_control == NULL)
    {
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    if(ps_table == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_TABLE_NULL); 
        return; 
    }
    if(v_table_size == 0u)
    {
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_TABLE_ZERO); 
        return; 
    }
    ps_sch_control->e_error                 = m_SCHEDULER_OK;
    ps_sch_control->v_penalty_down_count    = 0;
    ps_sch_control->v_retry_down_count      = 0;
    ps_sch_control->v_time_run_now_us       = 0; 
    ps_sch_control->v_time_run_max_us       = 0;
    ps_sch_control->ps_table_list           = ps_table;
    ps_sch_control->p_table_name            = (char *)p_table_name;
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
void f_scheduler_run(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    if(ps_sch_control->pf_func == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_RUN_NULL);
        return;
    }

    //+ Check interval
    if(ps_sch_control-> v_penalty_down_count != 0)
    {
        ps_sch_control->v_penalty_down_count--;
        return;
    }
    f_scheduler_run_time_start(ps_sch_control);
    ps_sch_control->pf_func(ps_sch_control);
    f_scheduler_run_time_finish(ps_sch_control);
}

//===================================================================
/*#### Move to the next workflow "다음 테이블 위치 변경"
---------------------------------------------------------------------
Start of the next overflow position 0           "다음 위치가 넘는 경우 자동 0으로 동작"
+ *ps_sch_control : control variable pointer    "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_next(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    if(ps_sch_control->ps_table_list == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_RUN_NULL); 
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
void f_scheduler_jump(ts_scheduler_control *ps_sch_control, tu32 v_jump_num)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    if(ps_sch_control->v_table_size <= v_jump_num)
    {
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_JUMP_OVER);
        return;
    }
    if(ps_sch_control->ps_table_list == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_RUN_NULL); 
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
te_schedule_error f_scheduler_retry(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return m_SCHEDULER_ERR_JOB_CTRL_NULL; 
    }

    if(ps_sch_control->v_retry_down_count != 0)
    {
        ps_sch_control->v_retry_down_count--;
        if(ps_sch_control->v_retry_down_count == 0)
            { return m_SCHEDULER_ERR_RETRY; }
    }
    return m_SCHEDULER_OK;
}

//===================================================================
/*#### Error indication print "에러 정보 출력"
---------------------------------------------------------------------
+ *ps_sch_control   : variable pointer  "제어 포인터"
+ e_error           : Error type        "에러 종류"
-------------------------------------------------------------------*/
void f_scheduler_error(ts_scheduler_control *ps_sch_control, te_schedule_error e_error)
{
    if(ps_sch_control == d_NULL)
    { 
        f_dbg_print_string(gpa_sch_string_msg_list[(tu32)m_SCHEDULER_ERR_JOB_CTRL_NULL]); 
        return; 
    }
    //+ 처음 발생한 에러에 대해서만 한번 표시 이후 에러 표시는 무시되는 조건.
    if(ps_sch_control->e_error != m_SCHEDULER_OK)
        { return; }

    if((tu32)e_error >= M_SCH_MSG_LIST_LEN)
    {
        f_dbg_print_string(gpa_sch_string_msg_list[(tu32)m_SCHEDULER_ERR_JOB_CTRL_NULL]); 
        return; 
    }
    ps_sch_control->e_error = e_error; 
    f_dbg_print_string(ga_sch_string_error); 
    f_dbg_print_string(gpa_sch_string_msg_list[(tu32)e_error]); 
    f_dbg_print_string(" ");
    f_dbg_print_string(ps_sch_control->p_table_name);
    f_dbg_print_string(":");
    f_dbg_print_dec32(ps_sch_control->v_case_step);
}

//===================================================================
/*#### Scheduler setting start time "시작 타임 설정"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_run_time_start(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return;
    }
    f_base_tick_systick32_start(&ps_sch_control->v_time_run_count);
}

//===================================================================
/*#### Scheduler check finish time : 완료시간 측정
---------------------------------------------------------------------
update save time: now, max "현재,최대 시간 정보 저장"
Warning: run_Time MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ *ps_sch_control  : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_run_time_finish(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return;
    }
    tu32 v_time_us = f_base_tick_systick32_finish(&ps_sch_control->v_time_run_count);

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
/*#### Reset run time info "초기화 함수 수행 시간"
---------------------------------------------------------------------
+ *ps_sch_control  : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_run_time_info_reset(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
   ps_sch_control->v_time_run_now_us = 0;
   ps_sch_control->v_time_run_max_us = 0;
   ps_sch_control->v_time_run_tick_us = 0;
}
//===================================================================
/*#### Scheduler Cycle time "주기 타임"
---------------------------------------------------------------------
update save time: now, max "현재,최대 시간 정보 저장"
Warning: loop_Time MAX 17s = (4,294,967,295(32bit) / 250,000,000 Hz)
+ *ps_sch_control  : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_loop_time_update(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    tu32 v_time_us = f_base_tick_systick32_finish(&ps_sch_control->v_time_loop_count);
    f_base_tick_systick32_start(&ps_sch_control->v_time_loop_count);

    if(ps_sch_control->v_time_loop_now_us == 0)
    {  
        ps_sch_control->v_time_loop_now_us++;               //+ init Start
        ps_sch_control->v_time_loop_max_us = v_time_us;     //+ Set initial value
        ps_sch_control->v_time_loop_tick_us = v_time_us;    //+ Set initial value
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
/*#### Reset Loop time info "초기화 루프 수행 시간"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_loop_time_info_reset(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
   ps_sch_control->v_time_loop_now_us = 0;
   ps_sch_control->v_time_loop_max_us = 0;
   ps_sch_control->v_time_loop_tick_us = 0;
}
//===================================================================
/*#### Wait time Setting " (수행 대기시간 설정)"
+ Function to use f_scheduler_wait_check
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
+ v_time_us : microseconds
-------------------------------------------------------------------*/
void f_scheduler_wait_set(ts_scheduler_control *ps_sch_control, tu32 v_time_us)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    f_base_tick_systick32_start(&ps_sch_control->v_wait_tick_count);
    ps_sch_control->v_wait_set_us = v_time_us;
}
//===================================================================
/*#### Wait time check "수행 대기시간 확인"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
+ return : m_SCHEDULER_OK or m_SCHEDULER_WAIT
-------------------------------------------------------------------*/
te_schedule_error f_scheduler_wait_check(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return m_SCHEDULER_ERR_JOB_CTRL_NULL;
    }
    if( f_base_tick_systick32_finish(&ps_sch_control->v_wait_tick_count)
        >=  ps_sch_control->v_wait_set_us)
        { return m_SCHEDULER_OK; }
    return m_SCHEDULER_WAIT;
}
//===================================================================
/*#### Delay Setting "통신용 지연시간 설정"
+ Function to use f_scheduler_delay_check
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
+ v_time_us : microseconds
-------------------------------------------------------------------*/
void f_scheduler_delay_set(ts_scheduler_control *ps_sch_control, tu32 v_time_us)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return;
    }
    f_base_tick_systick32_start(&ps_sch_control->v_delay_tick_count);
    ps_sch_control->v_delay_set_us = v_time_us;
}
//===================================================================
/*#### Delay Check "통신용 지연시간 확인"
---------------------------------------------------------------------
+ *ps_sch_control : variable pointer "제어 포인터"
+ return : m_SCHEDULER_OK or m_SCHEDULER_WAIT
-------------------------------------------------------------------*/
te_schedule_error f_scheduler_delay_check(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return m_SCHEDULER_ERR_JOB_CTRL_NULL; 
    }
    if( f_base_tick_systick32_finish(&ps_sch_control->v_delay_tick_count)
        >=  ps_sch_control->v_delay_set_us)
        { return m_SCHEDULER_OK; }
    return m_SCHEDULER_WAIT;
}
//===================================================================
/*#### print time(NOW,MAX) title name "타임정보 제목 출력"
---------------------------------------------------------------------
+ e_level : Debug print level "디버깅 프린터 레벨"
-------------------------------------------------------------------*/
void f_scheduler_run_time_title_print(void)
{
    if(f_dbg_mode_get(m_DBG_MODE_SCHEDULE) == m_YESNO_YES)
    { 
        f_dbg_print_string("\r\nSystem infomation \r\n[Schedule name] [Run now/tick/max(us)] [Loop now/tick/max(us)]");
    }
}
//===================================================================
/*#### Scheduler run time print "스케쥴러 동작 시간 표시"
---------------------------------------------------------------------
+ *ps_sch_control   : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_run_time_info_print(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    if(f_dbg_mode_get(m_DBG_MODE_SCHEDULE) == m_YESNO_YES)
    {
        f_dbg_print_string("\r\nI: ");
        f_dbg_print_string(ps_sch_control->p_table_name);
        f_dbg_print_string("[");
        f_dbg_print_dec32(ps_sch_control->v_time_run_now_us);
        f_dbg_print_string("/");
        f_dbg_print_dec32(ps_sch_control->v_time_run_tick_us);
        f_dbg_print_string("/");
        f_dbg_print_dec32(ps_sch_control->v_time_run_max_us);
        f_dbg_print_string("] [");
        f_dbg_print_dec32(ps_sch_control->v_time_loop_now_us);
        f_dbg_print_string("/");
        f_dbg_print_dec32(ps_sch_control->v_time_loop_tick_us);
        f_dbg_print_string("/");
        f_dbg_print_dec32(ps_sch_control->v_time_loop_max_us);
        f_dbg_print_string("]");
    }
}

//===================================================================
/*#### Error Print Table Infomation "스케쥴러 에러 정보 위치 표시"
---------------------------------------------------------------------
+ *ps_sch_control   : variable pointer "제어 포인터"
-------------------------------------------------------------------*/
void f_scheduler_error_table_print(ts_scheduler_control *ps_sch_control)
{
    if(ps_sch_control == d_NULL)
    { 
        f_scheduler_error(ps_sch_control, m_SCHEDULER_ERR_JOB_CTRL_NULL); 
        return; 
    }
    if(f_dbg_mode_get(m_DBG_MODE_SCHEDULE) == m_YESNO_YES)
    {
    	f_base_tick_systick_run_time_print();
        f_dbg_print_string("\r\nE: ");
        f_dbg_print_string(ps_sch_control->p_table_name);
        f_dbg_print_string(" (Table:");
        f_dbg_print_dec32(ps_sch_control->v_table_pos);
        f_dbg_print_string(", Case: ");
        f_dbg_print_dec32(ps_sch_control->v_case_step);
        f_dbg_print_string(")");
        f_dbg_print_string(gpa_sch_string_msg_list[ps_sch_control->e_error]);
    }
}
