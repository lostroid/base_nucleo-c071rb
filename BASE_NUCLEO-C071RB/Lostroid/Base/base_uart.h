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

void f_Base_UART_Init(void);
void f_Base_UART_Module(void);
void f_Base_UART2_Init(void);
void f_Base_UART2_TX_Done_Count_UP(void);
te_YesNo f_Base_UART2_TX_Done_Check(void);
void f_Base_UART2_Log_Enable(void);
void f_Base_UART2_Log_Disable(void);

te_Result f_Base_UART2_DMA_Send(const tu8 *p_data, const tu32 v_len);
tu32 f_Base_UART2_TX_Buff_Write(const tu8 *p_data, const tu32 v_len);
te_Return f_Base_UART_TX_Idle_Check(USART_TypeDef *ps_uart);
void f_Base_Uart_Load_Print(void);

void f_Base_UART_Job_Start(ts_Scheduler_Control *ps_uart_ctrl);
void f_Base_UART2_Job_Rx_Counter_Update(ts_Scheduler_Control *ps_uart_ctrl);
void f_Base_UART2_Job_Tx_Check(ts_Scheduler_Control *ps_uart_ctrl);
void f_Base_UART2_Job_Rx_Check(ts_Scheduler_Control *ps_uart_ctrl);


#endif
