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

enum { m_LCD_ST7735S_INIT_JOB_TABLE_SIZE = 14, 
       m_LCD_ST7735S_RUN_JOB_TABLE_SIZE = 3, 
       m_LCD_ST7735S_BUFF_SIZE = 5120, 
       m_LCD_ST7735S_FPS = 31250 };   //+ 32FPS =  31250

static tu8 v_st7735s_buff_tx[m_LCD_ST7735S_BUFF_SIZE] = {0};
static tu8 v_st7735s_buff_rx[m_LCD_ST7735S_BUFF_SIZE] = {0};
static ts_Scheduler_Control gs_lcd_st7735s_Job_ctrl;
static void (*paf_lcd_st7735s_init_job_table[m_LCD_ST7735S_INIT_JOB_TABLE_SIZE])
            (ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl) = {
    f_LCD_ST7753S_Job_Start,  //+ 00
    f_LCD_ST7753S_Job_Reset_L,    //+ 01
    f_LCD_ST7753S_Job_Delay,   //+ 02
    f_LCD_ST7753S_Job_Reset_H,  //+ 03
    f_LCD_ST7753S_Job_Delay,   //+ 02
    f_LCD_ST7753S_Job_SWRESET,
    f_LCD_ST7753S_Job_SLPOUT,
    f_LCD_ST7753S_Job_DISPON,
    f_LCD_ST7753S_Job_INVOFF,
    f_LCD_ST7753S_Job_MADCTL,
    f_LCD_ST7753S_Job_CASET,
    f_LCD_ST7753S_Job_RASET,
    f_LCD_ST7753S_Job_COLMOD,
    f_LCD_ST7753S_Job_Init_Finish
};
static void (*paf_lcd_st7735s_run_job_table[m_LCD_ST7735S_RUN_JOB_TABLE_SIZE])
            (ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl) = {
    f_LCD_ST7753S_Job_Start,  //+ 00
    f_LCD_ST7753S_Job_RAMWR,
    f_LCD_ST7753S_Job_Image,    //+ 01
};

//===================================================================
/*#### LCD ST7735S initialize
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Init(void)
{
    f_LCD_ST7735S_Set_start_Mode();
}
//===================================================================
/*#### LCD ST7753S Module
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Module(void)
{
    f_Scheduler_Run(&gs_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7735S initialize
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Set_start_Mode(void)
{
    f_Scheduler_Init( 
        &gs_lcd_st7735s_Job_ctrl
        , paf_lcd_st7735s_init_job_table
        , "LCD_start_Job_table"
        , m_LCD_ST7735S_INIT_JOB_TABLE_SIZE);
    
    f_Scheduler_Config(
        &gs_lcd_st7735s_Job_ctrl
        , 0
        , 3
        , 0
        , m_LCD_ST7735S_FPS     //+ 32fps 31250
        , d_NULL);
    f_DBG_Print_String("\r\nI: LCD ST7735S Start.");
}
//===================================================================
/*#### LCD ST7735S initialize
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Set_Run_Mode(void)
{
    f_Scheduler_Init( 
        &gs_lcd_st7735s_Job_ctrl
        , paf_lcd_st7735s_run_job_table
        , "LCD_Run_Job_table"
        , m_LCD_ST7735S_RUN_JOB_TABLE_SIZE);
    
    f_Scheduler_Config(
        &gs_lcd_st7735s_Job_ctrl
        , 0
        , 3
        , 0
        , m_LCD_ST7735S_FPS     //+ 32fps 31250
        , d_NULL);
    f_DBG_Print_String("\r\nI: LCD ST7735S Run.");
}
//===================================================================
/*#### LCD ST7753S Error Send
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Error_Send_Fail(void)
{
    f_DBG_Print_String("\r\nE: LCD ST7735S send failed.");
}
//===================================================================
/*#### LCD ST7753S Error Send
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Error_Case(void)
{
    f_DBG_Print_String("\r\nE: LCD ST7735S case failed.");
}
//===================================================================
/*#### Font 8x16 
---------------------------------------------------------------------
v_offset_char : max 10 char[8x16]
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Font_Print(tu32 v_offset_x, const char *p_ascii, tu16 v_color_f, tu16 v_color_b)
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
                	f_DBG_Print_String("\r\nE: LCD ST7735S font Overflow.");
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
void f_LCD_ST7735S_Log_Print(tu32 v_line, tu32 v_postion)
{


}
//===================================================================
/*#### LCD ST7753S Job Start
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_Start(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    f_Scheduler_LoopTime_Update(ps_lcd_st7735s_Job_ctrl);
    f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7753S Job Reset Low
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_Reset_L(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    d_LCD_ST7735S_PIN_RES_L;
    d_LCD_ST7735S_PIN_CS_H;
    d_LCD_ST7735S_PIN_DC_H;
    d_LCD_ST7735S_PIN_BL_H;
    ps_lcd_st7735s_Job_ctrl->v_delay_set_us = 100u;      //+ 100us
    f_Base_Tick_Systick32_Start(&ps_lcd_st7735s_Job_ctrl->v_delay_tick_count);
    f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7753S Job Delay
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_Delay(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    if( f_Base_Tick_Systick32_Finish(&ps_lcd_st7735s_Job_ctrl->v_delay_tick_count)
        >=  ps_lcd_st7735s_Job_ctrl->v_delay_set_us)
    {
        f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl);
    }
}
//===================================================================
/*#### LCD ST7753S Job Reset High
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_Reset_H(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    d_LCD_ST7735S_PIN_RES_H;
    ps_lcd_st7735s_Job_ctrl->v_delay_set_us = 5000u;    //+ Run Wait time 5s
    f_Base_Tick_Systick32_Start(&ps_lcd_st7735s_Job_ctrl->v_delay_tick_count);
    f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD ST7753S Job SW reset
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_SWRESET(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_SWRESET;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Reset High
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_SLPOUT(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_SLPOUT;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Reset High
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_DISPON(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_DISPON;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job INVON
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_INVOFF(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_INVOFF;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job INVOFF
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_INVON(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_INVON;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job MADCTL
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_MADCTL(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_MADCTL;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        case 2:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_BIT_RDDMADCTL_MV
                                     | d_LCD_ST7735S_BIT_RDDMADCTL_MY;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job CASET
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_CASET(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_CASET;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        case 2:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = 0x00;
                v_st7735s_buff_tx[1] = 0x01;    //+ 0
                v_st7735s_buff_tx[2] = 0x00;    
                v_st7735s_buff_tx[3] = 0xA0;    //+ 159
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 4) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job RASET
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_RASET(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_RASET;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        case 2:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = 0x00;
                v_st7735s_buff_tx[1] = 0x02;    //+ 0
                v_st7735s_buff_tx[2] = 0x00;
                v_st7735s_buff_tx[3] = 0x81;    //+ 127
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 4) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job COLMOD
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_COLMOD(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_COLMOD;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        case 2:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 3:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = 0x05;    //+ 16bit mode
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job RAMWR
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_RAMWR(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                d_LCD_ST7735S_PIN_DC_L;     //+ Command Mode;
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                v_st7735s_buff_tx[0] = d_LCD_ST7735S_ADDR_RAMWR;
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx, v_st7735s_buff_rx, 1) == m_RESULT_OK)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Image
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_Image(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    static tu32 lv_cont;
    static tu16 lv_color_buff = 0;
    switch(ps_lcd_st7735s_Job_ctrl->v_case_step)
    {
        case 0:
        {
            if(f_Base_Tick_Systick32_Stopwatch_Check(&gs_lcd_st7735s_Job_ctrl.s_time_interval) == m_RETURN_OK)
            {
                lv_cont = 0;
                d_LCD_ST7735S_PIN_DC_H;     //+ Data Mode;
                lv_color_buff = 0x0000;
                f_Display_FPS_CountUp();
                ps_lcd_st7735s_Job_ctrl->v_case_step++;
            }
            break;
        }
        case 1:
        {
            
            f_LCD_ST7735S_Color_Fill(lv_color_buff);
            f_LCD_ST7735S_Monitor_View(lv_cont);
            ps_lcd_st7735s_Job_ctrl->v_case_step++;
            break;
        }
        case 2:
        {
            if(f_Base_SPI1_DMA_Idle(SPI1) == 0)
            {
                f_Base_SPI1_DMA_Send_Count_Check();
                if(f_Base_SPI1_DMA_Send(v_st7735s_buff_tx,v_st7735s_buff_rx, m_LCD_ST7735S_BUFF_SIZE) == m_RESULT_OK)
                    {  ps_lcd_st7735s_Job_ctrl->v_case_step++; }
                else
                    { f_LCD_ST7735S_Error_Send_Fail(); }
            }
            break;
        }
        case 3:
        {
            if(f_Base_SPI1_DMA_Send_Count_Check() == m_YESNO_YES)
            {
                lv_cont++;
                if(lv_cont == 8)
                    { f_Scheduler_Next(ps_lcd_st7735s_Job_ctrl); }
                else
                    { ps_lcd_st7735s_Job_ctrl->v_case_step -= 2; }
            }
            break;
        }
        default:
        {
            f_LCD_ST7735S_Error_Case();
            break;
        }
    }
}
//===================================================================
/*#### LCD ST7753S Job Init Finish
-------------------------------------------------------------------*/
void f_LCD_ST7753S_Job_Init_Finish(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl)
{
    f_LCD_ST7735S_Set_Run_Mode();
}
//===================================================================
/*#### LCD module load print "LCD 모듈 사용률 보기"
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Load_Print(void)
{
    f_Scheduler_RunTime_Info_print(&gs_lcd_st7735s_Job_ctrl);
}
//===================================================================
/*#### LCD module load print "LCD 모듈 사용률 보기"
-------------------------------------------------------------------*/
void f_LCD_ST7735S_Color_Fill(tu16 v_color)
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
void f_LCD_ST7735S_Voltage_Bar(tu32 v_voltage, tu16 v_color_f)
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
void f_LCD_ST7735S_Monitor_View(tu32 v_line)
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
            tu64 v_time = d_BASE_TICK_SYS_GET_US(f_Base_Tick_Systick_Get_Time64_Tick());  
            f_Display_Conversion_Dec32_Digit(a_string, &v_pos, 3, (tu32)(v_time / 86400000000));
            f_Display_Conversion_String(a_string, &v_pos, "d ");
            v_time %= 86400000000;
            f_Display_Conversion_Dec32_Digit(a_string, &v_pos, 2, (tu32)(v_time / 3600000000));
            f_Display_Conversion_String(a_string, &v_pos, ":");
            v_time %= 3600000000;
            f_Display_Conversion_Dec32_Digit(a_string, &v_pos, 2, (tu32)(v_time / 60000000));
            f_Display_Conversion_String(a_string, &v_pos, ":");
            v_time %= 60000000;
            f_Display_Conversion_Dec32_Digit(a_string, &v_pos, 2, (tu32)(v_time / 1000000));
            f_Display_Conversion_String(a_string, &v_pos, ".");
            v_time %= 1000000;
            f_Display_Conversion_Dec32_Digit(a_string, &v_pos, 6, v_time);
            f_LCD_ST7735S_Font_Print(0, a_string, 0xFFFF, 0x0000);
            break;
        }
        case 1: //+ Line 2
        {
             char a_string[32] = {0};   //+ Display: 20 bytes, 12 bytes spare
            tu32 v_pos = 0;
            f_Display_Conversion_String(a_string, &v_pos, "FPS: ");
            f_Display_Conversion_Dec32(a_string, &v_pos, f_Display_Get_FPS() );
            f_Display_Conversion_String(a_string, &v_pos, " (");
            f_Display_Conversion_Dec32(a_string, &v_pos, gs_lcd_st7735s_Job_ctrl.v_time_loop_now_us );
            f_Display_Conversion_String(a_string, &v_pos, "us)");
            f_LCD_ST7735S_Font_Print(0, a_string, 0xF9DF, 0x0000);
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
            tu32 v_voltage_uv = f_Base_ADC_Get_ADC_CH0();
            f_Display_Conversion_String(a_string, &v_pos, "ADC_CH0: ");
            f_Display_Conversion_Dec32(a_string, &v_pos, v_voltage_uv / 1000);
            f_Display_Conversion_String(a_string, &v_pos, ".");
            f_Display_Conversion_Dec32_Digit(a_string, &v_pos, 3, v_voltage_uv % 1000);
            f_Display_Conversion_String(a_string, &v_pos, " V");
            f_LCD_ST7735S_Font_Print(0, a_string, 0xFFE0, 0x0000);
            break;
        }
        case 4: //+ Line 5
        {
            tu32 v_voltage_uv = f_Base_ADC_Get_ADC_CH0();
            f_LCD_ST7735S_Voltage_Bar(v_voltage_uv, 0x03E0);
            break;
        }
        case 5: //+ Line 6
        {
            char a_string[32] = {0};   //+ Display: 20 bytes, 12 bytes spare
            tu32 v_pos = 0;
            tu32 v_voltage_uv = f_Base_ADC_Get_ADC_CH1();
            f_Display_Conversion_String(a_string, &v_pos, "ADC_CH1: ");
            f_Display_Conversion_Dec32(a_string, &v_pos, v_voltage_uv / 1000);
            f_Display_Conversion_String(a_string, &v_pos, ".");
            f_Display_Conversion_Dec32_Digit(a_string, &v_pos, 3, v_voltage_uv % 1000);
            f_Display_Conversion_String(a_string, &v_pos, " V");
            f_LCD_ST7735S_Font_Print(0, a_string, 0xFFE0, 0x0000);
            break;
        }
        case 6:  //+ Line 7
        {
            tu32 v_voltage_uv = f_Base_ADC_Get_ADC_CH1();
            f_LCD_ST7735S_Voltage_Bar(v_voltage_uv, 0x03E0);
            break;
        }
        case 7:  //+ Line 8
        {
            if(lv_ticker_cnt > 59)
                {lv_ticker_cnt = 0;}
            f_LCD_ST7735S_Font_Print(0,&la_ticker[lv_ticker_cnt], 0x07FF, 0x0000);
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


