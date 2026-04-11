/********************************************************************
* File:    base_ll_timer.h
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the time header.
*
* Revision History:
*   2026-02-19  New.
********************************************************************/
#ifndef H_LL_TIMER_H
#define H_LL_TIMER_H

#include "base_ll_timer_type.h"

void f_LL_TIMERx_Init(void);
void f_LL_TIMERx_Module(void);
void f_LL_TIMER1_Init(void);
void f_LL_TIMER1_CH1_CCR(tu16 v_cnt);
void f_LL_TIMER1_CH2_CCR(tu16 v_cnt);
void f_LL_TIMER1_CH1_Mode_set(te_LL_TIMER_Mode e_mode);
void f_LL_TIMER1_CH2_Mode_set(te_LL_TIMER_Mode e_mode);

#endif
