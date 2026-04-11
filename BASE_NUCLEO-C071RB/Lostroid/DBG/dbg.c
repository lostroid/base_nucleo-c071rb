/******************************************************************************
* File:    dbg.c
* Author:  LOSTROID
* Created: 2025-08-06
*
* Description:
* This is the debug Control.
*
* Revision History:
*   2025-08-06  New.
******************************************************************************/
#include "../Base_LL/base_ll_uart.h"
#include "dbg.h"

#define d_DBG_STRING_MAX          512
#define d_SPI_SEND_DBG_TARGET(DATA,LEN)   f_LL_UART2_TX_Buff_Write(DATA,LEN)
static tu32 gv_dbg_mode = 0;
//===================================================================
/*### DBG Init
---------------------------------------------------------------------

------------------------------------------------------------------ */
void f_DBG_Init(void)
{
    f_DBG_Set_Mode(m_DBG_MODE_SYSTEM);
}
//===================================================================
/*### Get debug mode
-------------------------------------------------------------------------------
+ e_mode : enum value
+ return : NO(0), YES(1)
---------------------------------------------------------------------------- */
tu32 f_DBG_Get_Mode(te_DBG_Mode e_mode)
{
    if((gv_dbg_mode & (tu32)e_mode) == 0)
        { return 0; }
    else
        { return 1; }
}
//===================================================================
/*### Set debug mode
-------------------------------------------------------------------------------
+ e_mode : enum value
---------------------------------------------------------------------------- */
void f_DBG_Set_Mode(te_DBG_Mode e_mode)
{
    gv_dbg_mode |= (tu32)e_mode;
}
//===================================================================
/*### Reset debug mode
-------------------------------------------------------------------------------
+ e_mode : enum value
---------------------------------------------------------------------------- */
void f_DBG_Reset_Mode(te_DBG_Mode e_mode)
{
    gv_dbg_mode &= (tu32)(~e_mode);
}
//===================================================================
/*### print string
-------------------------------------------------------------------------------
+ *p_data : String
---------------------------------------------------------------------------- */
tu32 f_DBG_Print_String(char *p_data)
{
    tu32 v_err = 0;
    tu32 v_len = 0;
    if(p_data == d_NULL)	                /* NULL Check */
        { v_err++; }
    else
    {
        while((tu8)p_data[v_len] != 0)
        {
            if(v_len > d_DBG_STRING_MAX)    /* MAX Len Cut */
                { break; }
            v_len++;
        }
       d_SPI_SEND_DBG_TARGET((tu8 *)p_data, v_len);
    }
    return(v_err);
}
//===================================================================
/*### Print Dec64
-------------------------------------------------------------------------------
+ v_dec : 64bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Dec64(tu64 v_dec)
{
    tu8 a_data[20];
    tu8 v_start = 0u;
    tu8 v_while = 0u;
    tu8 v_start_pos = 0u;
    tu64 v_temp = 10000000000000000000ull;
    while(v_while < 20u)
    {
        a_data[v_while] =  ((v_dec / v_temp) + (tu8)'0');
        v_dec %= v_temp;
        v_temp /= 10u;
        if(v_start == 0)
        {
            if(a_data[v_while] != (tu8)'0')
            {
                v_start_pos = v_while;
                v_start++;
            }
        }
        v_while++;
    }
    if(v_start == 0)
        { v_start_pos = 19; }
   d_SPI_SEND_DBG_TARGET(&a_data[v_start_pos], (tu8)(20u - v_start_pos));
}
//===================================================================
/*### Print Digit Dec64 
-------------------------------------------------------------------------------
DigtNum:5, 100 -> "00100"
+ v_digt_num : 32bit Fixed
+ v_dec : 64bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Dec64_Digit(tu32 v_digt_num, tu64 v_dec)
{
    tu8 a_data[20];
    tu8 v_start = 0u;
    tu8 v_while = 0u;
    tu8 v_start_pos = 0u;
    tu64 v_temp = 10000000000000000000ull;
    if(v_digt_num > 20)
    { v_digt_num = 20; }

    v_digt_num = 20 - v_digt_num;
    while(v_while < 20u)
    {
        if(v_while == v_digt_num)
        {
        	v_start_pos = v_while;
            v_start++;
        }
        a_data[v_while] =  ((v_dec / v_temp) + (tu8)'0');
        v_dec %= v_temp;
        v_temp /= 10u;
        if(v_start == 0)
        {
            if(a_data[v_while] != (tu8)'0')
            {
                v_start_pos = v_while;
                v_start++;
            }
        }
        v_while++;
    }
    if(v_start == 0)
    { v_start_pos = 19; }

   d_SPI_SEND_DBG_TARGET(&a_data[v_start_pos], (tu8)(20u - v_start_pos));
}
//===================================================================
/*### Print Dec32
-------------------------------------------------------------------------------
100 -> "100"
+ v_dec : 32bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Dec32(tu32 v_dec)
{
    tu8 a_data[10];
    tu8 v_start = 0u;
    tu8 v_while = 0u;
    tu8 v_start_pos = 0u;
    tu32 v_temp = 1000000000;
    while(v_while < 10u)
    {
        a_data[v_while] =  ((v_dec / v_temp) + (tu8)'0');
        v_dec %= v_temp;
        v_temp /= 10u;
        if(v_start == 0)
        {
            if(a_data[v_while] != (tu8)'0')
            {
                v_start_pos = v_while;
                v_start++;
            }
        }
        v_while++;
    }
    if(v_start == 0)
        { v_start_pos = 9; }

   d_SPI_SEND_DBG_TARGET(&a_data[v_start_pos], (tu8)(10u - v_start_pos));
}
//===================================================================
/*### Print Digit Dec32
-------------------------------------------------------------------------------
DigtNum:5, 100 -> "00100"
+ v_digt_num : 32bit Fixed
+ v_dec : 32bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Dec32_Digit(tu32 v_digt_num, tu32 v_dec)
{
    tu8 a_data[10];
    tu8 v_start = 0u;
    tu8 v_while = 0u;
    tu8 v_start_pos = 0u;
    tu32 v_temp = 1000000000;
    if(v_digt_num > 10)
        { v_digt_num = 10; }

    v_digt_num = 10 - v_digt_num;
    while(v_while < 10u)
    {
        if(v_while == v_digt_num)
        {
        	v_start_pos = v_while;
            v_start++;
        }
        a_data[v_while] =  ((v_dec / v_temp) + (tu8)'0');
        v_dec %= v_temp;
        v_temp /= 10u;
        if(v_start == 0)
        {
            if(a_data[v_while] != (tu8)'0')
            {
                v_start_pos = v_while;
                v_start++;
            }
        }
        v_while++;
    }
    if(v_start == 0)
        { v_start_pos = 9; }

   d_SPI_SEND_DBG_TARGET(&a_data[v_start_pos], (tu8)(10u - v_start_pos));
}
//===================================================================
/*### Print Hex64 (테스트 필요)
-------------------------------------------------------------------------------
 18,446,744,073,709,551,615 -> "FFFFFFFFFFFFFFFF"
+ v_hex64 : 64bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Hex64(tu64 v_hex64)
{
    tu8 a_data[16];
    tu8 v_temp = 0u;
    tu8 v_while = 0u;
    tu32 v_shift = 64;

    while(v_while < 16u)
    {
        v_shift -= 4; 
        v_temp = (0x0F & (v_hex64 >> v_shift));
        if((v_temp >= 0u) && (v_temp <= 9u))
            { a_data[v_while] = (tu8) (v_temp + (tu8)'0'); }
        else if((v_temp >= 10u) && (v_temp <= 15u))
            { a_data[v_while] = (tu8) (v_temp + (tu8)'7'); }    //- 65('A') = 10 + 55('7')
        v_while++;
    }
    d_SPI_SEND_DBG_TARGET(a_data, 16u);
}
//===================================================================
/*### Print Hex32
-------------------------------------------------------------------------------
 4,294,967,295 -> "FFFFFFFF"
+ v_hex32 : 32bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Hex32(tu32 v_hex32)
{
    tu8 a_data[8];
    tu8 v_temp = 0u;
    tu8 v_while = 0u;
    tu32 v_shift = 32;
    while(v_while < 8u)
    {
        v_shift -= 4; 
        v_temp = (0x0F & (v_hex32 >> v_shift));
        if((v_temp >= 0u) && (v_temp <= 9u))
            { a_data[v_while] = (tu8) (v_temp + (tu8)'0'); }
        else if((v_temp >= 10u) && (v_temp <= 15u))
            { a_data[v_while] = (tu8) (v_temp + (tu8)'7'); }	//- 65('A') = 10 + 55('7')
        v_while++;
    }
    d_SPI_SEND_DBG_TARGET(a_data, 8u);
}
//===================================================================
/*### Print Hex16
-------------------------------------------------------------------------------
 65,535 -> "FFFF"
+ v_hex16 : 16bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Hex16(tu16 v_hex16)
{
    tu8 a_data[4];
    tu8 v_temp = 0u;
    tu8 v_while = 0u;
    tu8 v_shift = 16;
    while(v_while < 4u)
    {
        v_shift -= 4; 
        v_temp = (0x0F & (v_hex16 >> v_shift));
        if((v_temp >= 0u) && (v_temp <= 9u))
            { a_data[v_while] = (tu8) (v_temp + (tu8)'0'); }
        else if((v_temp >= 10u) && (v_temp <= 15u))
            { a_data[v_while] = (tu8) (v_temp + (tu8)'7'); }    //- 65('A') = 10 + 55('7')
        v_while++;
    }
    d_SPI_SEND_DBG_TARGET(a_data, 4u);
}
//===================================================================
/*### Print Hex8
-------------------------------------------------------------------------------
 255 -> "FF"
+ v_hex8 : 8bit
---------------------------------------------------------------------------- */
void f_DBG_Print_Hex8(tu8 v_hex8)
{
    tu8 a_data[2];
    tu8 v_temp = 0u;

    v_temp = (0x0F & (v_hex8 >> 4));
    if((v_temp >= 0u) && (v_temp <= 9u))
        { a_data[0] = (tu8) (v_temp + (tu8)'0'); }
    else if((v_temp >= 10u) && (v_temp <= 15u))
        { a_data[0] = (tu8) (v_temp + (tu8)'7'); }  //- 65('A') = 10 + 55('7')
    v_temp = (0x0F & v_hex8);
    if((v_temp >= 0u) && (v_temp <= 9u))
        { a_data[1] = (tu8) (v_temp + (tu8)'0'); }
    else if((v_temp >= 10u) && (v_temp <= 15u))
        { a_data[1] = (tu8) (v_temp + (tu8)'7'); }	//- 65('A') = 10 + 55('7')
    d_SPI_SEND_DBG_TARGET(a_data, 2u);
}