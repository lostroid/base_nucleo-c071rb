/******************************************************************************
* File:    motor.c
* Author:  LOSTROID
* Created: 2026-03-07
*
* Description:
* This is the debug Control.
*
* Revision History:
*   2026-03-07  New.
******************************************************************************/
#include "../Base_LL/base_ll_uart.h"
#include "../Base_LL/base_ll_gpio.h"
#include "../Base_LL/base_ll_tick.h"
#include "../Base_LL/base_ll_timer.h"
#include "../DBG/dbg.h"
#include "../Scheduler/scheduler.h"
#include "motor.h"

/* Setting */
#define d_MOTOR_SW_DELAY_US             500000UL   /* 0.5Sec */

#define d_MOTOR_OPEN_TIMEOUT_US         3000000UL   /* 3Sec */
#define d_MOTOR_OPEN_STOP_TIME_US       300000      /* 300ms */
#define d_MOTOR_OPEN_START_DUTY         999         /* 99.9% */
#define d_MOTOR_OPEN_END_DUTY           300         /* 30.0% */

#define d_MOTOR_CLOSE_TIMEOUT_US        3000000UL   /* 3Sec */
#define d_MOTOR_CLOSE_STOP_TIME_US      10000       /* 1ms */
#define d_MOTOR_CLOSE_START_DUTY        700         /* 70.0% */
#define d_MOTOR_CLOSE_END_DUTY          200         /* 30.0% */

#define d_MOTOR_UNKNOWN_TIMEOUT_US      3000000UL   /* 3Sec */
#define d_MOTOR_UNKNOWN_STOP_TIME_US    1000        /* 1ms */
#define d_MOTOR_UNKNOWN_START_DUTY      500         /* 50.0% */
#define d_MOTOR_UNKNOWN_END_DUTY        300         /* 30.0% */


static ts_Scheduler_Ctrl gs_motor_Job_ctrl;
static ts_Motor_Context gs_motor_context;
enum { m_LL_MOTOR_Job_TABLE_SIZE = 6 };
static void (*paf_ll_motor_Job_table[m_LL_MOTOR_Job_TABLE_SIZE])(ts_Scheduler_Ctrl *ps_motor_ctrl) = {
    f_Motor_Open_Idile,       /*0*/
    f_Motor_Open_SW_Delay,    /*1*/
    f_Motor_Open_Start,       /*2*/
    f_Motor_Open_Unknown,     /*3*/
    f_Motor_Open_Open,        /*4*/
    f_Motor_Open_Close        /*5*/
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
        , paf_ll_motor_Job_table
        , "Motor_sch_table"
        , m_LL_MOTOR_Job_TABLE_SIZE);
    
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
/*### Motor Operation Idile 
---------------------------------------------------------------------
+ Power switch check
-------------------------------------------------------------------*/
void f_Motor_Open_Idile(ts_Scheduler_Ctrl *ps_motor_ctrl)
{
   ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    if(f_LL_GPIO_SW_Power_In() == m_GPIO_ON)
    {
        tu32 v_check = 0;
        for(tu32 v_for = 0; v_for < 5; v_for++)
            { v_check |= f_LL_GPIO_SW_Power_In(); }
        if(v_check == 0)
        { 
            f_LL_Tick_Systick32_Start(&s_context->v_time);
            f_Scheduler_Next(ps_motor_ctrl);
        }
    }
}
//===================================================================
/*### Motor Operation Delay
---------------------------------------------------------------------
+ Power switch delay
-------------------------------------------------------------------*/
void f_Motor_Open_SW_Delay(ts_Scheduler_Ctrl *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    if(f_LL_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_SW_DELAY_US)
    {
        f_Scheduler_Next(ps_motor_ctrl);
    }
}
//===================================================================
/*### Motor Operation Start check
---------------------------------------------------------------------
+ "동작 방향을 결정 합니다."
-------------------------------------------------------------------*/
void f_Motor_Open_Start(ts_Scheduler_Ctrl *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    if(f_LL_GPIO_MT_Open_In() == m_GPIO_ON )
    {
        if(s_context->e_state == m_MOTOR_LAST_STATE_OPEN)
            { f_Scheduler_Jump(ps_motor_ctrl, 5); } /* Start Close*/
        else
            { f_Scheduler_Next(ps_motor_ctrl); } /* Start Unknown*/
    }
    else if(f_LL_GPIO_MT_Close_In() == m_GPIO_ON )
    {
        if(s_context->e_state == m_MOTOR_LAST_STATE_CLOSE)
            { f_Scheduler_Jump(ps_motor_ctrl, 4); } /* Start open*/
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
void f_Motor_Open_Unknown(ts_Scheduler_Ctrl *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    switch(ps_motor_ctrl->v_case_step)
    {
        case 0:
        {
            // f_LL_Tick_Systick_Run_Stamp_Print();
            // f_DBG_Print_String("I: Unknown Close!");
            f_LL_Tick_Systick32_Start(&s_context->v_time);
            f_LL_TIMER1_CH1_CCR(d_MOTOR_UNKNOWN_START_DUTY); 
            f_LL_TIMER1_CH2_CCR(d_MOTOR_UNKNOWN_START_DUTY); 
            f_LL_TIMER1_CH1_Mode_set(m_LL_TIMER_MODE_PWM);
            f_LL_TIMER1_CH2_Mode_set(m_LL_TIMER_MODE_L);
            ps_motor_ctrl->v_case_step++;
            break;
        }
        case 1:
        {
            tu32 v_stop = 0;
            if(f_LL_GPIO_MT_Close_In() == m_GPIO_ON)
                { v_stop++; }
            if(f_LL_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_UNKNOWN_TIMEOUT_US)      /* Time out */
                { v_stop++; }

            if(v_stop != 0)
            {
                f_LL_Tick_Systick32_Start(&s_context->v_time);
                f_LL_TIMER1_CH1_CCR(d_MOTOR_UNKNOWN_END_DUTY); 
                f_LL_TIMER1_CH2_CCR(d_MOTOR_UNKNOWN_END_DUTY); 
                ps_motor_ctrl->v_case_step++;
            }
            break;
        }
        case 2:
        {
            if(f_LL_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_UNKNOWN_STOP_TIME_US)    /* Stop Delay */
            {
                f_LL_TIMER1_CH1_Mode_set(m_LL_TIMER_MODE_H);
                f_LL_TIMER1_CH2_Mode_set(m_LL_TIMER_MODE_H);
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
void f_Motor_Open_Open(ts_Scheduler_Ctrl *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    switch(ps_motor_ctrl->v_case_step)
    {
        case 0:
        {
            // f_LL_Tick_Systick_Run_Stamp_Print();
            // f_DBG_Print_String("I: OPEN!");
            f_LL_Tick_Systick32_Start(&s_context->v_time);
            f_LL_TIMER1_CH1_CCR(d_MOTOR_OPEN_START_DUTY); 
            f_LL_TIMER1_CH2_CCR(d_MOTOR_OPEN_START_DUTY); 
            f_LL_TIMER1_CH1_Mode_set(m_LL_TIMER_MODE_L);
            f_LL_TIMER1_CH2_Mode_set(m_LL_TIMER_MODE_PWM);
            ps_motor_ctrl->v_case_step++;
            break;
        }
        case 1:
        {
            tu32 v_stop = 0;
            if(f_LL_GPIO_MT_Open_In() == m_GPIO_ON)
                { v_stop++; }
            if(f_LL_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_OPEN_TIMEOUT_US)     /* Time out */
                { v_stop++; }
            if(v_stop != 0)
            {
                f_LL_Tick_Systick32_Start(&s_context->v_time);
                f_LL_TIMER1_CH1_CCR(d_MOTOR_OPEN_END_DUTY); 
                f_LL_TIMER1_CH2_CCR(d_MOTOR_OPEN_END_DUTY); 
                ps_motor_ctrl->v_case_step++;
            }
            break;
        }
        case 2:
        {
            if(f_LL_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_OPEN_STOP_TIME_US)   /* Stop Delay */
            {
                f_LL_TIMER1_CH1_Mode_set(m_LL_TIMER_MODE_H);
                f_LL_TIMER1_CH2_Mode_set(m_LL_TIMER_MODE_H);
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
void f_Motor_Open_Close(ts_Scheduler_Ctrl *ps_motor_ctrl)
{
    ts_Motor_Context *s_context = ps_motor_ctrl->ps_user_struct;
    switch(ps_motor_ctrl->v_case_step)
    {
        case 0:
        {
            // f_LL_Tick_Systick_Run_Stamp_Print();
            // f_DBG_Print_String("I: CLOSE!");
            f_LL_Tick_Systick32_Start(&s_context->v_time);
            f_LL_TIMER1_CH1_CCR(d_MOTOR_CLOSE_START_DUTY); 
            f_LL_TIMER1_CH2_CCR(d_MOTOR_CLOSE_START_DUTY); 
            f_LL_TIMER1_CH1_Mode_set(m_LL_TIMER_MODE_PWM);
            f_LL_TIMER1_CH2_Mode_set(m_LL_TIMER_MODE_L);
            ps_motor_ctrl->v_case_step++;
            break;
        }
        case 1:
        {
            tu32 v_stop = 0;
            if(f_LL_GPIO_MT_Close_In() == m_GPIO_ON)
                { v_stop++; }
            if(f_LL_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_CLOSE_TIMEOUT_US)    /* Time out */
                { v_stop++; }
            if(v_stop != 0)
            {
                f_LL_Tick_Systick32_Start(&s_context->v_time);
                f_LL_TIMER1_CH1_CCR(d_MOTOR_CLOSE_END_DUTY); 
                f_LL_TIMER1_CH2_CCR(d_MOTOR_CLOSE_END_DUTY); 
                ps_motor_ctrl->v_case_step++;
            }
            break;
        }
        case 2:
        {
            if(f_LL_Tick_Systick32_Finish(&s_context->v_time) >= d_MOTOR_CLOSE_STOP_TIME_US)      /* Stop Delay */
            {
                f_LL_TIMER1_CH1_Mode_set(m_LL_TIMER_MODE_H);
                f_LL_TIMER1_CH2_Mode_set(m_LL_TIMER_MODE_H);
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
void f_LL_Motor_Load_Print(void)
{
    f_Scheduler_RunTime_Info_print(&gs_motor_Job_ctrl, m_DBG_MODE_SYSTEM, gs_motor_Job_ctrl.p_table_name);
}