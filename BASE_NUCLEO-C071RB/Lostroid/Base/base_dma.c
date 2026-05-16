/********************************************************************
* File:    base_dma.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#include "base_dma.h"

//===================================================================
/*#### DMA Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_dma_init(void)
{
    tu32 v_ccr;
    v_ccr = DMA_CCR_MINC        //+ Memory address count up                 "메모리 주소 자동 증가"
          | DMA_CCR_PL_1        //+ Priority level 10: High
          | DMA_CCR_DIR         //+ Memory -> Peripheral                    "메모리에서 주변장치로 전송"
          | DMA_CCR_TCIE        //+ Transfer complete interrupt enable      "송신 완료 인터럽트 활성"
          | DMA_CCR_TEIE;       //+ Transfer error interrupt enable         "송신 에러 인터럽트 활성"
    f_base_dma_set(d_BASE_DMA1_CH1_SPI1_TX, &SPI1->DR, v_ccr);

    v_ccr = DMA_CCR_MINC        //+ Memory address count up                 "메모리 주소 자동 증가"
          | DMA_CCR_PL_1        //+ Priority level 10: High
          | DMA_CCR_TCIE        //+ Transfer complete interrupt enable      "송신 완료 인터럽트 활성"
          | DMA_CCR_TEIE;       //+ Transfer error interrupt enable         "송신 에러 인터럽트 활성"
    f_base_dma_set(d_BASE_DMA1_CH2_SPI1_RX, &SPI1->DR, v_ccr);
    
    v_ccr = DMA_CCR_MINC        //+ Memory address count up                 "메모리 주소 자동 증가"
          | DMA_CCR_DIR         //+ Memory -> Peripheral                    "메모리에서 주변장치로 전송"
          | DMA_CCR_TCIE        //+ Transfer complete interrupt enable      "송신 완료 인터럽트 활성"
          | DMA_CCR_TEIE;       //+ Transfer error interrupt enable         "송신 에러 인터럽트 활성"
    f_base_dma_set(d_BASE_DMA1_CH3_UART2_TX, &USART2->TDR, v_ccr);
    
    v_ccr = DMA_CCR_MINC        //+ Memory address count up                 "메모리 주소 자동 증가"
          | DMA_CCR_CIRC        //+ Circular mode                           "순환 구조 사용"
          | DMA_CCR_TEIE;       //+ Transfer error interrupt enable         "송신 완료 인터럽트 활성"
    f_base_dma_set(d_BASE_DMA1_CH4_UART2_RX, &USART2->RDR, v_ccr);

    v_ccr = DMA_CCR_MINC        //+ Memory address count up                 "메모리 주소 자동 증가"
          | DMA_CCR_TEIE        //+ Transfer error interrupt enable         "송신 완료 인터럽트 활성"
          | DMA_CCR_PSIZE_0     //+ Peripheral 16Bit                        "장치 16bit"
          | DMA_CCR_MSIZE_0;    //+ Memory 16bit                            "메모리 16bit"
    f_base_dma_set(d_BASE_DMA1_CH5_ADC1, &ADC1->DR, v_ccr);

    f_base_dmamux_channel_id_set(d_BASE_DMAMUX1_CH0_SPI1_TX, m_DMA_MUX_SPI2S1_TX_DMA);
    f_base_dmamux_channel_id_set(d_BASE_DMAMUX1_CH1_SPI1_RX, m_DMA_MUX_SPI2S1_RX_DMA);
    f_base_dmamux_channel_id_set(d_BASE_DMAMUX1_CH2_UART2_TX, m_DMA_MUX_USART2_TX_DMA);
    f_base_dmamux_channel_id_set(d_BASE_DMAMUX1_CH3_UART2_RX, m_DMA_MUX_USART2_RX_DMA);
    f_base_dmamux_channel_id_set(d_BASE_DMAMUX1_CH4_ADC1, m_DMA_MUX_ADC1_DMA);
}
//===================================================================
/*#### DMA Module
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_dma_module(void)
{

}
//===================================================================
/*#### DMA Memory Setting
---------------------------------------------------------------------
+ pa_dma_ch: DMAs_Channels pointer      "DMA 채널 포인터"
+ p_buffer_addr: Buffer address pointer "버퍼 주소"
+ v_length: Buffer size "버퍼 사이즈"
-------------------------------------------------------------------*/
void f_base_dma_memory_setting(DMA_Channel_TypeDef *ps_dma_ch, const tu8 *p_buffer_addr, const tu32 v_length)
{
    ps_dma_ch->CMAR     = (tu32)p_buffer_addr;
    ps_dma_ch->CNDTR    = v_length;
}
//===================================================================
/*#### DMA Setting
---------------------------------------------------------------------
+ peripheral: peripheral pointer "DMA 채널 포인터"
+ v_ccr_value: CCR Vaule         "설정"
-------------------------------------------------------------------*/
void f_base_dma_set(DMA_Channel_TypeDef *ps_dma_ch, volatile tu32 *p_peripheral, tu32 v_ccr_value)
{
    ps_dma_ch->CPAR     = (tu32)p_peripheral;
    ps_dma_ch->CCR      = v_ccr_value;
}
//===================================================================
/*#### DMA Start
---------------------------------------------------------------------
+ peripheral: peripheral pointer "DMA 채널 포인터"
-------------------------------------------------------------------*/
void f_base_dma_start(DMA_Channel_TypeDef *ps_dma_ch)
{
    ps_dma_ch->CCR |= DMA_CCR_EN;
}
//===================================================================
/*#### DMA stop
---------------------------------------------------------------------
+ peripheral: peripheral pointer "DMA 채널 포인터"
-------------------------------------------------------------------*/
void f_base_dma_stop(DMA_Channel_TypeDef *ps_dma_ch)
{
    ps_dma_ch->CCR &= (~DMA_CCR_EN);
}
//===================================================================
/*#### DMAMUX1 Channel0 CxCR ID Setting
---------------------------------------------------------------------
+ ps_dmaaux: DMAMUX Channel
+ e_id: ID
-------------------------------------------------------------------*/
void f_base_dmamux_channel_id_set(DMAMUX_Channel_TypeDef *ps_dmaaux, te_dmamux_multiplexer e_id)
{
    ps_dmaaux->CCR &= (tu32) (~DMAMUX_CxCR_DMAREQ_ID_Msk);              //+ Erase ID
    ps_dmaaux->CCR |= (tu32) (DMAMUX_CxCR_DMAREQ_ID_Msk & (tu32)e_id);  //+ Write ID
}
//===================================================================
/*#### UART1 TX Transfer Complete clear 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_dma1_ch1_spi1_tx_ctc(void)
{ 
    f_base_dma_stop(d_BASE_DMA1_CH1_SPI1_TX);
    DMA1->IFCR |= DMA_IFCR_CTCIF1; 
}
//===================================================================
/*#### UART1 RX Transfer Complete clear 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_dma1_ch2_spi1_rx_ctc(void)
{ 
    DMA1->IFCR |= DMA_IFCR_CTCIF2; 
}
//===================================================================
/*#### UART2 TX Transfer Complete clear 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_dma1_ch3_uart2_tx_ctc(void)
{
    f_base_dma_stop(d_BASE_DMA1_CH3_UART2_TX);
    DMA1->IFCR |= DMA_IFCR_CTCIF3; 
}
//===================================================================
/*#### UART2 RX Transfer Complete clear 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void d_base_dma1_ch4_uart2_rx_ctc(void)
{ 
    DMA1->IFCR |= DMA_IFCR_CTCIF4;
}
//===================================================================
/*#### ADC1 Transfer Complete clear 
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void d_Base_dma1_ch5_adc_ctc(void)
{ 
    f_base_dma_stop(d_BASE_DMA1_CH5_ADC1);
    DMA1->IFCR |= DMA_IFCR_CTCIF5;
}

