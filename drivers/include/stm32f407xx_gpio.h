/*
 * stm32f407xx_gpio.h
 *
 *  Created on: Jun 21, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_STM32F407XX_GPIO_H_
#define INCLUDE_STM32F407XX_GPIO_H_

#include <stdbool.h>
#include "stm32f407xx.h"

typedef struct {
	uint8_t GPIO_PinNumber;					/*!< possible values from @GPIO_PIN_NUM>*/
	uint8_t GPIO_PinMode;					/*!< possible values from @GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed;                  /*!< possible values from @GPIO_OUTPUT_SPEED_MODES >*/
	uint8_t GPIO_PinPuPdControl;            /*!< possible values from @GPIO_PUPD_MODES >*/
	uint8_t GPIO_PinOPType;                 /*!< possible values from @GPIO_OUTPUT_MODES >*/
	uint8_t GPIO_PinAltFunMode;             /*!< possible values from @GPIO_ALT_FN_MODES >*/ 
}GPIO_PinConfig_t;

typedef struct {
	GPIO_TypeDef* pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;

/* GPIO clock enable and disable macros */
#define GPIO_CLK_ENABLE 1
#define GPIO_CLK_DISABLE 0

/*
 * @GPIO_PIN_NUM
 * GPIO possible pin numbers
 */
#define GPIO_PIN_NO_0 0
#define GPIO_PIN_NO_1 1
#define GPIO_PIN_NO_2 2
#define GPIO_PIN_NO_3 3
#define GPIO_PIN_NO_4 4
#define GPIO_PIN_NO_5 5
#define GPIO_PIN_NO_6 6
#define GPIO_PIN_NO_7 7
#define GPIO_PIN_NO_8 8
#define GPIO_PIN_NO_9 9
#define GPIO_PIN_NO_10 10
#define GPIO_PIN_NO_11 11
#define GPIO_PIN_NO_12 12
#define GPIO_PIN_NO_13 13
#define GPIO_PIN_NO_14 14
#define GPIO_PIN_NO_15 15

/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ALT_FN 2
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_IT_FT 4
#define GPIO_MODE_IT_RT 5
#define GPIO_MODE_IT_RFT 6

/*
 * @GPIO_OUTPUT_MODES
 * GPIO pin possible output modes
 */
#define GPIO_OUTPUT_MODE_PUPL 0
#define GPIO_OUTPUT_MODE_OD 1

/*
 * @GPIO_OUTPUT_SPEED_MODES
 * GPIO pin possible output speed modes
 */
#define GPIO_OUTPUT_SPEED_LOW 0
#define GPIO_OUTPUT_SPEED_MEDIUM 1
#define GPIO_OUTPUT_SPEED_HIGH 2
#define GPIO_OUTPUT_SPEED_VERY_HIGH 3

/*
 * @GPIO_PUPD_MODES
 * GPIO pin possible pull up and pull down modes
 */
#define GPIO_NO_PUPD 0
#define GPIO_PIN_PU 1
#define GPIO_PIN_PD 2

/*
 * @GPIO_ALT_FN_MODES
 * GPIO possible pin numbers
 */
#define GPIO_ALT_FN_MODE_0 0
#define GPIO_ALT_FN_MODE_1 1
#define GPIO_ALT_FN_MODE_2 2
#define GPIO_ALT_FN_MODE_3 3
#define GPIO_ALT_FN_MODE_4 4
#define GPIO_ALT_FN_MODE_5 5
#define GPIO_ALT_FN_MODE_6 6
#define GPIO_ALT_FN_MODE_7 7
#define GPIO_ALT_FN_MODE_8 8
#define GPIO_ALT_FN_MODE_9 9
#define GPIO_ALT_FN_MODE_10 10
#define GPIO_ALT_FN_MODE_11 11
#define GPIO_ALT_FN_MODE_12 12
#define GPIO_ALT_FN_MODE_13 13
#define GPIO_ALT_FN_MODE_14 14
#define GPIO_ALT_FN_MODE_15 15

/* GPIO register reset macros */
#define GPIOA_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 0);\
									RCC->AHB1RSTR &= ~(1 << 0);\
									}while(0)
#define GPIOB_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 1);\
									RCC->AHB1RSTR &= ~(1 << 1);\
									}while(0)
#define GPIOC_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 2);\
									RCC->AHB1RSTR &= ~(1 << 2);\
									}while(0)
#define GPIOD_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 3);\
									RCC->AHB1RSTR &= ~(1 << 3);\
									}while(0)
#define GPIOE_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 4);\
									RCC->AHB1RSTR &= ~(1 << 4);\
									}while(0)
#define GPIOF_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 5);\
									RCC->AHB1RSTR &= ~(1 << 5);\
									}while(0)
#define GPIOG_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 6);\
									RCC->AHB1RSTR &= ~(1 << 6);\
									}while(0)
#define GPIOH_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 7);\
									RCC->AHB1RSTR &= ~(1 << 7);\
									}while(0)
#define GPIOI_REG_RESET()		do {\
									RCC->AHB1RSTR |= (1 << 8);\
									RCC->AHB1RSTR &= ~(1 << 8);\
									}while(0)


/* GPIO Driver APIs */

/**
 * @brief  GPIO initialization API
 *
 * @param  pGPIOHandle - GPIO handle
 *
 */
void GPIO_Init(GPIO_Handle_t* pGPIOHandle);

/**
 * @brief  GPIO de-initialization API
 *
 * @param  pGPIOx - GPIO port ID
 *
 */
void GPIO_Deinit(GPIO_TypeDef* pGPIOx);

/**
 * @brief  GPIO clock control API
 *
 * @param  pGPIOx - GPIO port ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void GPIO_Clock_Ctrl(GPIO_TypeDef* pGPIOx, bool enable);

/**
 * @brief  GPIO read from input GPIO pin
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  PinNumber -  pin number to read from GPIO port
 * @return the read value of the GPIO pin
 *
 */
uint8_t GPIO_ReadFromInputPin(GPIO_TypeDef* pGPIOx, uint8_t PinNumber);

/**
 * @brief  GPIO read from input GPIO port
 *
 * @param  pGPIOx - GPIO port ID
 * @return the read value of the GPIO port
 *
 */
uint16_t GPIO_ReadFromInputPort(GPIO_TypeDef* pGPIOx);

/**
 * @brief  GPIO write to output GPIO pin
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  PinNumber - pin number to write to GPIO port
 * @param  value the - value to be written to GPIO pin
 *
 */
void GPIO_WritetoOutputPin(GPIO_TypeDef* pGPIOx, uint8_t PinNumber, uint8_t value);

/**
 * @brief  GPIO write to output GPIO port
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  value     - value to be written to GPIO pin
 *
 */
void GPIO_WritetoOutputPort(GPIO_TypeDef* pGPIOx, uint16_t value);

/**
 * @brief  API to toggle GPIO output pin
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  PinNumber - the GPIO pin to toggle
 *
 */
void GPIO_ToggleOutputPin(GPIO_TypeDef* pGPIOx, uint8_t PinNumber);

/**
 * @brief  API to configure GPIO IRQ
 *
 * @param  IRQNumber   - Interrupt number
 * @param  IRQPriority - Interrupt priority
 * @param  IRQNumber   - IRQ number of the GPIO 
 * @param  enable      - if 0 then disable interrupt
 *				  		 if 1 then enable interrupt
 *
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable);

/**
 * @brief  API for GPIO interrupt handler
 *
 * @param  PinNumber - GPIO pin number
 *
 */
void GPIO_IRQHandler(uint8_t PinNumber);

#endif /* INCLUDE_STM32F407XX_GPIO_H_ */
