/********************************************************************
* File:    base_uart.h
* Author:  Lostroid
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_UART_H
#define H_BASE_UART_H

#include "stm32c071xx.h"
#include "../Scheduler/scheduler_type.h"
#include "base_uart_type.h"

void f_base_uart_init(void);
void f_base_uart_module(void);
void f_base_uart2_init(void);
void f_base_uart2_tx_done_count_up(void);
te_yes_no f_base_uart2_tx_done_check(void);
void f_base_uart2_log_enable(void);
void f_base_uart2_log_disable(void);

te_result f_base_uart2_dma_send(const tu8 *p_data, const tu32 v_len);
tu32 f_base_uart2_tx_buff_write(const tu8 *p_data, const tu32 v_len);
te_return f_base_uart_tx_idle_check(USART_TypeDef *ps_uart);
void f_base_uart_load_print(void);

void f_base_uart_job_start(ts_scheduler_control *ps_uart_ctrl);
void f_base_uart2_job_rx_counter_update(ts_scheduler_control *ps_uart_ctrl);
void f_base_uart2_job_tx_check(ts_scheduler_control *ps_uart_ctrl);
void f_base_uart2_job_rx_check(ts_scheduler_control *ps_uart_ctrl);


#endif
