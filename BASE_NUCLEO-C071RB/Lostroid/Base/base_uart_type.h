/********************************************************************
* File:    base_uart_type.h
* Author:  Lostroid
* Created: 2025-08-06
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_UART_TYPE_H
#define H_BASE_UART_TYPE_H

#include "typedef.h"

typedef struct
{
    const tu32 cv_max_size;     //+ Const Buff size "버퍼 사이즈"
    tu32 v_read_pos;            //+ Read Postion    "읽기 위치"
    tu32 v_write_pos;           //+ Write Postion   "쓰기 위치"
    tu32 v_Error;               //+ Error           "에러"
}ts_Base_Uart_Buff_Ctrol;

#endif
