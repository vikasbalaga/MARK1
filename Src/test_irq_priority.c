/*
 * test_irq_priority.c
 *
 *  Created on: Aug 29, 2025
 *      Author: vbalaga
 */
#include <stdint.h>
#include <stdio.h>
#include "core_cm4.h"
#include "stm32f407xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Testing the IRQ priorities and IRQ handling using TIMER2 and I2C1 event interrupts in the software using NVIC pending register */

#define TIMER2_IRQ_NUM 28
#define I2C1_EVENT_IRQ_NUM 31

void enable_irq_lines() {
    /* Set the priority for interrupts */
    NVIC->IPR[TIMER2_IRQ_NUM] = (NVIC_IRQ_PRIORITY_9 << (8 - NO_PRIORITY_BITS_IMPLEMENTED));
    NVIC->IPR[I2C1_EVENT_IRQ_NUM] = (NVIC_IRQ_PRIORITY_9 << (8 - NO_PRIORITY_BITS_IMPLEMENTED));

    /* Set the pending bit for I2C1 */
    uint8_t i2c_reg_idx = I2C1_EVENT_IRQ_NUM / 32;
    uint32_t i2c_bit_pos = I2C1_EVENT_IRQ_NUM % 32;
    
    NVIC->ISPR[i2c_reg_idx] |= (1 << i2c_bit_pos);

    /* Enable TIMER2 interrupt */
    uint8_t reg_idx = TIMER2_IRQ_NUM / 32;
    uint32_t bit_pos = TIMER2_IRQ_NUM % 32;
    NVIC->ISER[reg_idx] |= (1 << bit_pos);

    /* Enable I2C1 interrupt */
    NVIC->ISER[i2c_reg_idx] |= (1 << i2c_bit_pos);
}


int main(void)
{
    /* Loop forever */
    enable_irq_lines();
	for(;;);
}

void TIM2_IRQHandler() {
    printf("Inside TIMER2 IRQ handler\r\n");
    printf("Exiting TIMER2 IRQ handler\r\n");
}

void I2C1_EV_IRQHandler() {
    printf("Inside I2C1 IRQ handler\r\n");
    /* Set the pending bit for TIMER2 */
    uint8_t reg_idx = TIMER2_IRQ_NUM / 32;
    uint32_t bit_pos = TIMER2_IRQ_NUM % 32;

    NVIC->ISPR[reg_idx] |= (1 << bit_pos);
    printf("Exiting I2C1 IRQ handler\r\n");
}
