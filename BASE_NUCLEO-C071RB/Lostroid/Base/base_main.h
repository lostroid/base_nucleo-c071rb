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
    ts_Base_Tick_Context gs_time_1ms;
    ts_Base_Tick_Context gs_time_10ms;
    ts_Base_Tick_Context gs_time_100ms;
    ts_Base_Tick_Context gs_time_500ms;
    ts_Base_Tick_Context gs_time_1s;
}ts_Base_Main_Context;


void f_Base_Main_Initialize(void);
void f_Base_Main_Module(void);
void f_Base_Main_Time_Init(void);
void f_Base_Main(void);
void f_Base_Main_Vector_Setting(void);
void f_Base_Main_Job_Start(ts_Scheduler_Control *ps_job_ctrl);
void f_Base_Main_Job_1ms(ts_Scheduler_Control *ps_job_ctrl);
void f_Base_Main_Job_10ms(ts_Scheduler_Control *ps_job_ctrl);
void f_Base_Main_Job_100ms(ts_Scheduler_Control *ps_job_ctrl);
void f_Base_Main_Job_500ms(ts_Scheduler_Control *ps_job_ctrl);
void f_Base_Main_Job_1s(ts_Scheduler_Control *ps_job_ctrl);
void f_Base_Main_Load_Print(void);
void f_Base_Main_System_Print(void);

#endif
