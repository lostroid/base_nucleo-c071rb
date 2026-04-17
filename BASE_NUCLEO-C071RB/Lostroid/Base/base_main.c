/********************************************************************
* File:    base_main.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "stm32c071xx.h"
#include "../Scheduler/scheduler.h"
#include "../DBG/dbg.h"
#include "../Motor/motor.h"
#include "base_flash.h"
#include "base_gpio.h"
#include "base_nvic.h"
#include "base_timer.h"
#include "base_tick.h"
#include "base_uart.h"
#include "base_dma.h"
#include "base_rcc.h"
#include "base_main.h"

static ts_Scheduler_Control gs_base_main_Job_ctrl;
static ts_Base_Main_Context gs_base_main_context;
enum { m_BASE_MAIN_JOB_TABLE_SIZE = 6 };
static void (*paf_base_main_job_table[m_BASE_MAIN_JOB_TABLE_SIZE])(ts_Scheduler_Control *ps_main_job_ctrl) = {
    f_Base_Main_Job_Start,  //+ 00
    f_Base_Main_Job_1ms,    //+ 01
    f_Base_Main_Job_10ms,   //+ 02
    f_Base_Main_Job_100ms,  //+ 03
    f_Base_Main_Job_500ms,  //+ 04
    f_Base_Main_Job_1s      //+ 05
};
//===================================================================
/* Low Level Main (startup code)
-------------------------------------------------------------------*/
void f_Base_Main(void)
{
    f_Base_Main_Initialize();

    while (1)
    {
        f_Base_Main_Module();
    }
}
//===================================================================
/* LL Main Setting vector (Startup code)
-------------------------------------------------------------------*/
void f_Base_Main_Vector_Setting(void)
{
    SCB->VTOR = FLASH_BASE;     // Vector postion
}
//===================================================================
/* LL main initialize
-------------------------------------------------------------------*/
void f_Base_Main_Initialize(void)
{
    f_Base_RCC_Init();
    f_Base_Flash_Init();
    f_Base_NVIC_Init();
    f_Base_DMA_Init();
    f_Base_GPIO_Init();
    f_Base_UART_Init();
    f_Base_Tick_Init();
    f_Base_Timer_Init();
    f_Motor_Init();
    f_DBG_Init();
    f_Base_Main_Time_Init();
    f_DBG_Print_String(d_CONFIG_START);
    f_DBG_Print_String(d_CONFIG_FW_VER);

    f_Scheduler_Init( 
        &gs_base_main_Job_ctrl
        , paf_base_main_job_table
        , "main_Job_table"
        , m_BASE_MAIN_JOB_TABLE_SIZE);
    
    f_Scheduler_Config(
        &gs_base_main_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , &gs_base_main_context);
}
//===================================================================
/* LL main Module
-------------------------------------------------------------------*/
void f_Base_Main_Module(void)
{
    f_Scheduler_Run(&gs_base_main_Job_ctrl);
    f_Base_UART_Module();
    f_Motor_Module();
}
//===================================================================
/* Main Time Initialize
-------------------------------------------------------------------*/
void f_Base_Main_Time_Init(void)
{
    f_Base_Tick_Systick32_Stopwatch_Start(&gs_base_main_context.gs_time_1ms,    d_BASE_TICK_1MS);
    f_Base_Tick_Systick32_Stopwatch_Start(&gs_base_main_context.gs_time_10ms,   d_BASE_TICK_10MS);
    f_Base_Tick_Systick32_Stopwatch_Start(&gs_base_main_context.gs_time_100ms,  d_BASE_TICK_100MS);
    f_Base_Tick_Systick32_Stopwatch_Start(&gs_base_main_context.gs_time_500ms,  d_BASE_TICK_500MS);
    f_Base_Tick_Systick32_Stopwatch_Start(&gs_base_main_context.gs_time_1s,     d_BASE_TICK_1S);
}
//===================================================================
/* Start Main Job.
-------------------------------------------------------------------*/
void f_Base_Main_Job_Start(ts_Scheduler_Control *ps_main_job_ctrl)
{
    f_Scheduler_LoopTime_Update(ps_main_job_ctrl);
    f_Scheduler_Next(ps_main_job_ctrl);
}
//===================================================================
/* 1ms Time base.
-------------------------------------------------------------------*/
void f_Base_Main_Job_1ms(ts_Scheduler_Control *ps_main_job_ctrl)
{
    ts_Base_Main_Context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_Base_Tick_Systick32_Stopwatch_Check(&s_context->gs_time_1ms) == d_OK)
    {    
        
    }
    f_Scheduler_Next(ps_main_job_ctrl);
}
//===================================================================
/* 10ms Time base.
-------------------------------------------------------------------*/
void f_Base_Main_Job_10ms(ts_Scheduler_Control *ps_main_job_ctrl)
{
    ts_Base_Main_Context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_Base_Tick_Systick32_Stopwatch_Check(&s_context->gs_time_10ms) == d_OK)
    {

    }
    f_Scheduler_Next(ps_main_job_ctrl);
}
//===================================================================
/* 100ms Time base.
-------------------------------------------------------------------*/
void f_Base_Main_Job_100ms(ts_Scheduler_Control *ps_main_job_ctrl)
{
    static tu32 lv_led_onoff = 0;
    ts_Base_Main_Context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_Base_Tick_Systick32_Stopwatch_Check(&s_context->gs_time_100ms) == d_OK)
    {
        if(lv_led_onoff == 0)
        { 
            f_LL_GPIO_LED_LD1_OnOff(m_GPIO_ON);
            lv_led_onoff++;
        }
        else
        {
            f_LL_GPIO_LED_LD1_OnOff(m_GPIO_OFF);
            lv_led_onoff = 0;
        }
        f_Base_Main_System_Print();
        f_Base_Tick_Systick_Get_Time32_us();
        f_Base_Tick_Systick_Get_Time64_us();
    }
    f_Scheduler_Next(ps_main_job_ctrl);
}
//===================================================================
/* 500ms Time base.
-------------------------------------------------------------------*/
void f_Base_Main_Job_500ms(ts_Scheduler_Control *ps_main_job_ctrl)
{
    static tu32 lv_led_onoff = 0;
    ts_Base_Main_Context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_Base_Tick_Systick32_Stopwatch_Check(&s_context->gs_time_500ms) == d_OK)
    {
        if(lv_led_onoff == 0)
        { 
            f_LL_GPIO_LED_LD2_OnOff(m_GPIO_ON);
            lv_led_onoff++;
        }
        else
        {
            f_LL_GPIO_LED_LD2_OnOff(m_GPIO_OFF);
            lv_led_onoff = 0;
        }
    }
    f_Scheduler_Next(ps_main_job_ctrl);
}
//===================================================================
/* 1s Time base.
-------------------------------------------------------------------*/
void f_Base_Main_Job_1s(ts_Scheduler_Control *ps_main_job_ctrl)
{
    
    ts_Base_Main_Context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_Base_Tick_Systick32_Stopwatch_Check(&s_context->gs_time_1s) == d_OK)
    {

    }
    f_Scheduler_Next(ps_main_job_ctrl);
}

//===================================================================
/*### Main module load print : Main 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_Base_Main_Load_Print(void)
{
    f_Scheduler_RunTime_Info_print(&gs_base_main_Job_ctrl);
}

//===================================================================
/*### Main System print : Main 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_Base_Main_System_Print(void)
{
    f_DBG_Print_String("\r\n");
    f_Base_Tick_Systick_Run_Time_Print();
    f_Scheduler_RunTime_Title_print();
    f_Base_Main_Load_Print();
    f_Base_Motor_Load_Print();
    f_Base_Uart_Load_Print();
}