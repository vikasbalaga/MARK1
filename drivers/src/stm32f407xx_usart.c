/*
 * stm32f407xx_usart.c
 *
 *  Created on: Nov 15, 2025
 *      Author: vbalaga
 */

#include "stm32f407xx_usart.h"
#include "stm32f407xx_rcc.h"

/* USART Driver APIs */

/**
 * @brief  USART initialization API
 *
 * @param  pUSARTHandle - USART handle
 *
 */
void USART_Init(USART_Handle_t* pUSARTHandle) {
    /* Enable the clock */
    USART_Clock_Ctrl(pUSARTHandle->pUSARTx, USART_CLK_ENABLE);
    
    /* Configure the mode of the USART peripheral */
    if ((pUSARTHandle->USART_Config.Mode == USART_MODE_TX_ONLY) || (pUSARTHandle->USART_Config.Mode == USART_MODE_TX_RX))
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TE_Bit);
    if ((pUSARTHandle->USART_Config.Mode == USART_MODE_RX_ONLY) || (pUSARTHandle->USART_Config.Mode == USART_MODE_TX_RX))
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RE_Bit);

    /* Configure the stop bits of the USART peripheral */
    pUSARTHandle->pUSARTx->CR2 |= (pUSARTHandle->USART_Config.StopBits << USART_CR2_STOP_Bit);

    /* Configure the word length of the USART peripheral */
    pUSARTHandle->pUSARTx->CR1 |= (pUSARTHandle->USART_Config.WordLength << USART_CR1_M_Bit);

    /* Configure the parity control of the USART peripheral */
    if (pUSARTHandle->USART_Config.ParityCTRL != USART_PARITY_NONE) {
        /* Enable the parity control */
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_PCE_Bit);
        if (pUSARTHandle->USART_Config.ParityCTRL == USART_PARITY_ODD)
            pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_PS_Bit); //Odd parity
        else if (pUSARTHandle->USART_Config.ParityCTRL == USART_PARITY_EVEN)
            pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_PS_Bit); //Even parity
    }        

    /* Configure the HW flow control of the USART peripheral */
    if (pUSARTHandle->USART_Config.HwFlowControl != USART_HW_CTRL_NONE) {
        if ((pUSARTHandle->USART_Config.HwFlowControl == USART_HW_CTRL_CTS) || (pUSARTHandle->USART_Config.HwFlowControl == USART_HW_CTRL_CTS_RTS))
            pUSARTHandle->pUSARTx->CR3 |= (1 << USART_CR3_RTSE_Bit);
        if ((pUSARTHandle->USART_Config.HwFlowControl == USART_HW_CTRL_RTS) || (pUSARTHandle->USART_Config.HwFlowControl == USART_HW_CTRL_CTS_RTS))
            pUSARTHandle->pUSARTx->CR3 |= (1 << USART_CR3_CTSE_Bit);
    }

    /* Configure the oversampling mode */
    if (pUSARTHandle->USART_Config.OversamplingMode == OVERSAMPLING_MODE_8)
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_OVER8_Bit);
    else if (pUSARTHandle->USART_Config.OversamplingMode == OVERSAMPLING_MODE_16)
        pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_OVER8_Bit);

    /* Configure the Baud rate of the USART peripheral */
    USART_SetBaudRate(pUSARTHandle);

    // Enable the USART peripheral
    USART_Periph_Ctrl(pUSARTHandle->pUSARTx, USART_DEVICE_ENABLE);
}

/**
 * @brief  USART peripheral control API
 *
 * @param  pUSARTx  - USART peripheral ID
 * @param  enable - if 0 then disable USART
 *				  	if 1 then enable USART 
 *
 */
void USART_Periph_Ctrl(USART_TypeDef* pUSARTx, bool enable) {
	/* Enable the USART peripheral */
	if (enable == USART_DEVICE_ENABLE){
		pUSARTx->CR1 |= (1 << USART_CR1_UE_Bit);
	}
	/* Disable the USART peripheral */
	else if (enable == USART_DEVICE_DISABLE) {
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE_Bit);
	}
}

/**
 * @brief  USART de-initialization API
 *
 * @param  pUSARTx - USART peripheral ID
 *
 */
void USART_Deinit(USART_TypeDef* pUSARTx) {
	/* Reset the USART peripheral */
	if (pUSARTx == USART1)
		USART1_REG_RESET();
	else if (pUSARTx == USART2)
		USART2_REG_RESET();
	else if (pUSARTx == USART3)
		USART3_REG_RESET();
	else if (pUSARTx == USART6)
		USART6_REG_RESET();
}

/**
 * @brief  USART clock control API
 *
 * @param  pUSARTx  - USART peripheral ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void USART_Clock_Ctrl(USART_TypeDef* pUSARTx, bool enable) {
	/* Enable the clock */
	if (enable == USART_CLK_ENABLE){
		if (pUSARTx == USART1)
			USART1_CLK_ENABLE();
		else if (pUSARTx == USART2)
			USART2_CLK_ENABLE();
		else if (pUSARTx == USART3)
			USART3_CLK_ENABLE();
		else if (pUSARTx == USART6)
			USART6_CLK_ENABLE();
	}
	/* Disable the clock */
	else {
		if (pUSARTx == USART1)
			USART1_CLK_DISABLE();
		else if (pUSARTx == USART2)
			USART2_CLK_DISABLE();
		else if (pUSARTx == USART3)
			USART3_CLK_DISABLE();
		else if (pUSARTx == USART6)
			USART6_CLK_DISABLE();
	}
}

/**
 * @brief  USART Baud rate configuration API
 *
 * @param  pUSARTHandle - USART handle
 *
 */
void USART_SetBaudRate(USART_Handle_t* pUSARTHandle)
{
    uint32_t pclk, usartdiv, mantissa, fraction;
    uint8_t over8 = (pUSARTHandle->pUSARTx->CR1 >> USART_CR1_OVER8_Bit) & 0x01;

    /* 1. Get peripheral clock frequency */
    if ((pUSARTHandle->pUSARTx == USART1) || (pUSARTHandle->pUSARTx == USART6))
        pclk = RCC_GetPCLK2();
    else
        pclk = RCC_GetPCLK1();

    uint32_t baud = pUSARTHandle->USART_Config.BaudRate;

    /*
        Formula from Reference Manual:
        Oversampling by 16:
            USARTDIV = pclk / (16 * baud)

        Oversampling by 8:
            USARTDIV = pclk / (8 * baud)
    */

    if (over8 == 0)
    {
        /* -------- Oversampling by 16 -------- */
        usartdiv = (pclk + (baud / 2U)) / (baud * 16U);    // mantissa
        mantissa = usartdiv;

        fraction = (pclk / baud) - (mantissa * 16U);       // fraction * 16
        fraction &= 0x0F;                                  // lower 4 bits

        pUSARTHandle->pUSARTx->BRR = (mantissa << 4) | fraction;
    }
    else
    {
        /* -------- Oversampling by 8 -------- */
        usartdiv = (pclk + (baud / 2U)) / (baud * 8U);     // mantissa
        mantissa = usartdiv;

        fraction = (pclk / baud) - (mantissa * 8U);        // fraction * 8
        fraction &= 0x07;                                  // lower 3 bits

        pUSARTHandle->pUSARTx->BRR = (mantissa << 4) | fraction;
    }
}


/**
 * @brief  USART Transmit API (Blocking mode)
 *
 * @param  pUSARTHandle - USART handle
 * @param  pTxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 *
 */
void USART_Tx_Blocking(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t len) {
    uint16_t* pData;
    
    // Enable the USART peripheral
//    USART_Periph_Ctrl(pUSARTHandle->pUSARTx, USART_DEVICE_ENABLE);

    while (len > 0) {
        /* Wait until TXE flag is set */
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TXE_Bit));

        /* Check for word length */
        if (pUSARTHandle->USART_Config.WordLength == USART_WORD_LEN_9) {
            pData = (uint16_t*)(pTxBuffer);
            pUSARTHandle->pUSARTx->DR = (*pData & (uint16_t)0x1FF);

            /* Check for partiy control bit */
            if (pUSARTHandle->USART_Config.ParityCTRL != USART_PARITY_NONE) {
                /* Parity is enabled, so 9th bit will be parity itself, increment the buffer only once */
                pTxBuffer++;
                len -= 1;
            } else {
                pTxBuffer++;
                pTxBuffer++;
                len -= 2;
            }
        } else if (pUSARTHandle->USART_Config.WordLength == USART_WORD_LEN_8) {
            //pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);
        	uint8_t data = (*pTxBuffer & (uint8_t)0xFF);
        	pUSARTHandle->pUSARTx->DR = data;
            pTxBuffer++;
            len -= 1;
        }
    }

    /* Wait until TC flag is set in SR */
    while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TC_Bit));
}

/**
 * @brief  USART Receive API (Blocking mode)
 *
 * @param  pUSARTHandle - USART handle
 * @param  pRxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 *
 */
void USART_Rx_Blocking(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t len) {
    while (len > 0) {
        /* Wait until RXNE flag is set */
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_RXNE_Bit));

        /* Check for word length */
        if (pUSARTHandle->USART_Config.WordLength == USART_WORD_LEN_9) {
            *((uint16_t*)(pRxBuffer)) = (uint16_t)(pUSARTHandle->pUSARTx->DR & 0x1FF);
            if (pUSARTHandle->USART_Config.ParityCTRL != USART_PARITY_NONE) {
                /* Parity is enabled, so 9th bit will be parity itself, increment the buffer only once */
                len -= 1;
                pRxBuffer++;
            } else {
                len -= 2;
                pRxBuffer++;
                pRxBuffer++;
            }
        } else if (pUSARTHandle->USART_Config.WordLength == USART_WORD_LEN_8) {
            *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->DR & 0xFF);
            len -= 1;
            pRxBuffer++;
        }
    }
}

/**
 * @brief  USART Transmit API (Non Blocking mode)
 *
 * @param  pUSARTHandle - USART handle
 * @param  pTxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @return status     - USART device state
 *
 */
uint8_t USART_Tx_NonBlocking(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t len);

/**
 * @brief  USART Receive API (Non Blocking mode)
 *
 * @param  pUSARTHandle - USART handle
 * @param  pRxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @return status     - USART device state
 *
 */
uint8_t USART_Rx_NonBlocking(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t len);

/**
 * @brief  API to configure USART IRQ
 *
 * @param  IRQNumber   - Interrupt number
 * @param  IRQPriority - Interrupt priority
 * @param  IRQNumber   - IRQ number of the GPIO 
 * @param  enable      - if 0 then disable interrupt
 *				  		 if 1 then enable interrupt
 *
 */
void USART_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable) {
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
 * @brief  API for USART interrupt handler
 *
 * @param  pUSARTHandle - USART handle structure variable
 *
 */
void USART_IRQHandler(USART_Handle_t* pUSARTHandle);
