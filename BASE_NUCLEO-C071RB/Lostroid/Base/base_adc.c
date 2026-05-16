/********************************************************************
* File:    base_adc.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "stm32c071xx.h"
#include "../Scheduler/scheduler.h"
#include "../DBG/dbg.h"
#include "base_dma.h"
#include "base_tick.h"
#include "base_adc.h"


static ts_scheduler_control gs_base_adc_Job_ctrl;
enum { m_BASE_ADC_JOB_TABLE_SIZE = 3, };
static void (*paf_base_adc_job_table[m_BASE_ADC_JOB_TABLE_SIZE])(ts_scheduler_control *ps_adc_job_ctrl) = {
    f_base_adc_job_start,       //+ 00
    f_base_adc_job_complete,    //+ 01
    f_base_adc_job_check        //+ 02
};


enum { m_BASE_ADC_BUFF_SIZE = 2};
static tu16 gpa_adc_buff[m_BASE_ADC_BUFF_SIZE] = {0};
static ts_adc_info gs_adc_info = {0};

enum { m_BASE_ADC_TIME_10MS = 10000u };                     //+ 10ms = 10000us
static ts_base_tick_context gs_base_adc_tick_context_10ms;  //+ time control
//===================================================================
/* Base Base ADC initialize
-------------------------------------------------------------------*/
void f_base_adc_init(void)
{
    f_base_adc_regulator();
    f_scheduler_init( 
        &gs_base_adc_Job_ctrl
        , paf_base_adc_job_table
        , "adc_Job_table"
        , m_BASE_ADC_JOB_TABLE_SIZE);
    
    f_scheduler_config(
        &gs_base_adc_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , d_NULL);
        
    f_base_tick_time32_start_lap(&gs_base_adc_tick_context_10ms, m_BASE_ADC_TIME_10MS);
}
//===================================================================
/* Base Base ADC Module
-------------------------------------------------------------------*/
void f_base_adc_module(void)
{
    f_scheduler_run(&gs_base_adc_Job_ctrl);
}
//===================================================================
/*### ADC Ragulator
-------------------------------------------------------------------*/
void f_base_adc_regulator(void)
{
    tu32 v_timeout;
    tu32 v_avg = 0;
    ADC1->CR = ADC_CR_ADVREGEN;
    f_base_tick_time32_blocking_delay(10);   //+ Blocking delay 10us
    f_base_tick_time32_start_timer(&v_timeout);
    ADC1->CFGR1 = 0;
    for(tu32 v_for = 0; v_for < 8; v_for++)
    {
    	ADC1->CR = ADC_CR_ADCAL;
        while((ADC1->CR & ADC_CR_ADCAL) != 0)
        {
            if(f_base_tick_time32_check_timer(&v_timeout) > 10000000)
            {
                f_dbg_print_string("\r\n");
                f_base_tick_time64_run_time_print();
                f_dbg_print_string("\r\nE: ADC FAIL");
                return;
            }
        }
        v_avg += (ADC_CALFACT_CALFACT & ADC1->CALFACT);
    }
    v_avg /= 8;
    v_avg++;
    ADC1->CALFACT |= (ADC_CALFACT_CALFACT & v_avg);         //+ Calibration
    ADC1->CHSELR = ADC_CHSELR_CHSEL0                        //+ ADC_IN0
                 | ADC_CHSELR_CHSEL1;                       //+ ADC_IN1
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN;                         //+ DMA Setting
    ADC1->SMPR = ADC_SMPR_SMP1_2;                           //+ 7.5 CLK
    ADC->CCR = ADC_CCR_PRESC_1;                             //+ 24Mhz
    ADC1->CR |= ADC_CR_ADEN;
}
//===================================================================
/*### ADC ADCART Start
-------------------------------------------------------------------*/
void f_base_adc_adstart(void)
{
    ADC1->CR |= ADC_CR_ADSTART;
}
//===================================================================
/*### ADC Battery Infomaiton
---------------------------------------------------------------------
+ ADC_IN0
+ ADC_IN1
-------------------------------------------------------------------*/
void f_base_adc_info_print(void)
{
    tu32 v_voltage;
    f_dbg_print_string("\r\nI: ADC_IN0: ");
    v_voltage = d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch0);
    f_dbg_print_dec32(v_voltage / 1000);
    f_dbg_print_string(".");
    f_dbg_print_dec32_digit(3, v_voltage % 1000);
    f_dbg_print_string(" V\r\nI: ADC_IN1: ");
    v_voltage = d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch1);
    f_dbg_print_dec32(v_voltage / 1000);
    f_dbg_print_string(".");
    f_dbg_print_dec32_digit(3, v_voltage % 1000);
    f_dbg_print_string(" V");
}
//===================================================================
/*### ADC CH0 Get
---------------------------------------------------------------------
+ Return uV
-------------------------------------------------------------------*/
tu32 f_base_adc_ch0_get(void)
{
    return d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch0);
}
//===================================================================
/*### ADC CH1 Get
---------------------------------------------------------------------
+ Return uV
-------------------------------------------------------------------*/
tu32 f_base_adc_ch1_get(void)
{
    return d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch1);
}
//===================================================================
/*### ADC Job Start
-------------------------------------------------------------------*/
void f_base_adc_job_start(ts_scheduler_control *ps_adc_job_ctrl)
{
     f_scheduler_loop_time_update(ps_adc_job_ctrl);
    if(f_base_tick_time32_check_lap(&gs_base_adc_tick_context_10ms)
    == m_RETURN_OK)
    {
    	f_base_dma_memory_setting(d_BASE_DMA1_CH5_ADC1, (tu8 *)gpa_adc_buff, m_BASE_ADC_BUFF_SIZE);
        f_base_dma_start(d_BASE_DMA1_CH5_ADC1);
        f_base_adc_adstart();
        f_scheduler_next(ps_adc_job_ctrl);
    }
}
//===================================================================
/*### ADC Job DMA  Transfer Complete flag
-------------------------------------------------------------------*/
void f_base_adc_job_complete(ts_scheduler_control *ps_adc_job_ctrl)
{
    if(d_BASE_DMA1_CH5_ADC1_TC() != 0)
    {
        f_base_dma1_ch5_adc_ctc();
        gs_adc_info.v_adc_ch0 = gpa_adc_buff[0];
        gs_adc_info.v_adc_ch1 = gpa_adc_buff[1];
        f_scheduler_next(ps_adc_job_ctrl);
    }
}
//===================================================================
/*### ADC Job Wait
-------------------------------------------------------------------*/
void f_base_adc_job_check(ts_scheduler_control *ps_adc_job_ctrl)
{
    f_scheduler_next(ps_adc_job_ctrl);
}

//===================================================================
/*### ADC module load print : ADC 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_base_adc_load_print(void)
{
    f_scheduler_run_time_info_print(&gs_base_adc_Job_ctrl);
}

