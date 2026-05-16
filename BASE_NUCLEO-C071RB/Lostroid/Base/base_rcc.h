/********************************************************************
* File:    base_rcc.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_RCC_H
#define H_BASE_RCC_H

#include "base_rcc_type.h"

void f_base_rcc_init(void);
void f_base_rcc_module(void);
void f_base_rcc_apb_enable(void);
void f_base_rcc_hse_enable(void);
void f_base_rcc_hse_disable(void);
void f_base_rcc_hse_switch_select(void);
tu32 f_base_rcc_hse_ready(void);
void f_base_rcc_clock_enable(void);
void f_base_rcc_clock_selection(void);
void f_base_rcc_sleep_stip_clock_disable(void);

#endif
