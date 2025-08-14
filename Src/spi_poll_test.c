/*
 * spi_test.c
 *
 *  Created on: Jul 25, 2025
 *      Author: vbalaga
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f407xx_gpio.h"
#include "stm32f407xx_spi.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void delay() {
	for (int i = 0; i < 200000; i ++)
		;
}

/* Alternate function mapping for SPI2 peripheral
   AF mode   -> 5
   SPI2_NSS  -> PB9
   SPI2_SCK  -> PB10
   SPI2_MISO -> PB14
   SPI2_MOSI -> PB15
*/
void gpio_altfn_init(GPIO_Handle_t* gpio_handle) {
	gpio_handle->pGPIOx = GPIOB;

	gpio_handle->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALT_FN;
	gpio_handle->GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUTPUT_SPEED_MEDIUM;
	gpio_handle->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	gpio_handle->GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_MODE_PUPL;
	gpio_handle->GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FN_MODE_5;

	// Configure SPI2_NSS
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	//GPIO_Init(&gpio_handle);

	// Configure SPI2_SCK
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(gpio_handle);

	// Configure SPI2_MISO
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(gpio_handle);


	// Configure SPI2_MOSI
	gpio_handle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(gpio_handle);
}

void spi_init(SPI_Handle_t* spi_handle){
	spi_handle->pSPIx = SPI2;

	spi_handle->SPI_Config.SPI_DeviceMode = SPI_MODE_MASTER;
	spi_handle->SPI_Config.SPI_BusConfig = SPI_BUS_MODE_FULL_DUPLEX;
	spi_handle->SPI_Config.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	spi_handle->SPI_Config.SPI_DFF = SPI_DFF_SIZE_16;
	spi_handle->SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	spi_handle->SPI_Config.SPI_CPHA = SPI_CPHA_FIRST_EDGE;
	spi_handle->SPI_Config.SPI_SSM = SPI_SSM_ENABLE;
	SPI_Init(spi_handle);
}


char tx_data[] = "The truth is...I am Iron Man!";

int main(void)
{
	/* Configure GPIO pins in AF mode for SPI peripheral */
	GPIO_Handle_t gpio_handle;
	gpio_altfn_init(&gpio_handle);

	/* Configure SPI peripheral */	
	SPI_Handle_t spi_handle;
	spi_init(&spi_handle);

	SPI_Tx_Blocking(spi_handle.pSPIx, (uint8_t*)tx_data, strlen(tx_data));

    return 0;
}

