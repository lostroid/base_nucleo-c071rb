/******************************************************************************
* File:    lcd_gc9a01_type.h
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
@ DISPLAY TYPE : 1.28 inch IPS
@ Resolution: 240 (H) RGB x240 (V)
@ Control chip: gc9a01
@ Working temperature - 20 ~ 70 ℃
@ Logic voltage 3.3V
@ Backlight type: 2 white LED parallel
******************************************************************************/
#ifndef LCD_GC9A01_TYPE_H
#define LCD_GC9A01_TYPE_H
#include "../../types.h"
//============================================================================= [ Define ]
//----------------------------------------------------------------------------------------- Setting

//---------------------------------------------------------------------------------------- Register
#define d_LCD_GC9A01_RGEA_READ_DISPLAY_IDE_             0x04u
#define d_LCD_GC9A01_RGEA_READ_DISPLAY_STATUS           0x09u
#define d_LCD_GC9A01_RGEA_ENTER_SLEEP_MODE              0x10u
#define d_LCD_GC9A01_RGEA_SLEEP_OUT                     0x11u
#define d_LCD_GC9A01_RGEA_PARTIAL_MODE_ON               0x12u
#define d_LCD_GC9A01_RGEA_NORMAL_DISPLAY_MODE_ON        0x13u
#define d_LCD_GC9A01_RGEA_DISPLAY_INVERSION_OFF         0x20u
#define d_LCD_GC9A01_RGEA_DISPLAY_INVERSION_ON          0x21u
#define d_LCD_GC9A01_RGEA_DISPLAY_OFF                   0x28u
#define d_LCD_GC9A01_RGEA_DISPLAY_ON                    0x29u
#define d_LCD_GC9A01_RGEA_COLUMM_ADDRESS_SET            0x2Au
#define d_LCD_GC9A01_RGEA_PAGE_ADDRESS_SET              0x2Bu
#define d_LCD_GC9A01_RGEA_MEMORY_WRITE                  0x2Cu
#define d_LCD_GC9A01_RGEA_PARTIAL_AREA                  0x30u
#define d_LCD_GC9A01_RGEA_VERTICAL_SCROLLING_DEF        0x33u
#define d_LCD_GC9A01_RGEA_TEARING_EFFECT_LINE_OFF       0x34u
#define d_LCD_GC9A01_RGEA_TEARING_EFFECT_LINE_ON        0x35u
#define d_LCD_GC9A01_RGEA_MEMORY_LINE_OFF               0x36u
#define d_LCD_GC9A01_RGEA_VERTICAL_SCROLLING_STAD       0x37u
#define d_LCD_GC9A01_RGEA_IDLE_MODE_OFF                 0x38u
#define d_LCD_GC9A01_RGEA_IDLE_MODE_ON                  0x39u
#define d_LCD_GC9A01_RGEA_PIXEL_FORMAT_SET              0x3Au
#define d_LCD_GC9A01_RGEA_WRITE_MEMORY_CONTINUE         0x3Cu
#define d_LCD_GC9A01_RGEA_SET_TEAR_SCANLINE             0x44u
#define d_LCD_GC9A01_RGEA_GET_SCANLINE                  0x45u
#define d_LCD_GC9A01_RGEA_WRITE_DISPLAY_BRIGHTNESS      0x51u
#define d_LCD_GC9A01_RGEA_WRITE_CTRL_DISPLAY            0x53u
#define d_LCD_GC9A01_RGEA_READ_ID1                      0xDAu
#define d_LCD_GC9A01_RGEA_READ_ID2                      0xDBu
#define d_LCD_GC9A01_RGEA_READ_ID3                      0xDCu
#define d_LCD_GC9A01_RGEA_RGB_INTERFACE_SIGNAL_CONTROL  0xB0u
#define d_LCD_GC9A01_RGEA_RGB_BLANKING_PORCH_CONTROL    0xB5u
#define d_LCD_GC9A01_RGEA_RGB_DISPLAY_FUNCTION_CONTROL  0xB6u
#define d_LCD_GC9A01_RGEA_RGB_TE_CONTROL                0xBAu
#define d_LCD_GC9A01_RGEA_RGB_INTERFACE_CONTROL         0xF6u
#define d_LCD_GC9A01_RGEA_POWER_CRITERION_CONTROL       0xC1u
#define d_LCD_GC9A01_RGEA_VCORE_VOLTAGE_CONTROL         0xC7u
#define d_LCD_GC9A01_RGEA_VREG1A_VOLTAGE_CONTROL        0xC3u
#define d_LCD_GC9A01_RGEA_VREG1B_VOLTAGE_CONTROL        0xC4u
#define d_LCD_GC9A01_RGEA_VREG2A_VOLTAGE_CONTROL        0xC9u
#define d_LCD_GC9A01_RGEA_FRAME_RATE                    0xE8u
#define d_LCD_GC9A01_RGEA_SPI_2DATA_CONTROL             0xE9u
#define d_LCD_GC9A01_RGEA_CHARGE_PUMP_FREQUENT_CONTROL  0xECu
#define d_LCD_GC9A01_RGEA_INNER_RESISTER_ENABLE1        0xFEu
#define d_LCD_GC9A01_RGEA_INNER_RESISTER_ENABLE2        0xEFu
#define d_LCD_GC9A01_RGEA_SET_GAMMA1                    0xF0u
#define d_LCD_GC9A01_RGEA_SET_GAMMA2                    0xF1u
#define d_LCD_GC9A01_RGEA_SET_GAMMA3                    0xF2u
#define d_LCD_GC9A01_RGEA_SET_GAMMA4                    0xF3u
//===================================================================================== [ Typedef ]
typedef enum
{
    m_LCD_GC9A01_RGEA_READ_DISPLAY_IDE_             = 0x04u,
    m_LCD_GC9A01_RGEA_READ_DISPLAY_STATUS           = 0x09u,
    m_LCD_GC9A01_RGEA_ENTER_SLEEP_MODE              = 0x10u,
    m_LCD_GC9A01_RGEA_SLEEP_OUT                     = 0x11u,
    m_LCD_GC9A01_RGEA_PARTIAL_MODE_ON               = 0x12u,
    m_LCD_GC9A01_RGEA_NORMAL_DISPLAY_MODE_ON        = 0x13u,
    m_LCD_GC9A01_RGEA_DISPLAY_INVERSION_OFF         = 0x20u,
    m_LCD_GC9A01_RGEA_DISPLAY_INVERSION_ON          = 0x21u,
    m_LCD_GC9A01_RGEA_DISPLAY_OFF                   = 0x28u,
    m_LCD_GC9A01_RGEA_DISPLAY_ON                    = 0x29u,
    m_LCD_GC9A01_RGEA_COLUMM_ADDRESS_SET            = 0x2Au,
    m_LCD_GC9A01_RGEA_PAGE_ADDRESS_SET              = 0x2Bu,
    m_LCD_GC9A01_RGEA_MEMORY_WRITE                  = 0x2Cu,
    m_LCD_GC9A01_RGEA_PARTIAL_AREA                  = 0x30u,
    m_LCD_GC9A01_RGEA_VERTICAL_SCROLLING_DEF        = 0x33u,
    m_LCD_GC9A01_RGEA_TEARING_EFFECT_LINE_OFF       = 0x34u,
    m_LCD_GC9A01_RGEA_TEARING_EFFECT_LINE_ON        = 0x35u,
    m_LCD_GC9A01_RGEA_MEMORY_LINE_OFF               = 0x36u,
    m_LCD_GC9A01_RGEA_VERTICAL_SCROLLING_STAD       = 0x37u,
    m_LCD_GC9A01_RGEA_IDLE_MODE_OFF                 = 0x38u,
    m_LCD_GC9A01_RGEA_IDLE_MODE_ON                  = 0x39u,
    m_LCD_GC9A01_RGEA_PIXEL_FORMAT_SET              = 0x3Au,
    m_LCD_GC9A01_RGEA_WRITE_MEMORY_CONTINUE         = 0x3Cu,
    m_LCD_GC9A01_RGEA_SET_TEAR_SCANLINE             = 0x44u,
    m_LCD_GC9A01_RGEA_GET_SCANLINE                  = 0x45u,
    m_LCD_GC9A01_RGEA_WRITE_DISPLAY_BRIGHTNESS      = 0x51u,
    m_LCD_GC9A01_RGEA_WRITE_CTRL_DISPLAY            = 0x53u,
    m_LCD_GC9A01_RGEA_READ_ID1                      = 0xDAu,
    m_LCD_GC9A01_RGEA_READ_ID2                      = 0xDBu,
    m_LCD_GC9A01_RGEA_READ_ID3                      = 0xDCu,
    m_LCD_GC9A01_RGEA_RGB_INTERFACE_SIGNAL_CONTROL  = 0xB0u,
    m_LCD_GC9A01_RGEA_RGB_BLANKING_PORCH_CONTROL    = 0xB5u,
    m_LCD_GC9A01_RGEA_RGB_DISPLAY_FUNCTION_CONTROL  = 0xB6u,
    m_LCD_GC9A01_RGEA_RGB_TE_CONTROL                = 0xBAu,
    m_LCD_GC9A01_RGEA_RGB_INTERFACE_CONTROL         = 0xF6u,
    m_LCD_GC9A01_RGEA_POWER_CRITERION_CONTROL       = 0xC1u,
    m_LCD_GC9A01_RGEA_VCORE_VOLTAGE_CONTROL         = 0xC7u,
    m_LCD_GC9A01_RGEA_VREG1A_VOLTAGE_CONTROL        = 0xC3u,
    m_LCD_GC9A01_RGEA_VREG1B_VOLTAGE_CONTROL        = 0xC4u,
    m_LCD_GC9A01_RGEA_VREG2A_VOLTAGE_CONTROL        = 0xC9u,
    m_LCD_GC9A01_RGEA_FRAME_RATE                    = 0xE8u,
    m_LCD_GC9A01_RGEA_SPI_2DATA_CONTROL             = 0xE9u,
    m_LCD_GC9A01_RGEA_CHARGE_PUMP_FREQUENT_CONTROL  = 0xECu,
    m_LCD_GC9A01_RGEA_INNER_RESISTER_ENABLE1        = 0xFEu,
    m_LCD_GC9A01_RGEA_INNER_RESISTER_ENABLE2        = 0xEFu,
    m_LCD_GC9A01_RGEA_SET_GAMMA1                    = 0xF0u,
    m_LCD_GC9A01_RGEA_SET_GAMMA2                    = 0xF1u,
    m_LCD_GC9A01_RGEA_SET_GAMMA3                    = 0xF2u,
    m_LCD_GC9A01_RGEA_SET_GAMMA4                    = 0xF3u
}te_lcd_gc9a01_reg;


#endif