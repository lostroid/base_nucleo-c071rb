/********************************************************************
* File:    display.h
* Author:  Lostroid
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#ifndef H_DISPLAY_H
#define H_DISPLAY_H

#include "ST7735S/lcd_ST7735S.h"
#include "display_type.h"

extern const unsigned char xa_image_lostroid_icon[5000];

void f_display_init(void);
void f_display_module(void);

void f_display_fps_countUp(void);
void f_display_fps_update(void);
tu32 f_display_fps_get(void);

void f_display_conversion_string(char *p_save, tu32 *p_pos, char *p_data);
void f_display_conversion_dec64(char *p_save, tu32 *p_pos, tu64 v_dec);
void f_display_conversion_dec64_digit(char *p_save, tu32 *p_pos, tu32 v_min_digits, tu64 v_dec);
void f_display_conversion_dec32(char *p_save, tu32 *p_pos, tu32 v_dec);
void f_display_conversion_dec32_digit(char *p_save, tu32 *p_pos, tu32 v_min_digits, tu32 v_dec);
void f_display_conversion_hex64(char *p_save, tu32 *p_pos, tu64 v_hex64);
void f_display_conversion_hex32(char *p_save, tu32 *p_pos, tu32 v_hex32);
void f_display_conversion_hex16(char *p_save, tu32 *p_pos, tu16 v_hex16);
void f_display_conversion_hex8(char *p_save, tu32 *v_pos, tu8 v_hex8);

#endif
