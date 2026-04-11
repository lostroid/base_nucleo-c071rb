/******************************************************************************
* File:    base_typdef.h
* Author:  LOASTROID
* Created: 2025-06-02
*
* Description:
* This is the typdef Type.
*
* Revision History:
*   2025-06-02  MK  New project.
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
#ifndef H_BASE_TYPEDEF_H
#define H_BASE_TYPEDEF_H

typedef unsigned char               tu8;
typedef unsigned short              tu16;
typedef unsigned long               tu32;
typedef unsigned long long          tu64;

typedef signed char                 ts8;
typedef short                       ts16;
typedef long                        ts32;
typedef long long                   ts64;

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

typedef enum{
    m_IDLE = 0,
    m_DONE,
    m_BUSY
} te_State;

typedef enum{
    m_OK = 0,
    m_PASS = 0,
    m_FAIL
} te_Result;

typedef enum{
    m_OFF = 0,
    m_ON
} te_Enable;

typedef enum{
    m_NO = 0,
    m_YES
} te_YesNo;

typedef enum{
    m_RETURN_OK,
    m_RETURN_WAIT,
    m_RETURN_ERR_CRC,
    m_RETURN_ERR_FRAME,
    m_RETURN_ERR_TIMEOUT
}te_Return;

#define d_NULL  0u
#define d_ERROR 0xFFFFFFFF

#endif