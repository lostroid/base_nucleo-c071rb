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


static ts_Scheduler_Control gs_base_adc_Job_ctrl;
enum { m_BASE_ADC_JOB_TABLE_SIZE = 3, };
static void (*paf_base_adc_job_table[m_BASE_ADC_JOB_TABLE_SIZE])(ts_Scheduler_Control *ps_adc_job_ctrl) = {
    f_Base_ADC_Job_Start,       //+ 00
    f_Base_ADC_Job_Complete,    //+ 01
    f_Base_ADC_Job_Check        //+ 02
};


enum { m_BASE_ADC_BUFF_SIZE = 2};
static tu16 gpa_adc_buff[m_BASE_ADC_BUFF_SIZE] = {0};
static ts_adc_info gs_adc_info = {0};

enum { m_BASE_ADC_TIME_10MS = 10000u };                     //+ 10ms = 10000us
static ts_Base_Tick_Context gs_base_adc_tick_context_10ms;  //+ time control
//===================================================================
/* Base Base ADC initialize
-------------------------------------------------------------------*/
void f_Base_ADC_Init(void)
{
    f_Base_ADC_Regulator();
    f_Scheduler_Init( 
        &gs_base_adc_Job_ctrl
        , paf_base_adc_job_table
        , "adc_Job_table"
        , m_BASE_ADC_JOB_TABLE_SIZE);
    
    f_Scheduler_Config(
        &gs_base_adc_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , d_NULL);
        
    f_Base_Tick_Systick32_Stopwatch_Start(&gs_base_adc_tick_context_10ms, m_BASE_ADC_TIME_10MS);
}
//===================================================================
/* Base Base ADC Module
-------------------------------------------------------------------*/
void f_Base_ADC_Module(void)
{
    f_Scheduler_Run(&gs_base_adc_Job_ctrl);
}
//===================================================================
/*#### ADC Ragulator
-------------------------------------------------------------------*/
void f_Base_ADC_Regulator(void)
{
    tu32 v_timeout;
    tu32 v_avg = 0;
    ADC1->CR = ADC_CR_ADVREGEN;
    f_Base_Tick_Systick32_Blocking_Delay(10);   //+ Blocking delay 10us
    f_Base_Tick_Systick32_Start(&v_timeout);
    ADC1->CFGR1 = 0;
    for(tu32 v_for = 0; v_for < 8; v_for++)
    {
    	ADC1->CR = ADC_CR_ADCAL;
        while((ADC1->CR & ADC_CR_ADCAL) != 0)
        {
            if(f_Base_Tick_Systick32_Finish(&v_timeout) > 10000000)
            {
                f_DBG_Print_String("\r\n");
                f_Base_Tick_Systick_Run_Time_Print();
                f_DBG_Print_String("\r\nE: ADC FAIL");
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
/*#### ADC ADCART Start
-------------------------------------------------------------------*/
void f_Base_ADC_ADSTART(void)
{
    ADC1->CR |= ADC_CR_ADSTART;
}
//===================================================================
/*#### ADC Battery Infomaiton
---------------------------------------------------------------------
+ ADC_IN0
+ ADC_IN1
-------------------------------------------------------------------*/
void f_Base_ADC_Info_Print(void)
{
    tu32 v_voltage;
    f_DBG_Print_String("\r\nI: ADC_IN0: ");
    v_voltage = d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch0);
    f_DBG_Print_Dec32(v_voltage / 1000);
    f_DBG_Print_String(".");
    f_DBG_Print_Dec32_Digit(3, v_voltage % 1000);
    f_DBG_Print_String(" V\r\nI: ADC_IN1: ");
    v_voltage = d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch1);
    f_DBG_Print_Dec32(v_voltage / 1000);
    f_DBG_Print_String(".");
    f_DBG_Print_Dec32_Digit(3, v_voltage % 1000);
    f_DBG_Print_String(" V");
}
//===================================================================
/*#### ADC CH0 Get
---------------------------------------------------------------------
+ Return uV
-------------------------------------------------------------------*/
tu32 f_Base_ADC_Get_ADC_CH0(void)
{
    return d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch0);
}
//===================================================================
/*#### ADC CH1 Get
---------------------------------------------------------------------
+ Return uV
-------------------------------------------------------------------*/
tu32 f_Base_ADC_Get_ADC_CH1(void)
{
    return d_BASE_ADC_TO_VOLTAGE(gs_adc_info.v_adc_ch1);
}
//===================================================================
/*#### ADC Job Start
-------------------------------------------------------------------*/
void f_Base_ADC_Job_Start(ts_Scheduler_Control *ps_adc_job_ctrl)
{
     f_Scheduler_LoopTime_Update(ps_adc_job_ctrl);
    if(f_Base_Tick_Systick32_Stopwatch_Check(&gs_base_adc_tick_context_10ms) 
    == m_RETURN_OK)
    {
    	f_Base_DMA_Memory_Setting(d_BASE_DMA1_CH5_ADC1, (tu8 *)gpa_adc_buff, m_BASE_ADC_BUFF_SIZE);
        f_Base_DMA_Start(d_BASE_DMA1_CH5_ADC1);
        f_Base_ADC_ADSTART();
        f_Scheduler_Next(ps_adc_job_ctrl);
    }
}
//===================================================================
/*#### ADC Job DMA  Transfer Complete flag
-------------------------------------------------------------------*/
void f_Base_ADC_Job_Complete(ts_Scheduler_Control *ps_adc_job_ctrl)
{
    if(d_BASE_DMA1_CH5_ADC1_TC() != 0)
    {
        d_Base_DMA1_CH5_ADC_CTC();
        gs_adc_info.v_adc_ch0 = gpa_adc_buff[0];
        gs_adc_info.v_adc_ch1 = gpa_adc_buff[1];
        f_Scheduler_Next(ps_adc_job_ctrl);
    }
}
//===================================================================
/*#### ADC Job Wait
-------------------------------------------------------------------*/
void f_Base_ADC_Job_Check(ts_Scheduler_Control *ps_adc_job_ctrl)
{
    f_Scheduler_Next(ps_adc_job_ctrl);
}

//===================================================================
/*#### ADC module load print : ADC 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_Base_ADC_Load_Print(void)
{
    f_Scheduler_RunTime_Info_print(&gs_base_adc_Job_ctrl);
}

