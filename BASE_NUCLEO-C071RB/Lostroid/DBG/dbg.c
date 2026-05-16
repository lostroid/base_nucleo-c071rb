/********************************************************************
* File:    dbg.c
* Author:  LOSTROID
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#include "../Base/base_uart.h"
#include "dbg.h"

#define d_DBG_STRING_MAX          256
#define d_SPI_SEND_DBG_TARGET(DATA,LEN)   f_base_uart2_tx_buff_write(DATA,LEN)
static tu32 gv_dbg_mode = 0;
//===================================================================
/*### DBG Init
---------------------------------------------------------------------

------------------------------------------------------------------ */
void f_dbg_init(void)
{
    f_dbg_enable_mode_set(m_DBG_MODE_SYSTEM);
    f_dbg_enable_mode_set(m_DBG_MODE_SCHEDULE);
}
//===================================================================
/*### Get debug mode
---------------------------------------------------------------------
+ e_mode : enum value
+ return : m_YESNO_NO, m_YESNO_YES
-------------------------------------------------------------------*/
te_yes_no f_dbg_mode_get(te_dbg_mode e_mode)
{
    if((gv_dbg_mode & (tu32)e_mode) == 0)
        { return m_YESNO_NO; }
    else
        { return m_YESNO_YES; }
}
//===================================================================
/*### Active debug mode "디버깅 활성화"
---------------------------------------------------------------------
+ e_mode : enum value "디버깅 정보"
-------------------------------------------------------------------*/
void f_dbg_enable_mode_set(te_dbg_mode e_mode)
{
    gv_dbg_mode |= (tu32)e_mode;
}
//===================================================================
/*### Reset debug mode
---------------------------------------------------------------------
+ e_mode : enum value
-------------------------------------------------------------------*/
void f_dbg_disable_mode_set(te_dbg_mode e_mode)
{
    gv_dbg_mode &= (tu32)(~e_mode);
}
//===================================================================
/*### print string "문자열 출력"
---------------------------------------------------------------------
+ *p_data : String "배열 문자 포인터"
+ return :  0(ok), 1(error)
-------------------------------------------------------------------*/
tu32 f_dbg_print_string(const char *p_data)
{
    tu32 v_len = 0;
    if(p_data == d_NULL)    //+ NULL Point Check
        { return 1; }
    if(p_data[0] == '\0')   //+ NULL Check
        { return 1; }

    while(p_data[v_len] != '\0')
    {
        v_len++;
        if(v_len >= d_DBG_STRING_MAX)    //+ MAX Len Cut
            { break; }
    }
    d_SPI_SEND_DBG_TARGET((const tu8 *)p_data, v_len);
    return 0;
}
//===================================================================
/*### Print Dec64 "64비트 숫자 출력"
---------------------------------------------------------------------
+ v_dec : 64bit "64bit 입력"
-------------------------------------------------------------------*/
void f_dbg_print_dec64(tu64 v_dec)
{
    tu8 a_data[20];
    tu8 v_index = 20;

    if(v_dec == 0)  //+ zero
    {
        a_data[0] = '0';
        d_SPI_SEND_DBG_TARGET((const tu8*)a_data, 1);
        return;
    }
    while(v_dec > 0 && v_index > 0)
    {
        a_data[--v_index] = (v_dec % 10) + '0';
        v_dec /= 10;
    }
    d_SPI_SEND_DBG_TARGET((const tu8*)&a_data[v_index], 20 - v_index);
}
//===================================================================
/*### Print Digit Dec64  "자리수 고정 64bit 숫자 출력"
---------------------------------------------------------------------
DigtNum:5, 100 -> "00100"
+ v_min_digits : max 20 zero padding  "자리수 고정"
+ v_dec : 64bit                     "64비트 입력"
-------------------------------------------------------------------*/
void f_dbg_print_dec64_digit(tu32 v_min_digits, tu64 v_dec)
{
    tu8 a_data[20];
    tu8 v_index = 20;
    tu32 v_total_len = 0;

    if(v_min_digits == 0) 
        { return; }

    if(v_min_digits > 20)
        { v_min_digits = 20; }

    //+ number convert 
    if(v_dec == 0)
    {
        a_data[--v_index] = '0';
        v_total_len = 1;
    }
    else
    {
        while(v_dec > 0 && v_index > 0)
        {
            a_data[--v_index] = (v_dec % 10) + '0';
            v_dec /= 10;
        }
        v_total_len = 20 - v_index;
    }

    //= zero padding (minimum width)
    while(v_total_len < v_min_digits)
    {
        a_data[--v_index] = '0';
        v_total_len++;
    }

    d_SPI_SEND_DBG_TARGET((const tu8*)&a_data[v_index], v_total_len);
}
//===================================================================
/*### Print Dec32
---------------------------------------------------------------------
100 -> "100"
+ v_dec : 32bit
-------------------------------------------------------------------*/
void f_dbg_print_dec32(tu32 v_dec)
{
    tu8 a_data[10];
    tu8 v_index = 10;

    if(v_dec == 0)  //+ zero
    {
        a_data[0] = '0';
        d_SPI_SEND_DBG_TARGET((const tu8*)a_data, 1);
        return;
    }

    while(v_dec > 0 && v_index > 0)
    {
        a_data[--v_index] = (v_dec % 10) + '0';
        v_dec /= 10;
    }

    d_SPI_SEND_DBG_TARGET((const tu8*)&a_data[v_index], 10 - v_index);
}
//===================================================================
/*### Print Digit Dec32
---------------------------------------------------------------------
v_min_digits : 5(332 -> 00332), (12345678 -> 12345678)
+ v_min_digits : max 10
+ v_dec : 32bit
-------------------------------------------------------------------*/
void f_dbg_print_dec32_digit(tu32 v_min_digits, tu32 v_dec)
{
    tu8 a_data[10];
    tu8 v_index = 10;
    tu32 v_total_len = 0;

    if(v_min_digits == 0) 
        { return; }

    if(v_min_digits > 10)
        { v_min_digits = 10; }

    //+ number convert 
    if(v_dec == 0)
    {
        a_data[--v_index] = '0';
        v_total_len = 1;
    }
    else
    {
        while(v_dec > 0 && v_index > 0)
        {
            a_data[--v_index] = (v_dec % 10) + '0';
            v_dec /= 10;
        }
        v_total_len = 10 - v_index;
    }

    //= zero padding (minimum width)
    while(v_total_len < v_min_digits)
    {
        a_data[--v_index] = '0';
        v_total_len++;
    }

    d_SPI_SEND_DBG_TARGET((const tu8*)&a_data[v_index], v_total_len);
}
//===================================================================
/*### Print Hex64 (테스트 필요)
---------------------------------------------------------------------
 18,446,744,073,709,551,615 -> "FFFFFFFFFFFFFFFF"
+ v_hex64 : 64bit
-------------------------------------------------------------------*/
void f_dbg_print_hex64(tu64 v_hex64)
{
    tu8 a_data[16];
    tu8 v_temp = 0u;
    tu8 v_while = 0u;
    tu32 v_shift = 64;

    while(v_while < 16u)
    {
        v_shift -= 4; // + 4bit = 0xF
        v_temp = (tu8)(0x0F & (v_hex64 >> v_shift));
        //+ 65('A') = 10 + 55('7')
        a_data[v_while] = (v_temp <= 9u) ? (v_temp + (tu8)'0') : (v_temp + (tu8)'7'); 
        v_while++;
    }
    d_SPI_SEND_DBG_TARGET((const tu8*)a_data, 16u);
}
//===================================================================
/*### Print Hex32
---------------------------------------------------------------------
 4,294,967,295 -> "FFFFFFFF"
+ v_hex32 : 32bit
-------------------------------------------------------------------*/
void f_dbg_print_hex32(tu32 v_hex32)
{
    tu8 a_data[8];
    tu8 v_temp = 0u;
    tu8 v_while = 0u;
    tu32 v_shift = 32;
    while(v_while < 8u)
    {
        v_shift -= 4;  // + 4bit = 0xF
        v_temp = (tu8)(0x0F & (v_hex32 >> v_shift));
        //+ 65('A') = 10 + 55('7')
        a_data[v_while] = (v_temp <= 9u) ? (v_temp + (tu8)'0') : (v_temp + (tu8)'7'); 
        v_while++;
    }
    d_SPI_SEND_DBG_TARGET((const tu8*)a_data, 8u);
}
//===================================================================
/*### Print Hex16
---------------------------------------------------------------------
 65,535 -> "FFFF"
+ v_hex16 : 16bit
-------------------------------------------------------------------*/
void f_dbg_print_hex16(tu16 v_hex16)
{
    tu8 a_data[4];
    tu8 v_temp = 0u;
    tu8 v_while = 0u;
    tu8 v_shift = 16;
    while(v_while < 4u)
    {
        v_shift -= 4;  // + 4bit = 0xF
        v_temp = (tu8)(0x0F & (v_hex16 >> v_shift));
        //+ 65('A') = 10 + 55('7')
        a_data[v_while] = (v_temp <= 9u) ? (v_temp + (tu8)'0') : (v_temp + (tu8)'7');
        v_while++;
    }
    d_SPI_SEND_DBG_TARGET((const tu8*)a_data, 4u);
}
//===================================================================
/*### Print Hex8
---------------------------------------------------------------------
 255 -> "FF"
+ v_hex8 : 8bit
-------------------------------------------------------------------*/
void f_dbf_print_hex8(tu8 v_hex8)
{
    tu8 a_data[2];
    tu8 v_temp = 0u;

    v_temp = (0x0F & (v_hex8 >> 4));
    //+ 65('A') = 10 + 55('7')
    a_data[0] = (v_temp <= 9u) ? (v_temp + (tu8)'0') : (v_temp + (tu8)'7'); 

    v_temp = (0x0F & v_hex8);
    //+ 65('A') = 10 + 55('7')
    a_data[1] = (v_temp <= 9u) ? (v_temp + (tu8)'0') : (v_temp + (tu8)'7');
    d_SPI_SEND_DBG_TARGET((const tu8*)a_data, 2u);
}
//===================================================================
/*### Error Return print 
---------------------------------------------------------------------
+ p_ver: string
+ v_major: version
+ v_minor: version
-------------------------------------------------------------------*/
void f_dbg_print_version(char *p_ver, tu32 v_major, tu32 minor)
{
    f_dbg_print_string(p_ver);
    f_dbg_print_dec32_digit(2, (tu32)v_major);
    f_dbg_print_string(".");
    f_dbg_print_dec32_digit(2, (tu32)minor);
}
