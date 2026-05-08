/********************************************************************
* File:    base_dma_type.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
---------------------------------------------------------------------
01. a_ = array                         e.g. a_data[]
02. b_ = bit field                     e.g. b_Data
03. c_ = constant                      e.g, c_Data
04. d_ = #define macro                 e.g. d_Data
05. e_ = enum type                     e.g. e_Data
06. f_ = function                      e.g. f_Data
07. g_ = global (source file static)   e.g. gv_Data
08. i_ = inline function               e.g. i_Data
09. l_ = local static variable         e.g. lv_Data
10. m_ = enum member                   e.g. m_Data
11. p_ = pointer                       e.g. p_Data 
12. s_ = struct                        e.g. s_Data 
13. t_ = typedef                       e.g. t_Data 
14. u_ = union                         e.g. u_Data 
15. v_ = variable                      e.g. v_Data
16. x_ = extern variable               e.g. xv_data
********************************************************************/
#ifndef H_BASE_DMA_TYPE_H
#define H_BASE_DMA_TYPE_H

#include "types.h"

typedef enum{
    m_DMA_MUX_NONE,
    m_DMA_MUX_DMAMUX_GEN0_DMA,
    m_DMA_MUX_DMAMUX_GEN1_DMA,
    m_DMA_MUX_DMAMUX_GEN2_DMA,
    m_DMA_MUX_DMAMUX_GEN3_DMA,
    m_DMA_MUX_ADC1_DMA,
    m_DMA_MUX_RESERVED_6,
    m_DMA_MUX_RESERVED_7,
    m_DMA_MUX_RESERVED_8,
    m_DMA_MUX_RESERVED_9,
    m_DMA_MUX_I2C1_RX_DMA,
    m_DMA_MUX_I2C1_TX_DMA,
    m_DMA_MUX_I2C2_RX_DMA,
    m_DMA_MUX_I2C2_TX_DMA,
    m_DMA_MUX_RESERVED_14,
    m_DMA_MUX_RESERVED_15,
    m_DMA_MUX_SPI2S1_RX_DMA,
    m_DMA_MUX_SPI2S1_TX_DMA,
    m_DMA_MUX_SPI2_RX_DMA,
    m_DMA_MUX_SPI2_TX_DMA,
    m_DMA_MUX_TIM1_CH1_DMA,
    m_DMA_MUX_TIM1_CH2_DMA,
    m_DMA_MUX_TIM1_CH3_DMA,
    m_DMA_MUX_TIM1_CH4_DMA,
    m_DMA_MUX_TIM1_TRGI_COM_DMA,
    m_DMA_MUX_TIM1_UP_DMA,
    m_DMA_MUX_TIM2_CH1_DMA,
    m_DMA_MUX_TIM2_CH2_DMA,
    m_DMA_MUX_TIM2_CH3_DMA,
    m_DMA_MUX_TIM2_CH4_DMA,
    m_DMA_MUX_TIM2_TRGI_DMA,
    m_DMA_MUX_TIM2_UP_DMA,
    m_DMA_MUX_TIM3_CH1_DMA,
    m_DMA_MUX_TIM3_CH2_DMA,
    m_DMA_MUX_TIM3_CH3_DMA,
    m_DMA_MUX_TIM3_CH4_DMA,
    m_DMA_MUX_TIM3_TRGI_DMA,
    m_DMA_MUX_TIM3_UP_DMA,
    m_DMA_MUX_RESERVED_38,
    m_DMA_MUX_RESERVED_39,
    m_DMA_MUX_TIM15_CH1_DMA,
    m_DMA_MUX_TIM15_CH2_DMA,
    m_DMA_MUX_TIM15_TRGI_COM_DMA,
    m_DMA_MUX_TIM15_UP_DMA,
    m_DMA_MUX_TIM16_CH1_DMA,
    m_DMA_MUX_TIM16_TRGI_COM_DMA,
    m_DMA_MUX_TIM16_UP_DMA,
    m_DMA_MUX_TIM17_CH1_DMA,
    m_DMA_MUX_TIM17_TRGI_COM_DMA,
    m_DMA_MUX_TIM17_UP_DMA,
    m_DMA_MUX_USART1_RX_DMA,
    m_DMA_MUX_USART1_TX_DMA,
    m_DMA_MUX_USART2_RX_DMA,
    m_DMA_MUX_USART2_TX_DMA,
    m_DMA_MUX_USART3_RX_DMA,
    m_DMA_MUX_USART3_TX_DMA,
    m_DMA_MUX_USART4_RX_DMA,
    m_DMA_MUX_USART4_TX_DMA
}te_DMAMUX_Multiplexer;

typedef enum{
    m_DMAMUX_TRIGGER_EXTI0,
    m_DMAMUX_TRIGGER_EXTI1,
    m_DMAMUX_TRIGGER_EXTI2,
    m_DMAMUX_TRIGGER_EXTI3,
    m_DMAMUX_TRIGGER_EXTI4,
    m_DMAMUX_TRIGGER_EXTI5,
    m_DMAMUX_TRIGGER_EXTI6,
    m_DMAMUX_TRIGGER_EXTI7,
    m_DMAMUX_TRIGGER_EXTI8,
    m_DMAMUX_TRIGGER_EXTI9,
    m_DMAMUX_TRIGGER_EXTI10,
    m_DMAMUX_TRIGGER_EXTI11,
    m_DMAMUX_TRIGGER_EXTI12,
    m_DMAMUX_TRIGGER_EXTI13,
    m_DMAMUX_TRIGGER_EXTI14,
    m_DMAMUX_TRIGGER_EXTI15,
    m_DMAMUX_TRIGGER_DMAMUX_EVT0,
    m_DMAMUX_TRIGGER_DMAMUX_EVT1,
    m_DMAMUX_TRIGGER_DMAMUX_EVT2,
    m_DMAMUX_TRIGGER_DMAMUX_EVT3,
    m_DMAMUX_TRIGGER_RESERVED_20,
    m_DMAMUX_TRIGGER_RESERVED_22,
    m_DMAMUX_TRIGGER_TIM14_TRGO,
    m_DMAMUX_TRIGGER_RESERVED_24
}te_DMANUX_Trigger;

typedef enum{
    m_DMAMUX_TRIGGER_SYNC_EXTI0,
    m_DMAMUX_TRIGGER_SYNC_EXTI1,
    m_DMAMUX_TRIGGER_SYNC_EXTI2,
    m_DMAMUX_TRIGGER_SYNC_EXTI3,
    m_DMAMUX_TRIGGER_SYNC_EXTI4,
    m_DMAMUX_TRIGGER_SYNC_EXTI5,
    m_DMAMUX_TRIGGER_SYNC_EXTI6,
    m_DMAMUX_TRIGGER_SYNC_EXTI7,
    m_DMAMUX_TRIGGER_SYNC_EXTI8,
    m_DMAMUX_TRIGGER_SYNC_EXTI9,
    m_DMAMUX_TRIGGER_SYNC_EXTI10,
    m_DMAMUX_TRIGGER_SYNC_EXTI11,
    m_DMAMUX_TRIGGER_SYNC_EXTI12,
    m_DMAMUX_TRIGGER_SYNC_EXTI13,
    m_DMAMUX_TRIGGER_SYNC_EXTI14,
    m_DMAMUX_TRIGGER_SYNC_EXTI15,
    m_DMAMUX_TRIGGER_SYNC_DMAMUX_EVT0,
    m_DMAMUX_TRIGGER_SYNC_DMAMUX_EVT1,
    m_DMAMUX_TRIGGER_SYNC_DMAMUX_EVT2,
    m_DMAMUX_TRIGGER_SYNC_DMAMUX_EVT3,
    m_DMAMUX_TRIGGER_SYNC_RESERVED_20,
    m_DMAMUX_TRIGGER_SYNC_TIM14_TRGO,
    m_DMAMUX_TRIGGER_SYNC_RESERVED_22,
    m_DMAMUX_TRIGGER_SYNC_RESERVED_24
}te_DMANUX_Trigger_Sync;

#endif

