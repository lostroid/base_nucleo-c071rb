/********************************************************************
* File:    lcd_ST7735S.h
* Author:  Lostroid
* Created: 2026-04-29
* Encoding: UTF-8
********************************************************************/
#ifndef H_LCD_ST7735S_H
#define H_LCD_ST7735S_H

#include "lcd_ST7735S_type.h"

void f_LCD_ST7735S_Init(void);
void f_LCD_ST7735S_Set_start_Mode(void);
void f_LCD_ST7735S_Set_Run_Mode(void);
void f_LCD_ST7735S_Module(void);
void f_LCD_ST7735S_Error_Send_Fail(void);
void f_LCD_ST7735S_Error_Case(void);

void f_LCD_ST7735S_Font_Print(tu32 v_offset_x, const char *p_ascii, tu16 v_color_f, tu16 v_color_b);

void f_LCD_ST7753S_Job_Start(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_Reset_L(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_Delay(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_Reset_H(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_SWRESET(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_SLPOUT(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_DISPON(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_INVOFF(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_INVON(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_MADCTL(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_CASET(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_RASET(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_COLMOD(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_RAMWR(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_Image(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7753S_Job_Init_Finish(ts_Scheduler_Control *ps_lcd_st7735s_Job_ctrl);
void f_LCD_ST7735S_Load_Print(void);
void f_LCD_ST7735S_Color_Fill(tu16 v_color);
void f_LCD_ST7735S_Voltage_Bar(tu32 v_voltage, tu16 v_color_f);
void f_LCD_ST7735S_Monitor_View(tu32 v_line);

#endif
