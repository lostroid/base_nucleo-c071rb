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

void f_scheduler_config(
    ts_scheduler_control *ps_sch_control
    , tu32 v_penalty_set_value
    , tu32 v_retry_set_value
    , tu32 v_timeout_set_value_us
    , tu32 v_cycle_run_time_us
    , void *p_user_struct);

void f_scheduler_init( 
    ts_scheduler_control *ps_sch_control
    , void(*ps_table[])(ts_scheduler_control *ps_sch_control)
    , const char* p_table_name
    , tu32 v_table_size);
    
//+ Scheduler control
void f_scheduler_run(ts_scheduler_control *ps_sch_control);
void f_scheduler_next(ts_scheduler_control *ps_sch_control);
void f_scheduler_jump(ts_scheduler_control *ps_sch_control, tu32 v_jump_num);
te_schedule_error f_scheduler_retry(ts_scheduler_control *ps_sch_control);
void f_scheduler_error(ts_scheduler_control *ps_sch_control, te_schedule_error e_error);
//+ Scheduler time
void f_scheduler_run_time_start(ts_scheduler_control *ps_sch_control);
void f_scheduler_run_time_finish(ts_scheduler_control *ps_sch_control);
void f_scheduler_loop_time_update(ts_scheduler_control *ps_sch_control);
void f_scheduler_run_time_info_reset(ts_scheduler_control *ps_sch_control);
void f_scheduler_loop_time_info_reset(ts_scheduler_control *ps_sch_control);
//+ Shceduler delay
void f_scheduler_wait_set(ts_scheduler_control *ps_sch_control, tu32 v_time_us);
te_schedule_error f_scheduler_wait_check(ts_scheduler_control *ps_sch_control);
void f_scheduler_delay_set(ts_scheduler_control *ps_sch_control, tu32 v_time_us);
te_schedule_error f_scheduler_delay_check(ts_scheduler_control *ps_sch_control);
//+ Scheduler print
void f_scheduler_run_time_title_print(void);
void f_scheduler_run_time_info_print(ts_scheduler_control *ps_sch_control);
void f_scheduler_error_table_print(ts_scheduler_control *ps_sch_control);


#endif
