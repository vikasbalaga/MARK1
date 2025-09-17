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

#define IDLE_TASK_STACK_START SRAM_END
#define TASK1_STACK_START     (SRAM_END - (1 * TASK_STACK_SIZE))
#define TASK2_STACK_START     (SRAM_END - (2 * TASK_STACK_SIZE))
#define TASK3_STACK_START     (SRAM_END - (3 * TASK_STACK_SIZE))
#define TASK4_STACK_START     (SRAM_END - (4 * TASK_STACK_SIZE))

#define SCHEDULER_STACK_START   (SRAM_END - (5 * TASK_STACK_SIZE))

#define MAX_TASKS 5
#define TOTAL_GEN_REGISTERS 13

void idle_task_handler();
void task1_handler();
void task2_handler();
void task3_handler();
void task4_handler();

void scheduler_stack_init(uint32_t* scheduler_sp);

void task_stack_init();

void enable_processor_faults();

void switch_sp_to_psp();

// 0th index corresponds to idle task
uint32_t current_task_id = 1;

// Initilaize the global systick count for systick exception
uint32_t g_systick_count = 0;

typedef struct {
    uint32_t task_psp_ptr;
    uint32_t blocking_count;
    int task_state;
    void (*task_fn_ptr)(void);
} Task_Handle;

Task_Handle user_tasks[MAX_TASKS];

// Define task states
#define TASK_STATE_BLOCKED 0
#define TASK_STATE_READY 1

/* SysTick should trigger exception for every 1 ms i.e., tick rate is 1 kHz */
#define SYSTICK_TICK_RATE       1000 /* 1 kHz*/

uint32_t get_psp_of_task(void) {
    return user_tasks[current_task_id].task_psp_ptr;
}

void save_psp_of_task(uint32_t psp_value) {
    user_tasks[current_task_id].task_psp_ptr = psp_value;
}

void update_task_id() {
    int next_task_id = current_task_id;
    next_task_id++;
    next_task_id %= MAX_TASKS;
    // Ignore the idle task at index zero
    if (!next_task_id)
        next_task_id++;
    while (next_task_id != current_task_id) {
        if (user_tasks[next_task_id].task_state == TASK_STATE_READY) {
            //current_task_id = next_task_id;
            break;
        }
        next_task_id++;
        next_task_id %= MAX_TASKS;
    }
    // If all tasks are in blocking state fall back to idle task
    if (next_task_id == current_task_id)
        current_task_id = 0;
    else
    	current_task_id = next_task_id;
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
    // Idle task
    user_tasks[0].task_psp_ptr = IDLE_TASK_STACK_START;
    user_tasks[0].task_state = TASK_STATE_READY;
    user_tasks[0].task_fn_ptr = idle_task_handler;

    user_tasks[1].task_psp_ptr = TASK1_STACK_START;
    user_tasks[1].task_state = TASK_STATE_READY;
    user_tasks[1].task_fn_ptr = task1_handler;

    user_tasks[2].task_psp_ptr = TASK2_STACK_START;
    user_tasks[2].task_state = TASK_STATE_READY;
    user_tasks[2].task_fn_ptr = task2_handler;
    
    user_tasks[3].task_psp_ptr = TASK3_STACK_START;
    user_tasks[3].task_state = TASK_STATE_READY;
    user_tasks[3].task_fn_ptr = task3_handler;

    user_tasks[4].task_psp_ptr = TASK4_STACK_START;
    user_tasks[4].task_state = TASK_STATE_READY;
    user_tasks[4].task_fn_ptr = task4_handler;

    uint32_t* pPSP;
    for (int i = 0; i < MAX_TASKS; i++) {
        pPSP = (uint32_t*)user_tasks[i].task_psp_ptr;
        // xPSR
        pPSP--;
        *pPSP = 0x01000000;
        // PC
        pPSP--;
        *pPSP = (uint32_t)user_tasks[i].task_fn_ptr;
        // LR
        pPSP--;
        *pPSP = 0xFFFFFFFD;
        for (int j = 0; j < TOTAL_GEN_REGISTERS; j++) {
            pPSP--;
            *pPSP = 0;
        }
        /* Update the stack pointers accordingly */
        user_tasks[i].task_psp_ptr = (uint32_t)pPSP;
    }
}

void update_task() {
    // Invoke PendSV exception
    SCB->ICSR |= (1 << 28);
}

void block_current_task(uint32_t delay_count) {
    user_tasks[current_task_id].blocking_count = g_systick_count + delay_count;
    user_tasks[current_task_id].task_state = TASK_STATE_BLOCKED;
    update_task();
}

void idle_task_handler() {
    while(1);
}

void task1_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_GREEN);
        block_current_task(1000);
        GPIO_ToggleOutputPin(GPIOD, LED_GREEN);
        block_current_task(1000);
    }
}

void task2_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_ORANGE);
        block_current_task(500);
        GPIO_ToggleOutputPin(GPIOD, LED_ORANGE);
        block_current_task(500);
    }
}

void task3_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_RED);
        block_current_task(250);
        GPIO_ToggleOutputPin(GPIOD, LED_RED);
        block_current_task(250);
    }
}

void task4_handler() {
    while(1) {
        GPIO_ToggleOutputPin(GPIOD, LED_BLUE);
        block_current_task(125);
        GPIO_ToggleOutputPin(GPIOD, LED_BLUE);
        block_current_task(125);
    }
}

__attribute((naked)) void PendSV_Handler() {
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

// Check if any task blocking state is elapsed and then unblock it
void check_blocking_status() {
	for (int task_id = 1; task_id < MAX_TASKS; task_id++)
    if (user_tasks[task_id].task_state == TASK_STATE_BLOCKED) {
        if (user_tasks[task_id].blocking_count <= g_systick_count)
            user_tasks[task_id].task_state = TASK_STATE_READY;
    }
}

// Update global systick count and check task's blocking status
void SysTick_Handler() {
    g_systick_count++;
    check_blocking_status();
    update_task();
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


