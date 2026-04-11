/********************************************************************
* File:    base_ll_main.c
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the ll main control.
*
* Revision History:
*   2025-08-06  New.
********************************************************************/

#include "stm32c071xx.h"
#include "../Scheduler/scheduler.h"
#include "../DBG/dbg.h"
#include "../Motor/motor.h"
#include "base_ll_flash.h"
#include "base_ll_gpio.h"
#include "base_ll_nvic.h"
#include "base_ll_timer.h"
#include "base_ll_tick.h"
#include "base_ll_uart.h"
#include "base_ll_dma.h"
#include "base_ll_rcc.h"
#include "base_ll_main.h"

static ts_Scheduler_Ctrl gs_ll_main_Job_ctrl;
static ts_LL_Main_Context gs_ll_main_context;
enum { m_LL_MAIN_JOB_TABLE_SIZE = 5 };
static void (*paf_ll_main_job_table[m_LL_MAIN_JOB_TABLE_SIZE])(ts_Scheduler_Ctrl *ps_main_ctrl) = {
    f_LL_Main_Job_1ms,    // 00
    f_LL_Main_Job_10ms,   // 01
    f_LL_Main_Job_100ms,  // 02
    f_LL_Main_Job_500ms,  // 03
    f_LL_Main_Job_1s      // 04
};

//===================================================================
/* Low Level Main (startup code)
-------------------------------------------------------------------*/
void f_LL_Main(void)
{
    f_LL_Main_Initialize();

    while (1)
    {
        f_LL_Main_Module();
    }
}

//===================================================================
/*### LL Main Setting vector (Startup code)
-------------------------------------------------------------------*/
void f_LL_Main_Vector_Setting(void)
{
    SCB->VTOR = FLASH_BASE;     // Vector postion
}

//===================================================================
/* LL main initialize
-------------------------------------------------------------------*/
void f_LL_Main_Initialize(void)
{
    f_LL_Tick_Systick_Base32_Capture(&gs_ll_main_context.gs_time_1ms, 1000);
    f_LL_Tick_Systick_Base32_Capture(&gs_ll_main_context.gs_time_10ms, 10000);
    f_LL_Tick_Systick_Base32_Capture(&gs_ll_main_context.gs_time_100ms, 100000);
    f_LL_Tick_Systick_Base32_Capture(&gs_ll_main_context.gs_time_500ms, 500000);
    f_LL_Tick_Systick_Base32_Capture(&gs_ll_main_context.gs_time_1s, 1000000);

    f_LL_RCC_Init();
    f_LL_Flash_Init();
    f_LL_NVIC_Init();
    f_LL_DMA_Init();
    f_LL_GPIO_Init();
    f_LL_UART_Init();
    f_LL_TIMERx_Init();
    f_Motor_Init();
    f_DBG_Init();
    f_DBG_Print_String(d_LL_CONFIG_START);
    f_DBG_Print_String(d_LL_CONFIG_FW_VER);

    f_Scheduler_Init( 
        &gs_ll_main_Job_ctrl
        , paf_ll_main_job_table
        , "main_sch_table"
        , m_LL_MAIN_JOB_TABLE_SIZE);
    
    f_Scheduler_Config(
        &gs_ll_main_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , &gs_ll_main_context);
}

//===================================================================
/* LL main Module
-------------------------------------------------------------------*/
void f_LL_Main_Module(void)
{
    f_Scheduler_Run(&gs_ll_main_Job_ctrl);
    f_LL_UART_Module();
    f_Motor_Module();
}

//===================================================================
/*### 1ms Time base.
---------------------------------------------------------------------
+ *ps_main_ctrl : Main Schedlue
-------------------------------------------------------------------*/
void f_LL_Main_Job_1ms(ts_Scheduler_Ctrl *ps_main_ctrl)
{
    ts_LL_Main_Context *s_context = ps_main_ctrl->ps_user_struct;
    if(f_LL_Tick_Systick_Base32_Elapsed(&s_context->gs_time_1ms) == d_OK)
    {    
        
    }
    f_Scheduler_Next(ps_main_ctrl);
}

//===================================================================
/* 10ms Time base.
-------------------------------------------------------------------*/
void f_LL_Main_Job_10ms(ts_Scheduler_Ctrl *ps_main_ctrl)
{
    ts_LL_Main_Context *s_context = ps_main_ctrl->ps_user_struct;
    if(f_LL_Tick_Systick_Base32_Elapsed(&s_context->gs_time_10ms) == d_OK)
    {

    }
    f_Scheduler_Next(ps_main_ctrl);
}

//===================================================================
/* 100ms Time base.
-------------------------------------------------------------------*/
void f_LL_Main_Job_100ms(ts_Scheduler_Ctrl *ps_main_ctrl)
{
    static tu32 lv_led_onoff = 0;
    ts_LL_Main_Context *s_context = ps_main_ctrl->ps_user_struct;
    if(f_LL_Tick_Systick_Base32_Elapsed(&s_context->gs_time_100ms) == d_OK)
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
        f_LL_Main_System_Print();
        f_LL_Tick_Systick_Base32();
        f_LL_Tick_Systick_Base64();
    }
    f_Scheduler_Next(ps_main_ctrl);
}

//===================================================================
/* 500ms Time base.
-------------------------------------------------------------------*/
void f_LL_Main_Job_500ms(ts_Scheduler_Ctrl *ps_main_ctrl)
{
    static tu32 lv_led_onoff = 0;
    ts_LL_Main_Context *s_context = ps_main_ctrl->ps_user_struct;
    if(f_LL_Tick_Systick_Base32_Elapsed(&s_context->gs_time_500ms) == d_OK)
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
    f_Scheduler_Next(ps_main_ctrl);
}

//===================================================================
/* 1s Time base.
-------------------------------------------------------------------*/
void f_LL_Main_Job_1s(ts_Scheduler_Ctrl *ps_main_ctrl)
{
    ts_LL_Main_Context *s_context = ps_main_ctrl->ps_user_struct;
    if(f_LL_Tick_Systick_Base32_Elapsed(&s_context->gs_time_1s) == d_OK)
    {
        
    }
    f_Scheduler_Next(ps_main_ctrl);
}

//===================================================================
/*### Main module load print : Main 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_LL_Main_Load_Print(void)
{
    f_Scheduler_RunTime_Info_print(&gs_ll_main_Job_ctrl, m_DBG_MODE_SYSTEM, gs_ll_main_Job_ctrl.p_table_name);
}

//===================================================================
/*### Main System print : Main 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_LL_Main_System_Print(void)
{
    f_DBG_Print_String("\r\n");
    f_LL_Tick_Systick_Run_Stamp_Print();
    f_Scheduler_RunTime_Title_print(m_DBG_MODE_SYSTEM);
    f_LL_Main_Load_Print();
    f_LL_Motor_Load_Print();
    f_LL_Uart_Load_Print();
}