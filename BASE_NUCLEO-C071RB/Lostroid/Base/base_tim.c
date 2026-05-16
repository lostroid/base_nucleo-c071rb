/********************************************************************
* File:    base_tim.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "../DBG/dbg.h"
#include "stm32c071xx.h"
#include "base_tim.h"
#include "base_tick.h"


#define d_BASE_TIM1_CH1_COUNT()     (TIM3->CCR1)    //+ SW OK
#define d_BASE_TIM3_CH2_COUNT()     (TIM1->CCR2)    //+ SW A
#define d_BASE_TIM3_CH3_COUNT()     (TIM1->CCR3)    //+ SW B
#define d_TICK_TIM14_100US          (TIM14->CNT)    //+ 16bit TIM14 Base 100us count

static tu32 v_base_tim1_ch2_sw_a_cnt = 0;
static tu32 v_base_tim1_ch3_sw_b_cnt = 0;
static tu32 v_base_tim3_ch1_sw_ok_cnt = 0;
//===================================================================
/*#### Base TIMx initialize
-------------------------------------------------------------------*/
void f_base_tim_init(void)
{
	f_base_tim2_init();
    f_base_tim_input_captur_init();
}
//===================================================================
/*#### Base TIMx Module
-------------------------------------------------------------------*/
void f_base_tim_module(void)
{

}
//===================================================================
/*#### TIM4 time check  (OverFlow 6.5s, Max Delay Set 6s)
---------------------------------------------------------------------
+  Return : m_RETURN_OK, m_RETURN_WAIT
-------------------------------------------------------------------*/
te_return f_base_tim14_check_100us(ts_base_tick_context* ps_time)
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
/*#### Base TIM1 initialize
-------------------------------------------------------------------*/
void f_base_tim2_init(void)
{
    //+ TIM2
    TIM2->PSC = (d_BASE_TICK_HCLK / 1000000) - 1;       //+ 48Mhz -> 1Mhz
    TIM2->ARR = 1000 - 1;                               //+ 1Mhz -> 1khz
    f_base_tim2_ch1_ccr(999);                         //+ CCR 500
    f_base_tim2_ch2_ccr(999);                         //+ CCR 500
    f_base_tim2_ch1_mode_set(m_BASE_TIM_MODE_H);
    f_base_tim2_ch2_mode_set(m_BASE_TIM_MODE_H);
    TIM2->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;    //+ OUTPUT Compare enable
    TIM2->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;         //+ complementary output enable
    TIM2->BDTR = TIM_BDTR_MOE;                          //+ Main output enable
    TIM2->EGR = TIM_EGR_UG;                             //+ count update
    TIM2->CR1 = TIM_CR1_CEN;                            //+ Counter enable
    
    TIM1->PSC = (d_BASE_TICK_HCLK / 1000000) - 1;       //+ 48Mhz -> 1Mhz
    TIM1->CR1 = TIM_CR1_CEN;                            //+ Counter enable
    TIM3->PSC = (d_BASE_TICK_HCLK / 1000000) - 1;       //+ 48Mhz -> 1Mhz
    TIM3->CR1 = TIM_CR1_CEN;                            //+ Counter enable
}
//===================================================================
/*#### Base Time TIM1 1CH CCR
---------------------------------------------------------------------
MAX 16 bit
+ v_cnt : Compare cnt value 
-------------------------------------------------------------------*/
void f_base_tim2_ch1_ccr(tu16 v_cnt)
{
    TIM2->CCR1 = v_cnt;
}
//===================================================================
/*#### Base Time TIM1 2CH CCR
---------------------------------------------------------------------
MAX 16 bit
+ v_cnt : Compare cnt value 
-------------------------------------------------------------------*/
void f_base_tim2_ch2_ccr(tu16 v_cnt)
{
    TIM2->CCR2 = v_cnt;
}
//===================================================================
/*#### Base Time TIM1 1CH MODE SET
---------------------------------------------------------------------
+ e_mode : H, L, PWM
-------------------------------------------------------------------*/
void f_base_tim2_ch1_mode_set(te_Base_tim_mode e_mode)
{
    switch(e_mode)
    {
        case m_BASE_TIM_MODE_H:
        {
            TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
                        | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_0;  //+ Force H
            break;
        }
        case m_BASE_TIM_MODE_L:
        {
            TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
                        | TIM_CCMR1_OC1M_2 ;                    //+ Force L
            break;
        }
        case m_BASE_TIM_MODE_PWM:
        {
            TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_OC1M_Msk)) 
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
/*#### Base Time TIM1 2CH MODE SET
---------------------------------------------------------------------
+ e_mode : H, L, PWM
-------------------------------------------------------------------*/
void f_base_tim2_ch2_mode_set(te_Base_tim_mode e_mode)
{
    switch(e_mode)
    {
        case m_BASE_TIM_MODE_H:
        {
            TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_0;  //+ Force H
            break;
        }
        case m_BASE_TIM_MODE_L:
        {
            TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 ;                    //+ Force L
            break;
        }
        case m_BASE_TIM_MODE_PWM:
        {
            TIM2->CCMR1 = (TIM2->CCMR1 & (~TIM_CCMR1_OC2M_Msk)) 
                        | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;  //+ Force PWM
            break;
        }
        default:
        {
            break;
        }
    }
}
//===================================================================
/*#### Base Time Input captur
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tim_input_captur_init(void)
{
    
    TIM1->TISEL = d_BASE_TIM_TI1_SEL(d_BASE_TIM_TI_00)    //+ Not use
                | d_BASE_TIM_TI2_SEL(d_BASE_TIM_TI_00)    //+ SW A CH2
                | d_BASE_TIM_TI3_SEL(d_BASE_TIM_TI_00)    //+ SW B CH3
                | d_BASE_TIM_TI4_SEL(d_BASE_TIM_TI_00);   //+ Not use

    TIM3->TISEL = d_BASE_TIM_TI1_SEL(d_BASE_TIM_TI_00)    //+ SW OK
                | d_BASE_TIM_TI2_SEL(d_BASE_TIM_TI_00)    //+ Not use
                | d_BASE_TIM_TI3_SEL(d_BASE_TIM_TI_00);   //+ Not use


    TIM1->CCMR1 = d_BASE_TIM_CC2S(1)   //+ SW A CH2
                | d_BASE_TIM_IC2F(15); //+ SW OK /32 N:8

    TIM1->CCMR2 = d_BASE_TIM_CC3S(1)   //+ SW B CH3
                | d_BASE_TIM_IC3F(15); //+ SW OK /32 N:8

    TIM3->CCMR1 = d_BASE_TIM_CC1S(1)   //+ SW OK
                | d_BASE_TIM_IC1F(15); //+ SW OK /32

    //+ Capture/Compare output enable
    TIM1->CCER = TIM_CCER_CC2E | TIM_CCER_CC3E; //+ SW A B
    TIM3->CCER = TIM_CCER_CC1E;

    //+ Interrupt enable
    TIM1->DIER = TIM_DIER_CC2IE | TIM_DIER_CC3IE;
    TIM3->DIER = TIM_DIER_CC1IE;

}
//===================================================================
/*#### TIM1 CH2 SW A Count update
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tim1_ch2_sw_a_count_up(void)
{
    v_base_tim1_ch2_sw_a_cnt++;
}
//===================================================================
/*#### TIM1 CH3 SW A Count update
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tim1_ch3_sw_b_count_up(void)
{
    v_base_tim1_ch3_sw_b_cnt++;
}
//===================================================================
/*#### TIM3 CH1 SW A Count update
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tim3_ch1_sw_ok_count_up(void)
{
    v_base_tim3_ch1_sw_ok_cnt++;
}
//===================================================================
/*#### TIM1 CH2 SW A Count update
---------------------------------------------------------------------
+ return: cnt
-------------------------------------------------------------------*/
tu32 f_base_tim1_ch2_sw_a_count_get(void)
{
    return v_base_tim1_ch2_sw_a_cnt;
}
//===================================================================
/*#### TIM1 CH3 SW B Count update
---------------------------------------------------------------------
+ return: cnt
-------------------------------------------------------------------*/
tu32 f_base_tim1_ch3_sw_b_count_get(void)
{
    return v_base_tim1_ch3_sw_b_cnt;
}
//===================================================================
/*#### TIM3 CH1 SW A Count update
---------------------------------------------------------------------
+ return: cnt
-------------------------------------------------------------------*/
tu32 f_Base_TIM3_CH1_SW_OK_Get_Count(void)
{
    return v_base_tim3_ch1_sw_ok_cnt;
}
//===================================================================
/*#### Switch Count print
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_tim_info_print(void)
{

}
