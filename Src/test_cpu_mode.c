/*
 * test_cpu_mode.c
 *
 *  Created on: Aug 22, 2025
 *      Author: vbalaga
 */

#include <stdint.h>
#include <stdio.h>
#include "core_cm4.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Exploring Thread and Handler modes of the Cortex-M processor */

// Generate software interrupt for IRQ#3 i.e., RTC Wakeup
void generate_interrupt() {
    // Enable IRQ#3 in ISER register
    NVIC->ISER[0] |= (1 << 3);
    
    //Trigger interrupt via STIR, STIR is 9 bit wide i.e., bits[8:0]
    NVIC->STIR = (0x3 & 0x1FF);
}

// The CPU registers are not memory-mapped so need to access using inline assembly code
void change_access_level() {
	//read
	__asm volatile ("MRS R0,CONTROL");
	//modify
	__asm volatile ("ORR R0,R0,0x01");
	//write
	__asm volatile ("MSR CONTROL,R0");
}

int main(void)
{
	// By default CPU will be in thread mode + priviliged access
	printf("Hello World\n");

	// This will change the access level to unpriviliged
	change_access_level();

    // This function works in Thread mode
    generate_interrupt();
    printf("Bye World\n");
    /* Loop forever */
	for(;;);
}

// This function works in Handler mode
void RTC_WKUP_IRQHandler() {
	printf("Inside RTC IRQ Handler\n");
}

void HardFault_Handler() {
	printf("Inside Hard fault Handler\n");
}
