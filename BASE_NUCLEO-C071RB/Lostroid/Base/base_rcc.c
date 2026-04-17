/********************************************************************
* File:    base_rcc.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "base_flash.h"
#include "base_tick.h"
#include "base_rcc.h"
#include "stm32c071xx.h"

#define d_BASE_RCC_HSE_TIMEOUT   1000             //+ 1000 = 1ms

//===================================================================
/* LL RCC initialize
-------------------------------------------------------------------*/
void f_Base_RCC_Init(void)
{
    f_Base_Flash_Set_Latency01();                 //+ Modify flash latency
    f_Base_RCC_APB_Enable();                      //+ Enable APB
    f_Base_RCC_HSE_Enable();                      //+ Enable HSE
    if(f_Base_RCC_HSE_Ready() == 0)               //+ Check HSE RUN
        { f_Base_RCC_HSE_Switch_Select(); }
    else
        { f_Base_RCC_HSE_Disable(); }             //+ Fail
    f_Base_RCC_Clock_Selection();
    f_Base_RCC_Clock_Enable();                    //+ Periphearl clock enable
}
//===================================================================
/* LL RCC Module
-------------------------------------------------------------------*/
void f_Base_RCC_Module(void)
{

}

//===================================================================
/* LL RCC Power and Clock Enable
-------------------------------------------------------------------*/
void f_Base_RCC_APB_Enable(void)
{
    RCC->APBENR2 |= RCC_APBENR2_SYSCFGEN;           //+ APB Clock enable
    while( (RCC->APBENR2 & RCC_APBENR2_SYSCFGEN) != RCC_APBENR2_SYSCFGEN);
    
    RCC->APBENR1 |= RCC_APBENR1_PWREN;              //+ APB Power enable
    while( (RCC->APBENR1 & RCC_APBENR1_PWREN) != RCC_APBENR1_PWREN);
}
//===================================================================
/* LL RCC HSE Enable
-------------------------------------------------------------------*/
void f_Base_RCC_HSE_Enable(void)
{
    RCC->CR |= RCC_CR_HSEON;                        //+ HSE Enable
}
//===================================================================
/* LL RCC HSE Disable
-------------------------------------------------------------------*/
void f_Base_RCC_HSE_Disable(void)
{
    RCC->CR &= (~RCC_CR_HSEON);                     //+ HSE Disable
}
//===================================================================
/* LL RCC HSE Sysclk Set
-------------------------------------------------------------------*/
void f_Base_RCC_HSE_Switch_Select(void)
{
    RCC->CFGR |= RCC_CFGR_SW_0;                     //+ Select HSE Switch
    while( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_0);
}
//===================================================================
/* LL RCC HSE Ready
-------------------------------------------------------------------*/
tu32 f_Base_RCC_HSE_Ready(void)
{
    tu32 v_time;
    f_Base_Tick_Systick32_Start(&v_time);
    while((RCC->CR & RCC_CR_HSERDY) == 0)           //+ HSE Ready
    {
        //+ Time Out 1ms */
        if(f_Base_Tick_Systick32_Finish(&v_time) > 1000)
            { return 1; }   //+ Failed
    }
    return 0;               //+ OK
}
//===================================================================
/* LL RCC Clock Enable
-------------------------------------------------------------------*/
void f_Base_RCC_Clock_Enable(void)
{
    //+ GPIOF:5|-|GPIOD:3|GPIOC:2|GPIOB:1|GPIOA:0
    RCC->IOPENR = ( RCC_IOPENR_GPIOCEN 
                  | RCC_IOPENR_GPIOBEN 
                  | RCC_IOPENR_GPIOAEN);

    //+ CRC:12|-|FLASH:8|-|DMA1:0
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* PWR:28|DBG:27|-|I2C2:22|I2C1:21|USART4:19|USART3:18|USART2:17|CRS:16
       SPI2:14|USB:13|FD CAN1:12|WWDG:11|RTC:10|-|TIM3:1|TIM2:0 */
    RCC->APBENR1 |= ( RCC_APBENR1_PWREN 
                    | RCC_APBENR1_DBGEN 
                    | RCC_APBENR1_USART2EN);

    /*  ADC:20|TIM17:18|TIM16:17|TIM15:16
      TIM14:15|USART1:14|~|SPI1:12|TIM1:11|SYSCFG:0 */
    RCC->APBENR2 |= ( RCC_APBENR2_ADCEN 
                    | RCC_APBENR2_TIM14EN 
                    | RCC_APBENR2_TIM1EN
                    | RCC_APBENR2_USART1EN 
                    | RCC_APBENR2_SYSCFGEN);
}
//===================================================================
/* LL RCC Clock Selection
-------------------------------------------------------------------*/
void f_Base_RCC_Clock_Selection(void)
{
    RCC->CCIPR = 0;
    RCC->CCIPR2 = 0;
}
//===================================================================
/* LL RCC Sleep/Stop Clock Disable
-------------------------------------------------------------------*/
void f_Base_RCC_Sleep_Stip_Clock_Disable(void)
{

}