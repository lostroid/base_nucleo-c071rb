/********************************************************************
* File:    base_adc.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_ADC_H
#define H_BASE_ADC_H

#include "base_adc_type.h"
#include "../Scheduler/scheduler_type.h"

void f_base_adc_init(void);
void f_base_adc_module(void);
void f_base_adc_regulator(void);
void f_base_adc_adstart(void);
void f_base_adc_info_print(void);
tu32 f_base_adc_ch0_get(void);
tu32 f_base_adc_ch1_get(void);

void f_base_adc_job_start(ts_scheduler_control *ps_adc_job_ctrl);
void f_base_adc_job_complete(ts_scheduler_control *ps_adc_job_ctrl);
void f_base_adc_job_check(ts_scheduler_control *ps_adc_job_ctrl);

void f_base_adc_load_print(void);

#endif
