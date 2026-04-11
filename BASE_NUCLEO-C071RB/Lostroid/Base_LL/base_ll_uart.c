/********************************************************************
* File:    base_ll_uart.c
* Author:  Lostroid
* Created: 2025-08-06
*
* Description:
* This is the uart Type.
*
* Revision History:
*   2025-08-06  New.
********************************************************************/

#include "../CLI/cli_main.h"
#include "../Scheduler/scheduler.h"
#include "base_ll_main.h"
#include "base_ll_dma.h"
#include "base_ll_uart.h"

#define d_LL_UART_STRING_MAX          64
#define d_LL_UART1_TX_BUFFER_SIZE     512
#define d_LL_UART1_RX_BUFFER_SIZE     64
#define d_LL_UART2_TX_BUFFER_SIZE     512
#define d_LL_UART2_RX_BUFFER_SIZE     64

static tu32 gv_ll_uart1_log_enable = 0;

static tu8 gv_ll_uart1_tx_buff[d_LL_UART1_TX_BUFFER_SIZE];
static tu8 gv_ll_uart1_rx_buff[d_LL_UART1_RX_BUFFER_SIZE];
static tu8 gv_ll_uart2_tx_buff[d_LL_UART2_TX_BUFFER_SIZE];
static tu8 gv_ll_uart2_rx_buff[d_LL_UART2_RX_BUFFER_SIZE];

static tu32 gv_ll_uart1_tx_cnt_prev = 0;    /// UART1 TX Previous Interrupt count.
static tu32 gv_ll_uart1_tx_cnt_curr = 0;    /// UART1 TX Current Interrupt Count.
static tu32 gv_ll_uart2_tx_cnt_prev = 0;    /// UART2 TX Previous Interrupt count.
static tu32 gv_ll_uart2_tx_cnt_curr = 0;    /// UART2 TX Current Interrupt Count.


static ts_LL_Uart_Ctrol gs_ll_uart1_tx_pos = {
    .cv_max_size = d_LL_UART1_TX_BUFFER_SIZE,
    .v_read_pos = 0,
    .v_write_pos = 0,
    .v_Error = 0
};
static ts_LL_Uart_Ctrol gs_ll_uart1_rx_pos = {
    .cv_max_size = d_LL_UART1_RX_BUFFER_SIZE,
    .v_read_pos = 0,
    .v_write_pos = 0,
    .v_Error = 0
};
static ts_LL_Uart_Ctrol gs_ll_uart2_tx_pos = {
    .cv_max_size = d_LL_UART2_TX_BUFFER_SIZE,
    .v_read_pos = 0,
    .v_write_pos = 0,
    .v_Error = 0
};
static ts_LL_Uart_Ctrol gs_ll_uart2_rx_pos = {
    .cv_max_size = d_LL_UART2_RX_BUFFER_SIZE,
    .v_read_pos = 0,
    .v_write_pos = 0,
    .v_Error = 0
};

static ts_Scheduler_Ctrl gs_ll_uart_Job_ctrl;
enum { m_LL_UART_JOB_TABLE_SIZE = 4 };
static void (*paf_ll_uart_Job_table[m_LL_UART_JOB_TABLE_SIZE])(ts_Scheduler_Ctrl *ps_main_ctrl) = {
    f_LL_UART1_Job_Tx_Check,    /* 00 */
    f_LL_UART1_Job_Rx_Check,    /* 01 */
    f_LL_UART2_Job_Tx_Check,    /* 02 */
    f_LL_UART2_Job_Rx_Check     /* 03 */
};
///==================================================================
/*### UART Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_LL_UART_Init(void)
{
    f_Scheduler_Init( 
        &gs_ll_uart_Job_ctrl
        , paf_ll_uart_Job_table
        , "uart_Job_table"
        , m_LL_UART_JOB_TABLE_SIZE);
    
    f_Scheduler_Config(
        &gs_ll_uart_Job_ctrl
        , 0
        , 3
        , 0
        , 0
        , d_NULL);

    f_LL_UART1_Init();
    f_LL_UART2_Init();
}
///==================================================================
/*### UART Module
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_LL_UART_Module(void)
{
    f_Scheduler_Run(&gs_ll_uart_Job_ctrl);
}
///==================================================================
/*### UART1 TX done count up
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_LL_UART1_TX_Done_Count_UP(void)
{
    gv_ll_uart1_tx_cnt_curr++;
}
///==================================================================
/*### UART2 TX done count up
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_LL_UART2_TX_Done_Count_UP(void)
{
    gv_ll_uart2_tx_cnt_curr++;
}
///==================================================================
/*### UART1 TX done check
---------------------------------------------------------------------
+ return: m_YES, m_NO
-------------------------------------------------------------------*/
te_YesNo f_LL_UART1_TX_Done_Check(void)
{
    if(gv_ll_uart1_tx_cnt_prev == gv_ll_uart1_tx_cnt_curr)
        { return m_NO; }
    else 
    {
        gv_ll_uart1_tx_cnt_prev = gv_ll_uart1_tx_cnt_curr;
        return m_YES;
    }
}
///==================================================================
/*### UART2 TX done check
---------------------------------------------------------------------
+ return: m_YES, m_NO
-------------------------------------------------------------------*/
te_YesNo f_LL_UART2_TX_Done_Check(void)
{
    if(gv_ll_uart2_tx_cnt_prev == gv_ll_uart2_tx_cnt_curr)
        { return m_NO; }
    else 
    {
        gv_ll_uart2_tx_cnt_prev = gv_ll_uart2_tx_cnt_curr;
        return m_YES;
    }
}
///==================================================================
/* LL UART1 log enable
-------------------------------------------------------------------*/
void f_LL_UART1_Log_Enable(void)
{
    gv_ll_uart1_log_enable = 1;
}
///==================================================================
/* LL UART1 log diable
-------------------------------------------------------------------*/
void f_LL_UART1_Log_Disable(void)
{
    gv_ll_uart1_log_enable = 0;
}
///==================================================================
/*### UART1 Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_LL_UART1_Init(void)
{
    USART1->CR3 = 0;
    USART1->CR1 = 0;                //+ Disable
    USART1->CR1 = USART_CR1_TE
                | USART_CR1_RE;     //+ 송수신 TE(1), RE(1) 설정
    USART1->CR2 = 0;                //+ Reset
    USART1->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;  
    USART1->BRR = 0x00000030lu;     //+ x16 Sampling 48Mhz/16x1000000bps = (3d << 4 ) = 30h

    USART1->CR1 |= USART_CR1_UE;    //+ UART Enable
    f_LL_DMA_Memory_Setting(d_DMA1_UART1_RX, gv_ll_uart1_rx_buff, d_LL_UART1_RX_BUFFER_SIZE);
    f_LL_DMA_Start(d_DMA1_UART1_RX);
}
///==================================================================
/*### UART2 Initialize
---------------------------------------------------------------------
+ void
-------------------------------------------------------------------*/
void f_LL_UART2_Init(void)
{
    USART2->CR3 = 0;
    USART2->CR1 = 0;                //+ Disable
    USART2->CR1 = USART_CR1_TE
                | USART_CR1_RE;     //+ 송수신 TE(1), RE(1) 설정
    USART2->CR2 = 0;                //+ Reset
    USART2->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;                
    USART2->BRR = 0x00000030lu;     //+ x16 Sampling 48Mhz/16x1000000bps = (3d << 4 ) = 30h

    USART2->CR1 |= USART_CR1_UE;    //+ UART Enable
    f_LL_DMA_Memory_Setting(d_DMA1_UART2_RX, gv_ll_uart2_rx_buff, d_LL_UART1_RX_BUFFER_SIZE);
    f_LL_DMA_Start(d_DMA1_UART2_RX);
}

///==================================================================
/*### UART1 DMA SEND
---------------------------------------------------------------------
+ p_data: pointer
+ v_len: len
-------------------------------------------------------------------*/
te_Result f_LL_UART1_DMA_Send(const tu8 *p_data, const tu32 v_len)
{
    if((d_DMA1_UART1_TX->CCR & DMA_CCR_EN) == DMA_CCR_EN)
        { return m_FAIL; }
    else
    {
        f_LL_DMA_Memory_Setting(d_DMA1_UART1_TX, p_data, v_len);
        f_LL_DMA_Start(d_DMA1_UART1_TX);
        return m_OK;
    }
}

///==================================================================
/* LL UART1 write buffer write
-------------------------------------------------------------------*/
te_Result f_LL_UART2_DMA_Send(const tu8 *p_data, const tu32 v_len)
{
    if((d_DMA1_UART2_TX->CCR & DMA_CCR_EN) == DMA_CCR_EN)
        { return m_FAIL; }
    else
    {
        f_LL_DMA_Memory_Setting(d_DMA1_UART2_TX, p_data, v_len);
        f_LL_DMA_Start(d_DMA1_UART2_TX);
        return m_OK;
    }
}

///==================================================================
/* LL UART1 write buffer write
-------------------------------------------------------------------*/
tu32 f_LL_UART1_TX_Buff_Write(const tu8 *p_data, const tu32 v_len)
{
    tu32 v_write_pos = gs_ll_uart1_tx_pos.v_write_pos;  /* Last write postion*/
    tu32 v_read_pos  = gs_ll_uart1_tx_pos.v_read_pos;   /* Last read(send) postion*/
    tu32 v_while = 0;
    tu32 v_error = 0;

    while(v_while < v_len)
    {
        /// Next Write Position
        v_write_pos++;
        if(v_write_pos == gs_ll_uart1_tx_pos.cv_max_size)
            { v_write_pos = 0u; }                       /// Reset Ring Buff.
        /// Check Ring Buff Overflow Check..
        if(v_write_pos == v_read_pos)                                  
        {
            gs_ll_uart1_tx_pos.v_Error++;               /// Overflow count up
            v_error++;
            break;
        }
        else
        { gv_ll_uart1_tx_buff[v_write_pos] = p_data[v_while]; }  /// Buff Write 
        v_while++;
    }
    gs_ll_uart1_tx_pos.v_write_pos = v_write_pos;               /// update Write Postion
    return v_error;
}

///==================================================================
/* LL UART2 write buffer
-------------------------------------------------------------------*/
tu32 f_LL_UART2_TX_Buff_Write(const tu8 *p_data, const tu32 v_len)
{
    tu32 v_write_pos = gs_ll_uart2_tx_pos.v_write_pos;  /// Last write postion
    tu32 v_read_pos = gs_ll_uart2_tx_pos.v_read_pos;    /// Last read(send) postion
    tu32 v_while = 0;
    tu32 v_error = 0;

    while(v_while < v_len)
    {
        /// Next Write Position
        v_write_pos++;
        if(v_write_pos == gs_ll_uart2_tx_pos.cv_max_size)
            { v_write_pos = 0u; }                       /// Reset Ring Buff.
        /// Overflow Check
        if(v_write_pos == v_read_pos)                                  
        {
            gs_ll_uart2_tx_pos.v_Error++;               /// Overflow count up
            v_error++;
            break;
        }
        else
        { gv_ll_uart2_tx_buff[v_write_pos] = p_data[v_while]; }     /// Data Read & RXNE Clear
        v_while++;
    }
    gs_ll_uart2_tx_pos.v_write_pos = v_write_pos;                   /// update Write Postion
    return v_error;
}

///==================================================================
/* LL UART1 RX DMA Circular check
-------------------------------------------------------------------*/
void f_LL_UART1_Job_Rx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl)
{
    /* UART1 RX DMA1 Channel 2 */
    gs_ll_uart1_rx_pos.v_write_pos = gs_ll_uart1_rx_pos.cv_max_size
                                   - ((tu16)d_DMA1_CH2_UART1_RX_NDTR());
    f_Scheduler_Next(ps_uart_ctrl);
}

///==================================================================
/* LL UART2 RX DMA Circular check
-------------------------------------------------------------------*/
void f_LL_UART2_Job_Rx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl)
{
    /* UART2 RX DMA1 CHannel 3 */
    gs_ll_uart2_rx_pos.v_write_pos = gs_ll_uart2_rx_pos.cv_max_size 
                                   - ((tu32)d_DMA1_CH3_UART2_RX_NDTR());
    f_Scheduler_Next(ps_uart_ctrl);
}

///==================================================================
/* LL UART Tx DMA Send
-------------------------------------------------------------------*/
void f_LL_UART1_Job_Tx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl)
{
    /* UART1 TX DMA1 Channel 1 */
    tu32 v_write_pos = gs_ll_uart1_tx_pos.v_write_pos;
    tu32 v_read_pos  = gs_ll_uart1_tx_pos.v_read_pos;

    if(v_write_pos != v_read_pos)
    {
        tu16 v_BUTC_DataLen = 0;                    //+ Data len
        tu16 v_BUTC_Send_Pos = 0;                   //+ Send Position
        if(v_write_pos > v_read_pos)                //+ Check Circular over
        {
            v_BUTC_DataLen = v_write_pos - v_read_pos;
            v_BUTC_Send_Pos = v_write_pos;
        }
        else
        {
            v_BUTC_DataLen = (gs_ll_uart1_tx_pos.cv_max_size - v_read_pos);
            v_BUTC_DataLen--;
            v_BUTC_Send_Pos = gs_ll_uart1_tx_pos.cv_max_size;
            v_BUTC_Send_Pos--;
        }
        
        v_read_pos++;   /// Next Postion
        if(v_read_pos == gs_ll_uart1_tx_pos.cv_max_size)
        {   v_BUTC_DataLen = v_write_pos;
            v_BUTC_DataLen++;
            v_BUTC_Send_Pos = v_write_pos;
            v_read_pos = 0;
        }

        if(f_LL_UART_TX_Idle_Check(USART1) == m_RETURN_OK)
        {
            if(f_LL_UART2_DMA_Send(&gv_ll_uart1_tx_buff[v_read_pos], v_BUTC_DataLen) == m_OK)
                { gs_ll_uart1_tx_pos.v_read_pos = v_BUTC_Send_Pos; }
        }
    }
    f_Scheduler_Next(ps_uart_ctrl);
}

///==================================================================
/*### LL UART2 TX DMA Send : UART2 TX 실제 버퍼를 통해 DMA 전송
---------------------------------------------------------------------
+ *ps_uart  USART_TypeDef
+ Return
m_RETURN_WAIT
m_RETURN_OK
-------------------------------------------------------------------*/
void f_LL_UART2_Job_Tx_Check(ts_Scheduler_Ctrl *ps_uart_ctrl)
{
    /* UART2 TX DMA1 Channel 4 */
    tu32 v_write_pos = gs_ll_uart2_tx_pos.v_write_pos;
    tu32 v_read_pos  = gs_ll_uart2_tx_pos.v_read_pos;

    if(v_write_pos != v_read_pos)
    {
        tu16 v_BUTC_DataLen = 0;                    //+ Data len
        tu16 v_BUTC_Send_Pos = 0;                   //+ Send Position
        if(v_write_pos > v_read_pos)                //+ Check Circular over
        {
            v_BUTC_DataLen = v_write_pos - v_read_pos;
            v_BUTC_Send_Pos = v_write_pos;
        }
        else
        {
            v_BUTC_DataLen = (gs_ll_uart2_tx_pos.cv_max_size - v_read_pos);
            v_BUTC_DataLen--;
            v_BUTC_Send_Pos = gs_ll_uart2_tx_pos.cv_max_size;
            v_BUTC_Send_Pos--;
        }
        
        v_read_pos++;   /// Next Postion
        if(v_read_pos == gs_ll_uart2_tx_pos.cv_max_size)
        {   v_BUTC_DataLen = v_write_pos;
            v_BUTC_DataLen++;
            v_BUTC_Send_Pos = v_write_pos;
            v_read_pos = 0;
        }

        if(f_LL_UART_TX_Idle_Check(USART2) == m_RETURN_OK)
        {
            if(f_LL_UART2_DMA_Send(&gv_ll_uart2_tx_buff[v_read_pos], v_BUTC_DataLen) == m_OK)
                { gs_ll_uart2_tx_pos.v_read_pos = v_BUTC_Send_Pos; }
        }
    }
    f_Scheduler_Next(ps_uart_ctrl);
}

///==================================================================
/* UART TX Idle check : TX 후유상태 확인
---------------------------------------------------------------------
+ *ps_uart  USART_TypeDef
+ Return
m_RETURN_WAIT
m_RETURN_OK
-------------------------------------------------------------------*/
te_Return f_LL_UART_TX_Idle_Check(USART_TypeDef *ps_uart)
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
/*### Uart module load print : Uart 모듈 사용률 보기
-------------------------------------------------------------------*/
void f_LL_Uart_Load_Print(void)
{
    f_Scheduler_RunTime_Info_print(&gs_ll_uart_Job_ctrl, m_DBG_MODE_SYSTEM, gs_ll_uart_Job_ctrl.p_table_name);
}
