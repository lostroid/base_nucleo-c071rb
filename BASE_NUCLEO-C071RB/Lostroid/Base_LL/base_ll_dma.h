/********************************************************************
* File:    base_ll_dma.h
* Author:  Lostroid
* Created: 2026-03-07
*
* Description:
* This is the LL DMA Control.
*
* Revision History:
*   2026-03-07  New.
********************************************************************/
#ifndef H_LL_DMA_H
#define H_LL_DMA_H

#include "stm32c071xx.h"
#include "base_ll_dma_type.h"

#define d_DMA1_UART1_TX             DMA1_Channel1
#define d_DMA1_UART1_RX             DMA1_Channel2
#define d_DMA1_UART2_TX             DMA1_Channel3
#define d_DMA1_UART2_RX             DMA1_Channel4

#define d_DMAMUX1_UART1_TX          DMAMUX1_Channel0
#define d_DMAMUX1_UART1_RX          DMAMUX1_Channel1
#define d_DMAMUX1_UART2_TX          DMAMUX1_Channel2
#define d_DMAMUX1_UART2_RX          DMAMUX1_Channel3

#define d_DMA1_CH1_UART1_TX_NDTR() ((tu16)d_DMA1_UART1_TX->CNDTR)
#define d_DMA1_CH2_UART1_RX_NDTR() ((tu16)d_DMA1_UART1_RX->CNDTR)
#define d_DMA1_CH4_UART2_TX_NDTR() ((tu16)d_DMA1_UART2_TX->CNDTR)
#define d_DMA1_CH3_UART2_RX_NDTR() ((tu16)d_DMA1_UART2_RX->CNDTR)

/// Transfer Complete flag
#define d_DMA1_CH1_UART1_TX_TC() (DMA1->ISR & DMA_ISR_TCIF1)
#define d_DMA1_CH2_UART1_RX_TC() (DMA1->ISR & DMA_ISR_TCIF2)
#define d_DMA1_CH3_UART2_TX_TC() (DMA1->ISR & DMA_ISR_TCIF3)
#define d_DMA1_CH4_UART2_RX_TC() (DMA1->ISR & DMA_ISR_TCIF4)

void f_LL_DMA_Init(void);
void f_LL_DMA_Module(void);
void f_LL_DMA_Memory_Setting(DMA_Channel_TypeDef *ps_dma_ch, const tu8 *p_buffer_addr, const tu32 v_length);
void f_LL_DMA_Setting(DMA_Channel_TypeDef *ps_dma_ch, volatile tu32 *p_peripheral, tu32 v_ccr_value);
void f_LL_DMA_Start(DMA_Channel_TypeDef *ps_dma_ch);
void f_LL_DMA_Stop(DMA_Channel_TypeDef *ps_dma_ch);
void f_LL_DMAMUX_Channel_ID_Set(DMAMUX_Channel_TypeDef *ps_dmaaux, te_DMAMUX_Multiplexer e_id);
void d_DMA1_CH1_UART1_TX_CTC(void);
void d_DMA1_CH2_UART1_RX_CTC(void);
void d_DMA1_CH3_UART2_TX_CTC(void);
void d_DMA1_CH4_UART2_RX_CTC(void);


#endif
