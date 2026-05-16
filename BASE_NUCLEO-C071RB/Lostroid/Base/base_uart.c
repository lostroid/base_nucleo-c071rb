/********************************************************************
* File:    base_uart.c
* Author:  Lostroid
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/

#include "../CLI/cli_main.h"
#include "../Scheduler/scheduler.h"
#include "base_main.h"
#include "base_dma.h"
#include "base_uart.h"

#define d_BASE_UART_STRING_MAX          64
#define d_BASE_UART2_TX_BUFFER_SIZE     1024
#define d_BASE_UART2_RX_BUFFER_SIZE     64

static tu32 gv_base_uart2_log_enable = 0;

static tu8 ga_base_uart2_tx_buff[d_BASE_UART2_TX_BUFFER_SIZE];
static tu8 ga_base_uart2_rx_buff[d_BASE_UART2_RX_BUFFER_SIZE];

static tu32 gv_base_uart2_tx_cnt_prev = 0;  //+ UART2 TX Previous Interrupt count "UART2 TX 기존 카운터"
static tu32 gv_base_uart2_tx_cnt_curr = 0;  //+ UART2 TX Current Interrupt Count  "UART2 TX 현재 카운터"

static ts_base_uart_buff_ctrol gs_base_uart2_tx_pos = {
    .cv_max_size = d_BASE_UART2_TX_BUFFER_SIZE,
    .v_read_pos = 0,
    .v_write_pos = 0,
    .v_Error = 0
};
static ts_base_uart_buff_ctrol gs_base_uart2_rx_pos = {
    .cv_max_size = d_BASE_UART2_RX_BUFFER_SIZE,
    .v_read_pos = 0,
    .v_write_pos = 0,
    .v_Error = 0
};

static ts_scheduler_control gs_base_uart_Job_ctrl;
enum { m_BASE_UART_JOB_TABLE_SIZE = 4 };
static void (*paf_base_uart_Job_table[m_BASE_UART_JOB_TABLE_SIZE])(ts_scheduler_control *ps_job_ctrl) = {
    f_base_uart_job_start,              //+ 00
    f_base_uart2_job_tx_check,          //+ 01
    f_base_uart2_job_rx_counter_update, //+ 02
    f_base_uart2_job_rx_check           //+ 03
};
//===================================================================
/*#### UART Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_uart_init(void)
{
    f_scheduler_init( 
        &gs_base_uart_Job_ctrl
        , paf_base_uart_Job_table
        , "uart_Job_table"
        , m_BASE_UART_JOB_TABLE_SIZE);
    
    f_scheduler_config(
        &gs_base_uart_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , d_NULL);

    f_base_uart2_init();
}
//===================================================================
/*#### UART Module
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_uart_module(void)
{
    f_scheduler_run(&gs_base_uart_Job_ctrl);
}
//===================================================================
/*#### UART2 Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_uart2_init(void)
{
    USART2->CR3 = 0;
    USART2->CR1 = 0;                //+ Disable
    USART2->CR1 = USART_CR1_TE
                | USART_CR1_RE;     //+ 송수신 TE(1), RE(1) 설정
    USART2->CR2 = 0;                //+ Reset
    USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;                
    USART2->BRR = 0x00000030lu;     //+ x16 Sampling 48Mhz/16x1000000bps = (3d << 4 ) = 30h

    USART2->CR1 |= USART_CR1_UE;    //+ UART Enable
    f_base_dma_memory_setting(d_BASE_DMA1_CH4_UART2_RX, ga_base_uart2_rx_buff, d_BASE_UART2_RX_BUFFER_SIZE);
    f_base_dma_start(d_BASE_DMA1_CH4_UART2_RX);
}
//===================================================================
/*#### UART2 TX done count up
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_base_uart2_tx_done_count_up(void)
{
    gv_base_uart2_tx_cnt_curr++;
}
//===================================================================
/*#### UART2 TX done check
---------------------------------------------------------------------
+ return: m_YES, m_NO
-------------------------------------------------------------------*/
te_yes_no f_base_uart2_tx_done_check(void)
{
    if(gv_base_uart2_tx_cnt_prev != gv_base_uart2_tx_cnt_curr)
    {
        gv_base_uart2_tx_cnt_prev = gv_base_uart2_tx_cnt_curr;
        return m_YESNO_YES;
    }
    else 
        { return m_YESNO_NO; }
}
//===================================================================
/* Base UART1 log enable
-------------------------------------------------------------------*/
void f_base_uart2_log_enable(void)
{
    gv_base_uart2_log_enable = 1;
}
//===================================================================
/* Base UART1 log diable
-------------------------------------------------------------------*/
void f_base_uart2_log_disable(void)
{
    gv_base_uart2_log_enable = 0;
}

//===================================================================
/* Base UART1 write buffer write
-------------------------------------------------------------------*/
te_result f_base_uart2_dma_send(const tu8 *p_data, const tu32 v_len)
{
    if((d_BASE_DMA1_CH3_UART2_TX->CCR & DMA_CCR_EN) == DMA_CCR_EN)
        { return m_RESULT_FAIL; }
    else
    {
        f_base_dma_memory_setting(d_BASE_DMA1_CH3_UART2_TX, p_data, v_len);
        f_base_dma_start(d_BASE_DMA1_CH3_UART2_TX);
        return m_RESULT_OK;
    }
}
//===================================================================
/* Base UART2 write buffer
-------------------------------------------------------------------*/
tu32 f_base_uart2_tx_buff_write(const tu8 *p_data, const tu32 v_len)
{
    tu32 v_write_pos = gs_base_uart2_tx_pos.v_write_pos;  /// Last write postion
    tu32 v_read_pos = gs_base_uart2_tx_pos.v_read_pos;    /// Last read(send) postion
    tu32 v_while = 0;
    tu32 v_error = 0;

    while(v_while < v_len)
    {
        /// Next Write Position
        v_write_pos++;
        if(v_write_pos == gs_base_uart2_tx_pos.cv_max_size)
            { v_write_pos = 0u; }                       /// Reset Ring Buff.
        /// Overflow Check
        if(v_write_pos == v_read_pos)                                  
        {
            gs_base_uart2_tx_pos.v_Error++;               /// Overflow count up
            v_error++;
            break;
        }
        else
        { ga_base_uart2_tx_buff[v_write_pos] = p_data[v_while]; }     /// Data Read & RXNE Clear
        v_while++;
    }
    gs_base_uart2_tx_pos.v_write_pos = v_write_pos;                   /// update Write Postion
    return v_error;
}
//===================================================================
/* UART TX Idle check : TX 후유상태 확인
---------------------------------------------------------------------
+ *ps_uart  USART_TypeDef
+ Return
m_RETURN_WAIT
m_RETURN_OK
-------------------------------------------------------------------*/
te_return f_base_uart_tx_idle_check(USART_TypeDef *ps_uart)
{
    //+ USART Enable Check : UART 활성 상태 확인
    if( (ps_uart->CR1 & (USART_CR1_UE | USART_CR1_TE)) != (USART_CR1_UE | USART_CR1_TE) )
        { return m_RETURN_WAIT; }
    //+ USART TX Idle check : TX 후유 상태 확인
    if( (ps_uart->ISR & (USART_ISR_TC | USART_ISR_TXE_TXFNF)) != (USART_ISR_TC | USART_ISR_TXE_TXFNF) )
        { return m_RETURN_WAIT; }
    else
        { return m_RETURN_OK; }
}
//===================================================================
/*#### Uart module load print : Uart 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_base_uart_load_print(void)
{
    f_scheduler_run_time_info_print(&gs_base_uart_Job_ctrl);
}
//===================================================================
/* UART Start
-------------------------------------------------------------------*/
void f_base_uart_job_start(ts_scheduler_control *ps_uart_ctrl)
{
    f_scheduler_loop_time_update(ps_uart_ctrl);
    f_scheduler_next(ps_uart_ctrl);
}
//===================================================================
/* UART2 RX DMA Circular check
-------------------------------------------------------------------*/
void f_base_uart2_job_rx_counter_update(ts_scheduler_control *ps_uart_ctrl)
{
    /* UART2 RX DMA1 CHannel 3 */
    gs_base_uart2_rx_pos.v_write_pos = gs_base_uart2_rx_pos.cv_max_size - ((tu32)d_BASE_DMA1_CH3_UART2_RX_NDTR());
    f_scheduler_next(ps_uart_ctrl);
}
//===================================================================
/*#### UART2 TX DMA Send : UART2 TX 실제 버퍼를 통해 DMA 전송
---------------------------------------------------------------------
+ *ps_uart  USART_TypeDef
+ Return : m_RETURN_WAIT, m_RETURN_OK
-------------------------------------------------------------------*/
void f_base_uart2_job_tx_check(ts_scheduler_control *ps_uart_ctrl)
{
    tu32 v_write_pos = gs_base_uart2_tx_pos.v_write_pos;
    tu32 v_read_pos  = gs_base_uart2_tx_pos.v_read_pos;

    if(f_base_uart_tx_idle_check(USART2) == m_RETURN_OK)
    {
        if(v_write_pos != v_read_pos)
        {
            tu16 v_uart_data_len = 0;                    //+ Data len
            tu16 v_uart_send_pos = 0;                   //+ Send Position
            if(v_write_pos > v_read_pos)                //+ Check Circular over
            {
                v_uart_data_len = v_write_pos - v_read_pos;
                v_uart_send_pos = v_write_pos;
            }
            else
            {
                v_uart_data_len = (gs_base_uart2_tx_pos.cv_max_size - v_read_pos);
                v_uart_data_len--;
                v_uart_send_pos = gs_base_uart2_tx_pos.cv_max_size;
                v_uart_send_pos--;
            }
            
            v_read_pos++;   /// Next Postion
            if(v_read_pos == gs_base_uart2_tx_pos.cv_max_size)
            {   v_uart_data_len = v_write_pos;
                v_uart_data_len++;
                v_uart_send_pos = v_write_pos;
                v_read_pos = 0;
            }
            if(f_base_uart2_dma_send(&ga_base_uart2_tx_buff[v_read_pos], v_uart_data_len) == m_RESULT_OK)
                { gs_base_uart2_tx_pos.v_read_pos = v_uart_send_pos; }
        }
    }
    f_scheduler_next(ps_uart_ctrl);
}
//===================================================================
/*#### Check UART2 RX data "UART2의 수신데이터를 확인"
---------------------------------------------------------------------
+ *ps_uart_ctrl: Control pointer "제어 변수 포인터"
-------------------------------------------------------------------*/
void f_base_uart2_job_rx_check(ts_scheduler_control *ps_uart_ctrl)
{
    tu32 v_write_pos = gs_base_uart2_rx_pos.v_write_pos;
    tu32 v_read_pos = gs_base_uart2_rx_pos.v_read_pos;
    
    if(v_write_pos == gs_base_uart2_rx_pos.cv_max_size)
    {
        (void)USART2->RDR;
        USART2->ICR = (USART_ICR_ORECF | USART_ICR_EOBCF | USART_ICR_CMCF | USART_ICR_FECF);
        USART2->CR3 |= USART_CR3_EIE;
        f_base_dma_memory_setting(d_BASE_DMA1_CH4_UART2_RX, ga_base_uart2_rx_buff, d_BASE_UART2_RX_BUFFER_SIZE);
        v_write_pos = v_read_pos; 
    }

    while(v_write_pos != v_read_pos)
    {
        tu8 v_data = ga_base_uart2_rx_buff[v_read_pos];
        v_read_pos++; 
        if(v_read_pos == gs_base_uart2_rx_pos.cv_max_size)
            { v_read_pos = 0;  }
        
        f_base_uart2_tx_buff_write(&v_data, 1);
        //+ data check
        gs_base_uart2_rx_pos.v_read_pos = v_read_pos;
    }
    f_scheduler_next(ps_uart_ctrl);
}

