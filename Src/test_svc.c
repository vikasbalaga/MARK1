/*
 * test_svc.c
 *
 *  Created on: Sep 6, 2025
 *      Author: vbalaga
 */
#include <stdint.h>
#include <stdio.h>
#include "core_cm4.h"
#include "stm32f407xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Sample test for using SVC to perform basic math operations
   Use following SVC codes for each operation
   SVC #7  -> Add
   SVC #8  -> Sub
   SVC #9  -> Mul
   SVC #10 -> Div */

// Add operation
int32_t add_num(int32_t x, int32_t y) {
    // Trigger SVC exception
    __asm("SVC #7");

    //Capture result from R0
    int32_t res;
    __asm volatile("mov %0, r0" : "=r"(res));
    return res;
}

// Sub operation
int32_t sub_num(int32_t x, int32_t y) {
    // Trigger SVC exception
    __asm("SVC #8");

    //Capture result from R0
    int32_t res;
    __asm volatile("mov %0, r0" : "=r"(res));
    return res;
}

// Mul operation
int32_t mul_num(int32_t x, int32_t y) {
    // Trigger SVC exception
    __asm("SVC #9");

    //Capture result from R0
    int32_t res;
    __asm volatile("mov %0, r0" : "=r"(res));
    return res;
}

// Div operation
int32_t div_num(int32_t x, int32_t y) {
    // Trigger SVC exception
    __asm("SVC #10");

    //Capture result from R0
    int32_t res;
    __asm volatile("mov %0, r0" : "=r"(res));
    return res;
}

int main(void)
{
    int32_t x = 10, y = 20, res;

    // Add
    res = add_num(x, y);
    printf("result of add is %ld\n", res);

    // Sub
    res = sub_num(x, y);
    printf("result of sub is %ld\n", res);

    // Mul
    res = mul_num(x, y);
    printf("result of mul is %ld\n", res);

    // Div
    res = div_num(x, y);
    printf("result of div is %ld\n", res);
	for(;;);
}

/* Naked function to avoid prologue and epilogue code which might corrupt stack pointer */
__attribute__ ((naked)) void SVC_Handler(void) {
    __asm("MRS R0,MSP");
    __asm("B SVC_Compute");
}

void SVC_Compute(uint32_t* pstack) {
    uint8_t* return_addr = (uint8_t*)(pstack[6]);
    /* Decrement by 2 (thumb) to get to SVC instruction */
    return_addr -= 2;

    /* Extract SVC number which is the LSB portion */
    uint8_t svc_num = *return_addr;
    
    printf("SVC number is %d\n", svc_num);

    // Extract arguments from CPU registers
    int32_t arg0, arg1, res;
    arg0 = pstack[0]; //R0
    arg1 = pstack[1]; //R1
    
    switch (svc_num) {
        case 7:
            res = arg0 + arg1;
            break;
        case 8:
            res = arg0 - arg1;
            break;
        case 9:
            res = arg0 * arg1;
            break;
        case 10:
            res = arg0 / arg1;
            break;
        default:
            printf("Invalid SVC code \n");
    }
	// return result using R0
    pstack[0] = res;
}
