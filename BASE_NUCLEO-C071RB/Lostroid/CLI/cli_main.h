/******************************************************************************
* File:    cli_main.h
* Author:  Lostroid
* Created: 2025-08-06
*
* Description:
* This is the CLI control.
*
* Revision History:
*   2025-08-06  New.
******************************************************************************/
#ifndef H_CLI_MAIN_H
#define H_CLI_MAIN_H
#include "../base_LL/base_ll_uart_type.h"
#include "cli_main_type.h"

void f_Base_Cli_Init(void);
void f_Base_Cli_Module(void);
void f_Base_CLI_Name(void);
void f_Base_Cli_BuffCheck(tu8* pa_Data,ts_LL_Uart_Ctrol* s_Postion);
tu32 f_Base_Cli_Command_Check(tu8* pa_Data);
void f_Base_Cli_MsgError(te_CLI_MAIN_MSG e_Massage);
tu32 f_Base_Cli_CompareString(tu8 *p_Data1, const tu8 *p_Data2);
tu32 f_Base_Cli_StringToDec(tu8 *p_Data);
tu32 f_Base_Cli_StringToHex(tu8 *p_Data);

#endif
