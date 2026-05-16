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

void f_base_spi_init(void);
void f_base_spi_module(void);
void f_base_spi1_init(void);
void f_base_spi1_dma_count_up(void);
te_yes_no f_base_spi1_dma_count_check(void);
tu32 f_base_spi1_dma_idle(SPI_TypeDef *ps_spi);
te_result f_base_spi1_dma_send(const tu8 *p_tx_buff, tu8 *p_rx_buff, tu16 v_len);

#endif
