/********************************************************************
* File:    base_gpio_type.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_GPIO_TYPE_H
#define H_BASE_GPIO_TYPE_H

#include "typedef.h"

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

#define d_GPIO_MODE_00(MODE)    (MODE << 0UL)
#define d_GPIO_MODE_01(MODE)    (MODE << 2UL)
#define d_GPIO_MODE_02(MODE)    (MODE << 4UL)
#define d_GPIO_MODE_03(MODE)    (MODE << 6UL)
#define d_GPIO_MODE_04(MODE)    (MODE << 8UL)
#define d_GPIO_MODE_05(MODE)    (MODE << 10UL)
#define d_GPIO_MODE_06(MODE)    (MODE << 12UL)
#define d_GPIO_MODE_07(MODE)    (MODE << 14UL)
#define d_GPIO_MODE_08(MODE)    (MODE << 16UL)
#define d_GPIO_MODE_09(MODE)    (MODE << 18UL)
#define d_GPIO_MODE_10(MODE)    (MODE << 20UL)
#define d_GPIO_MODE_11(MODE)    (MODE << 22UL)
#define d_GPIO_MODE_12(MODE)    (MODE << 24UL)
#define d_GPIO_MODE_13(MODE)    (MODE << 26UL)
#define d_GPIO_MODE_14(MODE)    (MODE << 28UL)
#define d_GPIO_MODE_15(MODE)    (MODE << 30UL)

#define d_GPIO_PUPD_00(PUDU)    (PUDU << 0UL)
#define d_GPIO_PUPD_01(PUDU)    (PUDU << 2UL)
#define d_GPIO_PUPD_02(PUDU)    (PUDU << 4UL)
#define d_GPIO_PUPD_03(PUDU)    (PUDU << 6UL)
#define d_GPIO_PUPD_04(PUDU)    (PUDU << 8UL)
#define d_GPIO_PUPD_05(PUDU)    (PUDU << 10UL)
#define d_GPIO_PUPD_06(PUDU)    (PUDU << 12UL)
#define d_GPIO_PUPD_07(PUDU)    (PUDU << 14UL)
#define d_GPIO_PUPD_08(PUDU)    (PUDU << 16UL)
#define d_GPIO_PUPD_09(PUDU)    (PUDU << 18UL)
#define d_GPIO_PUPD_10(PUDU)    (PUDU << 20UL)
#define d_GPIO_PUPD_11(PUDU)    (PUDU << 22UL)
#define d_GPIO_PUPD_12(PUDU)    (PUDU << 24UL)
#define d_GPIO_PUPD_13(PUDU)    (PUDU << 26UL)
#define d_GPIO_PUPD_14(PUDU)    (PUDU << 28UL)
#define d_GPIO_PUPD_15(PUDU)    (PUDU << 30UL)

#define d_GPIO_00_AFSEL(AF)    (AF << 0UL)
#define d_GPIO_01_AFSEL(AF)    (AF << 4UL)
#define d_GPIO_02_AFSEL(AF)    (AF << 8UL)
#define d_GPIO_03_AFSEL(AF)    (AF << 12UL)
#define d_GPIO_04_AFSEL(AF)    (AF << 16UL)
#define d_GPIO_05_AFSEL(AF)    (AF << 20UL)
#define d_GPIO_06_AFSEL(AF)    (AF << 24UL)
#define d_GPIO_07_AFSEL(AF)    (AF << 28UL)
#define d_GPIO_08_AFSEL(AF)    (AF << 0UL)
#define d_GPIO_09_AFSEL(AF)    (AF << 4UL)
#define d_GPIO_10_AFSEL(AF)    (AF << 8UL)
#define d_GPIO_11_AFSEL(AF)    (AF << 12UL)
#define d_GPIO_12_AFSEL(AF)    (AF << 16UL)
#define d_GPIO_13_AFSEL(AF)    (AF << 20UL)
#define d_GPIO_14_AFSEL(AF)    (AF << 24UL)
#define d_GPIO_15_AFSEL(AF)    (AF << 28UL)

#define d_GPIO_A00_MODE_UART1_TX            d_GPIO_MODE_00(d_GPIO_MODE_AF)
#define d_GPIO_A01_MODE_UART1_RX            d_GPIO_MODE_01(d_GPIO_MODE_AF)
#define d_GPIO_A02_MODE_UART2_TX            d_GPIO_MODE_02(d_GPIO_MODE_AF)
#define d_GPIO_A03_MODE_UART2_RX            d_GPIO_MODE_03(d_GPIO_MODE_AF)
#define d_GPIO_A04_MODE_DC5V_IN4            d_GPIO_MODE_04(d_GPIO_MODE_ANALOG)
#define d_GPIO_A05_MODE_LED_LD1             d_GPIO_MODE_05(d_GPIO_MODE_OUTPUT)
#define d_GPIO_A06_MODE_C5V_IN6             d_GPIO_MODE_06(d_GPIO_MODE_ANALOG)
#define d_GPIO_A07_MODE_C12V_IN7            d_GPIO_MODE_07(d_GPIO_MODE_ANALOG)
#define d_GPIO_A08_MODE_MT_POWER            d_GPIO_MODE_08(d_GPIO_MODE_OUTPUT)
#define d_GPIO_A09_MODE_DISABLE             d_GPIO_MODE_09(d_GPIO_MODE_ANALOG)
#define d_GPIO_A10_MODE_DISABLE             d_GPIO_MODE_10(d_GPIO_MODE_ANALOG)
#define d_GPIO_A11_MODE_SW_A                d_GPIO_MODE_11(d_GPIO_MODE_INPUT)
#define d_GPIO_A12_MODE_SW_B                d_GPIO_MODE_12(d_GPIO_MODE_INPUT)
#define d_GPIO_A13_MODE_DBG_SWDIO           d_GPIO_MODE_13(d_GPIO_MODE_AF)
#define d_GPIO_A14_MODE_DBG_SWCLK           d_GPIO_MODE_14(d_GPIO_MODE_AF)
#define d_GPIO_A15_MODE_MT_OPEN             d_GPIO_MODE_15(d_GPIO_MODE_AF)

#define d_GPIO_B00_MODE_BATTERY_TEMP_IN17   d_GPIO_MODE_00(d_GPIO_MODE_ANALOG)
#define d_GPIO_B01_MODE_MOTOR_IN18          d_GPIO_MODE_01(d_GPIO_MODE_ANALOG)
#define d_GPIO_B02_MODE_DISABLE             d_GPIO_MODE_02(d_GPIO_MODE_ANALOG)
#define d_GPIO_B03_MODE_MT_CLOSE            d_GPIO_MODE_03(d_GPIO_MODE_AF)
#define d_GPIO_B04_MODE_MT_SENSOR_OPEN      d_GPIO_MODE_04(d_GPIO_MODE_INPUT)
#define d_GPIO_B05_MODE_MT_SENSOR_CLOSE     d_GPIO_MODE_05(d_GPIO_MODE_INPUT)
#define d_GPIO_B06_MODE_STAT_OUT            d_GPIO_MODE_06(d_GPIO_MODE_OUTPUT)
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
