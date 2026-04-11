/******************************************************************************
* File:    scheduler.h
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
#ifndef H_SCHEDULER_H
#define H_SCHEDULER_H

#include "../Base_LL/base_ll_timer_type.h"
#include "../DBG/dbg_type.h"
#include "scheduler_type.h"

void f_Scheduler_Config(
    ts_Scheduler_Ctrl *ps_sch_ctrl
    , te_SCHEDULER_PRIORITY e_priority_cfg
    , tu32 v_retry_max_count
    , tu32 v_timeout_us_cfg
    , tu32 v_cycle_run_time_us
    , void *p_user_struct);

void f_Scheduler_Init( 
    ts_Scheduler_Ctrl *ps_sch_ctrl
    , void(*ps_table[])(ts_Scheduler_Ctrl *ps_sch_ctrl)
    , char* p_table_name
    , tu32 v_table_size);

void f_Scheduler_Run(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_Next(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_Jump(ts_Scheduler_Ctrl *ps_sch_ctrl, tu32 v_jump_num);
tu32 f_Scheduler_Retry(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_Error(ts_Scheduler_Ctrl *ps_sch_ctrl, te_SCHEDULER_ERR e_error);
void f_Scheduler_RunTime_Start(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_RunTime_Finish(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_CycleTime_Update(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_RunTime_Info_Reset(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_CycleTime_Info_Reset(ts_Scheduler_Ctrl *ps_sch_ctrl);
void f_Scheduler_RunTime_Title_print(te_DBG_Mode e_level);
void f_Scheduler_RunTime_Info_print(ts_Scheduler_Ctrl *ps_sch_ctrl, te_DBG_Mode e_level, char *p_name_String);

#endif
