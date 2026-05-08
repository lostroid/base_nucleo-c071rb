/********************************************************************
* File:    base_tim.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_TIM_H
#define H_BASE_TIM_H

#include "base_tim_type.h"
#include "base_tick_type.h"

void f_Base_TIMx_Init(void);
void f_Base_TIMx_Module(void);
void f_Base_TIM2_Init(void);
void f_Base_TIM2_CH1_CCR(tu16 v_cnt);
void f_Base_TIM2_CH2_CCR(tu16 v_cnt);
void f_Base_TIM2_CH1_Mode_set(te_Base_TIM_Mode e_mode);
void f_Base_TIM2_CH2_Mode_set(te_Base_TIM_Mode e_mode);
te_Return f_Base_TIM14_Check_100us(ts_Base_Tick_Context* ps_time);
void f_Base_TIMx_Input_Captur_Init(void);

void f_Base_TIM1_CH2_SW_A_Count_Up(void);
void f_Base_TIM1_CH3_SW_B_Count_Up(void);
void f_Base_TIM3_CH1_SW_OK_Count_Up(void);

tu32 f_Base_TIM1_CH2_SW_A_Get_Count(void);
tu32 f_Base_TIM1_CH3_SW_B_Get_Count(void);
tu32 f_Base_TIM3_CH1_SW_B_Get_Count(void);

void f_Base_TIMx_Info_Print(void);

#endif
