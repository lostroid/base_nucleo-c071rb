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

void f_dbg_init(void);
te_yes_no f_dbg_mode_get(te_dbg_mode e_mode);
void f_dbg_enable_mode_set(te_dbg_mode e_mode);
void f_dbg_disable_mode_set(te_dbg_mode e_mode);
tu32 f_dbg_print_string(const char *p_Data);
void f_dbg_print_dec64(tu64 v_Dec);
void f_dbg_print_dec64_digit(tu32 v_DigtNum, tu64 v_Dec);
void f_dbg_print_dec32(tu32 v_Dec);
void f_dbg_print_dec32_digit(tu32 v_DigtNum, tu32 v_Dec);
void f_dbg_print_hex64(tu64 v_Hex64);
void f_dbg_print_hex32(tu32 v_Hex32);
void f_dbg_print_hex16(tu16 v_Hex16);
void f_dbf_print_hex8(tu8 v_Hex8);
void f_dbg_print_version(char *p_ver, tu32 v_major, tu32 minor);

#endif
