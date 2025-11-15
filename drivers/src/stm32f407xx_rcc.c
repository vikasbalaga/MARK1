/*
 * stm32f407xx_rcc.c
 *
 *  Created on: Oct 19, 2025
 *      Author: vbalaga
 */
#include "stm32f407xx_rcc.h"

//AHB clock prescaler values
uint16_t HCLK_PreScaler[] = {1, 2, 4, 8, 16, 64, 128, 256, 512};

//APB1 clock prescaler values
uint8_t PCLK1_PreScaler[] = {1, 2, 4, 8, 16};

/**
 * @brief  Get clock source freq
 *
 * @return sys_clk - system clock frequency in Hz
 *
 */
uint32_t RCC_Get_Sysclk(void) {
    uint32_t sys_clk = 0;
    uint8_t clk_src = 0;
    clk_src = ((RCC->CFGR >> 2) & RCC_CLK_SRC_Msk);
    if (clk_src == 0)
        sys_clk = HSI_CLK;
    else if (clk_src == 1)
        sys_clk = HSE_CLK;
    else if (clk_src == 2)
        sys_clk = RCC_GetPLLClk();
    return sys_clk;
}

/**
 * @brief  Get PLL Clock value
 *
 * @return pllclk - PLL Clock value in Hz
 *
 */
uint32_t RCC_GetPLLClk(void) {
    uint32_t temp = 0;
    return temp;
}



/**
 * @brief  Get AHB1 Clock value
 *
 * @return hclk1 - AHB1 Clock value in Hz
 *
 */
uint32_t RCC_GetHCLK1(void) {
    uint32_t temp, sys_clk, hclk1;
    uint16_t hclk_presc;
    sys_clk = RCC_Get_Sysclk();
    
    //Get AHB1 prescaler value
    temp = ((RCC->CFGR >> 4) & 0xF);
    if (temp <= 7)
        temp = 0;
    else
        temp -= 7;
    hclk_presc = HCLK_PreScaler[temp];
    hclk1 = sys_clk / hclk_presc;
    return hclk1;
}

/**
 * @brief  Get APB1 Clock value
 *
 * @return pclk1 - APB1 Clock value in Hz
 *
 */
uint32_t RCC_GetPCLK1(void) {
    uint32_t temp, hclk1, pclk1;
    uint8_t pclk_presc;
    hclk1 = RCC_GetHCLK1();
    
    //Get APB1 prescaler value
    temp = ((RCC->CFGR >> 10) & 0x3);
    if (temp <= 3)
        temp = 0;
    else
        temp -= 3;
    pclk_presc = PCLK1_PreScaler[temp];
    pclk1 = hclk1 / pclk_presc;
    return pclk1;
}

