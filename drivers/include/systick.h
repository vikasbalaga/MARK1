/*
 * systick.h
 *
 *  Created on: Sep 13, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_SYSTICK_H_
#define INCLUDE_SYSTICK_H_

#include "core_cm4.h"

/* By default SysTick runs with HSI clock */
#define SYSTICK_CLK                 HSI_CLK

#define SYSTICK_TIMER_ENABLE        1
#define SYSTICK_TIMER_DISABLE       0

/* SysTick Driver APIs */

/**
 * @brief  SysTick initialization API
 *
 * @param  tick_freq - freq at which SysTick exception will trigger
 *
 */
void systick_init(uint32_t tick_freq);

/**
 * @brief  SysTick enable API
 *
 * @param  enable - flag to enable or disable SysTick timer
 *
 */
void systick_enable(bool enable);

#endif /* INCLUDE_SYSTICK_H_ */
