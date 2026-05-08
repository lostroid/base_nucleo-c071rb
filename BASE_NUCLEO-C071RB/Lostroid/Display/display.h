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

void f_Display_Init(void);
void f_Display_Module(void);

void f_Display_FPS_CountUp(void);
void f_Display_FPS_Update(void);
tu32 f_Display_Get_FPS(void);

void f_Display_Conversion_String(char *p_save, tu32 *p_pos, char *p_data);
void f_Display_Conversion_Dec64(char *p_save, tu32 *p_pos, tu64 v_dec);
void f_Display_Conversion_Dec64_Digit(char *p_save, tu32 *p_pos, tu32 v_min_digits, tu64 v_dec);
void f_Display_Conversion_Dec32(char *p_save, tu32 *p_pos, tu32 v_dec);
void f_Display_Conversion_Dec32_Digit(char *p_save, tu32 *p_pos, tu32 v_min_digits, tu32 v_dec);
void f_Display_Conversion_Hex64(char *p_save, tu32 *p_pos, tu64 v_hex64);
void f_Display_Conversion_Hex32(char *p_save, tu32 *p_pos, tu32 v_hex32);
void f_Display_Conversion_Hex16(char *p_save, tu32 *p_pos, tu16 v_hex16);
void f_Display_Conversion_Hex8(char *p_save, tu32 *v_pos, tu8 v_hex8);

#endif
