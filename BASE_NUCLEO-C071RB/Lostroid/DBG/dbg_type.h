/********************************************************************
* File:    dbg_type.h
* Author:  LOSTROID
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#ifndef H_DBG_TYPE_H
#define H_DBG_TYPE_H

#include "../Base/config.h"

#define d_DBG_ZONE_00   0
#define d_DBG_ZONE_01   (1 << 0)
#define d_DBG_ZONE_02   (1 << 1)
#define d_DBG_ZONE_03   (1 << 2)
#define d_DBG_ZONE_04   (1 << 3)
#define d_DBG_ZONE_05   (1 << 4)
#define d_DBG_ZONE_06   (1 << 5)
#define d_DBG_ZONE_07   (1 << 6)
#define d_DBG_ZONE_08   (1 << 7)
#define d_DBG_ZONE_09   (1 << 8)
#define d_DBG_ZONE_10   (1 << 9)
#define d_DBG_ZONE_11   (1 << 10)
#define d_DBG_ZONE_12   (1 << 11)
#define d_DBG_ZONE_13   (1 << 12)
#define d_DBG_ZONE_14   (1 << 13)
#define d_DBG_ZONE_15   (1 << 14)
#define d_DBG_ZONE_16   (1 << 15)
#define d_DBG_ZONE_17   (1 << 16)
#define d_DBG_ZONE_18   (1 << 17)
#define d_DBG_ZONE_19   (1 << 18)
#define d_DBG_ZONE_20   (1 << 19)
#define d_DBG_ZONE_21   (1 << 20)
#define d_DBG_ZONE_22   (1 << 21)
#define d_DBG_ZONE_23   (1 << 22)
#define d_DBG_ZONE_24   (1 << 23)
#define d_DBG_ZONE_25   (1 << 24)
#define d_DBG_ZONE_26   (1 << 25)
#define d_DBG_ZONE_27   (1 << 26)
#define d_DBG_ZONE_28   (1 << 27)
#define d_DBG_ZONE_29   (1 << 28)
#define d_DBG_ZONE_30   (1 << 29)
#define d_DBG_ZONE_31   (1 << 30)
#define d_DBG_ZONE_32   (1 << 31)

typedef enum
{
    m_DBG_COLOR_MODE_DISABLE,
    m_DBG_COLOR_MODE_ENABLE
}te_DBG_Color_Mode;

typedef enum
{
    m_DBG_MODE_SYSTEM   = d_DBG_ZONE_01,
    m_DBG_MODE_SCHEDLUE = d_DBG_ZONE_02,
    m_DBG_MODE_UART     = d_DBG_ZONE_03,
    m_DBG_MODE_SPI      = d_DBG_ZONE_04,
    m_DBG_MODE_SAI      = d_DBG_ZONE_05,
    m_DBG_MODE_CAN      = d_DBG_ZONE_06,
    m_DBG_MODE_RTC      = d_DBG_ZONE_07,
    m_DBG_MODE_WDG      = d_DBG_ZONE_08,
    m_DBG_MODE_TIM      = d_DBG_ZONE_09,
    m_DBG_MODE_I2C      = d_DBG_ZONE_10,
    m_DBG_MODE_I3C      = d_DBG_ZONE_11,
    m_DBG_MODE_I2S      = d_DBG_ZONE_12,
    m_DBG_MODE_ADC      = d_DBG_ZONE_13,
    m_DBG_MODE_DAC      = d_DBG_ZONE_14,
    m_DBG_MODE_USB      = d_DBG_ZONE_15,
    m_DBG_MODE_ETH      = d_DBG_ZONE_16,
    m_DBG_MODE_SDIO     = d_DBG_ZONE_17,
    m_DBG_MODE_DCMI     = d_DBG_ZONE_18
}te_DBG_Mode;

#endif
