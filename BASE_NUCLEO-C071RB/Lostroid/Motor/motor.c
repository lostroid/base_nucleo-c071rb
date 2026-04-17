/********************************************************************
* File:    motor.c
* Author:  LOSTROID
* Created: 2026-03-07
* Encoding: UTF-8
********************************************************************/
#include "../Base/base_uart.h"
#include "../Base/base_gpio.h"
#include "../Base/base_tick.h"
#include "../Base/base_timer.h"
#include "../DBG/dbg.h"
#include "../Scheduler/scheduler.h"
#include "motor.h"

/* Setting */
#define d_MOTOR_SW_DELAY_US             500000UL    //+ 0.5Sec

#define d_MOTOR_OPEN_TIMEOUT_US         3000000UL   //+ 3Sec
#define d_MOTOR_OPEN_STOP_TIME_US       300000      //+ 300ms
#define d_MOTOR_OPEN_START_DUTY         999         //+ 99.9%
#define d_MOTOR_OPEN_END_DUTY           300         //+ 30.0%

#define d_MOTOR_CLOSE_TIMEOUT_US        3000000UL   //+ 3Sec
#define d_MOTOR_CLOSE_STOP_TIME_US      10000       //+ 1ms
#define d_MOTOR_CLOSE_START_DUTY        999         //+ 70.0%
#define d_MOTOR_CLOSE_END_DUTY          300         //+ 30.0%

#define d_MOTOR_UNKNOWN_TIMEOUT_US      3000000UL   //+ 3Sec
#define d_MOTOR_UNKNOWN_STOP_TIME_US    1000        //+ 1ms
#define d_MOTOR_UNKNOWN_START_DUTY      700         //+ 50.0%
#define d_MOTOR_UNKNOWN_END_DUTY        300         //+ 30.0%


static ts_Scheduler_Control gs_motor_Job_ctrl;
static ts_Motor_Context gs_motor_context;
enum { m_BASE_MOTOR_Job_TABLE_SIZE = 7 };
static void (*paf_base_motor_Job_table[m_BASE_MOTOR_Job_TABLE_SIZE])(ts_Scheduler_Control *ps_motor_ctrl) = {
    f_Motor_Job_Start,      //+ 00
    f_Motor_Job_Idile,      //+ 01
    f_Motor_Job_SW_Delay,   //+ 02
    f_Motor_Job_Select,     //+ 03
    f_Motor_Job_Unknown,    //+ 04
    f_Motor_Job_Open,       //+ 05
    f_Motor_Job_Close       //+ 06
};
//===================================================================
/*### Motor Initialize 
---------------------------------------------------------------------
+ Power switch check
-------------------------------------------------------------------*/
void f_Motor_Init(void)
{
    gs_motor_context.e_state = m_MOTOR_LAST_STATE_UNKNOWN;

    f_Scheduler_Init( 
        &gs_motor_Job_ctrl
        , paf_base_motor_Job_table
        , "Motor_Job_table"
        , m_BASE_MOTOR_Job_TABLE_SIZE);
    
    f_Scheduler_Config(
        &gs_motor_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , &gs_motor_context);
}
//===================================================================
/*### Motor Modulue 
---------------------------------------------------------------------
+ Power switch check
-------------------------------------------------------------------*/
void f_Motor_Module(void)
{
    f_Scheduler_Run(&gs_motor_Job_ctrl);
}
//===================================================================
/*### Motor Start
---------------------------------------------------------------------
+ ps_motor_ctrl : Schecluder control
-------------------------------------------------------------------*/
void f_Motor_Job_Start(ts_Scheduler_Control *ps_motor_ctrl)
{
    f_Scheduler_LoopTime_Update(ps_motor_ctrl);
    f_Scheduler_Next(ps_motor_ctrl);
}
//===================================================================
/*### Motor Operation Idile 
---------------------------------------------------------------------
+ Power switch check
-------------------------------------------------------------------*/
void f_Motor_Job_Idile(ts_Scheduler_Control *ps_motor_ctrl)
{
   ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    if(f_Base_GPIO_SW_Power_In() == m_GPIO_ON)
    {
        tu32 v_check = 0;
        for(tu32 v_for = 0; v_for < 5; v_for++)
            { v_check |= f_Base_GPIO_SW_Power_In(); }
        if(v_check == 0)
        { 
            f_Base_Tick_Systick32_Start(&s_context->v_time);
            f_Scheduler_Next(ps_motor_ctrl);
        }
    }
    else
    {
        f_Scheduler_Jump(ps_motor_ctrl, 0);
    }
}
//===================================================================
/*### Motor Operation Delay
---------------------------------------------------------------------
+ Power switch delay
-------------------------------------------------------------------*/
void f_Motor_Job_SW_Delay(ts_Scheduler_Control *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    if(f_Base_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_SW_DELAY_US)
    {
        f_Scheduler_Next(ps_motor_ctrl);
    }
}
//===================================================================
/*### Motor Operation Start check
---------------------------------------------------------------------
+ "동작 방향을 결정 합니다."
-------------------------------------------------------------------*/
void f_Motor_Job_Select(ts_Scheduler_Control *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    if(f_Base_GPIO_MT_Open_In() == m_GPIO_ON )
    {
        if(s_context->e_state == m_MOTOR_LAST_STATE_OPEN)
            { f_Scheduler_Jump(ps_motor_ctrl, 6); } /* Start Close*/
        else
            { f_Scheduler_Next(ps_motor_ctrl); } /* Start Unknown*/
    }
    else if(f_Base_GPIO_MT_Close_In() == m_GPIO_ON )
    {
        if(s_context->e_state == m_MOTOR_LAST_STATE_CLOSE)
            { f_Scheduler_Jump(ps_motor_ctrl, 5); } /* Start open*/
        else
            { f_Scheduler_Next(ps_motor_ctrl); } /* Start Unknown*/
    }
    else
    {
        s_context->e_state = m_MOTOR_LAST_STATE_UNKNOWN;
        f_Scheduler_Next(ps_motor_ctrl); /* Start Unknown Close*/
    }
}
//===================================================================
/*### Motor Operation Start check
---------------------------------------------------------------------
+ "DBOT 열기 닫기 상태를 알수가 없으므로 천천히 닫기"
-------------------------------------------------------------------*/
void f_Motor_Job_Unknown(ts_Scheduler_Control *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    switch(ps_motor_ctrl->v_case_step)
    {
        case 0:
        {
            f_Base_Tick_Systick32_Start(&s_context->v_time);
            f_Base_TIMER1_CH1_CCR(d_MOTOR_UNKNOWN_START_DUTY); 
            f_Base_TIMER1_CH2_CCR(d_MOTOR_UNKNOWN_START_DUTY); 
             f_Base_TIMER1_CH1_Mode_set(m_BASE_TIMER_MODE_PWM);
             f_Base_TIMER1_CH2_Mode_set(m_BASE_TIMER_MODE_L);
            ps_motor_ctrl->v_case_step++;
            break;
        }
        case 1:
        {
            tu32 v_stop = 0;
            if(f_Base_GPIO_MT_Close_In() == m_GPIO_ON)
                { v_stop++; }
            if(f_Base_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_UNKNOWN_TIMEOUT_US)      /* Time out */
                { v_stop++; }

            if(v_stop != 0)
            {
                f_Base_Tick_Systick32_Start(&s_context->v_time);
                f_Base_TIMER1_CH1_CCR(d_MOTOR_UNKNOWN_END_DUTY); 
                f_Base_TIMER1_CH2_CCR(d_MOTOR_UNKNOWN_END_DUTY); 
                ps_motor_ctrl->v_case_step++;
            }
            break;
        }
        case 2:
        {
            if(f_Base_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_UNKNOWN_STOP_TIME_US)    /* Stop Delay */
            {
                f_Base_TIMER1_CH1_Mode_set(m_BASE_TIMER_MODE_H);
                f_Base_TIMER1_CH2_Mode_set(m_BASE_TIMER_MODE_H);
                s_context->e_state = m_MOTOR_LAST_STATE_CLOSE;
                f_Scheduler_Jump(ps_motor_ctrl, 0);
            }
            break;
        }
        default:
        {
            /* Error code */
            break;
        }
    }
}
//===================================================================
/*### Motor Operation Start check
---------------------------------------------------------------------
+ "DBOT 열기 동작"
-------------------------------------------------------------------*/
void f_Motor_Job_Open(ts_Scheduler_Control *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    switch(ps_motor_ctrl->v_case_step)
    {
        case 0:
        {
            f_Base_Tick_Systick32_Start(&s_context->v_time);
            f_Base_TIMER1_CH1_CCR(d_MOTOR_OPEN_START_DUTY); 
            f_Base_TIMER1_CH2_CCR(d_MOTOR_OPEN_START_DUTY); 
            f_Base_TIMER1_CH1_Mode_set(m_BASE_TIMER_MODE_L);
            f_Base_TIMER1_CH2_Mode_set(m_BASE_TIMER_MODE_PWM);
            ps_motor_ctrl->v_case_step++;
            break;
        }
        case 1:
        {
            tu32 v_stop = 0;
            if(f_Base_GPIO_MT_Open_In() == m_GPIO_ON)
                { v_stop++; }
            if(f_Base_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_OPEN_TIMEOUT_US)     /* Time out */
                { v_stop++; }
            if(v_stop != 0)
            {
                f_Base_Tick_Systick32_Start(&s_context->v_time);
                f_Base_TIMER1_CH1_CCR(d_MOTOR_OPEN_END_DUTY); 
                f_Base_TIMER1_CH2_CCR(d_MOTOR_OPEN_END_DUTY); 
                ps_motor_ctrl->v_case_step++;
            }
            break;
        }
        case 2:
        {
            if(f_Base_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_OPEN_STOP_TIME_US)   /* Stop Delay */
            {
                f_Base_TIMER1_CH1_Mode_set(m_BASE_TIMER_MODE_H);
                f_Base_TIMER1_CH2_Mode_set(m_BASE_TIMER_MODE_H);
                s_context->e_state = m_MOTOR_LAST_STATE_OPEN;
                f_Scheduler_Jump(ps_motor_ctrl, 0);
            }
            break;
        }
        default:
        {
            /* Error code */
            break;
        }
    }
}
//===================================================================
/*### Motor Operation Start check
---------------------------------------------------------------------
+ "DBOT 닫기 동작"
-------------------------------------------------------------------*/
void f_Motor_Job_Close(ts_Scheduler_Control *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    switch(ps_motor_ctrl->v_case_step)
    {
        case 0:
        {
            f_Base_Tick_Systick32_Start(&s_context->v_time);
            f_Base_TIMER1_CH1_CCR(d_MOTOR_CLOSE_START_DUTY); 
            f_Base_TIMER1_CH2_CCR(d_MOTOR_CLOSE_START_DUTY); 
            f_Base_TIMER1_CH1_Mode_set(m_BASE_TIMER_MODE_PWM);
            f_Base_TIMER1_CH2_Mode_set(m_BASE_TIMER_MODE_L);
            ps_motor_ctrl->v_case_step++;
            break;
        }
        case 1:
        {
            tu32 v_stop = 0;
            if(f_Base_GPIO_MT_Close_In() == m_GPIO_ON)
                { v_stop++; }
            if(f_Base_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_CLOSE_TIMEOUT_US)    /* Time out */
                { v_stop++; }
            if(v_stop != 0)
            {
                f_Base_Tick_Systick32_Start(&s_context->v_time);
                f_Base_TIMER1_CH1_CCR(d_MOTOR_CLOSE_END_DUTY); 
                f_Base_TIMER1_CH2_CCR(d_MOTOR_CLOSE_END_DUTY); 
                ps_motor_ctrl->v_case_step++;
            }
            break;
        }
        case 2:
        {
            if(f_Base_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_CLOSE_STOP_TIME_US)      /* Stop Delay */
            {
                f_Base_TIMER1_CH1_Mode_set(m_BASE_TIMER_MODE_H);
                f_Base_TIMER1_CH2_Mode_set(m_BASE_TIMER_MODE_H);
                s_context->e_state = m_MOTOR_LAST_STATE_CLOSE;
                f_Scheduler_Jump(ps_motor_ctrl, 0);
            }
            break;
        }
        default:
        {
            /* Error code */
            break;
        }
    }
}

//===================================================================
/*### Main module load print : Main 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_Base_Motor_Load_Print(void)
{
    f_Scheduler_RunTime_Info_print(&gs_motor_Job_ctrl);
}