/********************************************************************
* File:    display_ST7789V2.c
* Author:  Lostroid
* Created: 2026-04-29
* Encoding: UTF-8
********************************************************************/

#include "../../Base/base_adc.h"
#include "../../Base/base_gpio.h"
#include "../../Base/base_spi.h"
#include "../../Base/base_tick.h"
#include "../../Font/font_default.h"
#include "../../Scheduler/scheduler.h"
#include "../../DBG/dbg.h"
#include "../display.h"
#include "lcd_ST7735S.h"

#define d_LCD_ST7735S_PIN_CS_H      d_BASE_GPIO_SPI1_CS_H
#define d_LCD_ST7735S_PIN_CS_L      d_BASE_GPIO_SPI1_CS_L
#define d_LCD_ST7735S_PIN_RES_H     d_BASE_GPIO_LCD_RESET_H
#define d_LCD_ST7735S_PIN_RES_L     d_BASE_GPIO_LCD_RESET_L
#define d_LCD_ST7735S_PIN_DC_H      d_BASE_GPIO_LCD_DC_H
#define d_LCD_ST7735S_PIN_DC_L      d_BASE_GPIO_LCD_DC_L
#define d_LCD_ST7735S_PIN_BL_H      d_BASE_GPIO_LCD_BL_H
#define d_LCD_ST7735S_PIN_BL_L      d_BASE_GPIO_LCD_BL_L

#define d_LCD_ST7735S_SPI_CNT_CHECK             f_base_spi1_dma_count_check()
#define d_LCD_ST7735S_SPI_IDLE_CHECK            f_base_spi1_dma_idle(SPI1)
#define d_LCD_ST7735S_SPI_SEND(TX, RX, LEN)     f_base_spi1_dma_send(TX, RX, LEN)


enum { m_LCD_ST7735S_INIT_JOB_TABLE_SIZE = 14, 
       m_LCD_ST7735S_RUN_JOB_TABLE_SIZE = 3, 
       m_LCD_ST7735S_BUFF_SIZE = 5120, 
       m_LCD_ST7735S_FPS = 31250 };   //+ 32FPS =  31250

static tu8 v_st7735s_buff_tx[m_LCD_ST7735S_BUFF_SIZE] = {0};
static tu8 v_st7735s_buff_rx[m_LCD_ST7735S_BUFF_SIZE] = {0};
static ts_scheduler_control gs_lcd_st7735s_Job_ctrl;
static void (*paf_lcd_st7735s_init_job_table[m_LCD_ST7735S_INIT_JOB_TABLE_SIZE])
            (ts_scheduler_control *ps_lcd_st7735s_Job_ctrl) = {
    f_lcd_st7735s_job_start,        //+ 00
    f_lcd_st7735s_job_reset_low,      //+ 01
    f_lcd_st7735s_job_delay,        //+ 02
    f_lcd_st7735s_job_reset_high,      //+ 03
    f_lcd_st7735s_job_delay,        //+ 04
    f_lcd_st7735s_job_swreset,      //+ 05
    f_lcd_st7735s_Job_slpout,       //+ 06
    f_lcd_st7735s_job_dispon,       //+ 07
    f_lcd_st7735s_job_invoff,       //+ 08
    f_lcd_st7735s_job_madctl,       //+ 09
    f_lcd_st7735s_job_caset,        //+ 10
    f_lcd_st7735s_job_raset,        //+ 11
    f_lcd_st7735s_job_colmod,       //+ 12
    f_lcd_st7735s_job_init_finish   //+ 13
};
static void (*paf_lcd_st7735s_run_job_table[m_LCD_ST7735S_RUN_JOB_TABLE_SIZE])
            (ts_scheduler_control *ps_lcd_st7735s_Job_ctrl) = {
    f_lcd_st7735s_job_start,    //+ 00
    f_lcd_st7735s_job_ramwr,    //+ 01
    f_lcd_st7735s_job_image,    //+ 02
};

//===================================================================
/*#### LCD ST7735S initialize
-------------------------------------------------------------------*/
void f_lcd_st7735s_init(void)
{
    f_lcd_st7735s_set_start_mode();
}
//===================================================================
/*#### LCD ST7753S Module
-------------------------------------------------------------------*/
void f_lcd_st7735s_module(void)
{
    f_scheduler_run(&gs_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7735S initialize
-------------------------------------------------------------------*/
void f_lcd_st7735s_set_start_mode(void)
{
    f_scheduler_init( 
        &gs_lcd_st7735s_Job_ctrl
        , paf_lcd_st7735s_init_job_table
        , "LCD_start_Job_table"
        , m_LCD_ST7735S_INIT_JOB_TABLE_SIZE);
    
    f_scheduler_config(
        &gs_lcd_st7735s_Job_ctrl
        , 0
        , 3
        , 0
        , m_LCD_ST7735S_FPS     //+ 32fps 31250
        , d_NULL);
    f_dbg_print_string("\r\nI: LCD ST7735S Start.");
}
//===================================================================
/*#### LCD ST7735S initialize
-------------------------------------------------------------------*/
void f_lcd_st7735s_set_run_mode(void)
{
    f_scheduler_init( 
        &gs_lcd_st7735s_Job_ctrl
        , paf_lcd_st7735s_run_job_table
        , "LCD_Run_Job_table"
        , m_LCD_ST7735S_RUN_JOB_TABLE_SIZE);
    
    f_scheduler_config(
        &gs_lcd_st7735s_Job_ctrl
        , 0
        , 3
        , 0
        , m_LCD_ST7735S_FPS     //+ 32fps 31250
        , d_NULL);
    f_dbg_print_string("\r\nI: LCD ST7735S Run.");
}
//===================================================================
/*#### LCD ST7753S Error Send
-------------------------------------------------------------------*/
void f_lcd_st7735s_error_send_fail(void)
{
    f_dbg_print_string("\r\nE: LCD ST7735S send failed.");
}
//===================================================================
/*#### LCD ST7753S Error Send
-------------------------------------------------------------------*/
void f_lcd_st7735s_error_case(void)
{
    f_dbg_print_string("\r\nE: LCD ST7735S case failed.");
}
//===================================================================
/*#### Font 8x16 
---------------------------------------------------------------------
v_offset_char : max 10 char[8x16]
-------------------------------------------------------------------*/
void f_lcd_st7735s_font_print(tu32 v_offset_x, const char *p_ascii, tu16 v_color_f, tu16 v_color_b)
{
    tu32 v_font_x = v_offset_x;
    tu32 v_limit = ((160 - v_offset_x) / 8);

    while (*p_ascii)
    {
        if(v_limit == 0)
            { break; }
        v_limit--;
        tu8 v_idx = (tu8)(*p_ascii);
        
        for (tu32 v_row = 0; v_row < 16; v_row++)
        {
            tu32 v_postion = (v_font_x * 2) + (v_row * 320);
            tu8 v_font_byte = xa_font_ascii[v_idx][v_row];

            for (tu32 v_col = 0; v_col < 8; v_col++)
            {
                tu32 v_bit = (v_font_byte >> (7 - v_col)) & 0x01;
                tu16 v_color = v_bit ? v_color_f : v_color_b;

                v_st7735s_buff_tx[v_postion++] = (tu8)(v_color >> 8);
                v_st7735s_buff_tx[v_postion++] = (tu8)(v_color);
                if(v_postion > m_LCD_ST7735S_BUFF_SIZE)
                {
                	f_dbg_print_string("\r\nE: LCD ST7735S font Overflow.");
                	return;
                }
            }
        }
        v_font_x += 8;
        p_ascii++;
    }
}
//===================================================================
/*#### LCD ST7753S Error Send
------------------------------------------------------------------*/
void f_lcd_st7735s_Log_Print(tu32 v_line, tu32 v_postion)
{


}
//===================================================================
/*#### LCD ST7753S Job Start
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_start(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    f_scheduler_loop_time_update(ps_lcd_st7735s_Job_ctrl);
    f_scheduler_next(ps_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7753S Job Reset Low
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_reset_low(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    d_LCD_ST7735S_PIN_RES_L;
    d_LCD_ST7735S_PIN_CS_H;
    d_LCD_ST7735S_PIN_DC_H;
    d_LCD_ST7735S_PIN_BL_H;
    ps_lcd_st7735s_Job_ctrl->v_delay_set_us = 100u;      //+ 100us
    f_base_tick_systick32_start(&ps_lcd_st7735s_Job_ctrl->v_delay_tick_count);
    f_scheduler_next(ps_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7753S Job Delay
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_delay(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    if( f_base_tick_systick32_finish(&ps_lcd_st7735s_Job_ctrl->v_delay_tick_count)
        >=  ps_lcd_st7735s_Job_ctrl->v_delay_set_us)
    {
        f_scheduler_next(ps_lcd_st7735s_Job_ctrl);
    }
}
//===================================================================
/*#### LCD ST7753S Job Reset High
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_reset_high(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    d_LCD_ST7735S_PIN_RES_H;
    ps_lcd_st7735s_Job_ctrl->v_delay_set_us = 5000u;    //+ Run Wait time 5s
    f_base_tick_systick32_start(&ps_lcd_st7735s_Job_ctrl->v_delay_tick_count);
    f_scheduler_next(ps_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7753S Job SW reset
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_swreset(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_SWRESET;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Reset High
-------------------------------------------------------------------*/
void f_lcd_st7735s_Job_slpout(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_SLPOUT;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Reset High
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_dispon(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_DISPON;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job INVON
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_invoff(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_INVOFF;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job INVOFF
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_invon(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_INVON;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job MADCTL
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_madctl(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_MADCTL;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        case 2:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_BIT_RDDMADCTL_MV
                                     | d_LCD_ST7735S_BIT_RDDMADCTL_MY;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job CASET
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_caset(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_CASET;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        case 2:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = 0x00;
                v_st7735s_buff_tx[1] = 0x01;    //+ 0
                v_st7735s_buff_tx[2] = 0x00;    
                v_st7735s_buff_tx[3] = 0xA0;    //+ 159
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 4) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job RASET
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_raset(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_RASET;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        case 2:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = 0x00;
                v_st7735s_buff_tx[1] = 0x02;    //+ 0
                v_st7735s_buff_tx[2] = 0x00;
                v_st7735s_buff_tx[3] = 0x81;    //+ 127
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 4) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job COLMOD
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_colmod(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_COLMOD;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        case 2:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = 0x05;    //+ 16bit mode
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job RAMWR
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_ramwr(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_RAMWR;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Image
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_image(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    static tu32 lv_cont;
    static tu16 lv_color_buff = 0;
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_base_tick_systick32_stopwatch_check(&gs_lcd_st7735s_Job_ctrl.s_time_interval) == m_RETURN_OK)
            {
                lv_cont = 0;
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                lv_color_buff = 0x0000;
                f_display_fps_countUp();
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            
            f_lcd_st7735s_color_fill(lv_color_buff);
            f_lcd_st7735s_monitor_view(lv_cont);
            ps_lcd_st7735s_Job_ctrl->v_case_step++;
            break;
        }
        case 2:
        {
            if(d_LCD_ST7735S_SPI_IDLE_CHECK == 0)
            {
                d_LCD_ST7735S_SPI_CNT_CHECK;
                if(d_LCD_ST7735S_SPI_SEND(v_st7735s_buff_tx,v_st7735s_buff_rx, m_LCD_ST7735S_BUFF_SIZE) == m_RESULT_OK)
                    {  ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_lcd_st7735s_error_send_fail(); }
            }
            break;
        }
        case 3:
        {
            if(d_LCD_ST7735S_SPI_CNT_CHECK == m_YESNO_YES)
            {
                lv_cont++;
                if(lv_cont == 8)
                    { f_scheduler_next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { ps_lcd_st7735s_Job_ctrl->v_case_step -= 2; }
            }
            break;
        }
        default:
        {
            f_lcd_st7735s_error_case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Init Finish
-------------------------------------------------------------------*/
void f_lcd_st7735s_job_init_finish(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl)
{
    f_lcd_st7735s_set_run_mode();
}
//===================================================================
/*#### LCD module load print "LCD 모듈 사용률 보기"
-------------------------------------------------------------------*/
void f_lcd_st7735s_load_print(void)
{
    f_scheduler_run_time_info_print(&gs_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD module load print "LCD 모듈 사용률 보기"
-------------------------------------------------------------------*/
void f_lcd_st7735s_color_fill(tu16 v_color)
{
    tu32 v_line = 0;
    while(m_LCD_ST7735S_BUFF_SIZE > v_line)
    {
        v_st7735s_buff_tx[v_line] = (tu8)(v_color >> 8);    //+ 16bit mode
        v_line++;
        v_st7735s_buff_tx[v_line] = (tu8)v_color;           //+ 16bit mode
        v_line++;
    }
}
//===================================================================
/*#### LCD Voltage Bar "LCD 전압 발"
-------------------------------------------------------------------*/
void f_lcd_st7735s_voltage_bar(tu32 v_voltage, tu16 v_color_f)
{
    tu32 v_bar = (v_voltage * 48485) / 1000000;
    if(v_bar > 160)
        { v_bar = 160; }
    for(tu32 v_line = 0; v_line < 16; v_line++)
    {
        tu32 v_offset = (v_line * 320);
        for(tu32 v_pixel = 0; v_pixel < v_bar; v_pixel++)
        {
            v_st7735s_buff_tx[v_offset++] = (tu8)(v_color_f >> 8);    //+ 16bit mode
            v_st7735s_buff_tx[v_offset++] = (tu8)v_color_f;           //+ 16bit mode
        }
    }
}
//===================================================================
/*#### LCD module LCD viwe "화면 구성"
-------------------------------------------------------------------*/
void f_lcd_st7735s_monitor_view(tu32 v_line)
{
    static char la_ticker[] = "                    This is a bare-metal demo of Lostroid.                    ";
    static tu8 lv_ticker_cnt = 0;
    static tu8 lv_time_delay = 3;
    switch(v_line)
    {
        case 0: //+ Line 1
        {
            char a_string[32] = {0};   //+ Display: 20 bytes, 12 bytes spare
            tu32 v_pos = 0;
            tu64 v_time = d_BASE_TICK_SYS_GET_US(f_base_tick_systick_time64_get());  
            f_display_conversion_dec32_digit(a_string, &v_pos, 3, (tu32)(v_time / 86400000000));
            f_display_conversion_string(a_string, &v_pos, "d ");
            v_time %= 86400000000;
            f_display_conversion_dec32_digit(a_string, &v_pos, 2, (tu32)(v_time / 3600000000));
            f_display_conversion_string(a_string, &v_pos, ":");
            v_time %= 3600000000;
            f_display_conversion_dec32_digit(a_string, &v_pos, 2, (tu32)(v_time / 60000000));
            f_display_conversion_string(a_string, &v_pos, ":");
            v_time %= 60000000;
            f_display_conversion_dec32_digit(a_string, &v_pos, 2, (tu32)(v_time / 1000000));
            f_display_conversion_string(a_string, &v_pos, ".");
            v_time %= 1000000;
            f_display_conversion_dec32_digit(a_string, &v_pos, 6, v_time);
            f_lcd_st7735s_font_print(0, a_string, 0xFFFF, 0x0000);
            break;
        }
        case 1: //+ Line 2
        {
             char a_string[32] = {0};   //+ Display: 20 bytes, 12 bytes spare
            tu32 v_pos = 0;
            f_display_conversion_string(a_string, &v_pos, "FPS: ");
            f_display_conversion_dec32(a_string, &v_pos, f_display_fps_get() );
            f_display_conversion_string(a_string, &v_pos, " (");
            f_display_conversion_dec32(a_string, &v_pos, gs_lcd_st7735s_Job_ctrl.v_time_loop_now_us );
            f_display_conversion_string(a_string, &v_pos, "us)");
            f_lcd_st7735s_font_print(0, a_string, 0xF9DF, 0x0000);
            break;
        }
        case 2: //+ Line 3
        {
            
            break;
        }
        case 3: //+ Line 4
        {
            char a_string[32] = {0};   //+ Display: 20 bytes, 12 bytes spare
            tu32 v_pos = 0;
            tu32 v_voltage_uv = f_base_adc_ch0_get();
            f_display_conversion_string(a_string, &v_pos, "ADC_CH0: ");
            f_display_conversion_dec32(a_string, &v_pos, v_voltage_uv / 1000);
            f_display_conversion_string(a_string, &v_pos, ".");
            f_display_conversion_dec32_digit(a_string, &v_pos, 3, v_voltage_uv % 1000);
            f_display_conversion_string(a_string, &v_pos, " V");
            f_lcd_st7735s_font_print(0, a_string, 0xFFE0, 0x0000);
            break;
        }
        case 4: //+ Line 5
        {
            tu32 v_voltage_uv = f_base_adc_ch0_get();
            f_lcd_st7735s_voltage_bar(v_voltage_uv, 0x03E0);
            break;
        }
        case 5: //+ Line 6
        {
            char a_string[32] = {0};   //+ Display: 20 bytes, 12 bytes spare
            tu32 v_pos = 0;
            tu32 v_voltage_uv = f_base_adc_ch1_get();
            f_display_conversion_string(a_string, &v_pos, "ADC_CH1: ");
            f_display_conversion_dec32(a_string, &v_pos, v_voltage_uv / 1000);
            f_display_conversion_string(a_string, &v_pos, ".");
            f_display_conversion_dec32_digit(a_string, &v_pos, 3, v_voltage_uv % 1000);
            f_display_conversion_string(a_string, &v_pos, " V");
            f_lcd_st7735s_font_print(0, a_string, 0xFFE0, 0x0000);
            break;
        }
        case 6:  //+ Line 7
        {
            tu32 v_voltage_uv = f_base_adc_ch1_get();
            f_lcd_st7735s_voltage_bar(v_voltage_uv, 0x03E0);
            break;
        }
        case 7:  //+ Line 8
        {
            if(lv_ticker_cnt > 59)
                {lv_ticker_cnt = 0;}
            f_lcd_st7735s_font_print(0,&la_ticker[lv_ticker_cnt], 0x07FF, 0x0000);
            if(lv_time_delay == 0)
            {
                lv_time_delay = 3;
                lv_ticker_cnt++;
            }
            else
                { lv_time_delay--; }
            break;
        }
    }
}


