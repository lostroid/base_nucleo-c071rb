/******************************************************************************
* File:    dbg_type.h
* Author:  LOSTROID
* Created: 2025-08-06
*
* Description:
* This is the debug Type.
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
******************************************************************************/
#ifndef H_DBG_TYPE_H
#define H_DBG_TYPE_H

#include "../Base_LL/base_ll_config.h"

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
    m_DBG_ZONE_LIST_SYSTEM = d_DBG_ZONE_00,     // SYSTEM ERROR (시스템 에러 상시)

}te_DBG_Zone_List;

typedef enum
{
    m_DBG_COLOR_MODE_DISABLE,
    m_DBG_COLOR_MODE_ENABLE
}te_DBG_Color_Mode;

typedef enum
{
    m_DBG_ERR_NO_ERROR,     /* 에러 없음 (No error) */
    m_DBG_ERR_BUG,          /* 버그 (Bug) */
    m_DBG_ERR_TYPE,         /* 타입 (Type)) */
    m_DBG_ERR_LEN,          /* 길이 (Len) */
    m_DBG_ERR_CRC,          /* CRC 에러 */
    m_DBG_ERR_VALUE,        /* 값 (Value) */
    m_DBG_ERR_TIME_OUT,     /* 타임 아웃 (Timeout) */
    m_DBG_ERR_READ,         
    m_DBG_ERR_READ_OVF,
    m_DBG_ERR_WRITE,
    m_DBG_ERR_WRITE_OVF
}te_DBG_ERR_Code;

typedef enum
{
    m_DBG_MODE_SYSTEM = d_DBG_ZONE_01,       /* 디버그 기본 */
    m_DBG_MODE_SCHEDLUE = d_DBG_ZONE_02,
    m_DBG_MODE_UART = d_DBG_ZONE_03,
    m_DBG_MODE_SPI = d_DBG_ZONE_04,
    m_DBG_MODE_SAI = d_DBG_ZONE_05,
    m_DBG_MODE_CAN = d_DBG_ZONE_06,
    m_DBG_MODE_RTC = d_DBG_ZONE_07,
    m_DBG_MODE_WDG = d_DBG_ZONE_08,
    m_DBG_MODE_TIM = d_DBG_ZONE_09,
    m_DBG_MODE_I2C = d_DBG_ZONE_10,
    m_DBG_MODE_I3C = d_DBG_ZONE_11,
    m_DBG_MODE_I2S = d_DBG_ZONE_12,
    m_DBG_MODE_ADC = d_DBG_ZONE_13,
    m_DBG_MODE_DAC = d_DBG_ZONE_14,
    m_DBG_MODE_USB = d_DBG_ZONE_15,
    m_DBG_MODE_ETH = d_DBG_ZONE_16,
    m_DBG_MODE_SDIO = d_DBG_ZONE_17,
    m_DBG_MODE_DCMI = d_DBG_ZONE_18
}te_DBG_Mode;

#endif
