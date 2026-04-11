/********************************************************************
* File:    base_ll_main.h
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the main control.
*
* Revision History:
*   2026-02-19  New.
********************************************************************/
#ifndef H_LL_MAIN_H
#define H_LL_MAIN_H

#include "../Scheduler/scheduler_type.h"
#include "base_ll_timer_type.h"
#include "base_ll_main_type.h"

typedef struct
{
    ts_LL_Tick_Context gs_time_1ms;
    ts_LL_Tick_Context gs_time_10ms;
    ts_LL_Tick_Context gs_time_100ms;
    ts_LL_Tick_Context gs_time_500ms;
    ts_LL_Tick_Context gs_time_1s;
}ts_LL_Main_Context;


void f_LL_Main_Initialize(void);
void f_LL_Main_Module(void);
void f_LL_Main(void);
void f_LL_Main_Vector_Setting(void);
void f_LL_Main_Job_1ms(ts_Scheduler_Ctrl *ps_main_ctrl);
void f_LL_Main_Job_10ms(ts_Scheduler_Ctrl *ps_main_ctrl);
void f_LL_Main_Job_100ms(ts_Scheduler_Ctrl *ps_main_ctrl);
void f_LL_Main_Job_500ms(ts_Scheduler_Ctrl *ps_main_ctrl);
void f_LL_Main_Job_1s(ts_Scheduler_Ctrl *ps_main_ctrl);
void f_LL_Main_Load_Print(void);
void f_LL_Main_System_Print(void);

#endif
