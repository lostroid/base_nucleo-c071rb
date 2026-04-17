/********************************************************************
* File:    cli_main_type.h
* Author:  Lostroid
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#ifndef H_CLI_MAIN_TYPE_H
#define H_CLI_MAIN_TYPE_H

#include "../Base/config.h"

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
