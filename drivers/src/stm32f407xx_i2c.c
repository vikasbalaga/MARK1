/*
 * stm32f407xx_i2c.c
 *
 *  Created on: Oct 11, 2025
 *      Author: vbalaga
 */
#include "stm32f407xx_i2c.h"
#include "stm32f407xx_rcc.h"

/* I2C driver helper functions */

static void I2C_Generate_Start_Cond(I2C_TypeDef* pI2Cx) {
    pI2Cx->CR1 |= (1 << I2C_CR1_START_BIT);
}

static void I2C_Generate_Stop_Cond(I2C_TypeDef* pI2Cx) {
    pI2Cx->CR1 |= (1 << I2C_CR1_STOP_BIT);
}

static void I2C_Execute_AddrPhase(I2C_TypeDef* pI2Cx, uint8_t SlaveAddr, uint8_t do_read) {
    SlaveAddr = SlaveAddr << 1;
    // Need to send 7-bit slave address along with R/nW bit as LSB
    if (do_read)
        SlaveAddr |= (1 << 0);
    else
        SlaveAddr &= (~(1 << 0));
    pI2Cx->DR = SlaveAddr;
}

static void I2C_Clear_AddrFlag(I2C_TypeDef* pI2Cx) {
    //Read SR1 and SR2 registers to clear the ADDR flag
    uint32_t temp = pI2Cx->SR1;
    temp = pI2Cx->SR2;
    (void)temp;
}

/* I2C IRQ (Non Blocking) helper functions */

static void I2C_Tx_NonBlocking_IRQHandler(I2C_Handle_t* pI2CHandle) {
    // Write to DR
    pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);
    (pI2CHandle->pTxBuffer)++;
    pI2CHandle->TxLen--;
}

static void I2C_Rx_NonBlocking_IRQHandler(I2C_Handle_t* pI2CHandle) {
    if ((pI2CHandle->RxLen == 2) || pI2CHandle->Rx1ByteFlag) {
        if (!pI2CHandle->Rx1ByteFlag)
            //Disable ACK bit in CR1 register
            pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_BIT);
        
        // Generate stop condition
        I2C_Generate_Stop_Cond(pI2CHandle->pI2Cx);
    }
    *(pI2CHandle->pRxBuffer) = pI2CHandle->pI2Cx->DR;
    (pI2CHandle->pRxBuffer)++;
    (pI2CHandle->RxLen)--;
}

static void I2C_Evt_Callback(I2C_Handle_t* pI2CHandle, uint8_t event) {
    if (pI2CHandle->cb_fn) {
        pI2CHandle->cb_fn(pI2CHandle->cb_params, event);
    }
}

static void I2C_Clear_Handle(I2C_Handle_t* pI2CHandle) {
    // Clear the irq enable flags
    pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN_BIT);
    pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN_BIT);

    if (pI2CHandle->DeviceState == I2C_BUSY_TX) {
        pI2CHandle->pTxBuffer = NULL;
        pI2CHandle->TxLen = 0;
        pI2CHandle->DeviceAddr = 0;
    } else if (pI2CHandle->DeviceState == I2C_BUSY_RX) {
        pI2CHandle->pRxBuffer = NULL;
        pI2CHandle->RxLen = 0;
        pI2CHandle->Rx1ByteFlag = 0;
        pI2CHandle->DeviceAddr = 0;
        
        // Re configure the ACKing bit as per init params
        pI2CHandle->pI2Cx->CR1 |= ((pI2CHandle->I2C_Config.ACK_Control) << I2C_CR1_ACK_BIT);
    }
    pI2CHandle->DeviceState = I2C_READY;
}

/* I2C Driver APIs */

/**
 * @brief  I2C initialization API
 *
 * @param  pI2CHandle - I2C handle
 *
 */
void I2C_Init(I2C_Handle_t* pI2CHandle) {
    // Enable peripheral clock
    I2C_Clock_Ctrl(pI2CHandle->pI2Cx, I2C_CLK_ENABLE);
    
    //Configure FREQ field of CR2 register
    uint8_t freq = (RCC_GetPCLK1() / 1000000U);
    pI2CHandle->pI2Cx->CR2 |= (freq & 0x3F);
    
    //Configure slave address in OAR1 register
    pI2CHandle->pI2Cx->OAR1 |= ((pI2CHandle->I2C_Config.DeviceAddr) << 1);
    
    //The 14th bit of OAR1 should be set to 1 by software
    pI2CHandle->pI2Cx->OAR1 |= (1 << 14);
    
    //Configure CCR bits in CCR register
    uint16_t ccr_value = 0;
    float Tpclk1_us = 0, Tscl_us = 0;

    Tscl_us = (1 / (pI2CHandle->I2C_Config.SCL_Speed));
    Tpclk1_us = (1 / RCC_GetPCLK1());

    if (pI2CHandle->I2C_Config.SCL_Speed <= I2C_SPEED_SM) {
        /* Standard mode -> Tscl = (2 * CCR * Tpclk1) */
        ccr_value = (uint16_t)(Tscl_us / (2 * Tpclk1_us));
    } else if (pI2CHandle->I2C_Config.SCL_Speed <= I2C_SPEED_FM) {
        // Configure the fast mode
        pI2CHandle->pI2Cx->CCR |= (1 << 15);
        
        // Configure the duty cycles
        pI2CHandle->pI2Cx->CCR |= (pI2CHandle->I2C_Config.FMDutyCycle << 14);
        
        /* Fast mode */
        if (pI2CHandle->I2C_Config.FMDutyCycle == I2C_FM_DUTY_CYCL_2) {
            /* For Duty cycle = 0 -> Tscl = (3 * CCR * Tpclk1) */
            ccr_value = (uint16_t)(Tscl_us / (3 * Tpclk1_us));
        } else if (pI2CHandle->I2C_Config.FMDutyCycle == I2C_FM_DUTY_CYCL_16_9) {
            /* For Duty cycle = 1 -> Tscl = (25 * CCR * Tpclk1) */
            ccr_value = (uint16_t)(Tscl_us / (25 * Tpclk1_us));
        }
    }
    pI2CHandle->pI2Cx->CCR |= (ccr_value & 0xFFF);
    
    uint8_t temp;
    // Configure TRISE register
    if (pI2CHandle->I2C_Config.SCL_Speed <= I2C_SPEED_SM) {
        // Standard mode
        temp = (RCC_GetPCLK1() / 1000000U)  + 1;
    } else if (pI2CHandle->I2C_Config.SCL_Speed <= I2C_SPEED_FM) {
        temp = ((RCC_GetPCLK1() * 300) / 1000000000U)  + 1;
    }
    pI2CHandle->pI2Cx->TRISE |= (temp & 0x3f);

    // Enable the I2C peripheral
    I2C_Periph_Ctrl(pI2CHandle->pI2Cx, I2C_DEVICE_ENABLE);
  
    //Configure ACK bit in CR1 register
    pI2CHandle->pI2Cx->CR1 |= ((pI2CHandle->I2C_Config.ACK_Control) << I2C_CR1_ACK_BIT);
}

/**
 * @brief  I2C peripheral control API
 *
 * @param  pI2Cx  - I2C peripheral ID
 * @param  enable - if 0 then disable I2C
 *				  	if 1 then enable I2C 
 *
 */
void I2C_Periph_Ctrl(I2C_TypeDef* pI2Cx, bool enable) {
	/* Enable the I2C peripheral */
	if (enable == I2C_DEVICE_ENABLE){
		pI2Cx->CR1 |= (1 << I2C_CR1_PE_BIT);
	}
	/* Disable the I2C peripheral */
	else if (enable == I2C_DEVICE_DISABLE) {
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE_BIT);
	}
}

/**
 * @brief  I2C de-initialization API
 *
 * @param  pI2Cx - I2C peripheral ID
 *
 */
void I2C_Deinit(I2C_TypeDef* pI2Cx) {
	/* Reset the I2C peripheral */
	if (pI2Cx == I2C1)
		I2C1_REG_RESET();
	else if (pI2Cx == I2C2)
		I2C2_REG_RESET();
	else if (pI2Cx == I2C3)
		I2C3_REG_RESET();
}

/**
 * @brief  I2C clock control API
 *
 * @param  pI2Cx  - I2C peripheral ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void I2C_Clock_Ctrl(I2C_TypeDef* pI2Cx, bool enable) {
	/* Enable the clock */
	if (enable == I2C_CLK_ENABLE){
		if (pI2Cx == I2C1)
			I2C1_CLK_ENABLE();
		else if (pI2Cx == I2C2)
			I2C2_CLK_ENABLE();
		else if (pI2Cx == I2C3)
			I2C3_CLK_ENABLE();
	}
	/* Disable the clock */
	else {
		if (pI2Cx == I2C1)
			I2C1_CLK_DISABLE();
		else if (pI2Cx == I2C2)
			I2C2_CLK_DISABLE();
		else if (pI2Cx == I2C3)
			I2C3_CLK_DISABLE();
	}
}

/**
 * @brief  I2C Master Transmit API (Blocking mode)
 *
 * @param  pI2CHandle - I2C handle
 * @param  TxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @param  SlaveAddr  - slave address to which data needs to be transmitted
 *
 */
void I2C_Master_Tx_Blocking(I2C_Handle_t* pI2CHandle, uint8_t* TxBuffer, uint32_t len, uint8_t SlaveAddr) {
    // Generate start condition
    I2C_Generate_Start_Cond(pI2CHandle->pI2Cx);

    // Check the SB flag in SR1 register and wait until it is set
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_SB_BIT));
    
    //Initiate address phase
    I2C_Execute_AddrPhase(pI2CHandle->pI2Cx, SlaveAddr, I2C_OPERATION_WRITE);
    
    //Confirm address phase is done, by checking the ADDR flag in SR1 register
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_ADDR_BIT));
    
    //Clear the address flag by reading SR1 and SR2 registers
    I2C_Clear_AddrFlag(pI2CHandle->pI2Cx);
    
    //Transmit data until length becomes zero
    while(len > 0) {
        // Wait until TXE flag is set
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TXE_BIT));
        pI2CHandle->pI2Cx->DR = *TxBuffer;
        TxBuffer++;
        len--;
    }
    
    // After len becomes zero, wait until TXE and BTF flags are set
    while(!(I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TXE_BIT) && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_BTF_BIT)));
    
    // Generate stop condition
    I2C_Generate_Stop_Cond(pI2CHandle->pI2Cx);
}

/**
 * @brief  I2C Master Receive API (Blocking mode)
 *
 * @param  pI2CHandle - I2C handle
 * @param  RxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @param  SlaveAddr  - slave address to which data needs to be transmitted
 *
 */
void I2C_Master_Rx_Blocking(I2C_Handle_t* pI2CHandle, uint8_t* RxBuffer, uint32_t len, uint8_t SlaveAddr) {
    // Generate start condition
    I2C_Generate_Start_Cond(pI2CHandle->pI2Cx);

    // Check the SB flag in SR1 register and wait until it is set
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_SB_BIT));

    //Initiate address phase
    I2C_Execute_AddrPhase(pI2CHandle->pI2Cx, SlaveAddr, I2C_OPERATION_READ);

    //Confirm address phase is done, by checking the ADDR flag in SR1 register
    while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_ADDR_BIT));

    if (len == 1) {
        // In this case first disable ACK, after receiving 1 byte, end reception with STOP condition and then read from DR
        //Disable ACK bit in CR1 register
        pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_BIT);

        //Clear the address flag by reading SR1 and SR2 registers
        I2C_Clear_AddrFlag(pI2CHandle->pI2Cx);

        // Wait until RXNE flag is set
        while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_RXNE_BIT));

        // Generate stop condition
        I2C_Generate_Stop_Cond(pI2CHandle->pI2Cx);

        // Receive the data
        *RxBuffer = pI2CHandle->pI2Cx->DR;
        return;
    } else {
        //Clear the address flag by reading SR1 and SR2 registers
        I2C_Clear_AddrFlag(pI2CHandle->pI2Cx);

        //Receive data until length becomes zero
        while(len > 0) {
            // Wait until RXNE flag is set
            while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_RXNE_BIT));
            if (len == 2) {
                // In this case first disable ACK, after receiving the byte, end reception with STOP condition
                //Disable ACK bit in CR1 register
                pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_BIT);
                
                // Generate stop condition
                I2C_Generate_Stop_Cond(pI2CHandle->pI2Cx);
            }
            *RxBuffer = pI2CHandle->pI2Cx->DR;
            RxBuffer++;
            len--;
        }
    }
    //Re configure ACK bit in CR1 register
    pI2CHandle->pI2Cx->CR1 |= ((pI2CHandle->I2C_Config.ACK_Control) << I2C_CR1_ACK_BIT);
}

/**
 * @brief  I2C Master Transmit API (Non Blocking mode)
 *
 * @param  pI2CHandle - I2C handle
 * @param  TxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @param  SlaveAddr  - slave address to which data needs to be transmitted
 * @return status     - I2C device state
 *
 */
uint8_t I2C_Master_Tx_NonBlocking(I2C_Handle_t* pI2CHandle, uint8_t* TxBuffer, uint32_t len, uint8_t SlaveAddr) {
    // Check if device is not busy
    uint8_t state = pI2CHandle->DeviceState;
    if (state == I2C_READY) {
        state = I2C_BUSY_TX;

        // Configure the transmission handle structure
        pI2CHandle->pTxBuffer = TxBuffer;
        pI2CHandle->TxLen = len;
        pI2CHandle->DeviceAddr = SlaveAddr;

        // Generate START condition
        I2C_Generate_Start_Cond(pI2CHandle->pI2Cx);
        
        //Enable event and error irq enable flags
        pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR2_ITERREN_BIT);
        pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR2_ITEVTEN_BIT);
        pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR2_ITBUFEN_BIT);
    }
    return state;
}

/**
 * @brief  I2C Master Receive API (Non Blocking mode)
 *
 * @param  pI2CHandle - I2C handle
 * @param  RxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @param  SlaveAddr  - slave address to which data needs to be transmitted
 * @return status     - I2C device state
 *
 */
uint8_t I2C_Master_Rx_NonBlocking(I2C_Handle_t* pI2CHandle, uint8_t* RxBuffer, uint32_t len, uint8_t SlaveAddr) {
    // Check if device is not busy
    uint8_t state = pI2CHandle->DeviceState;
    if (state == I2C_READY) {
        state = I2C_BUSY_RX;

        // Configure the reception handle structure
        pI2CHandle->pRxBuffer = RxBuffer;
        pI2CHandle->RxLen = len;
        pI2CHandle->DeviceAddr = SlaveAddr;

        // Generate START condition
        I2C_Generate_Start_Cond(pI2CHandle->pI2Cx);
        
        //Enable event and error irq enable flags
        pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR2_ITERREN_BIT);
        pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR2_ITEVTEN_BIT);
        pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR2_ITBUFEN_BIT);
    }
    return state;
}


/**
 * @brief  API to configure I2C IRQ
 *
 * @param  IRQNumber   - Interrupt number
 * @param  IRQPriority - Interrupt priority
 * @param  IRQNumber   - IRQ number of the GPIO 
 * @param  enable      - if 0 then disable interrupt
 *				  		 if 1 then enable interrupt
 *
 */
void I2C_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable) {
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
 * @brief  API for I2C interrupt handler
 *
 * @param  pI2CHandle - I2C handle structure variable
 *
 */
void I2C_IRQHandler(I2C_Handle_t* pI2CHandle) {
    uint32_t evten, erren, bufen;
    evten = (pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN_BIT));
    erren = (pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITERREN_BIT));
    bufen = (pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN_BIT));
    
    // Check for event irq
    
    // Check for SB event
    if (evten && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_SB_BIT)) {
        // Initiate address phase
        if (pI2CHandle->DeviceState == I2C_BUSY_TX)
            I2C_Execute_AddrPhase(pI2CHandle->pI2Cx, pI2CHandle->DeviceAddr, I2C_OPERATION_WRITE);
        else if (pI2CHandle->DeviceState == I2C_BUSY_TX)
            I2C_Execute_AddrPhase(pI2CHandle->pI2Cx, pI2CHandle->DeviceAddr, I2C_OPERATION_READ);
    }
    
    // Check for ADDR event
    if (evten && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_ADDR_BIT)) {
        if ((pI2CHandle->DeviceState == I2C_BUSY_RX) && (pI2CHandle->Rx1ByteFlag)) {
            // Disable ACKing
            pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK_BIT);
        }
            I2C_Clear_AddrFlag(pI2CHandle->pI2Cx);
    }

    // Check for BTF event
    if (evten && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_BTF_BIT)) {
        // In case of transmission, it means shift and data registers are empty, so end transmission
        if (pI2CHandle->DeviceState == I2C_BUSY_TX) {
            if (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TXE_BIT)) {
                if (pI2CHandle->TxLen == 0) {
                    // Generate stop condition
                    I2C_Generate_Stop_Cond(pI2CHandle->pI2Cx);
                    I2C_Evt_Callback(pI2CHandle, I2C_EVT_TX_COMPLETE);
                }
            }
        } else if (pI2CHandle->DeviceState == I2C_BUSY_TX) {
            // Do nothing
        }
    }

    // Check for STOPF event
    if (evten && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_STOPF_BIT)) {
        /* This event will occur only for slave, when it detects STOP condition
           To clear it, read SR1 followed by writing something to CR1 */
           pI2CHandle->pI2Cx->CR1 |= (0x0000);
           
           // Clear the handle params
           
           
           // Call application callback
           I2C_Evt_Callback(pI2CHandle, I2C_EVT_STOP_COMPLETE);
    }

    // Check for TXE event
    if (evten && bufen && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TXE_BIT)) {
        // Perform transmission only if device is master
        if (pI2CHandle->pI2Cx->SR2 &= (1 << I2C_SR2_MSL_BIT)) {
            if ((pI2CHandle->DeviceState == I2C_BUSY_TX) && pI2CHandle->TxLen)
                I2C_Tx_NonBlocking_IRQHandler(pI2CHandle);
            if (pI2CHandle->TxLen == 0) {
                // Clear the Handle params
                I2C_Clear_Handle(pI2CHandle);
                
                // Call application callback
                I2C_Evt_Callback(pI2CHandle, I2C_EVT_RX_COMPLETE);
            }
        }
    }

    // Check for RXNE event
    if (evten && bufen && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_RXNE_BIT)) {
        if ((pI2CHandle->DeviceState == I2C_BUSY_RX) && pI2CHandle->RxLen) {
            I2C_Rx_NonBlocking_IRQHandler(pI2CHandle);
        }
        if (pI2CHandle->RxLen == 0) {
            // Clear the Handle params
            I2C_Clear_Handle(pI2CHandle);
            
            // Call application callback
            I2C_Evt_Callback(pI2CHandle, I2C_EVT_RX_COMPLETE);
        }
    }
    
    // Check for error irqs
    
    // Check for bus error
    if (erren && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_BERR_BIT)) {
        // Clear the flag and notify application
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_BERR_BIT)
        I2C_Evt_Callback(pI2CHandle, I2C_EVT_BERR);
    }
    
    // Check for ARLO error
    if (erren && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_ARLO_BIT)) {
        // Clear the flag and notify application
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_ARLO_BIT)
        I2C_Evt_Callback(pI2CHandle, I2C_EVT_ARLO);
    }

    // Check for AF error
    if (erren && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_AF_BIT)) {
        // Clear the flag and notify application
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_AF_BIT)
        I2C_Evt_Callback(pI2CHandle, I2C_EVT_AF);
    }

    // Check for OVR error
    if (erren && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_OVR_BIT)) {
        // Clear the flag and notify application
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_OVR_BIT)
        I2C_Evt_Callback(pI2CHandle, I2C_EVT_OVR);
    }

    // Check for TIMEOUT error
    if (erren && I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TIMEOUT_BIT)) {
        // Clear the flag and notify application
        pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_TIMEOUT_BIT)
        I2C_Evt_Callback(pI2CHandle, I2C_EVT_TIMEOUT);
    }
}