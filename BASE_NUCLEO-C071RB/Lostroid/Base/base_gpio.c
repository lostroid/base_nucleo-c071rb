/********************************************************************
* File:    base_flash.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "base_gpio.h"
//===================================================================
/* Base GPIO initialize
-------------------------------------------------------------------*/
void f_base_gpio_init(void)
{
    f_base_gpio_mode_init();
    f_base_gpio_speed_initialize();
    f_base_gpio_speed_initialize();
    f_base_gpio_af_initialize();
}
//===================================================================
/* Base GPIO MODE initialize (Input,Output,AF,Analog)
-------------------------------------------------------------------*/
void f_base_gpio_mode_init(void)
{
	GPIOA->MODER = ( d_GPIO_A00_MODE_ADC_IN0
                   | d_GPIO_A01_MODE_ADC_IN1
                   | d_GPIO_A02_MODE_UART2_TX
                   | d_GPIO_A03_MODE_UART2_RX
                   | d_GPIO_A04_MODE_DC5V_IN4
                   | d_GPIO_A05_MODE_LED_LD1
                   | d_GPIO_A06_MODE_SPI1_MISO
                   | d_GPIO_A07_MODE_SPI1_MOSI
                   | d_GPIO_A08_MODE_LCD_BL
                   | d_GPIO_A09_MODE_LCD_DC
                   | d_GPIO_A10_MODE_DISABLE
                   | d_GPIO_A11_MODE_SW_A
                   | d_GPIO_A12_MODE_SW_B
                   | d_GPIO_A13_MODE_DBG_SWDIO
                   | d_GPIO_A14_MODE_DBG_SWCLK
                   | d_GPIO_A15_MODE_SPI_CS );

    GPIOB->MODER = ( d_GPIO_B00_MODE_BATTERY_TEMP_IN17
                   | d_GPIO_B01_MODE_MOTOR_IN18
                   | d_GPIO_B02_MODE_DISABLE
                   | d_GPIO_B03_MODE_LCD_RES
                   | d_GPIO_B04_MODE_MT_SENSOR_OPEN
                   | d_GPIO_B05_MODE_MT_SENSOR_CLOSE
                   | d_GPIO_B06_MODE_SPI1_SCK
                   | d_GPIO_B07_MODE_STAT_IN
                   | d_GPIO_B08_MODE_SW_POWER
                   | d_GPIO_B09_MODE_DISABLE
                   | d_GPIO_B10_MODE_DISABLE
                   | d_GPIO_B11_MODE_DISABLE
                   | d_GPIO_B12_MODE_DISABLE
                   | d_GPIO_B13_MODE_DISABLE
                   | d_GPIO_B14_MODE_DISABLE
                   | d_GPIO_B15_MODE_DISABLE );

    GPIOC->MODER = ( d_GPIO_C00_MODE_DISABLE
                   | d_GPIO_C01_MODE_DISABLE
                   | d_GPIO_C02_MODE_DISABLE
                   | d_GPIO_C03_MODE_DISABLE
                   | d_GPIO_C04_MODE_DISABLE
                   | d_GPIO_C05_MODE_DISABLE
                   | d_GPIO_C06_MODE_SW_OK
                   | d_GPIO_C07_MODE_DISABLE
                   | d_GPIO_C08_MODE_DISABLE
                   | d_GPIO_C09_MODE_LED_LD2
                   | d_GPIO_C10_MODE_DISABLE
                   | d_GPIO_C11_MODE_DISABLE
                   | d_GPIO_C12_MODE_DISABLE
                   | d_GPIO_C13_MODE_DISABLE
                   | d_GPIO_C14_MODE_OSCX_IN
                   | d_GPIO_C15_MODE_OSCX_OUT );
}
//===================================================================
/* Base GPIO SPEED initialize (Very-Low, Low, High, Very-High)
-------------------------------------------------------------------*/
void f_base_gpio_speed_initialize(void)
{
    GPIOA->OSPEEDR = d_GPIO_SPEED_06(d_GPIO_SPEED_VERY_HIGH)    //+ SPI MISO
                   | d_GPIO_SPEED_07(d_GPIO_SPEED_VERY_HIGH)    //+ SPI MOSI
                   | d_GPIO_SPEED_09(d_GPIO_SPEED_VERY_HIGH)    //+ LCD DC
                   | d_GPIO_SPEED_13(d_GPIO_SPEED_VERY_HIGH)    //+ Reset value
                   | d_GPIO_SPEED_15(d_GPIO_SPEED_VERY_HIGH);   //+ SPI CS

    GPIOB->OSPEEDR = d_GPIO_SPEED_06(d_GPIO_SPEED_VERY_HIGH);   //+ SPI SCK;

    GPIOC->OSPEEDR = 0;
}
//===================================================================
/* Base GPIO Pull-up, pull-down initialize
-------------------------------------------------------------------*/
void f_base_gpio_pupd_initialize(void)
{
    GPIOA->PUPDR = d_GPIO_PUPD_14(d_GPIO_PUPD_DOWN)
                 | d_GPIO_PUPD_13(d_GPIO_PUPD_UP);    //+ Reset value
    GPIOB->PUPDR = 0;
    GPIOC->PUPDR = 0;
}
//===================================================================
/* Base GPIO AF initialize
-------------------------------------------------------------------*/
void f_base_gpio_af_initialize(void)
{
    GPIOA->AFR[0] = d_GPIO_02_AFSEL(1)      //+ USART2 TX 
                  | d_GPIO_03_AFSEL(1)      //+ USART2 RX
                  | d_GPIO_06_AFSEL(0)      //+ SPI1_MISO
                  | d_GPIO_07_AFSEL(0);     //+ SPI1_MOSI

    GPIOA->AFR[1] = d_GPIO_15_AFSEL(2);     //+ TIM1_CH1
    GPIOB->AFR[0] = d_GPIO_03_AFSEL(1)      //+ TIM1_CH2
                  | d_GPIO_06_AFSEL(10);    //+ SPI1_CLK
}