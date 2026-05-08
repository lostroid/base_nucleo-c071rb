/******************************************************************************
* File:    lcd_ili9341_type.h
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
DISPLAY TYPE : 262K COLOR TFT-LCD
Resolution: 240 (H) RGB x320 (V)
Control chip: ILI9341
Working temperature - 20 ~ 70 ℃
Logic voltage 3.3V
Backlight type: 4 white LED parallel
PIN  1 : GND 
PIN  2 : /RESET
PIN  3 : SCL (SPI CLK) 
PIN  4 : RS/A0 (DATA/Command) 
PIN  5 : CS  (SPI)
PIN  6 : SDA (SPI IN) 
PIN  7 : SDO (SPI OUT)
PIN  8 : GND 
PIN  9 : VCC
PIN 10 : A (LED)
PIN 11 : K1 (LED) 
PIN 12 : K2 (LED)
PIN 13 : K3 (LED) 
PIN 14 : K4 (LED) 
PIN 15 : NC (XL Touch scrren)
PIN 16 : NC (YU Touch scrren)
PIN 17 : NC (XR Touch screen)
PIN 18 : NC (YD Touch screen) 
**************************************************************************************************/
#ifndef LCD_ILI9341_TYPE_H
#define LCD_ILI9341_TYPE_H
#include "../../types.h"

//====================================================================================== [ Define ]
//----------------------------------------------------------------------------------------- Setting

//---------------------------------------------------------------------------------------- Register
#define d_LCD_ILI9341_REGA_NO_OPERATION                     0x00u
#define d_LCD_ILI9341_REGA_SOFTWARE_RESET                   0x01u
#define d_LCD_ILI9341_REGA_READ_DISPLAY_IDEN                0x04u
#define d_LCD_ILI9341_REGA_READ_DISPLAY_STTATUS             0x09u
#define d_LCD_ILI9341_REGA_READ_DISPLAY_POWER_MODE          0x0Au
#define d_LCD_ILI9341_REGA_READ_DISPLAY_MADCTL              0x0Bu
#define d_LCD_ILI9341_REGA_READ_DISPLAY_PIXEL_FORMAT        0x0Cu
#define d_LCD_ILI9341_REGA_READ_DISPLAY_IMAGE_FORMAT        0x0Du
#define d_LCD_ILI9341_REGA_READ_DISPLAY_SIGNAL_MODE         0x0Eu
#define d_LCD_ILI9341_REGA_READ_DISPLAY_SELF_DIAGNOSTIC     0x0Fu
#define d_LCD_ILI9341_REGA_ENTER_SLEEP_MODE                 0x10u
#define d_LCD_ILI9341_REGA_SLEEP_OUT                        0x11u
#define d_LCD_ILI9341_REGA_PARIAL_MODE_ON                   0x12u
#define d_LCD_ILI9341_REGA_NOMAL_DISPLAY_MODE_ON            0x13u
#define d_LCD_ILI9341_REGA_DISPLAY_INVERSION_OFF            0x20u
#define d_LCD_ILI9341_REGA_DISPLAY_INVERSION_ON             0x21u
#define d_LCD_ILI9341_REGA_GAMMA_SET                        0x26u
#define d_LCD_ILI9341_REGA_DISPLAY_OFF                      0x28u
#define d_LCD_ILI9341_REGA_DISPLAY_ON                       0x29u
#define d_LCD_ILI9341_REGA_COLUMN_ADDRESS_SET               0x2Au
#define d_LCD_ILI9341_REGA_PAGE_ADDRESS_SET                 0x2Bu
#define d_LCD_ILI9341_REGA_MEMORY_WRITE                     0x2Cu
#define d_LCD_ILI9341_REGA_COLOR_SET                        0x2Du
#define d_LCD_ILI9341_REGA_MEMORY_READ                      0x2Eu
#define d_LCD_ILI9341_REGA_PARTIAL_AREA                     0x30u
#define d_LCD_ILI9341_REGA_VERTICAL_SCROLLING_DEFINITION    0x33u
#define d_LCD_ILI9341_REGA_TEARING_EFFECT_LINE_OFF          0x34u
#define d_LCD_ILI9341_REGA_TEARING_EFFECT_LINE_ON           0x35u
#define d_LCD_ILI9341_REGA_MEMORY_ACCESS_CONTROL            0x36u
#define d_LCD_ILI9341_REGA_VERTICAL_SCROLLING_START_ADDR    0x37u
#define d_LCD_ILI9341_REGA_IDLE_MODE_OFF                    0x38u
#define d_LCD_ILI9341_REGA_IDLE_MODE_ON                     0x39u
#define d_LCD_ILI9341_REGA_PIXEL_FORMAT_SET                 0x3Au
#define d_LCD_ILI9341_REGA_WRITE_MEMORY_CONTINUE            0x3Cu
#define d_LCD_ILI9341_REGA_READ_MEMORY_CONTINUE             0x3Eu
#define d_LCD_ILI9341_REGA_SET_TEAR_SCANLINE                0x44u
#define d_LCD_ILI9341_REGA_GET_SCANLINE                     0x45u
#define d_LCD_ILI9341_REGA_WRITE_DISPLAY_BRIGHTNESS         0x51u
#define d_LCD_ILI9341_REGA_READ_DISPLAY_BRIGHTNESS          0x52u
#define d_LCD_ILI9341_REGA_WRITE_CTRL_DISPLAY               0x53u
#define d_LCD_ILI9341_REGA_READ_CTRL_DISPLAY                0x54u
#define d_LCD_ILI9341_REGA_WRITE_CONTENT_ADAPTIVE_BC        0x55u
#define d_LCD_ILI9341_REGA_READ_CONTENT_ADAPTIVE_BC         0x56u
#define d_LCD_ILI9341_REGA_WRITE_CABC_MB                    0x5Eu
#define d_LCD_ILI9341_REGA_READ_CABC_MB                     0x5Fu
#define d_LCD_ILI9341_REGA_READ_ID1                         0xDAu
#define d_LCD_ILI9341_REGA_READ_ID2                         0xDBu
#define d_LCD_ILI9341_REGA_READ_ID3                         0xDCu
#define d_LCD_ILI9341_REGA_RGB_INTERFACE_SIGNAL_CONTROL     0xB0u
#define d_LCD_ILI9341_REGA_FRAME_CONTROL_NORMAL_MODE        0xB1u
#define d_LCD_ILI9341_REGA_FRAME_CONTROL_IDLE_MODE          0xB2u
#define d_LCD_ILI9341_REGA_FRAME_CONTROL_PARTIAL_MODE       0xB3u
#define d_LCD_ILI9341_REGA_DISPLAY_INVERSION_CONTROL        0xB4u
#define d_LCD_ILI9341_REGA_BLANKING_PORCH_CONTROL           0xB5u
#define d_LCD_ILI9341_REGA_DISPLAY_FUNCTION_CONTROL         0xB6u
#define d_LCD_ILI9341_REGA_ENTRY_MODE_SET                   0xB7u
#define d_LCD_ILI9341_REGA_BACKLIGHT_CONTROL1               0xB8u
#define d_LCD_ILI9341_REGA_BACKLIGHT_CONTROL2               0xB9u
#define d_LCD_ILI9341_REGA_BACKLIGHT_CONTROL3               0xBAu
#define d_LCD_ILI9341_REGA_BACKLIGHT_CONTROL4               0xBBu
#define d_LCD_ILI9341_REGA_BACKLIGHT_CONTROL5               0xBCu
#define d_LCD_ILI9341_REGA_BACKLIGHT_CONTROL7               0xBEu
#define d_LCD_ILI9341_REGA_BACKLIGHT_CONTROL8               0xBFu
#define d_LCD_ILI9341_REGA_POWER_CONTROL1                   0xC0u
#define d_LCD_ILI9341_REGA_POWER_CONTROL2                   0xC1u
#define d_LCD_ILI9341_REGA_VCOM_CONTROL1                    0xC5u
#define d_LCD_ILI9341_REGA_VCOM_CONTROL2                    0xC7u
#define d_LCD_ILI9341_REGA_NV_MEMORY_WRITE                  0xD0u
#define d_LCD_ILI9341_REGA_NV_MEMORY_PROTECTION_KEY         0xD1u
#define d_LCD_ILI9341_REGA_NV_MEMORY_STATUS_READ            0xD2u
#define d_LCD_ILI9341_REGA_READ_ID4                         0xD3u
#define d_LCD_ILI9341_REGA_POSITIVE_GAMMA_CORRECTION        0xE0u
#define d_LCD_ILI9341_REGA_NEGATIVE_GAMMA_CORRECTION        0xE1u
#define d_LCD_ILI9341_REGA_DIGITAL_GAMMA_CONTROL1           0xE2u
#define d_LCD_ILI9341_REGA_DIGITAL_GAMMA_CONTROL2           0xE3u
#define d_LCD_ILI9341_REGA_INTERFACE_CONTROL                0xF6u
//===================================================================================== [ Typedef ]
typedef enum
{
    m_LCD_ILI9341_REGA_NO_OPERATION                   = 0x00,
    m_LCD_ILI9341_REGA_SOFTWARE_RESET                 = 0x01,
    m_LCD_ILI9341_REGA_READ_DISPLAY_IDEN              = 0x04,
    m_LCD_ILI9341_REGA_READ_DISPLAY_STTATUS           = 0x09,
    m_LCD_ILI9341_REGA_READ_DISPLAY_POWER_MODE        = 0x0A,
    m_LCD_ILI9341_REGA_READ_DISPLAY_MADCTL            = 0x0B,
    m_LCD_ILI9341_REGA_READ_DISPLAY_PIXEL_FORMAT      = 0x0C,
    m_LCD_ILI9341_REGA_READ_DISPLAY_IMAGE_FORMAT      = 0x0D,
    m_LCD_ILI9341_REGA_READ_DISPLAY_SIGNAL_MODE       = 0x0E,
    m_LCD_ILI9341_REGA_READ_DISPLAY_SELF_DIAGNOSTIC   = 0x0F,
    m_LCD_ILI9341_REGA_ENTER_SLEEP_MODE               = 0x10,
    m_LCD_ILI9341_REGA_SLEEP_OUT                      = 0x11,
    m_LCD_ILI9341_REGA_PARIAL_MODE_ON                 = 0x12,
    m_LCD_ILI9341_REGA_NOMAL_DISPLAY_MODE_ON          = 0x13,
    m_LCD_ILI9341_REGA_DISPLAY_INVERSION_OFF          = 0x20,
    m_LCD_ILI9341_REGA_DISPLAY_INVERSION_ON           = 0x21,
    m_LCD_ILI9341_REGA_GAMMA_SET                      = 0x26,
    m_LCD_ILI9341_REGA_DISPLAY_OFF                    = 0x28,
    m_LCD_ILI9341_REGA_DISPLAY_ON                     = 0x29,
    m_LCD_ILI9341_REGA_COLUMN_ADDRESS_SET             = 0x2A,
    m_LCD_ILI9341_REGA_PAGE_ADDRESS_SET               = 0x2B,
    m_LCD_ILI9341_REGA_MEMORY_WRITE                   = 0x2C,
    m_LCD_ILI9341_REGA_COLOR_SET                      = 0x2D,
    m_LCD_ILI9341_REGA_MEMORY_READ                    = 0x2E,
    m_LCD_ILI9341_REGA_PARTIAL_AREA                   = 0x30,
    m_LCD_ILI9341_REGA_VERTICAL_SCROLLING_DEFINITION  = 0x33,
    m_LCD_ILI9341_REGA_TEARING_EFFECT_LINE_OFF        = 0x34,
    m_LCD_ILI9341_REGA_TEARING_EFFECT_LINE_ON         = 0x35,
    m_LCD_ILI9341_REGA_MEMORY_ACCESS_CONTROL          = 0x36,
    m_LCD_ILI9341_REGA_VERTICAL_SCROLLING_START_ADDR  = 0x37,
    m_LCD_ILI9341_REGA_IDLE_MODE_OFF                  = 0x38,
    m_LCD_ILI9341_REGA_IDLE_MODE_ON                   = 0x39,
    m_LCD_ILI9341_REGA_PIXEL_FORMAT_SET               = 0x3A,
    m_LCD_ILI9341_REGA_WRITE_MEMORY_CONTINUE          = 0x3C,
    m_LCD_ILI9341_REGA_READ_MEMORY_CONTINUE           = 0x3E,
    m_LCD_ILI9341_REGA_SET_TEAR_SCANLINE              = 0x44,
    m_LCD_ILI9341_REGA_GET_SCANLINE                   = 0x45,
    m_LCD_ILI9341_REGA_WRITE_DISPLAY_BRIGHTNESS       = 0x51,
    m_LCD_ILI9341_REGA_READ_DISPLAY_BRIGHTNESS        = 0x52,
    m_LCD_ILI9341_REGA_WRITE_CTRL_DISPLAY             = 0x53,
    m_LCD_ILI9341_REGA_READ_CTRL_DISPLAY              = 0x54,
    m_LCD_ILI9341_REGA_WRITE_CONTENT_ADAPTIVE_BC      = 0x55,
    m_LCD_ILI9341_REGA_READ_CONTENT_ADAPTIVE_BC       = 0x56,
    m_LCD_ILI9341_REGA_WRITE_CABC_MB                  = 0x5E,
    m_LCD_ILI9341_REGA_READ_CABC_MB                   = 0x5F,
    m_LCD_ILI9341_REGA_READ_ID1                       = 0xDA,
    m_LCD_ILI9341_REGA_READ_ID2                       = 0xDB,
    m_LCD_ILI9341_REGA_READ_ID3                       = 0xDC,
    m_LCD_ILI9341_REGA_RGB_INTERFACE_SIGNAL_CONTROL   = 0xB0,
    m_LCD_ILI9341_REGA_FRAME_CONTROL_NORMAL_MODE      = 0xB1,
    m_LCD_ILI9341_REGA_FRAME_CONTROL_IDLE_MODE        = 0xB2,
    m_LCD_ILI9341_REGA_FRAME_CONTROL_PARTIAL_MODE     = 0xB3,
    m_LCD_ILI9341_REGA_DISPLAY_INVERSION_CONTROL      = 0xB4,
    m_LCD_ILI9341_REGA_BLANKING_PORCH_CONTROL         = 0xB5,
    m_LCD_ILI9341_REGA_DISPLAY_FUNCTION_CONTROL       = 0xB6,
    m_LCD_ILI9341_REGA_ENTRY_MODE_SET                 = 0xB7,
    m_LCD_ILI9341_REGA_BACKLIGHT_CONTROL1             = 0xB8,
    m_LCD_ILI9341_REGA_BACKLIGHT_CONTROL2             = 0xB9,
    m_LCD_ILI9341_REGA_BACKLIGHT_CONTROL3             = 0xBA,
    m_LCD_ILI9341_REGA_BACKLIGHT_CONTROL4             = 0xBB,
    m_LCD_ILI9341_REGA_BACKLIGHT_CONTROL5             = 0xBC,
    m_LCD_ILI9341_REGA_BACKLIGHT_CONTROL7             = 0xBE,
    m_LCD_ILI9341_REGA_BACKLIGHT_CONTROL8             = 0xBF,
    m_LCD_ILI9341_REGA_POWER_CONTROL1                 = 0xC0,
    m_LCD_ILI9341_REGA_POWER_CONTROL2                 = 0xC1,
    m_LCD_ILI9341_REGA_VCOM_CONTROL1                  = 0xC5,
    m_LCD_ILI9341_REGA_VCOM_CONTROL2                  = 0xC7,
    m_LCD_ILI9341_REGA_NV_MEMORY_WRITE                = 0xD0,
    m_LCD_ILI9341_REGA_NV_MEMORY_PROTECTION_KEY       = 0xD1,
    m_LCD_ILI9341_REGA_NV_MEMORY_STATUS_READ          = 0xD2,
    m_LCD_ILI9341_REGA_READ_ID4                       = 0xD3,
    m_LCD_ILI9341_REGA_POSITIVE_GAMMA_CORRECTION      = 0xE0,
    m_LCD_ILI9341_REGA_NEGATIVE_GAMMA_CORRECTION      = 0xE1,
    m_LCD_ILI9341_REGA_DIGITAL_GAMMA_CONTROL1         = 0xE2,
    m_LCD_ILI9341_REGA_DIGITAL_GAMMA_CONTROL2         = 0xE3,
    m_LCD_ILI9341_REGA_INTERFACE_CONTROL              = 0xF6
}te_lcd_ili9341_reg;


#endif