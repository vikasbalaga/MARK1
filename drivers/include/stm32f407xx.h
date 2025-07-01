/*
 * stm32f407xx.h
 *
 *  Created on: Jun 14, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_STM32F407XX_H_
#define INCLUDE_STM32F407XX_H_

#include <stdint.h>
#define __IO 					volatile
#include "core_cm4.h"

/* Base addresses of FLASH, SRAM and ROM memories */
#define FLASH_BASE				0x08000000U /* 0x08000000-0x080FFFFF 1 MB */
#define SRAM1_BASE				0x20000000U /* 0x20000000-0x2001BFFF 128 KB */
#define SRAM2_BASE				0x08000000U /* 0x2001C000-0x2001FFFF 16 KB */
#define SRAM 					SRAM1_BASE
#define ROM_BASE				0x1FFF0000U /* 0x1FFF0000-0x1FFF77FF 30 KB */

/* AHBx and APBx peripheral base addresses */
#define PERIPH_BASE				0x40000000U /* Peripheral base address */
#define APB1_BASE				PERIPH_BASE /* APB1 peripheral base address 0x40000000U*/
#define APB2_BASE				(PERIPH_BASE + 0x10000) /* APB2 peripheral base address 0x40010000 */
#define AHB1_BASE				(PERIPH_BASE + 0x20000) /* AHB1 peripheral base address 0x40020000 */
#define AHB2_BASE				(PERIPH_BASE + 0x10000000U) /* AHB2 peripheral base address 0x50000000 */
#define AHB3_BASE				(PERIPH_BASE + 0x60000000U) /* AHB2 peripheral base address 0xA0000000 */

/* Base addresses of peripherals on AHB1 bus
 * TO DO : Complete for other peripherals
 */
#define GPIOA_BASE				(AHB1_BASE + 0X0000) /* GPIOA base addr (0x40020000-0x400203FF) */
#define GPIOB_BASE				(AHB1_BASE + 0X0400) /* GPIOB base addr (0x40020400-0x400207FF) */
#define GPIOC_BASE				(AHB1_BASE + 0X0800) /* GPIOC base addr (0x40020800-0x40020BFF) */
#define GPIOD_BASE				(AHB1_BASE + 0X0C00) /* GPIOD base addr (0x40020C00-0x40020FFF) */
#define GPIOE_BASE				(AHB1_BASE + 0X1000) /* GPIOE base addr (0x40021000-0x400213FF) */
#define GPIOF_BASE				(AHB1_BASE + 0X1400) /* GPIOF base addr (0x40021400-0x400217FF) */
#define GPIOG_BASE				(AHB1_BASE + 0X1800) /* GPIOG base addr (0x40021800-0x40021BFF) */
#define GPIOH_BASE				(AHB1_BASE + 0X1C00) /* GPIOH base addr (0x40021C00-0x40021FFF) */
#define GPIOI_BASE				(AHB1_BASE + 0X2000) /* GPIOI base addr (0x40022000-0x400223FF) */

#define RCC_BASE				(AHB1_BASE + 0X3800) /* GPIOI base addr (0x40022000-0x400223FF) */

/* Base addresses of peripherals on APB1 bus
 * TO DO : Complete for other peripherals
 */
#define SPI2_BASE				(APB1_BASE + 0X3800) /* GPIOA base addr (0x40020000-0x400203FF) */
#define SPI3_BASE				(APB1_BASE + 0X3C00) /* GPIOA base addr (0x40020000-0x400203FF) */

#define USART2_BASE				(APB1_BASE + 0X4400) /* GPIOA base addr (0x40020000-0x400203FF) */
#define USART3_BASE				(APB1_BASE + 0X4800) /* GPIOA base addr (0x40020000-0x400203FF) */

#define UART4_BASE				(APB1_BASE + 0X4C00) /* GPIOA base addr (0x40020000-0x400203FF) */
#define UART5_BASE				(APB1_BASE + 0X5000) /* GPIOA base addr (0x40020000-0x400203FF) */

#define I2C1_BASE				(APB1_BASE + 0X5400) /* GPIOA base addr (0x40020000-0x400203FF) */
#define I2C2_BASE				(APB1_BASE + 0X5800) /* GPIOA base addr (0x40020000-0x400203FF) */
#define I2C3_BASE				(APB1_BASE + 0X5C00) /* GPIOA base addr (0x40020000-0x400203FF) */

/* Base addresses of peripherals on APB2 bus
 * TO DO : Complete for other peripherals
 */
#define USART1_BASE				(APB2_BASE + 0X1000) /* USART1 base addr (0x40011000-0x400113FF) */
#define USART6_BASE				(APB2_BASE + 0X1400) /* USART6 base addr (0x40011400-0x400117FF) */
#define SPI1_BASE				(APB2_BASE + 0X3000) /* SPI1 base addr (0x40013000-0x400133FF) */
#define SPI4_BASE				(APB2_BASE + 0X3400) /* SPI4 base addr (0x40013400-0x400137FF) */
#define SYSCFG_BASE				(APB2_BASE + 0X3800) /* SYSCFG base addr (0x40013800-0x40013BFF) */
#define EXTI_BASE				(APB2_BASE + 0X3C00) /* EXTI base addr (0x40013C00-0x40013FFF) */

/******************* Peripheral register definiton structures ********************/

/* Structure definiton for GPIOx */
typedef struct {
	__IO uint32_t MODER;			/*!< GPIO port mode register, Address offset: 0x00 */
	__IO uint32_t OTYPER;			/*!< GPIO port output type register, Address offset: 0x04 */
	__IO uint32_t OSPEEDR;			/*!< GPIO port output speed register, Address offset: 0x08 */
	__IO uint32_t PUPDR;			/*!< GPIO port pull-up/pull-down register, Address offset: 0x0C */
	__IO uint32_t IDR;				/*!< GPIO port input data register, Address offset: 0x10 */
	__IO uint32_t ODR;				/*!< GPIO port output data register, Address offset: 0x14 */
	__IO uint32_t BSRR;				/*!< GPIO port bit set/reset register, Address offset: 0x18 */
	__IO uint32_t LCKR;				/*!< GPIO port configuration lock register, Address offset: 0x1C */
	__IO uint32_t AFR[2];			/*!< GPIO port alternate function register, AFR[0] : alternate function low register Address offset: 0x20
	AFR[1] : alternate function high register Address offset: 0x24 */
} GPIO_TypeDef;

/* Peripheral definiton for GPIOx */
#define GPIOA						((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB						((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOC						((GPIO_TypeDef*)GPIOC_BASE)
#define GPIOD						((GPIO_TypeDef*)GPIOD_BASE)
#define GPIOE						((GPIO_TypeDef*)GPIOE_BASE)
#define GPIOF						((GPIO_TypeDef*)GPIOF_BASE)
#define GPIOG						((GPIO_TypeDef*)GPIOG_BASE)
#define GPIOH						((GPIO_TypeDef*)GPIOH_BASE)
#define GPIOI						((GPIO_TypeDef*)GPIOI_BASE)

/* Structure definiton for RCC */
typedef struct {
	__IO uint32_t CR;					/*!< RCC clock control register,               Address offset: 0x00 */
	__IO uint32_t PLLCFGR;				/*!< RCC PLL configuration register,               Address offset: 0x04 */
	__IO uint32_t CFGR;					/*!< RCC clock configuration register,               Address offset: 0x08 */
	__IO uint32_t CIR;					/*!< RCC clock interrupt register,               Address offset: 0x0C */
	__IO uint32_t AHB1RSTR;				/*!< RCC AHB1 peripheral reset register,               Address offset: 0x10 */
	__IO uint32_t AHB2RSTR;				/*!< RCC AHB2 peripheral reset register register,               Address offset: 0x14 */
	__IO uint32_t AHB3RSTR;				/*!< RCC AHB3 peripheral reset register register,               Address offset: 0x18 */
	__IO uint32_t RESERVED0;			/*!< Reserved Address offset: 0x1C */
	__IO uint32_t APB1RSTR;				/*!< RCC APB1 peripheral reset register register,               Address offset: 0x20 */
	__IO uint32_t APB2RSTR;				/*!< RCC APB2 peripheral reset register,               Address offset: 0x24 */
	__IO uint32_t RESERVED1[2];			/*!< Reserved Address offset: 0x28-0x2C */
	__IO uint32_t AHB1ENR;				/*!< RCC AHB1 peripheral clock enable register,               Address offset: 0x30 */
	__IO uint32_t AHB2ENR;				/*!< RCC AHB2 peripheral clock enable register,               Address offset: 0x34 */
	__IO uint32_t AHB3ENR;				/*!< RCC AHB3 peripheral clock enable register,               Address offset: 0x38 */
	__IO uint32_t RESERVED2;			/*!< Reserved Address offset: 0x3C */
	__IO uint32_t APB1ENR;				/*!< RCC APB1 peripheral clock enable register,               Address offset: 0x40 */
	__IO uint32_t APB2ENR;				/*!< RCC APB2 peripheral clock enable register,               Address offset: 0x44 */
	__IO uint32_t RESERVED3[2];			/*!< Reserved Address offset: 0x48-0x4C */
	__IO uint32_t AHB1LPENR;			/*!< RCC AHB1 peripheral clock enable in low power mode register,               Address offset: 0x50 */
	__IO uint32_t AHB2LPENR;			/*!< RCC AHB2 peripheral clock enable in low power mode register,               Address offset: 0x54 */
	__IO uint32_t AHB3LPENR;			/*!< RCC AHB3 peripheral clock enable in low power mode register,               Address offset: 0x58 */
	__IO uint32_t RESERVED4;			/*!< Reserved Address offset: 0x5C */
	__IO uint32_t APB1LPENR;			/*!< RCC APB1 peripheral clock enable in low power mode register,               Address offset: 0x60 */
	__IO uint32_t APB2LPENR;			/*!< RCC APB2 peripheral clock enable in low power mode register,               Address offset: 0x64 */
	__IO uint32_t RESERVED5[2];			/*!< Reserved Address offset: 0x68-0x6C */
	__IO uint32_t BDCR;					/*!< RCC backup domain control register,               Address offset: 0x70 */
	__IO uint32_t CSR;					/*!< RCC clock control and status register,               Address offset: 0x74 */
	__IO uint32_t RESERVED6[2];			/*!< Reserved Address offset: 0x78-0x7C */
	__IO uint32_t SSCGR;				/*!< RCC  spread spectrum clock generation register,               Address offset: 0x80 */
	__IO uint32_t PLLI2SCFGR;			/*!< RCC PLLI2S configuration register,               Address offset: 0x84 */
}RCC_TypeDef;

/* Peripheral definiton for RCC */
#define RCC						((RCC_TypeDef*)RCC_BASE)


/* Structure definiton for EXTI */
typedef struct {
	__IO uint32_t IMR;					/*!< EXTI interrupt mask register, Address offset: 0x00 */
	__IO uint32_t EMR;					/*!< EXTI event mask register, Address offset: 0x04 */
	__IO uint32_t RTSR;					/*!< EXTI rising trigger selection register, Address offset: 0x08 */
	__IO uint32_t FTSR;					/*!< EXTI falling trigger selection register, Address offset: 0x0C */
	__IO uint32_t SWIER;				/*!< EXTI software interrupt and event trigger selection register, Address offset: 0x10 */
	__IO uint32_t PR;					/*!< EXTI pending register, Address offset: 0x14 */
} EXTI_TypeDef;

#define EXTI						((EXTI_TypeDef*)EXTI_BASE)

/* Structure definiton for SYSCFG */
typedef struct {
	__IO uint32_t MEMRMP;				/*!< SYSCFG memory remap register, Address offset: 0x00 */
	__IO uint32_t PMC;					/*!< SYSCFG peripheral mode configuration register, Address offset: 0x04 */
	__IO uint32_t EXTICR[4];			/*!< SYSCFG external interrupt configuration register 1 to 4, Address offset: 0x08 - 0x14 */
	__IO uint32_t RESERVED[2];			/*!< Reserved, Address offset: 0x18 - 0x520 */
	__IO uint32_t CMPCR;				/*!< SYSCFG compensation cell control register, Address offset: 0x20 */
} SYSCFG_TypeDef;

#define SYSCFG						((SYSCFG_TypeDef*)SYSCFG_BASE)

#define GPIO_DECODE_TO_EXTI(x)		((x == GPIOA)? 0 : \
									(x == GPIOB)? 1 : \
									(x == GPIOC)? 2 : \
									(x == GPIOD)? 3 : \
									(x == GPIOE)? 4 : \
									(x == GPIOF)? 5 : \
									(x == GPIOG)? 6 : \
									(x == GPIOH)? 7 : \
									(x == GPIOI)? 8 : 0)


/* Clock enable and disable macros for GPIOx */
#define GPIOA_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_CLK_ENABLE()		(RCC->AHB1ENR |= (1 << 8))

#define GPIOA_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_CLK_DISABLE()		(RCC->AHB1ENR &= ~(1 << 8))

/* Clock enable and disable macros for I2Cx */
#define I2C1_CLK_ENABLE()			(RCC->APB1ENR |= (1 << 21))
#define I2C2_CLK_ENABLE()			(RCC->APB1ENR |= (1 << 22))
#define I2C3_CLK_ENABLE()			(RCC->APB1ENR |= (1 << 23))

#define I2C1_CLK_DISABLE()		(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_CLK_DISABLE()		(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_CLK_DISABLE()		(RCC->APB1ENR &= ~(1 << 23))

/* Clock enable and disable macros for SPIx */
#define SPI1_CLK_ENABLE()			(RCC->APB2ENR |= (1 << 12))
#define SPI2_CLK_ENABLE()			(RCC->APB1ENR |= (1 << 14))
#define SPI3_CLK_ENABLE()			(RCC->APB1ENR |= (1 << 15))

#define SPI1_CLK_DISABLE()			(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_CLK_DISABLE()			(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_CLK_DISABLE()			(RCC->APB1ENR &= ~(1 << 15))

/* Clock enable and disable macros for USARTx */
#define USART1_CLK_ENABLE()		(RCC->APB2ENR |= (1 << 4))
#define USART2_CLK_ENABLE()		(RCC->APB1ENR |= (1 << 17))
#define USART3_CLK_ENABLE()		(RCC->APB1ENR |= (1 << 18))
#define USART6_CLK_ENABLE()		(RCC->APB2ENR |= (1 << 5))

#define USART1_CLK_DISABLE()		(RCC->APB2ENR &= ~(1 << 4))
#define USART2_CLK_DISABLE()		(RCC->APB1ENR &= ~(1 << 17))
#define USART3_CLK_DISABLE()		(RCC->APB1ENR &= ~(1 << 18))
#define USART6_CLK_DISABLE()		(RCC->APB2ENR &= ~(1 << 5))

/* Clock enable and disable macros for SYSCFG */
#define SYSCFG_CLK_ENABLE()		(RCC->APB2ENR |= (1 << 14))

#define SYSCFG_CLK_DISABLE()		(RCC->APB2ENR &= ~(1 << 14))

/* EXTI IRQ Numbers */
#define EXTI0_IRQ_NUM			6
#define EXTI1_IRQ_NUM			7
#define EXTI2_IRQ_NUM			8
#define EXTI3_IRQ_NUM			9
#define EXTI4_IRQ_NUM			10
#define EXTI9_5_IRQ_NUM			23
#define EXTI15_10_IRQ_NUM		40

#define NO_PRIORITY_BITS_IMPLEMENTED 4

#endif /* INCLUDE_STM32F407XX_H_ */
