/********************************************************************
* File:    base_flash.c
* Author:  Lostroid
* Created: 2026-02-19
* Encoding: UTF-8
********************************************************************/

#include "stm32c071xx.h"
#include "base_flash.h"

//===================================================================
/* Base LL flash initialize
-------------------------------------------------------------------*/
void f_Base_Flash_Init(void)
{
    
}
//===================================================================
/* Base LL flash Latency initialize
-------------------------------------------------------------------*/
void f_Base_Flash_Set_Latency01(void)
{
    FLASH->ACR &= (~FLASH_ACR_LATENCY);     //+ Clean
    FLASH->ACR |= (FLASH_ACR_LATENCY_0);    //+ ≤ 48 001 (2 HCLK cycles)
}