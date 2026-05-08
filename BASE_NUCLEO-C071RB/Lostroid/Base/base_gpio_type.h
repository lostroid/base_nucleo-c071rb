/********************************************************************
* File:    base_gpio_type.h
* Author:  Lostroid
* Created: 2026-02-19
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
********************************************************************/
#ifndef H_BASE_GPIO_TYPE_H
#define H_BASE_GPIO_TYPE_H

#include "types.h"

#define d_GPIO_MODE_INPUT       (0x00UL)
#define d_GPIO_MODE_OUTPUT      (0x01UL)
#define d_GPIO_MODE_AF          (0x02UL)
#define d_GPIO_MODE_ANALOG      (0x03UL)

#define d_GPIO_SPEED_VERY_LOW   (0x00UL)
#define d_GPIO_SPEED_LOW        (0x01UL)
#define d_GPIO_SPEED_HIGH       (0x02UL)
#define d_GPIO_SPEED_VERY_HIGH  (0x03UL)

#define d_GPIO_PUPD_NO          (0x00UL)
#define d_GPIO_PUPD_UP          (0x01UL)
#define d_GPIO_PUPD_DOWN        (0x02UL)
#define d_GPIO_PUPD_RES         (0x03UL)

#define d_GPIO_MODE_00(D_MODE)      (D_MODE << 0UL)
#define d_GPIO_MODE_01(D_MODE)      (D_MODE << 2UL)
#define d_GPIO_MODE_02(D_MODE)      (D_MODE << 4UL)
#define d_GPIO_MODE_03(D_MODE)      (D_MODE << 6UL)
#define d_GPIO_MODE_04(D_MODE)      (D_MODE << 8UL)
#define d_GPIO_MODE_05(D_MODE)      (D_MODE << 10UL)
#define d_GPIO_MODE_06(D_MODE)      (D_MODE << 12UL)
#define d_GPIO_MODE_07(D_MODE)      (D_MODE << 14UL)
#define d_GPIO_MODE_08(D_MODE)      (D_MODE << 16UL)
#define d_GPIO_MODE_09(D_MODE)      (D_MODE << 18UL)
#define d_GPIO_MODE_10(D_MODE)      (D_MODE << 20UL)
#define d_GPIO_MODE_11(D_MODE)      (D_MODE << 22UL)
#define d_GPIO_MODE_12(D_MODE)      (D_MODE << 24UL)
#define d_GPIO_MODE_13(D_MODE)      (D_MODE << 26UL)
#define d_GPIO_MODE_14(D_MODE)      (D_MODE << 28UL)
#define d_GPIO_MODE_15(D_MODE)      (D_MODE << 30UL)

#define d_GPIO_SPEED_00(D_SPEED)    (D_SPEED << 0UL)
#define d_GPIO_SPEED_01(D_SPEED)    (D_SPEED << 2UL)
#define d_GPIO_SPEED_02(D_SPEED)    (D_SPEED << 4UL)
#define d_GPIO_SPEED_03(D_SPEED)    (D_SPEED << 6UL)
#define d_GPIO_SPEED_04(D_SPEED)    (D_SPEED << 8UL)
#define d_GPIO_SPEED_05(D_SPEED)    (D_SPEED << 10UL)
#define d_GPIO_SPEED_06(D_SPEED)    (D_SPEED << 12UL)
#define d_GPIO_SPEED_07(D_SPEED)    (D_SPEED << 14UL)
#define d_GPIO_SPEED_08(D_SPEED)    (D_SPEED << 16UL)
#define d_GPIO_SPEED_09(D_SPEED)    (D_SPEED << 18UL)
#define d_GPIO_SPEED_10(D_SPEED)    (D_SPEED << 20UL)
#define d_GPIO_SPEED_11(D_SPEED)    (D_SPEED << 22UL)
#define d_GPIO_SPEED_12(D_SPEED)    (D_SPEED << 24UL)
#define d_GPIO_SPEED_13(D_SPEED)    (D_SPEED << 26UL)
#define d_GPIO_SPEED_14(D_SPEED)    (D_SPEED << 28UL)
#define d_GPIO_SPEED_15(D_SPEED)    (D_SPEED << 30UL)

#define d_GPIO_PUPD_00(D_PUDU)      (D_PUDU << 0UL)
#define d_GPIO_PUPD_01(D_PUDU)      (D_PUDU << 2UL)
#define d_GPIO_PUPD_02(D_PUDU)      (D_PUDU << 4UL)
#define d_GPIO_PUPD_03(D_PUDU)      (D_PUDU << 6UL)
#define d_GPIO_PUPD_04(D_PUDU)      (D_PUDU << 8UL)
#define d_GPIO_PUPD_05(D_PUDU)      (D_PUDU << 10UL)
#define d_GPIO_PUPD_06(D_PUDU)      (D_PUDU << 12UL)
#define d_GPIO_PUPD_07(D_PUDU)      (D_PUDU << 14UL)
#define d_GPIO_PUPD_08(D_PUDU)      (D_PUDU << 16UL)
#define d_GPIO_PUPD_09(D_PUDU)      (D_PUDU << 18UL)
#define d_GPIO_PUPD_10(D_PUDU)      (D_PUDU << 20UL)
#define d_GPIO_PUPD_11(D_PUDU)      (D_PUDU << 22UL)
#define d_GPIO_PUPD_12(D_PUDU)      (D_PUDU << 24UL)
#define d_GPIO_PUPD_13(D_PUDU)      (D_PUDU << 26UL)
#define d_GPIO_PUPD_14(D_PUDU)      (D_PUDU << 28UL)
#define d_GPIO_PUPD_15(D_PUDU)      (D_PUDU << 30UL)

#define d_GPIO_00_AFSEL(D_AF)       (D_AF << 0UL)      //+ AFR[0]
#define d_GPIO_01_AFSEL(D_AF)       (D_AF << 4UL)      //+ AFR[0]
#define d_GPIO_02_AFSEL(D_AF)       (D_AF << 8UL)      //+ AFR[0]
#define d_GPIO_03_AFSEL(D_AF)       (D_AF << 12UL)     //+ AFR[0]
#define d_GPIO_04_AFSEL(D_AF)       (D_AF << 16UL)     //+ AFR[0]
#define d_GPIO_05_AFSEL(D_AF)       (D_AF << 20UL)     //+ AFR[0]
#define d_GPIO_06_AFSEL(D_AF)       (D_AF << 24UL)     //+ AFR[0]
#define d_GPIO_07_AFSEL(D_AF)       (D_AF << 28UL)     //+ AFR[0]

#define d_GPIO_08_AFSEL(D_AF)       (D_AF << 0UL)      //+ AFR[1]
#define d_GPIO_09_AFSEL(D_AF)       (D_AF << 4UL)      //+ AFR[1]
#define d_GPIO_10_AFSEL(D_AF)       (D_AF << 8UL)      //+ AFR[1]
#define d_GPIO_11_AFSEL(D_AF)       (D_AF << 12UL)     //+ AFR[1]
#define d_GPIO_12_AFSEL(D_AF)       (D_AF << 16UL)     //+ AFR[1]
#define d_GPIO_13_AFSEL(D_AF)       (D_AF << 20UL)     //+ AFR[1]
#define d_GPIO_14_AFSEL(D_AF)       (D_AF << 24UL)     //+ AFR[1]
#define d_GPIO_15_AFSEL(D_AF)       (D_AF << 28UL)     //+ AFR[1]

#define d_GPIO_A00_MODE_ADC_IN0             d_GPIO_MODE_00(d_GPIO_MODE_ANALOG)
#define d_GPIO_A01_MODE_ADC_IN1             d_GPIO_MODE_01(d_GPIO_MODE_ANALOG)
#define d_GPIO_A02_MODE_UART2_TX            d_GPIO_MODE_02(d_GPIO_MODE_AF)
#define d_GPIO_A03_MODE_UART2_RX            d_GPIO_MODE_03(d_GPIO_MODE_AF)
#define d_GPIO_A04_MODE_DC5V_IN4            d_GPIO_MODE_04(d_GPIO_MODE_ANALOG)
#define d_GPIO_A05_MODE_LED_LD1             d_GPIO_MODE_05(d_GPIO_MODE_OUTPUT)
#define d_GPIO_A06_MODE_SPI1_MISO           d_GPIO_MODE_06(d_GPIO_MODE_AF)
#define d_GPIO_A07_MODE_SPI1_MOSI           d_GPIO_MODE_07(d_GPIO_MODE_AF)
#define d_GPIO_A08_MODE_LCD_BL              d_GPIO_MODE_08(d_GPIO_MODE_OUTPUT)
#define d_GPIO_A09_MODE_LCD_DC              d_GPIO_MODE_09(d_GPIO_MODE_OUTPUT)
#define d_GPIO_A10_MODE_DISABLE             d_GPIO_MODE_10(d_GPIO_MODE_ANALOG)
#define d_GPIO_A11_MODE_SW_A                d_GPIO_MODE_11(d_GPIO_MODE_INPUT)
#define d_GPIO_A12_MODE_SW_B                d_GPIO_MODE_12(d_GPIO_MODE_INPUT)
#define d_GPIO_A13_MODE_DBG_SWDIO           d_GPIO_MODE_13(d_GPIO_MODE_AF)
#define d_GPIO_A14_MODE_DBG_SWCLK           d_GPIO_MODE_14(d_GPIO_MODE_AF)
#define d_GPIO_A15_MODE_SPI_CS              d_GPIO_MODE_15(d_GPIO_MODE_OUTPUT)

#define d_GPIO_B00_MODE_BATTERY_TEMP_IN17   d_GPIO_MODE_00(d_GPIO_MODE_ANALOG)
#define d_GPIO_B01_MODE_MOTOR_IN18          d_GPIO_MODE_01(d_GPIO_MODE_ANALOG)
#define d_GPIO_B02_MODE_DISABLE             d_GPIO_MODE_02(d_GPIO_MODE_ANALOG)
#define d_GPIO_B03_MODE_LCD_RES             d_GPIO_MODE_03(d_GPIO_MODE_OUTPUT)
#define d_GPIO_B04_MODE_MT_SENSOR_OPEN      d_GPIO_MODE_04(d_GPIO_MODE_INPUT)
#define d_GPIO_B05_MODE_MT_SENSOR_CLOSE     d_GPIO_MODE_05(d_GPIO_MODE_INPUT)
#define d_GPIO_B06_MODE_SPI1_SCK            d_GPIO_MODE_06(d_GPIO_MODE_AF)
#define d_GPIO_B07_MODE_STAT_IN             d_GPIO_MODE_07(d_GPIO_MODE_INPUT)
#define d_GPIO_B08_MODE_SW_POWER            d_GPIO_MODE_08(d_GPIO_MODE_INPUT)
#define d_GPIO_B09_MODE_DISABLE             d_GPIO_MODE_09(d_GPIO_MODE_ANALOG)
#define d_GPIO_B10_MODE_DISABLE             d_GPIO_MODE_10(d_GPIO_MODE_ANALOG)
#define d_GPIO_B11_MODE_DISABLE             d_GPIO_MODE_11(d_GPIO_MODE_ANALOG)
#define d_GPIO_B12_MODE_DISABLE             d_GPIO_MODE_12(d_GPIO_MODE_ANALOG)
#define d_GPIO_B13_MODE_DISABLE             d_GPIO_MODE_13(d_GPIO_MODE_ANALOG)
#define d_GPIO_B14_MODE_DISABLE             d_GPIO_MODE_14(d_GPIO_MODE_ANALOG)
#define d_GPIO_B15_MODE_DISABLE             d_GPIO_MODE_15(d_GPIO_MODE_ANALOG)

#define d_GPIO_C00_MODE_DISABLE             d_GPIO_MODE_00(d_GPIO_MODE_ANALOG)
#define d_GPIO_C01_MODE_DISABLE             d_GPIO_MODE_01(d_GPIO_MODE_ANALOG)
#define d_GPIO_C02_MODE_DISABLE             d_GPIO_MODE_02(d_GPIO_MODE_ANALOG)
#define d_GPIO_C03_MODE_DISABLE             d_GPIO_MODE_03(d_GPIO_MODE_ANALOG)
#define d_GPIO_C04_MODE_DISABLE             d_GPIO_MODE_04(d_GPIO_MODE_ANALOG)
#define d_GPIO_C05_MODE_DISABLE             d_GPIO_MODE_05(d_GPIO_MODE_ANALOG)
#define d_GPIO_C06_MODE_SW_OK               d_GPIO_MODE_06(d_GPIO_MODE_INPUT)
#define d_GPIO_C07_MODE_DISABLE             d_GPIO_MODE_07(d_GPIO_MODE_ANALOG)
#define d_GPIO_C08_MODE_DISABLE             d_GPIO_MODE_08(d_GPIO_MODE_ANALOG)
#define d_GPIO_C09_MODE_LED_LD2             d_GPIO_MODE_09(d_GPIO_MODE_OUTPUT)
#define d_GPIO_C10_MODE_DISABLE             d_GPIO_MODE_10(d_GPIO_MODE_ANALOG)
#define d_GPIO_C11_MODE_DISABLE             d_GPIO_MODE_11(d_GPIO_MODE_ANALOG)
#define d_GPIO_C12_MODE_DISABLE             d_GPIO_MODE_12(d_GPIO_MODE_ANALOG)
#define d_GPIO_C13_MODE_DISABLE             d_GPIO_MODE_13(d_GPIO_MODE_ANALOG)
#define d_GPIO_C14_MODE_OSCX_IN             d_GPIO_MODE_14(d_GPIO_MODE_ANALOG)
#define d_GPIO_C15_MODE_OSCX_OUT            d_GPIO_MODE_15(d_GPIO_MODE_ANALOG)

typedef enum {
    m_GPIO_ON,
    m_GPIO_OFF
}te_GPIO_Output;

#endif
