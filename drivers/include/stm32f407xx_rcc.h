/*
 * stm32f407xx_rcc.h
 *
 *  Created on: Oct 19, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_STM32F407XX_RCC_H_
#define INCLUDE_STM32F407XX_RCC_H_

#include "stm32f407xx.h"

#define RCC_CLK_SRC_Msk             0x3

/* RCC Driver APIs */

/**
 * @brief  Get clock source freq
 *
 * @return sys_clk - system clock frequency in Hz
 *
 */
uint32_t RCC_Get_Sysclk(void);

/**
 * @brief  Get AHB1 Clock value
 *
 * @return hclk1 - AHB1 Clock value in Hz
 *
 */
uint32_t RCC_GetHCLK1(void);

/**
 * @brief  Get APB1 Clock value
 *
 * @return pclk1 - APB1 Clock value in Hz
 *
 */
uint32_t RCC_GetPCLK1(void);

/**
 * @brief  Get PLL Clock value
 *
 * @return pllclk - PLL Clock value in Hz
 *
 */
uint32_t RCC_GetPLLClk(void);

#endif /* INCLUDE_STM32F407XX_RCC_H_ */
