/******************************************************************************
* File:    cli_main_type.h
* Author:  Lostroid
* Created: 2025-08-06
*
* Description:
* This is the CLI Type.
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
#ifndef H_CLI_MAIN_TYPE_H
#define H_CLI_MAIN_TYPE_H

#include "../Base_LL/base_ll_config.h"

#define d_CLI_MAIN_CMD_START            "\r\nSTART CLI v00.10 "
#define d_CLI_MAIN_CMD_NAME             "\r\nDBOT# "
#define d_CLI_MAIN_CMD_HISTORY_SIZE     10      /// command History size. 
#define d_CLI_MAIN_CMD_BUFF_SIZE        64      /// command max len. (MAX 255)
#define d_CLI_MAIN_NULL                 0
#define d_CLI_MAIN_PASS                 0
#define d_CLI_MAIN_FAIL                 1
#define d_CLI_MAIN_FUNC_SIZE            26

typedef enum
{
    m_CLI_MAIN_MSG_NO_CMD,
    m_CLI_MAIN_MSG_ERR,
    m_CLI_MAIN_MSG_BUSY,
    m_CLI_MAIN_MSG_LEN,
    m_CLI_MAIN_MSG_ADDR
}te_CLI_MAIN_MSG;


#endif
