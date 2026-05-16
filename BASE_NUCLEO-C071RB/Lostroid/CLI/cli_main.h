/********************************************************************
* File:    cli_main.h
* Author:  Lostroid
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#ifndef H_CLI_MAIN_H
#define H_CLI_MAIN_H
#include "../base/base_uart_type.h"
#include "cli_main_type.h"

void f_cli_init(void);
void f_cli_module(void);
void f_cli_base_name(void);
void f_cli_Buff_check(tu8* pa_Data,ts_base_uart_buff_ctrol* s_Postion);
tu32 f_cli_command_check(tu8* pa_Data);
void f_cli_msg_error(te_CLI_MAIN_MSG e_Massage);
tu32 f_cli_compare_string(tu8 *p_Data1, const tu8 *p_Data2);
tu32 f_cli_string_to_dec(tu8 *p_Data);
tu32 f_cli_string_to_hex(tu8 *p_Data);

#endif
