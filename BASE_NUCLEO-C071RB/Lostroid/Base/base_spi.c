/********************************************************************
* File:    base_spi.c
* Author:  Lostroid
* Created: 2026-05-01
* Encoding: UTF-8
********************************************************************/

#include "../CLI/cli_main.h"
#include "../Scheduler/scheduler.h"
#include "base_main.h"
#include "base_gpio.h"
#include "base_dma.h"
#include "base_spi.h"

static ts_base_spi_count gs_base_spi1_count = {
    .v_new = 0,
    .v_old = 0
};

//===================================================================
/*#### SPI Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_spi_init(void)
{   
    d_BASE_GPIO_SPI1_CS_H;
    f_base_spi1_init();
}
//===================================================================
/*#### SPI Module
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_spi_module(void)
{

}
//===================================================================
/*#### SPI1 Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_spi1_init(void)
{   
    SPI1->CR1 = SPI_CR1_MSTR        //+ 24Mzh,, Master, Mode0
              //| SPI_CR1_BR_0        //+ 001: fPCLK/4
              | SPI_CR1_SSM
              | SPI_CR1_SSI;
    SPI1->CR2 = SPI_CR2_FRXTH       //+ FIFO 1/4 (8bit)
              | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0  //+ (8bit)
              | SPI_CR2_TXDMAEN     //+ Tx buffer DMA enable
              | SPI_CR2_RXDMAEN;    //+ Rx buffer DMA enable
    SPI1->CR1 |= SPI_CR1_SPE;       //+ Enable
}
//===================================================================
/*#### SPI1 DMA Count up.
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_spi1_dma_count_up(void)
{
    d_BASE_GPIO_SPI1_CS_H;
    gs_base_spi1_count.v_new++;
}
//===================================================================
/*#### SPI1 DMA Count check. "SPI1 송수신 완료 상태 카운터 검사"
---------------------------------------------------------------------
+ return: m_YESNO_YES, m_YESNO_NO
-------------------------------------------------------------------*/
te_yes_no f_base_spi1_dma_count_check(void)
{
    if(gs_base_spi1_count.v_old != gs_base_spi1_count.v_new)
    { 
        gs_base_spi1_count.v_old = gs_base_spi1_count.v_new;
        return m_YESNO_YES; 
    }
    else
        { return m_YESNO_NO; }
}
//===================================================================
/*#### SPI1 DMA Idle Check  
---------------------------------------------------------------------
+ retrun: 1(running), 0(Idle)
-------------------------------------------------------------------*/
tu32 f_base_spi1_dma_idle(SPI_TypeDef *ps_spi)
{
    tu32 v_check = (SPI_SR_TXE) 
                 ^ ((SPI_SR_FTLVL | SPI_SR_FRLVL | SPI_SR_TXE | SPI_SR_RXNE | SPI_SR_BSY) & ps_spi->SR);
    if(v_check == 0)
    {
        f_base_dma_stop(d_BASE_DMA1_CH1_SPI1_TX);   //+ Re dheck DMA disable
        f_base_dma_stop(d_BASE_DMA1_CH2_SPI1_RX);   //+ Re check DMA disable.
        return 0;
    }
    return 1;
}

//===================================================================
/*#### SPI1 DMA Send "SPI1 전송"
---------------------------------------------------------------------
+ p_tx : Send buff pointer      "전송버퍼 포인터"
+ p_rx : Recive buff pointer    "수신버퍼 포인터"
+ v_len : Data len              "데이터 길이"
-------------------------------------------------------------------*/
te_result f_base_spi1_dma_send(const tu8 *p_tx_buff, tu8 *p_rx_buff, tu16 v_len)
{
    
    if((d_BASE_DMA1_CH1_SPI1_TX->CCR & DMA_CCR_EN) == DMA_CCR_EN)
        { return m_RESULT_FAIL; }
    else
    {
        d_BASE_GPIO_SPI1_CS_L;
        f_base_dma_memory_setting(d_BASE_DMA1_CH2_SPI1_RX, p_rx_buff, v_len);
        f_base_dma_memory_setting(d_BASE_DMA1_CH1_SPI1_TX, p_tx_buff, v_len);
        f_base_dma_start(d_BASE_DMA1_CH2_SPI1_RX);
        f_base_dma_start(d_BASE_DMA1_CH1_SPI1_TX);
        return m_RESULT_OK;
    }
}
