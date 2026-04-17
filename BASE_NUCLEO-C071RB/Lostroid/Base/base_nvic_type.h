/********************************************************************
* File:    base_nvic_type.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_NVIC_TYPE_H
#define H_BASE_NVIC_TYPE_H

#include "typedef.h"

typedef enum
{
    m_NVIC_WWDG,
    m_NVIC_PVD_VDDIO2,
    m_NVIC_RTC,
    m_NVIC_FLASH,
    m_NVIC_RCC_CRS,
    m_NVIC_EXTI0_1,
    m_NVIC_EXTI2_3,
    m_NVIC_EXTI4_15,
    m_NVIC_USB_DRD_FS,
    m_NVIC_DMA1_Channel1,
    m_NVIC_DMA1_Channel2_3,
    m_NVIC_DMAMUX1_DMA1_CH4_5,
    m_NVIC_ADC1,
    m_NVIC_TIM1_BRK_UP_TRG_COM,
    m_NVIC_TIM1_CC,
    m_NVIC_TIM2,
    m_NVIC_TIM3,
    m_NVIC_RESERVED_17,
    m_NVIC_RESERVED_18,
    m_NVIC_TIM14,
    m_NVIC_RESERVED_20,
    m_NVIC_TIM16,
    m_NVIC_TIM17,
    m_NVIC_I2C1,
    m_NVIC_I2C2,
    m_NVIC_SPI1,
    m_NVIC_SPI2,
    m_NVIC_USART1,
    m_NVIC_USART2,
    m_NVIC_USART3_4,
    m_NVIC_FDCAN_IT0,
    m_NVIC_FDCAN_IT1
}te_NVIC;

typedef enum
{
    m_IRQ0_WWDG = 0,
    m_IRQ1_PVD_VDDIO2 = 4,
    m_IRQ2_RTC = 8,
    m_IRQ3_FLASH = 12,

    m_IRQ4_RCC_CRS = 0,
    m_IRQ5_EXTI0_1 = 4,
    m_IRQ6_EXTI2_3 = 8,
    m_IRQ7_EXTI4_15 = 12,
    
    m_IRQ8_USB_DRD_FS = 0,
    m_IRQ9_DMA1_Channel1 = 4,
    m_IRQ10_DMA1_Channel2_3 = 8,
    m_IRQ11_DMAMUX1_DMA1_CH4_5 = 12,
    
    m_IRQ12_ADC1 = 0,
    m_IRQ13_TIM1_BRK_UP_TRG_COM = 4,
    m_IRQ14_TIM1_CC = 8,
    m_IRQ15_TIM2 = 12 ,
    
    m_IRQ16_TIM3 = 0,
    m_IRQ17_RESERVED_17 = 4,
    m_IRQ18_RESERVED_18 = 8,
    m_IRQ19_TIM14 = 12,
    
    m_IRQ20_RESERVED_20 = 0,
    m_IRQ21_TIM16 = 4,
    m_IRQ22_TIM17 = 8,
    m_IRQ23_I2C1 = 12,
    
    m_IRQ24_I2C2 = 0,
    m_IRQ25_SPI1 = 4,
    m_IRQ26_SPI2 = 8,
    m_IRQ27_USART1 = 12,

    m_IRQ28_USART2 = 0,
    m_IRQ29_USART3_4 = 4,
    m_IRQ30_FDCAN_IT0 = 8,
    m_IRQ31_FDCAN_IT1 = 12
}te_IRQ;


#endif
