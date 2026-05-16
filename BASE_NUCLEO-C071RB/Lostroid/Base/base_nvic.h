/********************************************************************
* File:    base_nvic.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_NVIC_H
#define H_BASE_NVIC_H

#include "base_nvic_type.h"

void f_base_nvic_init(void);
void f_base_nvic_set_flag(te_nvic e_nvic);
void f_base_nvic_set_clear(te_nvic e_nvic);
void f_base_nvic_pending_clear(te_nvic e_nvic);
void f_base_nvic_set_priority(te_irq e_irq, tu32 v_priority);

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void WWDG_IRQHandler(void);
void PVD_VDDIO2_IRQHandler(void);
void RTC_IRQHandler(void);
void FLASH_IRQHandler(void);
void RCC_CRS_IRQHandler(void);
void EXTI0_1_IRQHandler(void);
void EXTI2_3_IRQHandler(void);
void EXTI4_15_IRQHandler(void);
void USB_DRD_FS_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_3_IRQHandler(void);
void DMAMUX1_DMA1_CH4_5_IRQHandler(void);
void ADC1_IRQHandler(void);
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM14_IRQHandler(void);
void TIM16_IRQHandler(void);
void TIM17_IRQHandler(void);
void I2C1_IRQHandler(void);
void I2C2_IRQHandler(void);
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

#endif
