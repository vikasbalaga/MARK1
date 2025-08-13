/*
 * stm32f407xx_gpio.c
 *
 *  Created on: Jun 21, 2025
 *      Author: vbalaga
 */
#include "stm32f407xx_gpio.h"

/**
 * @brief  GPIO initialization API
 *
 * @param  pGPIOHandle - GPIO handle
 *
 */
void GPIO_Init(GPIO_Handle_t* pGPIOHandle){

    /* Enable GPIO clock */
    GPIO_Clock_Ctrl(pGPIOHandle->pGPIOx, 1);
    
	/* Configure GPIO mode */
	uint32_t temp = 0;
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode < GPIO_MODE_IT_FT) {
		/* Non-interrupt based modes */
		temp = (0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~temp; //Clearing

		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode) << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);	
		pGPIOHandle->pGPIOx->MODER |= temp; //Setting	
	}
	else {
		/* Interrupt based modes */
		/* Configure rising / falling trigger */
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//Configure falling trigger selection, make sure to clear the RTSR
			EXTI->RTSR |= (0 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			//Configure rising trigger selection, make sure to clear the FTSR
			EXTI->FTSR |= (0 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			//Configure falling and rising trigger selection
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		/* Enable clock for SYSCFG */
		SYSCFG_CLK_ENABLE();

		/* Configure GPIO port selection in EXTI */
		uint8_t reg_idx = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t reg_offset = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		SYSCFG->EXTICR[reg_idx] |= (GPIO_DECODE_TO_EXTI(pGPIOHandle->pGPIOx) << (4 * reg_offset));

		/* Enable the interrupt in EXTI interrupt mask register */
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}
	
	/* Configure GPIO speed */
	temp = 0;
	temp = (0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~temp; //Clearing

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed) << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDR |= temp; //Setting
	
	/* Configure GPIO PUPD settings */
	temp = 0;
	temp = (0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~temp; //Clearing

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl) << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR |= temp; //Setting
	
	/* Configure GPIO output type */
	temp = 0;
	temp = (0x1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~temp; //Clearing

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType) << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	
	/* Configure GPIO Alt functionality */
	temp = 0;
	uint8_t buf_idx = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
	uint8_t reg_pos = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;

	temp = (0xF << (4 * reg_pos));
	pGPIOHandle->pGPIOx->AFR[buf_idx] &= ~temp; //Clearing

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode) << (4 * reg_pos);
	pGPIOHandle->pGPIOx->AFR[buf_idx] |= temp; //Setting
}

/**
 * @brief  GPIO de-initialization API
 *
 * @param  pGPIOx - GPIO port ID
 *
 */
void GPIO_Deinit(GPIO_TypeDef* pGPIOx){
	/* Reset the GPIO peripheral */
	if (pGPIOx == GPIOA)
		GPIOA_REG_RESET();
	else if (pGPIOx == GPIOB)
		GPIOB_REG_RESET();
	else if (pGPIOx == GPIOC)
		GPIOC_REG_RESET();
	else if (pGPIOx == GPIOD)
		GPIOD_REG_RESET();
	else if (pGPIOx == GPIOE)
		GPIOE_REG_RESET();
	else if (pGPIOx == GPIOF)
		GPIOF_REG_RESET();
	else if (pGPIOx == GPIOG)
		GPIOG_REG_RESET();
	else if (pGPIOx == GPIOH)
		GPIOH_REG_RESET();
	else if (pGPIOx == GPIOI)
		GPIOI_REG_RESET();
}

/**
 * @brief  GPIO clock control API
 *
 * @param  pGPIOx - GPIO port ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void GPIO_Clock_Ctrl(GPIO_TypeDef* pGPIOx, bool enable){
	/* Enable the clock */
	if (enable == GPIO_CLK_ENABLE){
		if (pGPIOx == GPIOA)
			GPIOA_CLK_ENABLE();
		else if (pGPIOx == GPIOB)
			GPIOB_CLK_ENABLE();
		else if (pGPIOx == GPIOC)
			GPIOC_CLK_ENABLE();
		else if (pGPIOx == GPIOD)
			GPIOD_CLK_ENABLE();
		else if (pGPIOx == GPIOE)
			GPIOE_CLK_ENABLE();
		else if (pGPIOx == GPIOF)
			GPIOF_CLK_ENABLE();
		else if (pGPIOx == GPIOG)
			GPIOG_CLK_ENABLE();
		else if (pGPIOx == GPIOH)
			GPIOH_CLK_ENABLE();
		else if (pGPIOx == GPIOI)
			GPIOI_CLK_ENABLE();
	}
	/* Disable the clock */
	else {
		if (pGPIOx == GPIOA)
			GPIOA_CLK_DISABLE();
		else if (pGPIOx == GPIOB)
			GPIOB_CLK_DISABLE();
		else if (pGPIOx == GPIOC)
			GPIOC_CLK_DISABLE();
		else if (pGPIOx == GPIOD)
			GPIOD_CLK_DISABLE();
		else if (pGPIOx == GPIOE)
			GPIOE_CLK_DISABLE();
		else if (pGPIOx == GPIOF)
			GPIOF_CLK_DISABLE();
		else if (pGPIOx == GPIOG)
			GPIOG_CLK_DISABLE();
		else if (pGPIOx == GPIOH)
			GPIOH_CLK_DISABLE();
		else if (pGPIOx == GPIOI)
			GPIOI_CLK_DISABLE();
	}
}

/**
 * @brief  GPIO read from input GPIO pin
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  PinNumber -  pin number to read from GPIO port
 * @return the read value of the GPIO pin
 *
 */
uint8_t GPIO_ReadFromInputPin(GPIO_TypeDef* pGPIOx, uint8_t PinNumber){
	uint8_t reg_val;
	reg_val = (uint8_t)((pGPIOx->IDR >> PinNumber) & (1 << 0));
	return reg_val;
}

/**
 * @brief  GPIO read from input GPIO port
 *
 * @param  pGPIOx - GPIO port ID
 * @return the read value of the GPIO port
 *
 */
uint16_t GPIO_ReadFromInputPort(GPIO_TypeDef* pGPIOx){
	uint16_t reg_val;
	reg_val = (uint16_t)pGPIOx->IDR;
	return reg_val;
}

/**
 * @brief  GPIO write to output GPIO pin
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  PinNumber - pin number to write to GPIO port
 * @param  value the - value to be written to GPIO pin
 *
 */
void GPIO_WritetoOutputPin(GPIO_TypeDef* pGPIOx, uint8_t PinNumber, uint8_t value){
	if (value == 1)
		pGPIOx->ODR |= (1 << PinNumber);
	else if (value == 0)
		pGPIOx->ODR &= ~(1 << PinNumber);
}

/**
 * @brief  GPIO write to output GPIO port
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  value     - value to be written to GPIO pin
 *
 */
void GPIO_WritetoOutputPort(GPIO_TypeDef* pGPIOx, uint16_t value){
	pGPIOx->ODR = value;
}

/**
 * @brief  API to toggle GPIO output pin
 *
 * @param  pGPIOx    - GPIO port ID
 * @param  PinNumber - the GPIO pin to toggle
 *
 */
void GPIO_ToggleOutputPin(GPIO_TypeDef* pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= (1 << PinNumber);
}

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
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable){
	uint8_t reg_idx = IRQNumber / 32;
	uint8_t reg_pos = IRQNumber % 32;
	/* Enable / Disable the interrupt */
	if (enable == 1) {
		NVIC->ISER[reg_idx] |= (1 << reg_pos);
	} else if (enable == 0) {
		NVIC->ICER[reg_idx] |= (1 << reg_pos);
	}
	/* Set the interrupt priority */
	NVIC->IPR[IRQNumber] = (IRQPriority << (8 - NO_PRIORITY_BITS_IMPLEMENTED));
}

/**
 * @brief  API for GPIO interrupt handler
 *
 * @param  PinNumber - GPIO pin number
 *
 */
void GPIO_IRQHandler(uint8_t PinNumber){
	EXTI->PR |= (1 << PinNumber);
}