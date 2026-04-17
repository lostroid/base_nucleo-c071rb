/********************************************************************
* File:    base_rcc.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_RCC_H
#define H_BASE_RCC_H

#include "base_rcc_type.h"

void f_Base_RCC_Init(void);
void f_Base_RCC_Module(void);
void f_Base_RCC_APB_Enable(void);
void f_Base_RCC_HSE_Enable(void);
void f_Base_RCC_HSE_Disable(void);
void f_Base_RCC_HSE_Switch_Select(void);
tu32 f_Base_RCC_HSE_Ready(void);
void f_Base_RCC_Clock_Enable(void);
void f_Base_RCC_Clock_Selection(void);
void f_Base_RCC_Sleep_Stip_Clock_Disable(void);

#endif
