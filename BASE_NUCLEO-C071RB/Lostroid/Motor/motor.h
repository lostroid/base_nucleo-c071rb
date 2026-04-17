/********************************************************************
* File:    motor.h
* Author: LOSTROID
* Created: 2026-03-07
* Encoding: UTF-8
********************************************************************/
#ifndef H_MOTOR_H
#define H_MOTOR_H

#include "../Base/base_main_type.h"
#include "../Scheduler/scheduler_type.h"
#include "motor_type.h"

void f_Motor_Init(void);
void f_Motor_Module(void);
void f_Motor_Job_Start(ts_Scheduler_Control *ps_motor_ctrl);
void f_Motor_Job_SW_Delay(ts_Scheduler_Control *ps_motor_ctrl);
void f_Motor_Job_Idile(ts_Scheduler_Control *ps_motor_ctrl);
void f_Motor_Job_Select(ts_Scheduler_Control *ps_motor_ctrl);
void f_Motor_Job_Unknown(ts_Scheduler_Control *ps_motor_ctrl);
void f_Motor_Job_Open(ts_Scheduler_Control *ps_motor_ctrl);
void f_Motor_Job_Close(ts_Scheduler_Control *ps_motor_ctrl);
void f_Base_Motor_Load_Print(void);
#endif
