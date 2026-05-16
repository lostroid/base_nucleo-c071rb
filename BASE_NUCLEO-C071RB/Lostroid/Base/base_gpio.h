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

void f_base_gpio_init(void);
void f_base_gpio_mode_init(void);
void f_base_gpio_speed_init(void);
void f_base_gpio_pupd_init(void);
void f_base_gpio_af_init(void);

void f_base_gpio_led_ld1_on_off(te_gpio_output e_onoff);
void f_base_gpio_led_ld2_on_off(te_gpio_output e_onoff);

te_gpio_output f_base_gpio_sw_ok_in(void);
te_gpio_output f_base_gpio_sw_ok_a_in(void);
te_gpio_output f_base_gpio_sw_ok_b_in(void);
te_gpio_output f_base_gpio_mt_open_in(void);
te_gpio_output f_base_gpio_mt_close_in(void);
te_gpio_output f_base_gpio_charge_in(void);
te_gpio_output f_base_gpio_sw_power_in(void);

#endif
