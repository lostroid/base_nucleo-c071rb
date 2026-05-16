/********************************************************************
* File:    types.c
* Author:  LOASTROID
* Created: 2025-12-04
* Encoding: UTF-8
********************************************************************/
#include "types.h"

const tu8 xca_return_ok[]            = "OK";            //+ OK                      "정상"
const tu8 xca_return_wait[]          = "WAIT";          //+ Processing or Waiting   "대기"
const tu8 xca_return_err_null[]      = "ERR NULL";      //+ Null pointer            "포인터 0 오류"
const tu8 xca_return_err_len[]       = "ERR LEN";       //+ Len                     "길이 오류"
const tu8 xca_return_err_crc[]       = "ERR CRC";       //+ CRC Error               "CRC 오류"
const tu8 xca_return_err_value[]     = "ERR VALUE";     //+ Value Out of Range      "값 범위 오류"
const tu8 xca_return_err_frame[]     = "ERR FRAME";     //+ Frame Error             "프레임 오류"
const tu8 xca_return_err_timeout[]   = "ERR_TIMEOUT";   //+ Timeout                 "시간오버 오류"
const tu8 xca_return_err_setting[]   = "ERR_SETTING";   //+ Configuration Error     "설정값 오류"
const tu8 xca_return_err_busy[]      = "ERR_BUSY";      //+ Resource Busy           "바쁨 오류"
const tu8 xca_return_err_overflow[]  = "ERR_OVERFLOW";  //+ Buffer Overflow         "버퍼 오버플로우 오류"
const tu8 xca_return_err_underflow[] = "ERR_UNDERFLOW"; //+ Buffer Underflow        "버퍼 언더플로우 오류"
const tu8 xca_return_err_not_init[]  = "ERR_NOT_INIT";  //+ Uninitialized           "초기화되지 않음 오류"
const tu8 xca_return_err_hw[]        = "ERR_HW";        //+ Hardware Error          "하드웨어 오류"
const tu8 xca_return_err_arg[]       = "ERR_ARG";       //+ Invalid Argument        "잘못된 인자 오류"
const tu8 xca_return_err_lock[]      = "ERR_LOCK";      //+ Failed to Lock Resource "리소스 잠금 실패 오류"
const tu8 xca_return_err_unknown[]   = "ERR_UNKNOWN";   //+ Unknown Error           "알 수 없는 오류 오류"
