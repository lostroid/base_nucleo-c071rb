/********************************************************************
* File:    types.h
* Author:  LOASTROID
* Created: 2025-12-04
* Encoding: UTF-8
---------------------------------------------------------------------
01. a_ = array                         e.g. a_data[]
02. b_ = bit field                     e.g. b_Data
03. c_ = constant                      e.g, c_Data
04. d_ = #define macro                 e.g. d_Data
05. e_ = enum type                     e.g. e_Data
06. f_ = function                      e.g. f_Data
07. g_ = global (source file static)   e.g. gv_Data
08. i_ = inline function               e.g. i_Data
09. l_ = local static variable         e.g. lv_Data
10. m_ = enum member                   e.g. m_Data
11. p_ = pointer                       e.g. p_Data 
12. s_ = struct                        e.g. s_Data 
13. t_ = typedef                       e.g. t_Data 
14. u_ = union                         e.g. u_Data 
15. v_ = variable                      e.g. v_Data
16. x_ = extern variable               e.g. xv_data
********************************************************************/
#ifndef H_TYPES_H
#define H_TYPES_H

typedef unsigned char           tu8;
typedef unsigned short          tu16;
typedef unsigned long           tu32;
typedef unsigned long long      tu64;

typedef signed char             ts8;
typedef signed short            ts16;
typedef signed long             ts32;
typedef signed long long        ts64;

#define d_IDLE  0u
#define d_BUSY  1u
#define d_DONE  2u

#define d_OK    0u
#define d_PASS  0u
#define d_FAIL  1u

#define d_OFF   0u
#define d_ON    1u

#define d_NO    0u
#define d_YES   1u

#define d_NULL  0u
#define d_ERROR 0xFFFFFFFF

extern const tu8 xca_return_ok[];
extern const tu8 xca_return_wait[];
extern const tu8 xca_return_err_null[];
extern const tu8 xca_return_err_len[];
extern const tu8 xca_return_err_crc[];
extern const tu8 xca_return_err_value[];
extern const tu8 xca_return_err_frame[];
extern const tu8 xca_return_err_timeout[];
extern const tu8 xca_return_err_setting[];
extern const tu8 xca_return_err_busy[];
extern const tu8 xca_return_err_overflow[];
extern const tu8 xca_return_err_underflow[];
extern const tu8 xca_return_err_not_init[];
extern const tu8 xca_return_err_hw[];
extern const tu8 xca_return_err_arg[];
extern const tu8 xca_return_err_lock[];
extern const tu8 xca_return_err_unknown[];

typedef enum {
    m_STATE_IDLE = 0,       //+ Idle: 대기 상태
    m_STATE_DONE,           //+ Done: 완료 상태
    m_STATE_BUSY            //+ Busy: 작업 중
} te_State;

typedef enum {
    m_RESULT_OK = 0,        //+ OK: 정상
    m_RESULT_PASS = 0,      //+ Pass: 통과
    m_RESULT_FAIL           //+ Fail: 실패
} te_Result;

typedef enum {
    m_ENABLE_OFF = 0,       //+ Off: 꺼짐
    m_ENABLE_ON             //+ On: 켜짐
} te_Enable;

typedef enum {
    m_YESNO_NO = 0,         //+ No: 아니오
    m_YESNO_YES             //+ Yes: 예
} te_YesNo;

typedef enum {
    m_RETURN_OK,            //+ OK                      "정상"
    m_RETURN_WAIT,          //+ Processing or Waiting   "대기"
    m_RETURN_ERR_NULL,      //+ Null pointer            "포인터 0 오류"
    m_RETURN_ERR_LEN,       //+ Len                     "길이 오류"
    m_RETURN_ERR_CRC,       //+ CRC Error               "CRC 오류"
    m_RETURN_ERR_VALUE,     //+ Value Out of Range      "값 범위 오류"
    m_RETURN_ERR_FRAME,     //+ Frame Error             "프레임 오류"
    m_RETURN_ERR_TIMEOUT,   //+ Timeout                 "시간오버 오류"
    m_RETURN_ERR_SETTING,   //+ Configuration Error     "설정값 오류"
    m_RETURN_ERR_BUSY,      //+ Resource Busy           "바쁨 오류"
    m_RETURN_ERR_OVERFLOW,  //+ Buffer Overflow         "버퍼 오버플로우 오류"
    m_RETURN_ERR_UNDERFLOW, //+ Buffer Underflow        "버퍼 언더플로우 오류"
    m_RETURN_ERR_NOT_INIT,  //+ Uninitialized           "초기화되지 않음 오류"
    m_RETURN_ERR_HW,        //+ Hardware Error          "하드웨어 오류"
    m_RETURN_ERR_ARG,       //+ Invalid Argument        "잘못된 인자 오류"
    m_RETURN_ERR_LOCK,      //+ Failed to Lock Resource "리소스 잠금 실패 오류"
    m_RETURN_ERR_UNKNOWN    //+ Unknown Error           "알 수 없는 오류 오류"
} te_Return;

#endif