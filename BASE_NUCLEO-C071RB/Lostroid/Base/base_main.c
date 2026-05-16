/********************************************************************
* File:    base_main.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "stm32c071xx.h"
#include "../Scheduler/scheduler.h"
#include "../Display/display.h"
#include "../Font/font_default.h"
#include "../DBG/dbg.h"

#include "base_syscfg.h"
#include "base_adc.h"
#include "base_flash.h"
#include "base_gpio.h"
#include "base_nvic.h"
#include "base_tim.h"
#include "base_tick.h"
#include "base_uart.h"
#include "base_spi.h"
#include "base_dma.h"
#include "base_rcc.h"
#include "base_main.h"

static ts_scheduler_control gs_base_main_Job_ctrl;
static ts_base_main_context gs_base_main_context;
enum { m_BASE_MAIN_JOB_TABLE_SIZE = 6 };
static void (*paf_base_main_job_table[m_BASE_MAIN_JOB_TABLE_SIZE])(ts_scheduler_control *ps_main_job_ctrl) = {
    f_base_main_job_start,  //+ 00
    f_base_main_job_1ms,    //+ 01
    f_base_main_job_10ms,   //+ 02
    f_base_main_job_100ms,  //+ 03
    f_base_main_job_500ms,  //+ 04
    f_base_main_job_1s      //+ 05
};
//===================================================================
/* Low Level Main (startup code)
-------------------------------------------------------------------*/
void f_base_main(void)
{
    f_base_main_init();

    while (1)
    {
        f_base_main_module();
    }
}
//===================================================================
/* Base Main Setting vector (Startup code)
-------------------------------------------------------------------*/
void f_base_main_vector_set(void)
{
    SCB->VTOR = FLASH_BASE;     // Vector postion
}
//===================================================================
/* Base main initialize
-------------------------------------------------------------------*/
void f_base_main_init(void)
{
    f_base_rcc_init();
    f_base_syscfg_init();
    f_base_flash_init();
    f_base_nvic_init();
    f_base_dma_Init();
    f_base_gpio_init();
    f_base_uart_init();
    f_base_tick_init();
    f_base_tim_init();
    f_base_adc_init();
    f_base_spi_init();
    f_base_main_time_init();
    f_display_init();
    f_dbg_init();
    f_dbg_print_string(d_CONFIG_START);
    f_dbg_print_string(d_CONFIG_FW_VER);

    f_scheduler_init( 
        &gs_base_main_Job_ctrl
        , paf_base_main_job_table
        , "main_Job_table"
        , m_BASE_MAIN_JOB_TABLE_SIZE);
    
    f_scheduler_config(
        &gs_base_main_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , &gs_base_main_context);
}
//===================================================================
/* Base main Module
-------------------------------------------------------------------*/
void f_base_main_module(void)
{
    f_scheduler_run(&gs_base_main_Job_ctrl);
    f_base_uart_module();
    f_base_adc_module();
    f_display_module();
}
//===================================================================
/* Main Time Initialize
-------------------------------------------------------------------*/
void f_base_main_time_init(void)
{
    f_base_tick_time32_start_lap(&gs_base_main_context.gs_time_1ms,    d_BASE_TICK_1MS);
    f_base_tick_time32_start_lap(&gs_base_main_context.gs_time_10ms,   d_BASE_TICK_10MS);
    f_base_tick_time32_start_lap(&gs_base_main_context.gs_time_100ms,  d_BASE_TICK_100MS);
    f_base_tick_time32_start_lap(&gs_base_main_context.gs_time_500ms,  d_BASE_TICK_500MS);
    f_base_tick_time32_start_lap(&gs_base_main_context.gs_time_1s,     d_BASE_TICK_1S);
}
//===================================================================
/* Start Main Job.
-------------------------------------------------------------------*/
void f_base_main_job_start(ts_scheduler_control *ps_main_job_ctrl)
{
    f_scheduler_loop_time_update(ps_main_job_ctrl);
    f_scheduler_next(ps_main_job_ctrl);
}
//===================================================================
/* 1ms Time base.
-------------------------------------------------------------------*/
void f_base_main_job_1ms(ts_scheduler_control *ps_main_job_ctrl)
{
    ts_base_main_context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_base_tick_time32_check_lap(&s_context->gs_time_1ms) == m_RETURN_OK)
    {    
        
    }
    f_scheduler_next(ps_main_job_ctrl);
}
//===================================================================
/* 10ms Time base.
-------------------------------------------------------------------*/
void f_base_main_job_10ms(ts_scheduler_control *ps_main_job_ctrl)
{
    ts_base_main_context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_base_tick_time32_check_lap(&s_context->gs_time_10ms) == m_RETURN_OK)
    {

    }
    f_scheduler_next(ps_main_job_ctrl);
}
//===================================================================
/* 100ms Time base.
-------------------------------------------------------------------*/
void f_base_main_job_100ms(ts_scheduler_control *ps_main_job_ctrl)
{
    static tu32 lv_led_onoff = 0;
    ts_base_main_context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_base_tick_time32_check_lap(&s_context->gs_time_100ms) == m_RETURN_OK)
    {
        if(lv_led_onoff == 0)
        { 
            d_BASE_GPIO_LED_LD1_H;
            lv_led_onoff++;
        }
        else
        {
            d_BASE_GPIO_LED_LD1_L;
            lv_led_onoff = 0;
        }
        f_base_main_system_print();
        f_base_tick_systick_time32_get();
        f_base_tick_systick_time64_get();
    }
    f_scheduler_next(ps_main_job_ctrl);
}
//===================================================================
/* 500ms Time base.
-------------------------------------------------------------------*/
void f_base_main_job_500ms(ts_scheduler_control *ps_main_job_ctrl)
{
    static tu32 lv_led_onoff = 0;
    ts_base_main_context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_base_tick_time32_check_lap(&s_context->gs_time_500ms) == m_RETURN_OK)
    {
        if(lv_led_onoff == 0)
        { 
            d_BASE_GPIO_LED_LD2_H;
            lv_led_onoff++;
        }
        else
        {
            d_BASE_GPIO_LED_LD2_L;
            lv_led_onoff = 0;
        }
    }
    f_scheduler_next(ps_main_job_ctrl);
}
//===================================================================
/* 1s Time base.
-------------------------------------------------------------------*/
void f_base_main_job_1s(ts_scheduler_control *ps_main_job_ctrl)
{
    
    ts_base_main_context *s_context = ps_main_job_ctrl->ps_user_struct;
    if(f_base_tick_time32_check_lap(&s_context->gs_time_1s) == m_RETURN_OK)
    {
        f_display_fps_update();
    }
    f_scheduler_next(ps_main_job_ctrl);
}

//===================================================================
/*### Main module load print : Main 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_base_main_load_print(void)
{
    f_scheduler_run_time_info_print(&gs_base_main_Job_ctrl);
}

//===================================================================
/*### Main System print : Main 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_base_main_system_print(void)
{
    f_dbg_print_string("\r\n");
    f_base_tick_time64_run_time_print();
    f_scheduler_run_time_title_print();
    f_base_main_load_print();
    f_base_uart_load_print();
    f_base_adc_load_print();
    f_lcd_st7735s_load_print();
    f_base_tim_info_print();
    f_base_adc_info_print();
}