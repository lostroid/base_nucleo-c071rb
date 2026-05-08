/******************************************************************************
* File:    lcd_st7753s_type.h
* Author:  Lostroid
* Created: 2025-08-06
*
* Description:
* This is the uart Type.
*
* Revision History:
*   2025-08-06  New.
-------------------------------------------------------------------------------
01. a = array variable                      e.g. a_Data[]
02. b = bit fields                          e.g. b_Data
03. c = const                               e.g, c_Data
04. d = define                              e.g. d_Data
05. e = enum Type                           e.g. e_Data
06. f = Function                            e.g. f_Data
07. g = Source file static variable         e.g. gv_Data
08. i = Inline                              e.g. i_Data
09. l = Static variables inside functions   e.g. lv_Data
10. m = enum member                         e.g. m_Data
11. p = Pointer                             e.g. p_Data 
12. s = struct                              e.g. s_Data 
13. t = typedef                             e.g. t_Data 
14. u = Union                               e.g. u_Data 
15. v = variable                            e.g. v_Data
16. x = extern                              e.g. xv_data
*******************************************************************************
Size 1.8 "IPS color LCD (60FPS SPI 30Mhz)
Resolution: 128 (H) RGB x 160 (V)
Interface type: 8bit 8080 MCU interface
Control chip: ST7735S
PIN 1 : NC
PIN 2 : GND
PIN 3 : LED-
PIN 4 : LED+
PIN 5 : GND
PIN 6 : /RESET
PIN 7 : A0
PIN 8 : SDA
PIN 9 : SCK
PIN 10 : VCC
PIN 11 : IOVCC
PIN 12 : CS
PIN 13 : GND
PIN 14 : NC
******************************************************************************/
#ifndef LCD_ST7735S_TYPE_H
#define LCD_ST7735S_TYPE_H
//============================================================================= [ Include ]
#include "../../types.h"
//============================================================================= [ Define ]
//---------------------------------------------------------------------------------------- Register
#define d_LCD_ST7735S_REGA_NOP          0x00u
#define d_LCD_ST7735S_REGA_SWRESET      0x01u
#define d_LCD_ST7735S_REGA_RDDID        0x04u
#define d_LCD_ST7735S_REGA_RDDST        0x09u
#define d_LCD_ST7735S_REGA_RDDPM        0x0Au
#define d_LCD_ST7735S_REGA_RDDMADCTL    0x0Bu
#define d_LCD_ST7735S_REGA_RDDCOLMOD    0x0Cu
#define d_LCD_ST7735S_REGA_RDDIM        0x0Du
#define d_LCD_ST7735S_REGA_RDDSM        0x0Eu
#define d_LCD_ST7735S_REGA_RDDSDR       0x0Fu
#define d_LCD_ST7735S_REGA_SLPIN        0x10u
#define d_LCD_ST7735S_REGA_SLPOUT       0x11u
#define d_LCD_ST7735S_REGA_PTLON        0x12u
#define d_LCD_ST7735S_REGA_NORON        0x13u
#define d_LCD_ST7735S_REGA_INVOFF       0x20u
#define d_LCD_ST7735S_REGA_INVON        0x21u
#define d_LCD_ST7735S_REGA_GAMSET       0x26u
#define d_LCD_ST7735S_REGA_DISPOFF      0x28u
#define d_LCD_ST7735S_REGA_DISPON       0x29u
#define d_LCD_ST7735S_REGA_CASET        0x2Au
#define d_LCD_ST7735S_REGA_RASET        0x2Bu
#define d_LCD_ST7735S_REGA_RAMWR        0x2Cu
#define d_LCD_ST7735S_REGA_RGBSET       0x2Du
#define d_LCD_ST7735S_REGA_RAMRD        0x2Eu
#define d_LCD_ST7735S_REGA_RTLAR        0x30u
#define d_LCD_ST7735S_REGA_SCRLAR       0x33u
#define d_LCD_ST7735S_REGA_TEOFF        0x34u
#define d_LCD_ST7735S_REGA_TEON         0x35u
#define d_LCD_ST7735S_REGA_MADCTL       0x36u
#define d_LCD_ST7735S_REGA_VSCSAD       0x37u
#define d_LCD_ST7735S_REGA_IDMOFF       0x38u
#define d_LCD_ST7735S_REGA_IDMON        0x39u
#define d_LCD_ST7735S_REGA_COLMOD       0x3Au
#define d_LCD_ST7735S_REGA_RDID1        0xDAu
#define d_LCD_ST7735S_REGA_RDID2        0xDBu
#define d_LCD_ST7735S_REGA_RDID3        0xDCu
//===================================================================================== [ Typedef ]
typedef enum
{
    m_LCD_ST7735S_REGA_NOP         = 0x00u,
    m_LCD_ST7735S_REGA_SWRESET     = 0x01u,
    m_LCD_ST7735S_REGA_RDDID       = 0x04u,
    m_LCD_ST7735S_REGA_RDDST       = 0x09u,
    m_LCD_ST7735S_REGA_RDDPM       = 0x0Au,
    m_LCD_ST7735S_REGA_RDDMADCTL   = 0x0Bu,
    m_LCD_ST7735S_REGA_RDDCOLMOD   = 0x0Cu,
    m_LCD_ST7735S_REGA_RDDIM       = 0x0Du,
    m_LCD_ST7735S_REGA_RDDSM       = 0x0Eu,
    m_LCD_ST7735S_REGA_RDDSDR      = 0x0Fu,
    m_LCD_ST7735S_REGA_SLPIN       = 0x10u,
    m_LCD_ST7735S_REGA_SLPOUT      = 0x11u,
    m_LCD_ST7735S_REGA_PTLON       = 0x12u,
    m_LCD_ST7735S_REGA_NORON       = 0x13u,
    m_LCD_ST7735S_REGA_INVOFF      = 0x20u,
    m_LCD_ST7735S_REGA_INVON       = 0x21u,
    m_LCD_ST7735S_REGA_GAMSET      = 0x26u,
    m_LCD_ST7735S_REGA_DISPOFF     = 0x28u,
    m_LCD_ST7735S_REGA_DISPON      = 0x29u,
    m_LCD_ST7735S_REGA_CASET       = 0x2Au,
    m_LCD_ST7735S_REGA_RASET       = 0x2Bu,
    m_LCD_ST7735S_REGA_RAMWR       = 0x2Cu,
    m_LCD_ST7735S_REGA_RGBSET      = 0x2Du,
    m_LCD_ST7735S_REGA_RAMRD       = 0x2Eu,
    m_LCD_ST7735S_REGA_RTLAR       = 0x30u,
    m_LCD_ST7735S_REGA_SCRLAR      = 0x33u,
    m_LCD_ST7735S_REGA_TEOFF       = 0x34u,
    m_LCD_ST7735S_REGA_TEON        = 0x35u
}te_lcd_st7735s_reg;



#endif