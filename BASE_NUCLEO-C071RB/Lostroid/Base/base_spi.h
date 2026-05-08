/********************************************************************
* File:    base_spi.h
* Author:  Lostroid
* Created: 2026-05-01
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_SPI_H
#define H_BASE_SPI_H

#include "stm32c071xx.h"
#include "../Scheduler/scheduler_type.h"
#include "base_spi_type.h"

void f_Base_SPI_Init(void);
void f_Base_SPI_Module(void);
void f_Base_SPI1_Init(void);
void f_Base_SPI1_DMA_Count_UP(void);
te_YesNo f_Base_SPI1_DMA_Send_Count_Check(void);
tu32 f_Base_SPI1_DMA_Idle(SPI_TypeDef *ps_spi);
te_Result f_Base_SPI1_DMA_Send(const tu8 *p_tx_buff, tu8 *p_rx_buff, tu16 v_len);

#endif
