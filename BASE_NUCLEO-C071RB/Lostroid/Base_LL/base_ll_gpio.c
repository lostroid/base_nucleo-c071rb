/********************************************************************
* File:    base_ll_flash.c
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the LL(Low Level) Flash Control.
*
* Revision History:
*   2025-08-06  New.
********************************************************************/

#include "stm32c071xx.h"
#include "base_ll_gpio.h"

#define d_LL_GPIO_LED_LD1_H         (GPIOA->BSRR = GPIO_BSRR_BS5)
#define d_LL_GPIO_LED_LD1_L         (GPIOA->BSRR = GPIO_BSRR_BR5)

#define d_LL_GPIO_LED_LD2_H         (GPIOC->BSRR = GPIO_BSRR_BS9)
#define d_LL_GPIO_LED_LD2_L         (GPIOC->BSRR = GPIO_BSRR_BR9)

#define d_LL_GPIO_MT_POWER_H        (GPIOA->BSRR = GPIO_BSRR_BS8)
#define d_LL_GPIO_MT_POWER_L        (GPIOA->BSRR = GPIO_BSRR_BR8)

#define d_LL_GPIO_MT_OPEN_H         (GPIOA->BSRR = GPIO_BSRR_BS15)
#define d_LL_GPIO_MT_OPEN_L         (GPIOA->BSRR = GPIO_BSRR_BR15)

#define d_LL_GPIO_MT_CLOSE_H        (GPIOB->BSRR = GPIO_BSRR_BS3)
#define d_LL_GPIO_MT_CLOSE_L        (GPIOB->BSRR = GPIO_BSRR_BR3)

#define d_LL_GPIO_MAIN_POWER_H      (GPIOB->BSRR = GPIO_BSRR_BS6)
#define d_LL_GPIO_MAIN_POWER_L      (GPIOB->BSRR = GPIO_BSRR_BR6)

#define d_LL_GPIO_SW_OK_IN          (GPIOC->IDR & GPIO_IDR_ID6)
#define d_LL_GPIO_SW_OK_A_IN        (GPIOA->IDR & GPIO_IDR_ID11)
#define d_LL_GPIO_SW_OK_B_IN        (GPIOA->IDR & GPIO_IDR_ID12)
#define d_LL_GPIO_MT_OPEN_IN        (GPIOB->IDR & GPIO_IDR_ID4)
#define d_LL_GPIO_MT_CLOSE_IN       (GPIOB->IDR & GPIO_IDR_ID5)
#define d_LL_GPIO_CHARGE_IN         (GPIOB->IDR & GPIO_IDR_ID7)
#define d_LL_GPIO_SW_POWER          (GPIOB->IDR & GPIO_IDR_ID8)

//===================================================================
/* LL GPIO initialize
-------------------------------------------------------------------*/
void f_LL_GPIO_Init(void)
{
    f_LL_GPIO_MODE_Initialize();
    f_LL_GPIO_SPEED_Initialize();
    f_LL_GPIO_SPEED_Initialize();
    f_LL_GPIO_AF_Initialize();

    f_LL_GPIO_Motor_Power_OnOff(m_GPIO_ON);
    f_LL_GPIO_Motor_Open_OnOff(m_GPIO_ON);
    f_LL_GPIO_Motor_Close_OnOff(m_GPIO_ON);
}
//===================================================================
/* LL GPIO MODE initialize (Input,Output,AF,Analog)
-------------------------------------------------------------------*/
void f_LL_GPIO_MODE_Initialize(void)
{
	GPIOA->MODER = ( d_GPIO_A00_MODE_UART1_TX
                   | d_GPIO_A01_MODE_UART1_RX
                   | d_GPIO_A02_MODE_UART2_TX
                   | d_GPIO_A03_MODE_UART2_RX
                   | d_GPIO_A04_MODE_DC5V_IN4
                   | d_GPIO_A05_MODE_LED_LD1
                   | d_GPIO_A06_MODE_C5V_IN6
                   | d_GPIO_A07_MODE_C12V_IN7
                   | d_GPIO_A08_MODE_MT_POWER
                   | d_GPIO_A09_MODE_DISABLE
                   | d_GPIO_A10_MODE_DISABLE
                   | d_GPIO_A11_MODE_SW_A
                   | d_GPIO_A12_MODE_SW_B
                   | d_GPIO_A13_MODE_DBG_SWDIO
                   | d_GPIO_A14_MODE_DBG_SWCLK
                   | d_GPIO_A15_MODE_MT_OPEN );

    GPIOB->MODER = ( d_GPIO_B00_MODE_BATTERY_TEMP_IN17
                   | d_GPIO_B01_MODE_MOTOR_IN18
                   | d_GPIO_B02_MODE_DISABLE
                   | d_GPIO_B03_MODE_MT_CLOSE
                   | d_GPIO_B04_MODE_MT_SENSOR_OPEN
                   | d_GPIO_B05_MODE_MT_SENSOR_CLOSE
                   | d_GPIO_B06_MODE_STAT_OUT
                   | d_GPIO_B07_MODE_STAT_IN
                   | d_GPIO_B08_MODE_SW_POWER
                   | d_GPIO_B09_MODE_DISABLE
                   | d_GPIO_B10_MODE_DISABLE
                   | d_GPIO_B11_MODE_DISABLE
                   | d_GPIO_B12_MODE_DISABLE
                   | d_GPIO_B13_MODE_DISABLE
                   | d_GPIO_B14_MODE_DISABLE
                   | d_GPIO_B15_MODE_DISABLE );

    GPIOC->MODER = ( d_GPIO_C00_MODE_DISABLE
                   | d_GPIO_C01_MODE_DISABLE
                   | d_GPIO_C02_MODE_DISABLE
                   | d_GPIO_C03_MODE_DISABLE
                   | d_GPIO_C04_MODE_DISABLE
                   | d_GPIO_C05_MODE_DISABLE
                   | d_GPIO_C06_MODE_SW_OK
                   | d_GPIO_C07_MODE_DISABLE
                   | d_GPIO_C08_MODE_DISABLE
                   | d_GPIO_C09_MODE_LED_LD2
                   | d_GPIO_C10_MODE_DISABLE
                   | d_GPIO_C11_MODE_DISABLE
                   | d_GPIO_C12_MODE_DISABLE
                   | d_GPIO_C13_MODE_DISABLE
                   | d_GPIO_C14_MODE_OSCX_IN
                   | d_GPIO_C15_MODE_OSCX_OUT );  
}
//===================================================================
/* LL GPIO SPEED initialize (Very-Low, Low, High, Very-High)
-------------------------------------------------------------------*/
void f_LL_GPIO_SPEED_Initialize(void)
{
    GPIOA->OSPEEDR = 0x0C000000;    /* Reset value */
    GPIOB->OSPEEDR = 0;
    GPIOC->OSPEEDR = 0;
}
//===================================================================
/* LL GPIO Pull-up, pull-down initialize
-------------------------------------------------------------------*/
void f_LL_GPIO_PUPD_Initialize(void)
{
    GPIOA->PUPDR = d_GPIO_PUPD_14(d_GPIO_PUPD_DOWN)
                 | d_GPIO_PUPD_13(d_GPIO_PUPD_UP);    /* Reset value */
    GPIOB->PUPDR = 0;
    GPIOC->PUPDR = 0;
}
//===================================================================
/* LL GPIO AF initialize
-------------------------------------------------------------------*/
void f_LL_GPIO_AF_Initialize(void)
{
    GPIOA->AFR[0] = d_GPIO_00_AFSEL(4) | d_GPIO_01_AFSEL(4)   /* AF USART1 */
                  | d_GPIO_02_AFSEL(1) | d_GPIO_03_AFSEL(1);  /* AF USART2 */
    GPIOA->AFR[1] = d_GPIO_15_AFSEL(2);     /* TIM1_CH1 */

    GPIOB->AFR[0] = d_GPIO_03_AFSEL(1);     /* TIM1_CH2 */
}

//===================================================================
/* LL GPIO LED LD1
-------------------------------------------------------------------*/
void f_LL_GPIO_LED_LD1_OnOff(te_GPIO_Output e_onoff)
{
    if(e_onoff == m_GPIO_OFF)
        { d_LL_GPIO_LED_LD1_H; }
    else
        { d_LL_GPIO_LED_LD1_L; }
}
//===================================================================
/* LL GPIO LED LD2
-------------------------------------------------------------------*/
void f_LL_GPIO_LED_LD2_OnOff(te_GPIO_Output e_onoff)
{
    if(e_onoff == m_GPIO_OFF)
        { d_LL_GPIO_LED_LD2_H; }
    else
        { d_LL_GPIO_LED_LD2_L; }
}
//===================================================================
/* LL GPIO AF initialize
-------------------------------------------------------------------*/
void f_LL_GPIO_Motor_Power_OnOff(te_GPIO_Output e_onoff)
{
    if(e_onoff == m_GPIO_OFF)
        { d_LL_GPIO_MT_POWER_H; }
    else
        { d_LL_GPIO_MT_POWER_L; }
}
//===================================================================
/* LL GPIO AF initialize
-------------------------------------------------------------------*/
void f_LL_GPIO_Motor_Open_OnOff(te_GPIO_Output e_onoff)
{
    if(e_onoff == m_GPIO_OFF)
        { d_LL_GPIO_MT_OPEN_L; }
    else
        { d_LL_GPIO_MT_OPEN_H; }
}
//===================================================================
/* LL GPIO AF initialize
-------------------------------------------------------------------*/
void f_LL_GPIO_Motor_Close_OnOff(te_GPIO_Output e_onoff)
{
    if(e_onoff == m_GPIO_OFF)
        { d_LL_GPIO_MT_CLOSE_L; }
    else
        { d_LL_GPIO_MT_CLOSE_H; }
}
//===================================================================
/* LL GPIO AF initialize
-------------------------------------------------------------------*/
void f_LL_GPIO_Main_Power_OnOff(te_GPIO_Output e_onoff)
{
        if(e_onoff == m_GPIO_OFF)
        { d_LL_GPIO_MAIN_POWER_L; }
    else
        { d_LL_GPIO_MAIN_POWER_H; }
}
//===================================================================
/* LL GPIO SW OK
-------------------------------------------------------------------*/
te_GPIO_Output f_LL_GPIO_SW_OK_In(void)
{
    if(d_LL_GPIO_SW_OK_IN == 0)
        { return m_GPIO_OFF; }
    return m_GPIO_ON;
}
//===================================================================
/* LL GPIO SW OK A
-------------------------------------------------------------------*/
te_GPIO_Output f_LL_GPIO_SW_OK_A_In(void)
{
    if(d_LL_GPIO_SW_OK_A_IN == 0)
        { return m_GPIO_OFF; }
    return m_GPIO_ON;
}
//===================================================================
/* LL GPIO SW OK B
-------------------------------------------------------------------*/
te_GPIO_Output f_LL_GPIO_SW_OK_B_In(void)
{
    if(d_LL_GPIO_SW_OK_B_IN == 0)
        { return m_GPIO_OFF; }
    return m_GPIO_ON;
}
//===================================================================
/* LL GPIO Motor open
-------------------------------------------------------------------*/
te_GPIO_Output f_LL_GPIO_MT_Open_In(void)
{
    if(d_LL_GPIO_MT_OPEN_IN == 0)
        { return m_GPIO_ON; }
    return m_GPIO_OFF;
}
//===================================================================
/* LL GPIO Motor close
-------------------------------------------------------------------*/
te_GPIO_Output f_LL_GPIO_MT_Close_In(void)
{
    if(d_LL_GPIO_MT_CLOSE_IN == 0)
        { return m_GPIO_ON; }
    return m_GPIO_OFF;
}
//===================================================================
/* LL GPIO Charge
-------------------------------------------------------------------*/
te_GPIO_Output f_LL_GPIO_Charge_In(void)
{
    if(d_LL_GPIO_CHARGE_IN == 0)
        { return m_GPIO_OFF; }
    return m_GPIO_ON;
}
//===================================================================
/* LL GPIO power
-------------------------------------------------------------------*/
te_GPIO_Output f_LL_GPIO_SW_Power_In(void)
{
    if(d_LL_GPIO_SW_POWER == 0)
        { return m_GPIO_ON; }
    return m_GPIO_OFF;
}
