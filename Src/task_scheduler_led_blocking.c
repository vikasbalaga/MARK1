/*
 * task_scheduler_led.c
 *
 *  Created on: Sep 17, 2025
 *      Author: vbalaga
 */
#include <stdint.h>
#include <stdio.h>
#include "stm32f407xx.h"
#include "systick.h"
#include "stm32f407xx_gpio.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Define size of stack allocated for each task and task scheduler (1 KB) starting from end of SRAM */
#define TASK_STACK_SIZE (1 * 1024)

#define TASK1_STACK_START   SRAM_END
#define TASK2_STACK_START   (SRAM_END - (1 * TASK_STACK_SIZE))
#define TASK3_STACK_START   (SRAM_END - (2 * TASK_STACK_SIZE))
#define TASK4_STACK_START   (SRAM_END - (3 * TASK_STACK_SIZE))

#define SCHEDULER_STACK_START   (SRAM_END - (4 * TASK_STACK_SIZE))

#define MAX_TASKS 4
#define TOTAL_GEN_REGISTERS 13

void task1_handler();
void task2_handler();
void task3_handler();
void task4_handler();

void scheduler_stack_init(uint32_t* scheduler_sp);

void task_stack_init();

void enable_processor_faults();

void switch_sp_to_psp();

uint32_t task_sp_pointers[MAX_TASKS] = {TASK1_STACK_START, TASK2_STACK_START, TASK3_STACK_START, TASK4_STACK_START};

void (*task_fn_pointers[MAX_TASKS])(void) = {task1_handler, task2_handler, task3_handler, task4_handler};

uint32_t current_task_id = 0;

/* SysTick should trigger exception for every 1 ms i.e., tick rate is 1 kHz */
#define SYSTICK_TICK_RATE       1000 /* 1 kHz*/

uint32_t get_psp_of_task(void) {
    return task_sp_pointers[current_task_id];
}

void save_psp_of_task(uint32_t psp_value) {
    task_sp_pointers[current_task_id] = psp_value;
}

void update_task_id() {
    current_task_id++;
    current_task_id %= MAX_TASKS;
}

/* Use task scheduling to toggle LEDs */

/* Map led names to their GPIO IDs */
#define LED_GREEN   GPIO_PIN_NO_12
#define LED_ORANGE  GPIO_PIN_NO_13
#define LED_RED     GPIO_PIN_NO_14
#define LED_BLUE    GPIO_PIN_NO_15

void led_gpio_init() {
	GPIO_Handle_t led_gpio;
	led_gpio.pGPIOx = GPIOD;
	led_gpio.GPIO_PinConfig.GPIO_PinNumber = LED_GREEN;
	led_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	led_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUTPUT_SPEED_VERY_HIGH;
	led_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_MODE_PUPL;
	GPIO_Clock_Ctrl(GPIOD, 1);
	GPIO_Init(&led_gpio);
	
	led_gpio.GPIO_PinConfig.GPIO_PinNumber = LED_ORANGE;
	GPIO_Init(&led_gpio);

	led_gpio.GPIO_PinConfig.GPIO_PinNumber = LED_RED;
	GPIO_Init(&led_gpio);

	led_gpio.GPIO_PinConfig.GPIO_PinNumber = LED_BLUE;
	GPIO_Init(&led_gpio);
}

/* Create macros to induce software delays */
#define DELAY_1MS       1250U
#define DELAY_1S        (1000U * DELAY_1MS)
#define DELAY_500MS     (500U  * DELAY_1MS)
#define DELAY_250MS     (250U  * DELAY_1MS)
#define DELAY_125MS     (125U  * DELAY_1MS)

void delay(uint32_t delay_count) {
    for (uint32_t i = 0; i < delay_count; i++);
}

int main(void) {
    enable_processor_faults();

    /* Initialize scheduler stack */
	scheduler_stack_init((uint32_t*)SCHEDULER_STACK_START);

    /* Initialize task stack */
    task_stack_init();

    /* switch SP to PSP */
    switch_sp_to_psp();
    
    /* Initialize LED GPIOs */
    led_gpio_init();

    systick_init(SYSTICK_TICK_RATE);

    /* Launch task1 */
    task1_handler();

    /* Loop forever */
	for(;;);
}

__attribute((naked)) void scheduler_stack_init(uint32_t* scheduler_sp) {
    /* Load MSP with scheduler stack pointer, which will be present in R0 */
    __asm volatile ("MSR MSP,%0\n": : "r" (scheduler_sp) : );
    /* Go back to main function using LR */
    __asm volatile ("BX LR");
}

__attribute((naked)) void switch_sp_to_psp() {
    /* Fetch the PSP of current task but before that make sure to preserve LR value */
    __asm volatile ("PUSH {LR}");
    __asm volatile ("BL get_psp_of_task");
    __asm volatile ("MSR PSP,R0");
    /* Pop the LR back */
    __asm volatile ("POP {LR}");

    /* Change SP to PSP using control register */
    __asm volatile ("MOV R0,#0x02");
    __asm volatile ("MSR CONTROL,R0");

    /* Go back to caller function */
    __asm volatile ("BX LR");
}

/* Initialize the stack for each task in following order :
    xPSR    = 0x00100000
    PC      = task_function_address
    LR      = 0xFFFFFFFD
    R0-R12  = 0 */
void task_stack_init() {
    uint32_t* pPSP;
    for (int i = 0; i < sizeof(task_sp_pointers)/sizeof(task_sp_pointers[0]); i++) {
        pPSP = (uint32_t*)task_sp_pointers[i];
        // xPSR
        pPSP--;
        *pPSP = 0x01000000;
        // PC
        pPSP--;
        *pPSP = (uint32_t)task_fn_pointers[i];
        // LR
        pPSP--;
        *pPSP = 0xFFFFFFFD;
        for (int j = 0; j < TOTAL_GEN_REGISTERS; j++) {
            pPSP--;
            *pPSP = 0;
        }
        /* Update the satck pointers accordingly */
        task_sp_pointers[i] = (uint32_t)pPSP;
    }
}

void task1_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_GREEN);
        delay(DELAY_1S);
        GPIO_ToggleOutputPin(GPIOD, LED_GREEN);
        delay(DELAY_1S);
    }
}

void task2_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_ORANGE);
        delay(DELAY_500MS);
        GPIO_ToggleOutputPin(GPIOD, LED_ORANGE);
        delay(DELAY_500MS);
    }
}

void task3_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_RED);
        delay(DELAY_250MS);
        GPIO_ToggleOutputPin(GPIOD, LED_RED);
        delay(DELAY_250MS);
    }
}

void task4_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_BLUE);
        delay(DELAY_125MS);
        GPIO_ToggleOutputPin(GPIOD, LED_BLUE);
        delay(DELAY_125MS);
    }
}

__attribute((naked)) void SysTick_Handler() {
    /* Save the context of current task */
    
    /* Fetch the current PSP value */
    __asm volatile ("MRS R0, PSP");

    /* Store R4-R11 registers into stack */
    __asm volatile ("STMDB R0!, {R4-R11}");

    /* Save LR value */
    __asm volatile ("PUSH {LR}");

    /* Save the updated PSP value */
    __asm volatile ("BL save_psp_of_task");
    
    /* Retrieve the context of next task */
    
    /* Update task id to point to next task */
    __asm volatile ("BL update_task_id");
    
    /* Fetch the current PSP value */
    __asm volatile ("BL get_psp_of_task");

    /* Update the R4-R11 registers from stack */
    __asm volatile ("LDMIA R0!, {R4-R11}");
    
    /* Update PSP */
    __asm volatile ("MSR PSP, R0");

    /* Retrieve LR value */
    __asm volatile ("POP {LR}");
    
    __asm volatile ("BX LR");
}

/* Enable mem manage, bus and usage faults */
void enable_processor_faults() {
    /* Mem manage fault */
    SCB->SHCRS |= (1 << 16);
    /* Bus fault */
    SCB->SHCRS |= (1 << 17);
    /* Usage fault */
    SCB->SHCRS |= (1 << 18);
}

void MemManage_Handler() {
    printf("Inside mem manage fault handler\n");
    while(1);
}

void BusFault_Handler() {
    printf("Inside bus fault handler\n");
    while(1);
}

void UsageFault_Handler() {
    printf("Inside usage fault handler\n");
    while(1);
}


