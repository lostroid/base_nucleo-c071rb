/********************************************************************
* File:    base_dma.h
* Author:  Lostroid
* Created: 2026-03-07
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_DMA_H
#define H_BASE_DMA_H

#include "stm32c071xx.h"
#include "base_dma_type.h"

#define d_BASE_DMA1_CH1_SPI1_TX             DMA1_Channel1
#define d_BASE_DMA1_CH2_SPI1_RX             DMA1_Channel2
#define d_BASE_DMA1_CH3_UART2_TX            DMA1_Channel3
#define d_BASE_DMA1_CH4_UART2_RX            DMA1_Channel4
#define d_BASE_DMA1_CH5_ADC1                DMA1_Channel5

#define d_BASE_DMAMUX1_CH0_SPI1_TX          DMAMUX1_Channel0
#define d_BASE_DMAMUX1_CH1_SPI1_RX          DMAMUX1_Channel1
#define d_BASE_DMAMUX1_CH2_UART2_TX         DMAMUX1_Channel2
#define d_BASE_DMAMUX1_CH3_UART2_RX         DMAMUX1_Channel3
#define d_BASE_DMAMUX1_CH4_ADC1             DMAMUX1_Channel4

#define d_BASE_DMA1_CH1_SPI1_TX_NDTR()      ((tu16)d_BASE_DMA1_CH1_SPI1_TX->CNDTR)
#define d_BASE_DMA1_CH2_SPI1_RX_NDTR()      ((tu16)d_BASE_DMA1_CH2_SPI1_RX->CNDTR)
#define d_BASE_DMA1_CH4_UART2_TX_NDTR()     ((tu16)d_BASE_DMA1_CH3_UART2_TX->CNDTR)
#define d_BASE_DMA1_CH3_UART2_RX_NDTR()     ((tu16)d_BASE_DMA1_CH4_UART2_RX->CNDTR)
#define d_BASE_DMA1_CH5_ADC1_NDTR()         ((tu16)d_BASE_DMA1_CH5_ADC1->CNDTR)

/// Transfer Complete flag
#define d_BASE_DMA1_CH1_SPI1_TX_TC()        (DMA1->ISR & DMA_ISR_TCIF1)
#define d_BASE_DMA1_CH2_SPI1_RX_TC()        (DMA1->ISR & DMA_ISR_TCIF2)
#define d_BASE_DMA1_CH3_UART2_TX_TC()       (DMA1->ISR & DMA_ISR_TCIF3)
#define d_BASE_DMA1_CH4_UART2_RX_TC()       (DMA1->ISR & DMA_ISR_TCIF4)
#define d_BASE_DMA1_CH5_ADC1_TC()           (DMA1->ISR & DMA_ISR_TCIF5)

void f_base_dma_init(void);
void f_base_dma_module(void);
void f_base_dma_memory_setting(DMA_Channel_TypeDef *ps_dma_ch, const tu8 *p_buffer_addr, const tu32 v_length);
void f_base_dma_set(DMA_Channel_TypeDef *ps_dma_ch, volatile tu32 *p_peripheral, tu32 v_ccr_value);
void f_base_dma_start(DMA_Channel_TypeDef *ps_dma_ch);
void f_base_dma_stop(DMA_Channel_TypeDef *ps_dma_ch);
void f_base_dmamux_channel_id_set(DMAMUX_Channel_TypeDef *ps_dmaaux, te_dmamux_multiplexer e_id);
void f_base_dma1_ch1_spi1_tx_ctc(void);
void f_base_dma1_ch2_spi1_rx_ctc(void);
void f_base_dma1_ch3_uart2_tx_ctc(void);
void d_base_dma1_ch4_uart2_rx_ctc(void);
void d_Base_dma1_ch5_adc_ctc(void);

#endif
