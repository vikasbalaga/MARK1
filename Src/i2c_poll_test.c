/*
 * i2c_poll_test.c
 *
 *  Created on: Nov 1, 2025
 *      Author: vbalaga
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f407xx_gpio.h"
#include "stm32f407xx_i2c.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void delay() {
	for (int i = 0; i < 200000; i ++)
		;
}

#define I2C1_DEVICE_ADDR    0x61
#define I2C2_DEVICE_ADDR    0x68

/* Alternate function mapping for I2C1 and I2C2 peripheral
   AF mode   -> 4
   For I2C1 :
   I2C1_SCL  -> PB6
   I2C1_SDA  -> PB7

   For I2C2 :
   I2C2_SCL  -> PB10
   I2C2_SDA  -> PB11
*/
void gpio_altfn_init(GPIO_Handle_t* gpio_handle) {
	gpio_handle->pGPIOx = GPIOB;

	gpio_handle->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALT_FN;
	gpio_handle->GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUTPUT_SPEED_MEDIUM;
	gpio_handle->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	gpio_handle->GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_MODE_OD;
	gpio_handle->GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FN_MODE_4;

	// Configure I2C1_SCL
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(gpio_handle);

	// Configure I2C1_SDA
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(gpio_handle);

	// Configure I2C2_SCL
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(gpio_handle);

	// Configure I2C2_SDA
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	GPIO_Init(gpio_handle);
}

char tx_data[] = "Words";

I2C_Handle_t i2c1_handle;
I2C_Handle_t i2c2_handle;

int main(void)
{
	/* Configure GPIO pins in AF mode for SPI peripheral */
	GPIO_Handle_t gpio_handle, btn_gpio;;
	gpio_altfn_init(&gpio_handle);

	/* Configure I2C1 and I2C2 peripherals */	
    i2c1_handle.pI2Cx = I2C1;
    i2c1_handle.I2C_Config.SCL_Speed = I2C_SPEED_SM;
    i2c1_handle.I2C_Config.DeviceAddr = I2C1_DEVICE_ADDR;
    i2c1_handle.I2C_Config.ACK_Control = I2C_ACK_ENABLE;
    i2c1_handle.I2C_Config.FMDutyCycle = I2C_FM_DUTY_CYCL_2;
    I2C_Init(&i2c1_handle);

    i2c2_handle.pI2Cx = I2C2;
    i2c2_handle.I2C_Config.SCL_Speed = I2C_SPEED_SM;
    i2c2_handle.I2C_Config.DeviceAddr = I2C2_DEVICE_ADDR;
    i2c2_handle.I2C_Config.ACK_Control = I2C_ACK_ENABLE;
    i2c2_handle.I2C_Config.FMDutyCycle = I2C_FM_DUTY_CYCL_2;
    I2C_Init(&i2c2_handle);

	/* Enable GPIO button interrupt to start transmission */
	btn_gpio.pGPIOx = GPIOA;
	btn_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	btn_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	btn_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUTPUT_SPEED_VERY_HIGH;
	btn_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	btn_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_MODE_PUPL;

	GPIO_IRQConfig(EXTI0_IRQ_NUM, NVIC_IRQ_PRIORITY_12, 1);	
	GPIO_Init(&btn_gpio);

    return 0;
}

void EXTI0_IRQHandler() {
	GPIO_IRQHandler(GPIO_PIN_NO_0);
	I2C_Master_Tx_Blocking(&i2c1_handle, (uint8_t*)tx_data, strlen(tx_data), i2c2_handle.I2C_Config.DeviceAddr);
}
