/********************************************************************
* File:    scheduler.h
* Author:  LOSTROID
* Created: 2025-06-23
* Encoding: UTF-8
********************************************************************/
#ifndef H_SCHEDULER_H
#define H_SCHEDULER_H

#include "../DBG/dbg_type.h"
#include "scheduler_type.h"

void f_Scheduler_Config(
    ts_Scheduler_Control *ps_sch_control
    , tu32 v_wait_penalty_count
    , tu32 v_retry_max_count
    , tu32 v_timeout_us_cfg
    , tu32 v_cycle_run_time_us
    , void *p_user_struct);

void f_Scheduler_Init( 
    ts_Scheduler_Control *ps_sch_control
    , void(*ps_table[])(ts_Scheduler_Control *ps_sch_control)
    , char* p_table_name
    , tu32 v_table_size);

void f_Scheduler_Run(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_Next(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_Jump(ts_Scheduler_Control *ps_sch_control, tu32 v_jump_num);
tu32 f_Scheduler_Retry(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_Error(ts_Scheduler_Control *ps_sch_control, te_SCHEDULER_ERR e_error);
void f_Scheduler_RunTime_Start(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_RunTime_Finish(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_LoopTime_Update(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_RunTime_Info_Reset(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_LoopTime_Info_Reset(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_RunTime_Title_print(void);
void f_Scheduler_RunTime_Info_print(ts_Scheduler_Control *ps_sch_control);
void f_Scheduler_Error_Table_print(ts_Scheduler_Control *ps_sch_control);

#endif
