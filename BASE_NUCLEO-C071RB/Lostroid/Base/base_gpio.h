/********************************************************************
* File:    base_gpio.h
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/
#ifndef H_BASE_GPIO_H
#define H_BASE_GPIO_H

#include "stm32c071xx.h"
#include "base_gpio_type.h"

#define d_BASE_GPIO_LED_LD1_H       (GPIOA->BSRR = GPIO_BSRR_BS5)
#define d_BASE_GPIO_LED_LD1_L       (GPIOA->BSRR = GPIO_BSRR_BR5)

#define d_BASE_GPIO_LED_LD2_H       (GPIOC->BSRR = GPIO_BSRR_BS9)
#define d_BASE_GPIO_LED_LD2_L       (GPIOC->BSRR = GPIO_BSRR_BR9)

#define d_BASE_GPIO_LCD_RESET_H     (GPIOB->BSRR = GPIO_BSRR_BS3)
#define d_BASE_GPIO_LCD_RESET_L     (GPIOB->BSRR = GPIO_BSRR_BR3)
#define d_BASE_GPIO_LCD_DC_H        (GPIOA->BSRR = GPIO_BSRR_BS9)
#define d_BASE_GPIO_LCD_DC_L        (GPIOA->BSRR = GPIO_BSRR_BR9)
#define d_BASE_GPIO_LCD_BL_H        (GPIOA->BSRR = GPIO_BSRR_BS8)
#define d_BASE_GPIO_LCD_BL_L        (GPIOA->BSRR = GPIO_BSRR_BR8)
#define d_BASE_GPIO_SPI1_CS_H       (GPIOA->BSRR = GPIO_BSRR_BS15)
#define d_BASE_GPIO_SPI1_CS_L       (GPIOA->BSRR = GPIO_BSRR_BR15)

void f_Base_GPIO_Init(void);
void f_Base_GPIO_MODE_Initialize(void);
void f_Base_GPIO_SPEED_Initialize(void);
void f_Base_GPIO_PUPD_Initialize(void);
void f_Base_GPIO_AF_Initialize(void);

void f_LL_GPIO_LED_LD1_OnOff(te_GPIO_Output e_onoff);
void f_LL_GPIO_LED_LD2_OnOff(te_GPIO_Output e_onoff);

te_GPIO_Output f_Base_GPIO_SW_OK_In(void);
te_GPIO_Output f_Base_GPIO_SW_OK_A_In(void);
te_GPIO_Output f_Base_GPIO_SW_OK_B_In(void);
te_GPIO_Output f_Base_GPIO_MT_Open_In(void);
te_GPIO_Output f_Base_GPIO_MT_Close_In(void);
te_GPIO_Output f_Base_GPIO_Charge_In(void);
te_GPIO_Output f_Base_GPIO_SW_Power_In(void);

#endif
