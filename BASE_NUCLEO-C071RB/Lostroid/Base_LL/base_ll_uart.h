/********************************************************************
* File:    base_ll_uart.h
* Author:  Lostroid
* Created: 2025-08-06
*
* Description:
* This is the uart Type.
*
* Revision History:
*   2025-08-06  New.
********************************************************************/
#ifndef H_LL_UART_H
#define H_LL_UART_H

#include "stm32c071xx.h"
#include "../Scheduler/scheduler_type.h"
#include "base_ll_uart_type.h"

void f_LL_UART_Init(void);
void f_LL_UART_Module(void);
void f_LL_UART1_TX_Done_Count_UP(void);
void f_LL_UART2_TX_Done_Count_UP(void);
te_YesNo f_LL_UART1_TX_Done_Check(void);
te_YesNo f_LL_UART2_TX_Done_Check(void);
void f_LL_UART1_Log_Enable(void);
void f_LL_UART1_Log_Disable(void);
void f_LL_UART1_Init(void);
void f_LL_UART2_Init(void);
te_Result f_LL_UART1_DMA_Send(const tu8 *p_data, const tu32 v_len);
te_Result f_LL_UART2_DMA_Send(const tu8 *p_data, const tu32 v_len);
tu32 f_LL_UART1_TX_Buff_Write(const tu8 *p_data, const tu32 v_len);
tu32 f_LL_UART2_TX_Buff_Write(const tu8 *p_data, const tu32 v_len);
void f_LL_UART1_Job_Rx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl);
void f_LL_UART2_Job_Rx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl);
void f_LL_UART1_Job_Tx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl);
void f_LL_UART2_Job_Tx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl);
te_Return f_LL_UART_TX_Idle_Check(USART_TypeDef *ps_uart);
void f_LL_Uart_Load_Print(void);

#endif
