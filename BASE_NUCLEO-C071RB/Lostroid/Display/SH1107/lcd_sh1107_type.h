/********************************************************************
* File:    lcd_sh1107_type.h
* Author:  Lostroid
* Created: 2026-05-11
* Encoding: UTF-8
---------------------------------------------------------------------
01. a_ = array                         e.g. a_data[]
02. b_ = bit field                     e.g. b_Data
03. c_ = constant                      e.g, c_Data
04. d_ = #define macro                 e.g. d_Data
05. e_ = enum type                     e.g. e_Data
06. f_ = function                      e.g. f_Data
07. g_ = global (source file static)   e.g. gv_Data
08. i_ = inline function               e.g. i_Data
09. l_ = local static variable         e.g. lv_Data
10. m_ = enum member                   e.g. m_Data
11. p_ = pointer                       e.g. p_Data 
12. s_ = struct                        e.g. s_Data 
13. t_ = typedef                       e.g. t_Data 
14. u_ = union                         e.g. u_Data 
15. v_ = variable                      e.g. v_Data
16. x_ = extern variable               e.g. xv_data
*********************************************************************
+ Support maximum 128 X 128 dot matrix panel
+ Embedded 128 X 128 bits SRAM
+ Operating voltage:
    - Logic voltage supply: VDD = 1.65V - 3.5V
    - DC-DC voltage supply: AVDD = 2.4V -3.5V
    - OLED Operating voltage supply: Vpp=7.0V - 16.5V
+ Maximum segment output current: 500mA
+ Maximum common sink current: 64mA
+ 8-bit 6800-series parallel interface, 8-bit 8080-series
    parallel interface, and 3-wire & 4-wire serial peripheral

interface.
+ 400KHz fast I2C bus interface
+ Programmable frame frequency and multiplexing ratio
+ Row re-mapping and column re-mapping
+ Vertical scrolling
+ On-chip oscillator
+ Available internal DC-DC converter
+ 256-step contrast control on monochrome passive OLED

panel
+ Low power consumption
    - Sleep mode: <5mA
+ Wide range of operating temperatures: -40 to +85°C
+ Available in COG form.

********************************************************************/
#ifndef H_LCD_SH1107_TYPE_H
#define H_LCD_SH1107_TYPE_H
#include "../../Base/types.h"



#endif
