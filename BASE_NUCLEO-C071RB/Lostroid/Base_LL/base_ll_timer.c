/********************************************************************
* File:    base_ll_timer.c
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the tim control.
*
* Revision History:
*   2025-08-06  New.
********************************************************************/

#include "../DBG/dbg.h"
#include "stm32c071xx.h"
#include "base_ll_timer.h"
#include "base_ll_tick.h"

//===================================================================
/*### LL TIMx initialize
-------------------------------------------------------------------*/
void f_LL_TIMERx_Init(void)
{
	f_LL_TIMER1_Init();
}
//===================================================================
/*### LL TIMx Module
-------------------------------------------------------------------*/
void f_LL_TIMERx_Module(void)
{

}
//===================================================================
/*### LL TIM1 initialize
-------------------------------------------------------------------*/
void f_LL_TIMER1_Init(void)
{
    TIM1->PSC = (d_LL_TICK_HCLK_CLK / 1000000) - 1;     //+ 48Mhz -> 1Mhz
    TIM1->ARR = 1000 - 1;                               //+ 1Mhz -> 1khz
    f_LL_TIMER1_CH1_CCR(999);                           //+ CCR 500
    f_LL_TIMER1_CH2_CCR(999);                           //+ CCR 500
    f_LL_TIMER1_CH1_Mode_set(m_LL_TIMER_MODE_H);
    f_LL_TIMER1_CH2_Mode_set(m_LL_TIMER_MODE_H);
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
void f_LL_TIMER1_CH1_CCR(tu16 v_cnt)
{
    TIM1->CCR1 = v_cnt;
}
//===================================================================
/*### LL Time TIM1 2CH CCR
---------------------------------------------------------------------
MAX 16 bit
+ v_cnt : Compare cnt value 
-------------------------------------------------------------------*/
void f_LL_TIMER1_CH2_CCR(tu16 v_cnt)
{
    TIM1->CCR2 = v_cnt;
}
//===================================================================
/*### LL Time TIM1 1CH MODE SET
---------------------------------------------------------------------
+ e_mode : H, L, PWM
-------------------------------------------------------------------*/
void f_LL_TIMER1_CH1_Mode_set(te_LL_TIMER_Mode e_mode)
{
    switch(e_mode)
    {
        case m_LL_TIMER_MODE_H:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
                        | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0;  //+ Force H
            break;
        }
        case m_LL_TIMER_MODE_L:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
                        | TIM_CCMR1_OC1M_2 ;                    //+ Force L
            break;
        }
        case m_LL_TIMER_MODE_PWM:
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
void f_LL_TIMER1_CH2_Mode_set(te_LL_TIMER_Mode e_mode)
{
    switch(e_mode)
    {
        case m_LL_TIMER_MODE_H:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_0;  //+ Force H
            break;
        }
        case m_LL_TIMER_MODE_L:
        {
            TIM1->CCMR1 = (TIM1->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 ;                    //+ Force L
            break;
        }
        case m_LL_TIMER_MODE_PWM:
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
