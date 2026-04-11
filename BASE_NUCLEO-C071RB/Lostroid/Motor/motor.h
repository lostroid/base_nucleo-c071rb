/******************************************************************************
* File:    motor.h
* Author: LOSTROID
* Created: 2026-03-07
*
* Description:
* This is the motor Control.
*
* Revision History:
*   2026-03-07  New.
******************************************************************************/
#ifndef H_MOTOR_H
#define H_MOTOR_H

#include "../Base_LL/base_ll_main_type.h"
#include "motor_type.h"

void f_Motor_Init(void);
void f_Motor_Module(void);
void f_Motor_Open_SW_Delay(ts_Scheduler_Ctrl *ps_motor_ctrl);
void f_Motor_Open_Idile(ts_Scheduler_Ctrl *ps_motor_ctrl);
void f_Motor_Open_Start(ts_Scheduler_Ctrl *ps_motor_ctrl);
void f_Motor_Open_Unknown(ts_Scheduler_Ctrl *ps_motor_ctrl);
void f_Motor_Open_Open(ts_Scheduler_Ctrl *ps_motor_ctrl);
void f_Motor_Open_Close(ts_Scheduler_Ctrl *ps_motor_ctrl);
void f_LL_Motor_Load_Print(void);
#endif
