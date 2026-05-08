/********************************************************************
* File:    lcd_st7735s_type.h
* Author:  Lostroid
* Created: 2026-04-29
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
1.Features
SIZE 1.8inch
Display Mode a-TFT
Display Format Graphic 128RGB*160 Dot-matrix
Input Data SPI interface
Drive IC ST7735S
Dimensional outline 35(W)*56 (H)*3.45+/-0.1(T)mm
Resolution 128RGB*160 Dots
LCD Active area 28.03 (W)*35.04 (H)
Pixel size 0.219(W)*0.219(H)
Viewing Direction 12 o’clock
Operating Temperature -20～70°C

2.Pin Descriptions：
PIN No Symbol Description
1 GND Ground
2 VCC Power 3.3V
3 SCL Serial clock pin.
4 SDA Serial data input
5 RES LCM Reset pin
This pin is reset signal input. When the pin is low, initialization of the chip is
executed. Keep this pin pull high during normal operation.
6 DC Data/Command Control
7 CS Chip Select
8 BLK Backlight control pin
********************************************************************/
#ifndef H_LCD_ST7735S_TYPE_H
#define H_LCD_ST7735S_TYPE_H
#include "../../Base/types.h"

#define d_LCD_ST7735S_ADDR_NOP           0x00
#define d_LCD_ST7735S_ADDR_SWRESET       0x01
#define d_LCD_ST7735S_ADDR_RDDID         0x04
#define d_LCD_ST7735S_ADDR_RDDST         0x09
#define d_LCD_ST7735S_ADDR_RDDPM         0x0A
#define d_LCD_ST7735S_ADDR_RDD_MADCTL    0x0B
#define d_LCD_ST7735S_ADDR_RDD_COLMOD    0x0C
#define d_LCD_ST7735S_ADDR_RDDIM         0x0D
#define d_LCD_ST7735S_ADDR_RDDSM         0x0E
#define d_LCD_ST7735S_ADDR_RDDSDR        0x0F
#define d_LCD_ST7735S_ADDR_SLPIN         0x10
#define d_LCD_ST7735S_ADDR_SLPOUT        0x11
#define d_LCD_ST7735S_ADDR_PTLON         0x12
#define d_LCD_ST7735S_ADDR_NORON         0x13
#define d_LCD_ST7735S_ADDR_INVOFF        0x20
#define d_LCD_ST7735S_ADDR_INVON         0x21
#define d_LCD_ST7735S_ADDR_GAMSET        0x26
#define d_LCD_ST7735S_ADDR_DISPOFF       0x28
#define d_LCD_ST7735S_ADDR_DISPON        0x29
#define d_LCD_ST7735S_ADDR_CASET         0x2A
#define d_LCD_ST7735S_ADDR_RASET         0x2B
#define d_LCD_ST7735S_ADDR_RAMWR         0x2C
#define d_LCD_ST7735S_ADDR_RGBSET        0x2D
#define d_LCD_ST7735S_ADDR_RAMRD         0x2E
#define d_LCD_ST7735S_ADDR_PTLAR         0x30
#define d_LCD_ST7735S_ADDR_SCRLAR        0x33
#define d_LCD_ST7735S_ADDR_TEOFF         0x34
#define d_LCD_ST7735S_ADDR_TEON          0x35
#define d_LCD_ST7735S_ADDR_MADCTL        0x36
#define d_LCD_ST7735S_ADDR_VSCSAD        0x37
#define d_LCD_ST7735S_ADDR_IDMOFF        0x38
#define d_LCD_ST7735S_ADDR_IDMON         0x39
#define d_LCD_ST7735S_ADDR_COLMOD        0x3A
#define d_LCD_ST7735S_ADDR_RDID1         0xDA
#define d_LCD_ST7735S_ADDR_RDID2         0xDB
#define d_LCD_ST7735S_ADDR_RDID3         0xDC

//+ RDDST
#define d_LCD_ST7735S_BIT_RDDST_BSTON       0x80
#define d_LCD_ST7735S_BIT_RDDST_MY          0x40
#define d_LCD_ST7735S_BIT_RDDST_MX          0x20
#define d_LCD_ST7735S_BIT_RDDST_MV          0x10
#define d_LCD_ST7735S_BIT_RDDST_ML          0x08
#define d_LCD_ST7735S_BIT_RDDST_RGB         0x04
#define d_LCD_ST7735S_BIT_RDDST_MH          0x02
#define d_LCD_ST7735S_BIT_RDDST_ST24        0x01

#define d_LCD_ST7735S_BIT_RDDST_ST23        0x80
#define d_LCD_ST7735S_BIT_RDDST_IFPF2       0x40
#define d_LCD_ST7735S_BIT_RDDST_IFPF1       0x20
#define d_LCD_ST7735S_BIT_RDDST_IFPF0       0x10
#define d_LCD_ST7735S_BIT_RDDST_IDMON       0x08
#define d_LCD_ST7735S_BIT_RDDST_PTLON       0x04
#define d_LCD_ST7735S_BIT_RDDST_SLOUT       0x02
#define d_LCD_ST7735S_BIT_RDDST_NORON       0x01

#define d_LCD_ST7735S_BIT_RDDST_GCS1        0x80
#define d_LCD_ST7735S_BIT_RDDST_GCS0        0x40
#define d_LCD_ST7735S_BIT_RDDST_TEM         0x20
#define d_LCD_ST7735S_BIT_RDDST_ST4         0x10
#define d_LCD_ST7735S_BIT_RDDST_ST3         0x08
#define d_LCD_ST7735S_BIT_RDDST_ST2         0x04
#define d_LCD_ST7735S_BIT_RDDST_ST1         0x02
#define d_LCD_ST7735S_BIT_RDDST_ST0         0x01

//+ RDDPM
#define d_LCD_ST7735S_BIT_RDDPM_BSTON       0x80
#define d_LCD_ST7735S_BIT_RDDPM_IDMON       0x40
#define d_LCD_ST7735S_BIT_RDDPM_PTLON       0x20
#define d_LCD_ST7735S_BIT_RDDPM_SLPOUT      0x10
#define d_LCD_ST7735S_BIT_RDDPM_NORON       0x08
#define d_LCD_ST7735S_BIT_RDDPM_DISON       0x04

//+ RDDMADCTL
#define d_LCD_ST7735S_BIT_RDDMADCTL_MY      0x80
#define d_LCD_ST7735S_BIT_RDDMADCTL_MX      0x40
#define d_LCD_ST7735S_BIT_RDDMADCTL_MV      0x20
#define d_LCD_ST7735S_BIT_RDDMADCTL_ML      0x10
#define d_LCD_ST7735S_BIT_RDDMADCTL_RGB     0x08
#define d_LCD_ST7735S_BIT_RDDMADCTL_MH      0x04

//+ RDDCOLMOD
#define d_LCD_ST7735S_BIT_RDDCOLMOD_IFPF2   0x04
#define d_LCD_ST7735S_BIT_RDDCOLMOD_IFPF1   0x02
#define d_LCD_ST7735S_BIT_RDDCOLMOD_IFPF0   0x01

//+ RDDIM
#define d_LCD_ST7735S_BIT_RDDIM_VSSON       0x80
#define d_LCD_ST7735S_BIT_RDDIM_D6          0x40
#define d_LCD_ST7735S_BIT_RDDIM_INVON       0x20
#define d_LCD_ST7735S_BIT_RDDIM_GCS2        0x04
#define d_LCD_ST7735S_BIT_RDDIM_GCS1        0x02
#define d_LCD_ST7735S_BIT_RDDIM_GCS0        0x01

//+ RDDSM
#define d_LCD_ST7735S_BIT_RDDSM_TEON        0x80
#define d_LCD_ST7735S_BIT_RDDSM_TEM         0x40

//+ RDDSDR
#define d_LCD_ST7735S_BIT_RDDSDR_RELD       0x80
#define d_LCD_ST7735S_BIT_RDDSDR_FUND       0x40
#define d_LCD_ST7735S_BIT_RDDSDR_ATTD       0x20
#define d_LCD_ST7735S_BIT_RDDSDR_BRD        0x10

//+ GAMSET
#define d_LCD_ST7735S_BIT_GAMSET_GC3        0x08
#define d_LCD_ST7735S_BIT_GAMSET_GC2        0x04
#define d_LCD_ST7735S_BIT_GAMSET_GC1        0x02
#define d_LCD_ST7735S_BIT_GAMSET_GC0        0x01

//+ TEON
#define d_LCD_ST7735S_BIT_TEON_TEM          0x01

//+ MADCTL
#define d_LCD_ST7735S_BIT_MADCTL_MY         0x80
#define d_LCD_ST7735S_BIT_MADCTL_MX         0x40
#define d_LCD_ST7735S_BIT_MADCTL_MV         0x20
#define d_LCD_ST7735S_BIT_MADCTL_ML         0x10
#define d_LCD_ST7735S_BIT_MADCTL_RGB        0x08
#define d_LCD_ST7735S_BIT_MADCTL_MH         0x04

//+ COLMOD
#define d_LCD_ST7735S_BIT_COLMOD_IFPF2      0x04
#define d_LCD_ST7735S_BIT_COLMOD_IFPF1      0x02
#define d_LCD_ST7735S_BIT_COLMOD_IFPF0      0x01

typedef enum
{
    m_LCD_ST7735S_ADDR_00_NOP          = 0x00,
    m_LCD_ST7735S_ADDR_01_SWRESET      = 0x01,
    m_LCD_ST7735S_ADDR_04_RDDID        = 0x04,
    m_LCD_ST7735S_ADDR_09_RDDST        = 0x09,
    m_LCD_ST7735S_ADDR_0A_RDDPM        = 0x0A,
    m_LCD_ST7735S_ADDR_0B_RDD_MADCTL   = 0x0B,
    m_LCD_ST7735S_ADDR_0C_RDD_COLMOD   = 0x0C,
    m_LCD_ST7735S_ADDR_0D_RDDIM        = 0x0D,
    m_LCD_ST7735S_ADDR_0E_RDDSM        = 0x0E,
    m_LCD_ST7735S_ADDR_0F_RDDSDR       = 0x0F,
    m_LCD_ST7735S_ADDR_10_SLPIN        = 0x10,
    m_LCD_ST7735S_ADDR_11_SLPOUT       = 0x11,
    m_LCD_ST7735S_ADDR_12_PTLON        = 0x12,
    m_LCD_ST7735S_ADDR_13_NORON        = 0x13,
    m_LCD_ST7735S_ADDR_20_INVOFF       = 0x20,
    m_LCD_ST7735S_ADDR_21_INVON        = 0x21,
    m_LCD_ST7735S_ADDR_26_GAMSET       = 0x26,
    m_LCD_ST7735S_ADDR_28_DISPOFF      = 0x28,
    m_LCD_ST7735S_ADDR_29_DISPON       = 0x29,
    m_LCD_ST7735S_ADDR_2A_CASET        = 0x2A,
    m_LCD_ST7735S_ADDR_2B_RASET        = 0x2B,
    m_LCD_ST7735S_ADDR_2C_RAMWR        = 0x2C,
    m_LCD_ST7735S_ADDR_2D_RGBSET       = 0x2D,
    m_LCD_ST7735S_ADDR_2E_RAMRD        = 0x2E,
    m_LCD_ST7735S_ADDR_30_PTLAR        = 0x30,
    m_LCD_ST7735S_ADDR_33_SCRLAR       = 0x33,
    m_LCD_ST7735S_ADDR_34_TEOFF        = 0x34,
    m_LCD_ST7735S_ADDR_35_TEON         = 0x35,
    m_LCD_ST7735S_ADDR_36_MADCTL       = 0x36,
    m_LCD_ST7735S_ADDR_37_VSCSAD       = 0x37,
    m_LCD_ST7735S_ADDR_38_IDMOFF       = 0x38,
    m_LCD_ST7735S_ADDR_39_IDMON        = 0x39,
    m_LCD_ST7735S_ADDR_3A_COLMOD       = 0x3A,
    m_LCD_ST7735S_ADDR_DA_RDID1        = 0xDA,
    m_LCD_ST7735S_ADDR_DB_RDID2        = 0xDB,
    m_LCD_ST7735S_ADDR_DC_RDID3        = 0xDC
}te_lcd_st7789v2_reg;

#endif
