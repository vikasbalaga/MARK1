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
void SPI_Init(SPI_Handle_t* pSPIHandle) {
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
    pSPIHandle->Tx_State = SPI_READY;
    pSPIHandle->Rx_State = SPI_READY;
}

/**
 * @brief  SPI de-initialization API
 *
 * @param  pSPIx - SPI peripheral ID
 *
 */
void SPI_Deinit(SPI_TypeDef* pSPIx) {
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
void SPI_Periph_Ctrl(SPI_TypeDef* pSPIx, bool enable) {
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
void SPI_Clock_Ctrl(SPI_TypeDef* pSPIx, bool enable) {
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
void SPI_Rx_Blocking(SPI_TypeDef* pSPIx, uint8_t* RxBuffer, uint32_t len) {
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

/* SPI IRQ (Non Blocking) helper functions */
static void SPI_Tx_NonBlocking_IRQHandler(SPI_Handle_t* pSPIHandle);

static void SPI_Rx_NonBlocking_IRQHandler(SPI_Handle_t* pSPIHandle);

static void SPI_OVR_Err_IRQHandler(SPI_Handle_t* pSPIHandle);

static void SPI_Evt_Callback(SPI_Handle_t* pSPIHandle, uint8_t event);

/**
 * @brief  SPI data transmit API (Non Blocking mode)
 *
 * @param  pSPIHandle - SPI handle
 * @param  TxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @return status     - SPI device state
 *
 */
uint8_t SPI_Tx_NonBlocking(SPI_Handle_t* pSPIHandle, uint8_t* TxBuffer, uint32_t len) {
    /* Check if SPI device is not busy */
    uint8_t status = pSPIHandle->Tx_State;
    if (status != SPI_BUSY_TX) {
        /* Enable the SPI peripheral */
        SPI_Periph_Ctrl(pSPIHandle->pSPIx, SPI_DEVICE_ENABLE);

        /* Update the state of the SPI device */
        status = pSPIHandle->Tx_State = SPI_BUSY_TX;

        /* Assign Tx buffer and len to the state variables */
        pSPIHandle->TxBuffer = TxBuffer;
        pSPIHandle->TxLen = len;

        /* Enable the TXEIE flag */
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE_BIT);
    }
    return status;
}

/**
 * @brief  SPI data receive API (Non Blocking mode)
 *
 * @param  pSPIHandle - SPI handle
 * @param  RxBuffer   - buffer to hold the received data
 * @param  len        - lenght of the data to be received
 * @return status     - SPI device state
 *
 */
uint8_t SPI_Rx_NonBlocking(SPI_Handle_t* pSPIHandle, uint8_t* RxBuffer, uint32_t len) {
    /* Check if SPI device is not busy */
    uint8_t status = pSPIHandle->Rx_State;
    if (status != SPI_BUSY_RX) {
        /* Enable the SPI peripheral */
        SPI_Periph_Ctrl(pSPIHandle->pSPIx, SPI_DEVICE_ENABLE);

        /* Update the state of the SPI device */
        status = pSPIHandle->Rx_State = SPI_BUSY_RX;

        /* Assign Tx buffer and len to the state variables */
        pSPIHandle->RxBuffer = RxBuffer;
        pSPIHandle->RxLen = len;

        /* Enable the RXNEIE flag */
        pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE_BIT);
    }
    return status;
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
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable) {
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
 * @brief  API for SPI interrupt handler
 *
 * @param  pSPIHandle - SPI handle structure variable
 *
 */
void SPI_IRQHandler(SPI_Handle_t* pSPIHandle) {
    /* Check the TXE event in the status register */
    uint8_t txe_flag = SPI_GetFlagStatus(pSPIHandle->pSPIx, SPI_SR_TXE_BIT);
    uint8_t txe_irq_enable = (pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE_BIT));

    if (txe_flag && txe_irq_enable) {
        /* TXE event is set */
        SPI_Tx_NonBlocking_IRQHandler(pSPIHandle);
    }

    /* Check the RXNE event in the status register */
    uint8_t rxne_flag = SPI_GetFlagStatus(pSPIHandle->pSPIx, SPI_SR_RXNE_BIT);
    uint8_t rxne_irq_enable = (pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE_BIT));

    if (rxne_flag && rxne_irq_enable) {
        /* RXNE event is set */
        SPI_Rx_NonBlocking_IRQHandler(pSPIHandle);
    }

    /* Check the OVR event in the status register */
    uint8_t ovr_flag = SPI_GetFlagStatus(pSPIHandle->pSPIx, SPI_SR_OVR_BIT);
    uint8_t ovr_irq_enable = (pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE_BIT));

    if (ovr_flag && ovr_irq_enable) {
        /* OVR event is set */
        SPI_OVR_Err_IRQHandler(pSPIHandle);
    }
}

/* SPI IRQ (Non Blocking) helper functions */
static void SPI_Evt_Callback(SPI_Handle_t* pSPIHandle, uint8_t event) {
    if (pSPIHandle->cb_fn) {
        pSPIHandle->cb_fn(pSPIHandle->cb_params, event);
    }
}


static void SPI_Tx_NonBlocking_IRQHandler(SPI_Handle_t* pSPIHandle) {
	if (!(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF_BIT))){
		/* 8-bit format */
		pSPIHandle->pSPIx->DR = *(pSPIHandle->TxBuffer);
		(pSPIHandle->TxLen)--;
		(pSPIHandle->TxBuffer)++;
	} else {
		/* 16-bit format */
		if (pSPIHandle->TxLen == 1) {
			pSPIHandle->pSPIx->DR = (uint16_t)(*(pSPIHandle->TxBuffer));
			pSPIHandle->TxLen -= 1;
		} else {
			pSPIHandle->pSPIx->DR = *((uint16_t*)(pSPIHandle->TxBuffer));
			pSPIHandle->TxLen -= 2;
			pSPIHandle->TxBuffer += 2;
		}
	}

	/* Close the SPI transmission */
    if (!pSPIHandle->TxLen) {
        /* Disable the TXEIE */
        pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE_BIT);

        /* Reset Tx buffer */
        pSPIHandle->TxBuffer = NULL;
        
        /* Reset SPI device to ready state */
        pSPIHandle->Tx_State = SPI_READY;
        
        /* Disable the SPI peripheral */
        SPI_Periph_Ctrl(pSPIHandle->pSPIx, SPI_DEVICE_DISABLE);
        SPI_Evt_Callback(pSPIHandle, SPI_TX_COMPLETE);
    }
}

static void SPI_Rx_NonBlocking_IRQHandler(SPI_Handle_t* pSPIHandle) {
	/* Check the data frame format */
	if (!(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF_BIT))){
		/* 8-bit format */
        *pSPIHandle->RxBuffer = pSPIHandle->pSPIx->DR;
		(pSPIHandle->RxLen)--;
		(pSPIHandle->RxBuffer)++;
	} else {
		/* 16-bit format */
		if (pSPIHandle->RxLen == 1) {
            *(pSPIHandle->RxBuffer) = (uint8_t)(pSPIHandle->pSPIx->DR);
			pSPIHandle->RxLen -= 1;
		} else {
            *((uint16_t*)(pSPIHandle->RxBuffer)) = pSPIHandle->pSPIx->DR;
			pSPIHandle->RxLen -= 2;
			pSPIHandle->RxBuffer += 2;
		}
	}

	/* Close the SPI reception */
    if (!pSPIHandle->RxLen) {
        /* Disable the RXNEIE */
        pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE_BIT);

        /* Reset Rx buffer */
        pSPIHandle->RxBuffer = NULL;
        
        /* Reset SPI device to ready state */
        pSPIHandle->Rx_State = SPI_READY;
        
        /* Disable the SPI peripheral */
        SPI_Periph_Ctrl(pSPIHandle->pSPIx, SPI_DEVICE_DISABLE);
        SPI_Evt_Callback(pSPIHandle, SPI_RX_COMPLETE);
    }
}

static void SPI_OVR_Err_IRQHandler(SPI_Handle_t* pSPIHandle) {
    /* If SPI is busy in transmission, ignore OVR event */
    if (pSPIHandle->Tx_State != SPI_BUSY_TX) {
        /* To clear the OVR event, read DR followed by SR */
        uint8_t temp;
        temp = pSPIHandle->pSPIx->DR;
        temp = pSPIHandle->pSPIx->SR;
        (void)temp;
        SPI_Evt_Callback(pSPIHandle, SPI_OVR_ERR);
    }
}

