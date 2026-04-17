/********************************************************************
* File:    base_timer.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "../DBG/dbg.h"
#include "stm32c071xx.h"
#include "base_timer.h"
#include "base_tick.h"

#define d_TICK_TIM14_100US      (TIM14->CNT)    //+ 16bit TIM14 Base 100us count

//===================================================================
/*### LL TIMx initialize
-------------------------------------------------------------------*/
void f_Base_Timer_Init(void)
{
	f_Base_TIMER1_Init();
}
//===================================================================
/*### LL TIMx Module
-------------------------------------------------------------------*/
void f_Base_Timer_Module(void)
{

}
//===================================================================
/*### TIM4 time check  (OverFlow 6.5s, Max Delay Set 6s)
---------------------------------------------------------------------
+  Return : m_RETURN_OK, m_RETURN_WAIT
-------------------------------------------------------------------*/
te_Return f_Base_TIM14_Check_100us(ts_Base_Tick_Context* ps_time)
{
    ps_time->v_ct = d_TICK_TIM14_100US;  //- 100us counter data.
    ps_time->v_tt += (tu32)((tu16)ps_time->v_ct - (tu16)ps_time->v_pt);
    ps_time->v_pt = ps_time->v_ct;
    if(ps_time->v_tt >= ps_time->v_st)
    {
        ps_time->v_tt -= ps_time->v_st;
        return m_RETURN_OK;
    }
    else
    {
        return m_RETURN_WAIT;
    }
}
//===================================================================
/*### LL TIM1 initialize
-------------------------------------------------------------------*/
void f_Base_TIMER1_Init(void)
{
    TIM1->PSC = (d_BASE_TICK_HCLK / 1000000) - 1;     //+ 48Mhz -> 1Mhz
    TIM1->ARR = 1000 - 1;                               //+ 1Mhz -> 1khz
    f_Base_TIMER1_CH1_CCR(999);                           //+ CCR 500
    f_Base_TIMER1_CH2_CCR(999);                           //+ CCR 500
    f_Base_TIMER1_CH1_Mode_set(m_BASE_TIMER_MODE_H);
    f_Base_TIMER1_CH2_Mode_set(m_BASE_TIMER_MODE_H);
    TIM1->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;    //+ OUTPUT Compare enable
    TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;         //+ complementary output enable
    TIM1->BDTR = TIM_BDTR_MOE;                          //+ Main output enable
    TIM1->EGR = TIM_EGR_UG;                             //+ count update
    TIM1->CR1 = TIM_CR1_CEN;                            //+ Counter enable
}
//===================================================================
/*### LL Time TIM1 1CH CCR
---------------------------------------------------------------------
MAX 16 bit
+ v_cnt : Compare cnt value 
-------------------------------------------------------------------*/
void f_Base_TIMER1_CH1_CCR(tu16 v_cnt)
{
    TIM1->CCR1 = v_cnt;
}
//===================================================================
/*### LL Time TIM1 2CH CCR
---------------------------------------------------------------------
MAX 16 bit
+ v_cnt : Compare cnt value 
-------------------------------------------------------------------*/
void f_Base_TIMER1_CH2_CCR(tu16 v_cnt)
{
    TIM1->CCR2 = v_cnt;
}
//===================================================================
/*### LL Time TIM1 1CH MODE SET
---------------------------------------------------------------------
+ e_mode : H, L, PWM
-------------------------------------------------------------------*/
void f_Base_TIMER1_CH1_Mode_set(te_Base_Timer_Mode e_mode)
{
    switch(e_mode)
    {
        case m_BASE_TIMER_MODE_H:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
                        | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0;  //+ Force H
            break;
        }
        case m_BASE_TIMER_MODE_L:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
                        | TIM_CCMR1_OC1M_2 ;                    //+ Force L
            break;
        }
        case m_BASE_TIMER_MODE_PWM:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
                        | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;  //+ Force PWM
            break;
        }
        default:
        {
            break;
        }
    }
}
//===================================================================
/*### LL Time TIM1 2CH MODE SET
---------------------------------------------------------------------
+ e_mode : H, L, PWM
-------------------------------------------------------------------*/
void f_Base_TIMER1_CH2_Mode_set(te_Base_Timer_Mode e_mode)
{
    switch(e_mode)
    {
        case m_BASE_TIMER_MODE_H:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_0;  //+ Force H
            break;
        }
        case m_BASE_TIMER_MODE_L:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 ;                    //+ Force L
            break;
        }
        case m_BASE_TIMER_MODE_PWM:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;  //+ Force PWM
            break;
        }
        default:
        {
            break;
        }
    }
}
