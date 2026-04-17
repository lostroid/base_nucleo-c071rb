/********************************************************************
* File:    dbg.h
* Author: LOSTROID
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#ifndef H_DBG_H
#define H_DBG_H

#include "../Base/base_main_type.h"
#include "dbg_type.h"

void f_DBG_Init(void);
te_YesNo f_DBG_Get_Mode(te_DBG_Mode e_mode);
void f_DBG_Set_Enable_Mode(te_DBG_Mode e_mode);
void f_DBG_Set_Disable_Mode(te_DBG_Mode e_mode);
tu32 f_DBG_Print_String(char *p_Data);
void f_DBG_Print_Dec64(tu64 v_Dec);
void f_DBG_Print_Dec64_Digit(tu32 v_DigtNum, tu64 v_Dec);
void f_DBG_Print_Dec32(tu32 v_Dec);
void f_DBG_Print_Dec32_Digit(tu32 v_DigtNum, tu32 v_Dec);
void f_DBG_Print_Hex64(tu64 v_Hex64);
void f_DBG_Print_Hex32(tu32 v_Hex32);
void f_DBG_Print_Hex16(tu16 v_Hex16);
void f_DBG_Print_Hex8(tu8 v_Hex8);

#endif
