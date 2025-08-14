/*
 * stm32f407xx_spi.c
 *
 *  Created on: Jul 19, 2025
 *      Author: vbalaga
 */
#include "stm32f407xx_spi.h"


/* SPI Driver APIs */

/**
 * @brief  SPI initialization API
 *
 * @param  pSPIHandle - SPI handle
 *
 */
void SPI_Init(SPI_Handle_t* pSPIHandle){
	/* Enable the clock for SPI */
	SPI_Clock_Ctrl(pSPIHandle->pSPIx, SPI_CLK_ENABLE);
	
	// Configure SPI_CR1 register
	uint32_t temp = 0;

	// Configure SPI mode
	temp |= (pSPIHandle->SPI_Config.SPI_DeviceMode << SPI_CR1_MSTR_BIT);

	// Configure SPI bus config
	if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_MODE_FULL_DUPLEX) {
		// Clear BIDIMODE bit
		temp &= ~(1 << SPI_CR1_BIDIMODE_BIT);
		// Clear RXONLY bit
		temp &= ~(1 << SPI_CR1_RXONLY_BIT);
	} else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_MODE_HALF_DUPLEX) {
		// Set BIDIMODE bit
		temp |= (1 << SPI_CR1_BIDIMODE_BIT);
	} else if (pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_MODE_SIMPLEX_RX) {
		// Clear BIDIMODE bit
		temp &= ~(1 << SPI_CR1_BIDIMODE_BIT);
		// Set RXONLY bit
		temp |= (1 << SPI_CR1_RXONLY_BIT);
	}

	// Configure SPI clock speed
	temp |= (pSPIHandle->SPI_Config.SPI_SclkSpeed << SPI_CR1_BR_BIT);

	// Configure SPI data frame format
	temp |= (pSPIHandle->SPI_Config.SPI_DFF << SPI_CR1_DFF_BIT);

	// Configure SPI CPOL
	temp |= (pSPIHandle->SPI_Config.SPI_CPOL << SPI_CR1_CPOL_BIT);

	// Configure SPI CPHA
	temp |= (pSPIHandle->SPI_Config.SPI_CPHA << SPI_CR1_CPHA_BIT);

	// Configure SPI SSM
	temp |= (pSPIHandle->SPI_Config.SPI_SSM << SPI_CR1_SSM_BIT);
	
	// Configure SPI SSI bit based on SSM configuration
	if (pSPIHandle->SPI_Config.SPI_SSM == SPI_SSM_ENABLE){
		// Set SSI bit to 1, to avoid MODF fault
		temp |= (1 << SPI_CR1_SSI_BIT);
	} else if (pSPIHandle->SPI_Config.SPI_SSM == SPI_SSM_DISABLE){
		// Set SSOE bit to configure NSS in output mode and avoid MODF
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_SSOE_BIT);
	}
	
	//Assign to the SPI CR1 register
	pSPIHandle->pSPIx->CR1 |= temp;
}

/**
 * @brief  SPI de-initialization API
 *
 * @param  pSPIx - SPI peripheral ID
 *
 */
void SPI_Deinit(SPI_TypeDef* pSPIx){
	/* Reset the SPI peripheral */
	if (pSPIx == SPI1)
		SPI1_REG_RESET();
	else if (pSPIx == SPI2)
		SPI2_REG_RESET();
	else if (pSPIx == SPI3)
		SPI3_REG_RESET();
}

/**
 * @brief  SPI peripheral control API
 *
 * @param  pSPIx  - SPI peripheral ID
 * @param  enable - if 0 then disable SPI
 *				  	if 1 then enable SPI 
 *
 */
void SPI_Periph_Ctrl(SPI_TypeDef* pSPIx, bool enable){
	/* Enable the SPI peripheral */
	if (enable == SPI_DEVICE_ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SPE_BIT);
	}
	/* Disable the SPI peripheral */
	else if (enable == SPI_DEVICE_DISABLE) {
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE_BIT);
	}
}

/**
 * @brief  SPI clock control API
 *
 * @param  pSPIx  - SPI peripheral ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void SPI_Clock_Ctrl(SPI_TypeDef* pSPIx, bool enable){
	/* Enable the clock */
	if (enable == SPI_CLK_ENABLE){
		if (pSPIx == SPI1)
			SPI1_CLK_ENABLE();
		else if (pSPIx == SPI2)
			SPI2_CLK_ENABLE();
		else if (pSPIx == SPI3)
			SPI3_CLK_ENABLE();
	}
	/* Disable the clock */
	else {
		if (pSPIx == SPI1)
			SPI1_CLK_DISABLE();
		else if (pSPIx == SPI2)
			SPI2_CLK_DISABLE();
		else if (pSPIx == SPI3)
			SPI3_CLK_DISABLE();
	}
}

/**
 * @brief  SPI data transmit API (Blocking mode)
 *
 * @param  pSPIx    - SPI peripheral ID
 * @param  TxBuffer - buffer holding data to be transmitted
 * @param  len      - length of the data to be transmitted
 * @return None
 *
 */
void SPI_Tx_Blocking(SPI_TypeDef* pSPIx, uint8_t* TxBuffer, uint32_t len) {
	/* Enable the SPI peripheral */
	SPI_Periph_Ctrl(pSPIx, SPI_DEVICE_ENABLE);

	/* Check if len > 0 then proceed for transmission */
	while (len > 0) {
		/* Check if transmit buffer is empty */
		if (SPI_GetFlagStatus(pSPIx, SPI_SR_TXE_BIT)){
			/* Check the data frame format */
			if (!(pSPIx->CR1 & (1 << SPI_CR1_DFF_BIT))){
				/* 8-bit format */
				pSPIx->DR = *TxBuffer;
				len--;
				TxBuffer++;
			} else {
				/* 16-bit format */
				if (len == 1) {
					pSPIx->DR = (uint16_t)(*TxBuffer);
					len -= 1;
				} else {
					pSPIx->DR = *((uint16_t*)TxBuffer);
					len -= 2;
					TxBuffer += 2;
				}
			}
		}
	}
	/* Disable the SPI peripheral */
	SPI_Periph_Ctrl(pSPIx, SPI_DEVICE_DISABLE);
}

/**
 * @brief  SPI data receive API (Blocking mode)
 *
 * @param  pSPIx    - SPI peripheral ID
 * @param  RxBuffer - buffer to hold the received data
 * @param  len      - length of the data to be received
 * @return None
 *
 */
void SPI_Rx_Blocking(SPI_TypeDef* pSPIx, uint8_t* RxBuffer, uint32_t len){
	/* Enable the SPI peripheral */
	SPI_Periph_Ctrl(pSPIx, SPI_DEVICE_ENABLE);

	/* Check if len > 0 then proceed for reception */
	while (len > 0) {
		/* Check if receive buffer is not empty */
		if (SPI_GetFlagStatus(pSPIx, SPI_SR_RXNE_BIT)){
			/* Check the data frame format */
			if (!(pSPIx->CR1 & (1 << SPI_CR1_DFF_BIT))){
				/* 8-bit format */
                *RxBuffer = pSPIx->DR;
				len--;
				RxBuffer++;
			} else {
				/* 16-bit format */
				if (len == 1) {
                    *RxBuffer = (uint8_t)pSPIx->DR;
					len -= 1;
				} else {
                    *((uint16_t*)RxBuffer) = pSPIx->DR;
					len -= 2;
					RxBuffer += 2;
				}
			}
		}
	}
	/* Disable the SPI peripheral */
	SPI_Periph_Ctrl(pSPIx, SPI_DEVICE_DISABLE);
}

/**
 * @brief  API to configure SPI IRQ
 *
 * @param  IRQNumber   - Interrupt number
 * @param  IRQPriority - Interrupt priority
 * @param  IRQNumber   - IRQ number of the GPIO 
 * @param  enable      - if 0 then disable interrupt
 *				  		 if 1 then enable interrupt
 *
 */
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable){
}

/**
 * @brief  API for SPI interrupt handler
 *
 * @param  pSPIHandle - SPI handle structure variable
 *
 */
void SPI_IRQHandler(SPI_Handle_t* pSPIHandle){
}



