/********************************************************************
* File:    base_ll_gpio.h
* Author:  Lostroid
* Created: 2026-02-19
*
* Description:
* This is the LL GPIO Control.
*
* Revision History:
*   2026-02-19  New.
********************************************************************/
#ifndef H_LL_GPIO_H
#define H_LL_GPIO_H

#include "base_ll_gpio_type.h"

void f_LL_GPIO_Init(void);
void f_LL_GPIO_MODE_Initialize(void);
void f_LL_GPIO_SPEED_Initialize(void);
void f_LL_GPIO_PUPD_Initialize(void);
void f_LL_GPIO_AF_Initialize(void);

void f_LL_GPIO_LED_LD1_OnOff(te_GPIO_Output e_onoff);
void f_LL_GPIO_LED_LD2_OnOff(te_GPIO_Output e_onoff);
void f_LL_GPIO_Motor_Power_OnOff(te_GPIO_Output e_onoff);
void f_LL_GPIO_Motor_Open_OnOff(te_GPIO_Output e_onoff);
void f_LL_GPIO_Motor_Close_OnOff(te_GPIO_Output e_onoff);
void f_LL_GPIO_Main_Power_OnOff(te_GPIO_Output e_onoff);

te_GPIO_Output f_LL_GPIO_SW_OK_In(void);
te_GPIO_Output f_LL_GPIO_SW_OK_A_In(void);
te_GPIO_Output f_LL_GPIO_SW_OK_B_In(void);
te_GPIO_Output f_LL_GPIO_MT_Open_In(void);
te_GPIO_Output f_LL_GPIO_MT_Close_In(void);
te_GPIO_Output f_LL_GPIO_Charge_In(void);
te_GPIO_Output f_LL_GPIO_SW_Power_In(void);

#endif
