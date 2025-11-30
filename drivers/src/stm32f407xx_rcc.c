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
uint8_t PCLK_PreScaler[] = {1, 2, 4, 8, 16};

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
 * @return hclk - AHB1 Clock value in Hz
 *
 */
uint32_t RCC_GetHCLK(void) {
    uint32_t temp, sys_clk, hclk;
    uint16_t hclk_presc;
    sys_clk = RCC_Get_Sysclk();
    
    //Get AHB1 prescaler value
    temp = ((RCC->CFGR >> 4) & 0xF);
    if (temp <= 7)
        temp = 0;
    else
        temp -= 7;
    hclk_presc = HCLK_PreScaler[temp];
    hclk = sys_clk / hclk_presc;
    return hclk;
}

/**
 * @brief  Get APB1 Clock value
 *
 * @return pclk1 - APB1 Clock value in Hz
 *
 */
uint32_t RCC_GetPCLK1(void) {
    uint32_t temp, hclk, pclk1;
    uint8_t pclk1_presc;
    hclk = RCC_GetHCLK();
    
    //Get APB1 prescaler value
    temp = ((RCC->CFGR >> 10) & 0x7);
    if (temp <= 3)
        temp = 0;
    else
        temp -= 3;
    pclk1_presc = PCLK_PreScaler[temp];
    pclk1 = hclk / pclk1_presc;
    return pclk1;
}

/**
 * @brief  Get APB2 Clock value
 *
 * @return pclk1 - APB2 Clock value in Hz
 *
 */
uint32_t RCC_GetPCLK2(void) {
    uint32_t temp, hclk, pclk2;
    uint8_t pclk2_presc;
    hclk = RCC_GetHCLK();
    
    //Get APB2 prescaler value
    temp = ((RCC->CFGR >> 10) & 0x7);
    if (temp <= 3)
        temp = 0;
    else
        temp -= 3;
    pclk2_presc = PCLK_PreScaler[temp];
    pclk2 = hclk / pclk2_presc;
    return pclk2;
}
