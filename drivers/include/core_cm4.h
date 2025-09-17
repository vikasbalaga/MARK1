/*
 * core_cm4.h
 *
 *  Created on: Jun 30, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_CORE_CM4_H_
#define INCLUDE_CORE_CM4_H_

#include <stdbool.h>
#include <stdint.h>
#define __IO 					volatile

/* Structure definiton for NVIC */
typedef struct {
	__IO uint32_t ISER[8];			/*!< NVIC interrupt set enable register, Address offset: 0x00 */
	__IO uint32_t RESERVED0[24];		/*!< Reserved, Address offset: 0x20 */
	__IO uint32_t ICER[8];			/*!< NVIC interrupt clear enable register, Address offset: 0x80 */
	__IO uint32_t RESERVED1[24];		/*!< Reserved, Address offset: 0x00 */
	__IO uint32_t ISPR[8];			/*!< NVIC interrupt set pending register, Address offset: 0x00 */
	__IO uint32_t RESERVED2[24];		/*!< Reserved, Address offset: 0x00 */
	__IO uint32_t ICPR[8];			/*!< NVIC interrupt clear pending register, Address offset: 0x00 */
	__IO uint32_t RESERVED3[24];		/*!< Reserved, Address offset: 0x00 */
	__IO uint32_t IABR[8];			/*!< NVIC active bit register, Address offset: 0x00 */
	__IO uint32_t RESERVED4[56];		/*!< Reserved, Address offset: 0x00 */
	__IO uint8_t IPR[240];			/*!< NVIC interrupt priority register, Address offset: 0x00 */
	__IO uint32_t RESERVED5[644];	/*!< Reserved, Address offset: 0x00 */
	__IO uint32_t STIR;				/*!< NVIC software trigger interrupt register, Address offset: 0x00 */
} NVIC_TypeDef;

#define NVIC						((NVIC_TypeDef*)0xE000E100)

/*
 * @NVIC_IRQ_PRIORITY_NUM
 * NVIC possible interrupt priority numbers
 */
#define NVIC_IRQ_PRIORITY_0 0
#define NVIC_IRQ_PRIORITY_1 1
#define NVIC_IRQ_PRIORITY_2 2
#define NVIC_IRQ_PRIORITY_3 3
#define NVIC_IRQ_PRIORITY_4 4
#define NVIC_IRQ_PRIORITY_5 5
#define NVIC_IRQ_PRIORITY_6 6
#define NVIC_IRQ_PRIORITY_7 7
#define NVIC_IRQ_PRIORITY_8 8
#define NVIC_IRQ_PRIORITY_9 9
#define NVIC_IRQ_PRIORITY_10 10
#define NVIC_IRQ_PRIORITY_11 11
#define NVIC_IRQ_PRIORITY_12 12
#define NVIC_IRQ_PRIORITY_13 13
#define NVIC_IRQ_PRIORITY_14 14
#define NVIC_IRQ_PRIORITY_15 15

/* Structure definiton for SysTick */
typedef struct {
    __IO uint32_t CSR;                   /*!< SysTick control and status register, Address offset: 0x00 */
    __IO uint32_t RVR;                   /*!< SysTick reload value register, Address offset: 0x04 */
    __IO uint32_t CVR;                   /*!< SysTick current value register, Address offset: 0x08 */
    __IO uint32_t CALIB;                 /*!< SysTick calibration value register, Address offset: 0x0C */
} SysTick_TypeDef;

#define SYSTICK                         ((SysTick_TypeDef*)0xE000E010)

/* Structure definition for System control not in SCB (like ACTLR) */
typedef struct {
    __IO uint32_t RESERVED0[2]; /* 0x000: Reserved */
    __IO uint32_t ACTLR;        /* 0x008: Auxiliary Control Register */
} SCnSCB_TypeDef;

#define SCnSCB                          ((SCnSCB_TypeDef)0xE000E000)

/* Structure definiton for SCB block */
typedef struct {
    __IO uint32_t CPUID;                  /*!< CPUID base register, Address offset: 0x00 */
    __IO uint32_t ICSR;                  /*!< Interrupt control and state register, Address offset: 0x04 */
    __IO uint32_t VTOR;                  /*!< Vector table offset register, Address offset: 0x08 */
    __IO uint32_t AIRCR;                  /*!< Application interrupt and reset control register, Address offset: 0x0C */
    __IO uint32_t SCR;                  /*!< System control register, Address offset: 0x10 */
    __IO uint32_t CCR;                  /*!< Configuration and control register, Address offset: 0x14 */
    __IO uint32_t SHPR1;                  /*!< System handler priority register 1, Address offset: 0x18 */
    __IO uint32_t SHPR2;                  /*!< System handler priority register 2, Address offset: 0x1C */
    __IO uint32_t SHPR3;                  /*!< System handler priority register 3, Address offset: 0x20 */
    __IO uint32_t SHCRS;                  /*!< System handler control and state register, Address offset: 0x24 */
    __IO uint32_t CFSR;                  /*!< Configurable fault status register, Address offset: 0x28 */
    __IO uint32_t HFSR;                  /*!< Hard fault status register, Address offset: 0x2C */
    __IO uint32_t RESERVED;;
    __IO uint32_t MMAR;                  /*!< Mem manage fault address register, Address offset: 0x34 */
    __IO uint32_t BFAR;                  /*!< Bus fault address register, Address offset: 0x38 */
    __IO uint32_t AFSR;                  /*!< Auxiliary fault status register, Address offset: 0x3C */
} SCB_TypeDef;

#define SCB                             ((SCB_TypeDef*)0xE000ED00)

/* Define clock sources and their frequency */
#define HSI_CLK                         (16000000U) /* 16 MHz */


#endif /* INCLUDE_CORE_CM4_H_ */
