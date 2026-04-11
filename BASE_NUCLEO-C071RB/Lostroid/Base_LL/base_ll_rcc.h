/********************************************************************
* File:    base_ll_rcc.h
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the RCC Control.
*
* Revision History:
*   2026-02-19  New.
********************************************************************/
#ifndef H_LL_RCC_H
#define H_LL_RCC_H

#include "base_ll_rcc_type.h"

void f_LL_RCC_Init(void);
void f_LL_RCC_Module(void);
void f_LL_RCC_Systick_Enable(void);
void f_LL_RCC_APB_Enable(void);
void f_LL_RCC_HSE_Enable(void);
void f_LL_RCC_HSE_Disable(void);
void f_LL_RCC_HSE_Switch_Select(void);
tu32 f_LL_RCC_HSE_Ready(void);
void f_LL_RCC_Clock_Enable(void);
void f_LL_RCC_Clock_Selection(void);
void f_LL_RCC_Sleep_Stip_Clock_Disable(void);

#endif
