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

void f_Base_ADC_Init(void);
void f_Base_ADC_Module(void);
void f_Base_ADC_Regulator(void);
void f_Base_ADC_ADSTART(void);
void f_Base_ADC_Info_Print(void);
tu32 f_Base_ADC_Get_ADC_CH0(void);
tu32 f_Base_ADC_Get_ADC_CH1(void);
void f_Base_ADC_Job_Start(ts_Scheduler_Control *ps_adc_job_ctrl);
void f_Base_ADC_Job_Complete(ts_Scheduler_Control *ps_adc_job_ctrl);
void f_Base_ADC_Job_Check(ts_Scheduler_Control *ps_adc_job_ctrl);
void f_Base_ADC_Load_Print(void);

#endif
