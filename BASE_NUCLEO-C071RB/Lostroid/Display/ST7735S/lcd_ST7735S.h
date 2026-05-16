/********************************************************************
* File:    lcd_ST7735S.h
* Author:  Lostroid
* Created: 2026-04-29
* Encoding: UTF-8
********************************************************************/
#ifndef H_LCD_ST7735S_H
#define H_LCD_ST7735S_H

#include "lcd_ST7735S_type.h"

void f_lcd_st7735s_init(void);
void f_lcd_st7735s_set_start_mode(void);
void f_lcd_st7735s_set_run_mode(void);
void f_lcd_st7735s_module(void);
void f_lcd_st7735s_error_send_fail(void);
void f_lcd_st7735s_error_case(void);

void f_lcd_st7735s_font_print(tu32 v_offset_x, const char *p_ascii, tu16 v_color_f, tu16 v_color_b);

void f_lcd_st7735s_job_start(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_reset_low(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_delay(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_reset_high(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_swreset(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_Job_slpout(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_dispon(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_invoff(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_invon(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_madctl(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_caset(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_raset(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_colmod(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_ramwr(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_image(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_job_init_finish(ts_scheduler_control *ps_lcd_st7735s_Job_ctrl);
void f_lcd_st7735s_load_print(void);
void f_lcd_st7735s_color_fill(tu16 v_color);
void f_lcd_st7735s_voltage_bar(tu32 v_voltage, tu16 v_color_f);
void f_lcd_st7735s_monitor_view(tu32 v_line);

#endif
