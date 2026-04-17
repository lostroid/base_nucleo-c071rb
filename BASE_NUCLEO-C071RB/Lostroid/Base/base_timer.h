/********************************************************************
* File:    base_timer.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_TIMER_H
#define H_BASE_TIMER_H

#include "base_timer_type.h"
#include "base_tick_type.h"

void f_Base_Timer_Init(void);
void f_Base_Timer_Module(void);
void f_Base_TIMER1_Init(void);
void f_Base_TIMER1_CH1_CCR(tu16 v_cnt);
void f_Base_TIMER1_CH2_CCR(tu16 v_cnt);
void f_Base_TIMER1_CH1_Mode_set(te_Base_Timer_Mode e_mode);
void f_Base_TIMER1_CH2_Mode_set(te_Base_Timer_Mode e_mode);
te_Return f_Base_TIM14_Check_100us(ts_Base_Tick_Context* ps_time);

#endif
