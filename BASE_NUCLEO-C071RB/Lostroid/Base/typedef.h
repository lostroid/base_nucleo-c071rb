/********************************************************************
* File:    base_typdef.h
* Author:  LOASTROID
* Created: 2025-06-02
* Encoding: UTF-8
---------------------------------------------------------------------
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
********************************************************************/
#ifndef H_BASE_TYPEDEF_H
#define H_BASE_TYPEDEF_H

#define D_ILP32_D       /// 32bit MCU
//#define D_LP64_D        /// 64bit MCU

#ifdef D_ILP32_D
    typedef unsigned char           tu8;
    typedef unsigned short          tu16;
    typedef unsigned long           tu32;
    typedef unsigned long long      tu64;

    typedef signed char             ts8;
    typedef short                   ts16;
    typedef long                    ts32;
    typedef long long               ts64;
#endif
#ifdef D_LP64_D
    typedef unsigned char           tu8;
    typedef unsigned short          tu16;
    typedef unsigned int            tu32;
    typedef unsigned long long      tu64;

    typedef signed char             ts8;
    typedef short                   ts16;
    typedef int                     ts32;
    typedef long long               ts64;
#endif

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
    m_RETURN_OK,            //+ Success: 성공
    m_RETURN_WAIT,          //+ Processing or Waiting: 처리 대기
    m_RETURN_ERR_CRC,       //+ CRC Error: CRC 오류
    m_RETURN_ERR_VALUE,     //+ Value Out of Range: 값 범위 오류
    m_RETURN_ERR_FRAME,     //+ Frame Error: 프레임 오류
    m_RETURN_ERR_TIMEOUT,   //+ Timeout: 시간 초과
    m_RETURN_ERR_SETTING,   //+ Configuration Error: 설정 오류
    m_RETURN_ERR_BUSY,      //+ Resource Busy: 자원 사용 중
    m_RETURN_ERR_OVERFLOW,  //+ Buffer Overflow: 버퍼 오버플로우
    m_RETURN_ERR_UNDERFLOW, //+ Buffer Underflow: 버퍼 언더플로우
    m_RETURN_ERR_NOT_INIT,  //+ Uninitialized: 초기화되지 않음
    m_RETURN_ERR_HW,        //+ Hardware Error: 하드웨어 오류
    m_RETURN_ERR_ARG,       //+ Invalid Argument: 잘못된 인자
    m_RETURN_ERR_LOCK,      //+ Failed to Lock Resource: 리소스 잠금 실패
    m_RETURN_ERR_UNKNOWN    //+ Unknown Error: 알 수 없는 오류
} te_Return;

#endif