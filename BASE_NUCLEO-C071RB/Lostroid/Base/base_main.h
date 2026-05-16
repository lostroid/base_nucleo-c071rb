/********************************************************************
* File:    base_main.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_MAIN_H
#define H_BASE_MAIN_H

#include "../Scheduler/scheduler_type.h"
#include "base_tim_type.h"
#include "base_main_type.h"

typedef struct{
    ts_base_tick_context gs_time_1ms;
    ts_base_tick_context gs_time_10ms;
    ts_base_tick_context gs_time_100ms;
    ts_base_tick_context gs_time_500ms;
    ts_base_tick_context gs_time_1s;
}ts_base_main_context;


void f_base_main_init(void);
void f_base_main_module(void);
void f_base_main_time_init(void);
void f_base_main(void);
void f_base_main_vector_set(void);

void f_base_main_job_start(ts_scheduler_control *ps_job_ctrl);
void f_base_main_job_1ms(ts_scheduler_control *ps_job_ctrl);
void f_base_main_job_10ms(ts_scheduler_control *ps_job_ctrl);
void f_base_main_job_100ms(ts_scheduler_control *ps_job_ctrl);
void f_base_main_job_500ms(ts_scheduler_control *ps_job_ctrl);
void f_base_main_job_1s(ts_scheduler_control *ps_job_ctrl);

void f_base_main_load_print(void);
void f_base_main_system_print(void);

#endif
