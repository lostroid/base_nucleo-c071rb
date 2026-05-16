/********************************************************************
* File:    base_tim_type.h
* Author:  Lostroid
* Created: 2026-02-19
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
#ifndef H_BASE_TIM_TYPE_H
#define H_BASE_TIM_TYPE_H

#include "types.h"

#define d_BASE_TIM_TI_00   0
#define d_BASE_TIM_TI_01   1
#define d_BASE_TIM_TI_02   2
#define d_BASE_TIM_TI_03   3
#define d_BASE_TIM_TI_04   4
#define d_BASE_TIM_TI_05   5
#define d_BASE_TIM_TI_06   6
#define d_BASE_TIM_TI_07   7
#define d_BASE_TIM_TI_08   8
#define d_BASE_TIM_TI_09   9
#define d_BASE_TIM_TI_10   10
#define d_BASE_TIM_TI_11   11
#define d_BASE_TIM_TI_12   12
#define d_BASE_TIM_TI_13   13
#define d_BASE_TIM_TI_14   14
#define d_BASE_TIM_TI_15   15

#define d_BASE_TIM_TI1_SEL(SEL)   (SEL)
#define d_BASE_TIM_TI2_SEL(SEL)   (SEL << 8)
#define d_BASE_TIM_TI3_SEL(SEL)   (SEL << 16)
#define d_BASE_TIM_TI4_SEL(SEL)   (SEL << 24)

#define d_BASE_TIM_CC1S(SEL)   (SEL)
#define d_BASE_TIM_CC2S(SEL)   (SEL << 8)
#define d_BASE_TIM_CC3S(SEL)   (SEL)
#define d_BASE_TIM_CC4S(SEL)   (SEL << 8)

#define d_BASE_TIM_IC1F(SEL)   (SEL << 4)
#define d_BASE_TIM_IC2F(SEL)   (SEL << 12)
#define d_BASE_TIM_IC3F(SEL)   (SEL << 4)
#define d_BASE_TIM_IC4F(SEL)   (SEL << 12)

typedef enum
{
    m_BASE_TIM_MODE_PWM,
    m_BASE_TIM_MODE_H,
    m_BASE_TIM_MODE_L

}te_Base_tim_mode;


#endif
