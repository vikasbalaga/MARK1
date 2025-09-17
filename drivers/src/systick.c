/*
 * systick.c
 *
 *  Created on: Sep 13, 2025
 *      Author: vbalaga
 */
#include "systick.h"


/* SysTick Driver APIs */

/**
 * @brief  SysTick initialization API
 *
 * @param  tick_freq - freq at which SysTick exception will trigger
 *
 */
void systick_init(uint32_t tick_freq) {
    /* Load the RVR with actual count - 1*/
    uint32_t reload_value = (SYSTICK_CLK / tick_freq) - 1;
    SYSTICK->RVR &= ~(0x00FFFFFF); //Clearing
    SYSTICK->RVR |= reload_value;  //Setting

    /* Configure clock source */
    SYSTICK->CSR |= (1 << 2);

    /* Enable exception request */
    SYSTICK->CSR |= (1 << 1);

    /* Enable the counter */
    SYSTICK->CSR |= (1 << 0);
}

/**
 * @brief  SysTick enable API
 *
 * @param  enable - flag to enable or disable SysTick timer
 *
 */
void systick_enable(bool enable) {
    if (enable == SYSTICK_TIMER_ENABLE)
        SYSTICK->CSR |= (1 << 0);
    else if (enable == SYSTICK_TIMER_DISABLE)
        SYSTICK->CSR |= (0 << 0);
}