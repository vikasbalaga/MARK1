/*
 * usart_poll_test.c
 *
 *  Created on: Nov 22, 2025
 *      Author: vbalaga
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f407xx_gpio.h"
#include "stm32f407xx_usart.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

uint8_t tx1_data[] = "Success is not a destination, it's a journey...\n";
uint8_t tx2_data[] = "It's a punishable offence...\n";
//uint8_t tx_data[] = "Testing USART3 peripheral using TeraTerm...";

uint8_t rx_data[25];

void delay() {
	for (int i = 0; i < 200000; i ++)
		;
}

/* Configure USART1 peripheral using GPIO Alternate functionality
   AF mode   -> 7
   For USART3 :
   USART3_TX  -> PB10
   USART3_RX  -> PB11 */

void gpio_altfn_init() {
    /* Configure the GPIO handle structure */
    GPIO_Handle_t usart1_gpio;
    usart1_gpio.pGPIOx = GPIOB;
    usart1_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALT_FN;
    usart1_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUTPUT_SPEED_MEDIUM;
    usart1_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    usart1_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUTPUT_MODE_PUPL;
    usart1_gpio.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_ALT_FN_MODE_7;
    
    // Configure USART1 TX
    usart1_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
    GPIO_Init(&usart1_gpio);

    // Configure USART1 RX
    usart1_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
    GPIO_Init(&usart1_gpio);
}

USART_Handle_t usart_handle;

void usart_init() {
    usart_handle.pUSARTx = USART3;
    usart_handle.USART_Config.Mode = USART_MODE_TX_ONLY;
    usart_handle.USART_Config.BaudRate = USART_BAUD_921600;
    usart_handle.USART_Config.StopBits = USART_STOPBITS_2;
    usart_handle.USART_Config.WordLength = USART_WORD_LEN_8;
    usart_handle.USART_Config.ParityCTRL = USART_PARITY_NONE;
    usart_handle.USART_Config.HwFlowControl = USART_HW_CTRL_NONE;
    usart_handle.USART_Config.OversamplingMode = OVERSAMPLING_MODE_8;
    
    USART_Init(&usart_handle);
}

int main() {
    // Initialize the GPIO in alternate functionality mode
    gpio_altfn_init();

    // Initialize the USART peripheral
    usart_init();

    USART_Tx_Blocking(&usart_handle, tx1_data, sizeof(tx1_data)/sizeof(tx1_data[0]));
    USART_Tx_Blocking(&usart_handle, tx2_data, sizeof(tx2_data)/sizeof(tx2_data[0]));
    
}
