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

void f_base_tim_init(void);
void f_base_tim_module(void);
void f_base_tim2_init(void);
void f_base_tim2_ch1_ccr(tu16 v_cnt);
void f_base_tim2_ch2_ccr(tu16 v_cnt);
void f_base_tim2_ch1_mode_set(te_base_tim_mode e_mode);
void f_base_tim2_ch2_mode_set(te_base_tim_mode e_mode);
te_return f_base_tim14_check_100us(ts_base_tick_context* ps_time);
void f_base_tim_input_captur_init(void);

void f_base_tim1_ch2_sw_a_count_up(void);
void f_base_tim1_ch3_sw_b_count_up(void);
void f_base_tim3_ch1_sw_ok_count_up(void);

tu32 f_base_tim1_ch2_sw_a_count_get(void);
tu32 f_base_tim1_ch3_sw_b_count_get(void);
tu32 f_base_tim3_ch1_sw_ok_count_get(void);

void f_base_tim_info_print(void);

#endif
