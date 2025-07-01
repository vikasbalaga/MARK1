/*
 * btn_gpio.c
 *
 *  Created on: Jun 30, 2025
 *      Author: vbalaga
 */
#include <stdint.h>
#include <stdio.h>
#include "stm32f407xx_gpio.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void delay() {
	for (int i = 0; i < 200000; i ++)
		;
}

int main(void)
{
	//printf("Hello World\n");
    /* Loop forever */
	//for(;;);
	GPIO_Handle_t led_gpio, btn_gpio;
	led_gpio.pGPIOx = GPIOD;
	led_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	led_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	led_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUTPUT_SPEED_VERY_HIGH;
	led_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	led_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_MODE_PUPL;
	GPIO_Clock_Ctrl(GPIOD, 1);
	GPIO_Init(&led_gpio);
	
	led_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&led_gpio);

	led_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&led_gpio);

	led_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&led_gpio);

	btn_gpio.pGPIOx = GPIOA;
	btn_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	btn_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	btn_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUTPUT_SPEED_VERY_HIGH;
	btn_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	btn_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_MODE_PUPL;
	GPIO_Clock_Ctrl(GPIOA, 1);
	GPIO_Init(&btn_gpio);


	while(1) {
	uint8_t read = GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0);
	if (read) {
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_14);
		delay();
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		delay();
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_13);
		delay();
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_15);
		delay();
	}
	}
}



