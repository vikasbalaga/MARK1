/*
 * stm32f407xx_i2c.h
 *
 *  Created on: Oct 11, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_STM32F407XX_I2C_H_
#define INCLUDE_STM32F407XX_I2C_H_

#include <stddef.h>
#include <stdbool.h>
#include "stm32f407xx.h"

typedef struct {
    uint32_t SCL_Speed;                     /*!< possible values from @I2C_SPEED_MODES>*/
    uint8_t DeviceAddr;                     /*!< 7-bit slave address, user configurable>*/
    uint8_t ACK_Control;                    /*!< possible values from @I2C_ACK_Control>*/
    uint16_t FMDutyCycle;                   /*!< possible values from @I2C_FM_DutyCycles>*/
} I2C_Config_t;

typedef void (*i2c_callback_t)(void*, uint8_t);

typedef struct {
    I2C_TypeDef* pI2Cx;                     /*!< I2C peripheral device ID>*/
    I2C_Config_t I2C_Config;                /*!< I2C Config>*/
    uint8_t* pTxBuffer;                     /*!< Tx buffer pointer>*/
    uint32_t TxLen;                         /*!< Tx buffer length>*/
    uint8_t* pRxBuffer;                     /*!< Rx buffer pointer>*/
    uint32_t RxLen;                         /*!< Rx buffer length>*/
    uint8_t DeviceState;                    /*!< Device state>*/
    uint8_t DeviceAddr;                     /*!< Slave device address>*/
    uint32_t Rx1ByteFlag;                   /*!< Flag to check if reception is for 1 byte only>*/
    uint8_t do_Sr;                          /*!< Perform repeated start>*/
    i2c_callback_t cb_fn;				    /*!< I2C call back function pointer>*/
	void* cb_params;					    /*!< I2C call back function params>*/
} I2C_Handle_t;

/*
 * @I2C_SPEED_MODES
 * I2C bus speed modes
 */
#define I2C_SPEED_SM                    100000U //Standard mode upto 100 kHz
#define I2C_SPEED_FM                    400000U //Fast mode upto 400 kHz

/*
 * @I2C_ACK_Control
 * I2C ACK control
 */
#define I2C_ACK_ENABLE                  1 //Enable ACKing
#define I2C_ACK_DISABLE                 0 //Disable ACKing

/*
 * @I2C_FM_DutyCycles
 * I2C fast mode duty cycles
 */
#define I2C_FM_DUTY_CYCL_2              0
#define I2C_FM_DUTY_CYCL_16_9           1

/* I2C peripheral enable and disable macros */
#define I2C_DEVICE_ENABLE 1
#define I2C_DEVICE_DISABLE 0

/* I2C clock enable and disable macros */
#define I2C_CLK_ENABLE 1
#define I2C_CLK_DISABLE 0

/*
 * I2C device state used in interrupt mode
 */
#define I2C_READY 0
#define I2C_BUSY_RX 1
#define I2C_BUSY_TX 2

/*
 * I2C event flags used in interrupt mode
 */
#define I2C_EVT_TX_COMPLETE 0
#define I2C_EVT_RX_COMPLETE 1
#define I2C_EVT_STOP_COMPLETE 2
#define I2C_EVT_BERR 3
#define I2C_EVT_ARLO 4
#define I2C_EVT_AF 5
#define I2C_EVT_OVR 6
#define I2C_EVT_TIMEOUT 7

/******************************************************************************************
I2C register bit position definitions
******************************************************************************************/

/* I2C CR1 register bit position definiitons */
#define I2C_CR1_PE_BIT 0
#define I2C_CR1_NOSTRETCH_BIT 7
#define I2C_CR1_START_BIT 8
#define I2C_CR1_STOP_BIT 9
#define I2C_CR1_ACK_BIT 10
#define I2C_CR1_SWRST_BIT 15

/* I2C CR2 register bit position definiitons */
#define I2C_CR2_FREQ_BIT 0
#define I2C_CR2_ITERREN_BIT 8
#define I2C_CR2_ITEVTEN_BIT 9
#define I2C_CR2_ITBUFEN_BIT 10

/* I2C OAR1 register bit position definiitons */
#define I2C_OAR1_ADD0_BIT 0
#define I2C_OAR1_ITERREN_ADD71_BIT 1
#define I2C_OAR1_ITEVTEN_98_BIT 8
#define I2C_OAR1_ADD_MODE_BIT 15

/* I2C SR1 register bit position definiitons */
#define I2C_SR1_SB_BIT 0
#define I2C_SR1_ADDR_BIT 1
#define I2C_SR1_BTF_BIT 2
#define I2C_SR1_ADD10_BIT 3
#define I2C_SR1_STOPF_BIT 4
#define I2C_SR1_RXNE_BIT 6
#define I2C_SR1_TXE_BIT 7
#define I2C_SR1_BERR_BIT 8
#define I2C_SR1_ARLO_BIT 9
#define I2C_SR1_AF_BIT 10
#define I2C_SR1_OVR_BIT 11
#define I2C_SR1_TIMEOUT_BIT 14

/* I2C SR2 register bit position definiitons */
#define I2C_SR2_MSL_BIT 0
#define I2C_SR2_BUSY_BIT 1
#define I2C_SR2_TRA_BIT 2
#define I2C_SR2_GENCALL_BIT 4
#define I2C_SR2_DUALF_BIT 7

/* I2C CCR register bit position definiitons */
#define I2C_CCR_CCR_BIT 0
#define I2C_CCR_DUTY_BIT 14
#define I2C_CCR_FS_BIT 15

/* I2C register reset macros */
#define I2C1_REG_RESET()		do {\
									RCC->APB2RSTR |= (1 << 21);\
									RCC->APB2RSTR &= ~(1 << 21);\
									}while(0)
#define I2C2_REG_RESET()		do {\
									RCC->APB1RSTR |= (1 << 22);\
									RCC->APB1RSTR &= ~(1 << 22);\
									}while(0)
#define I2C3_REG_RESET()		do {\
									RCC->APB1RSTR |= (1 << 23);\
									RCC->APB1RSTR &= ~(1 << 23);\
									}while(0)

#define I2C_BIT_RESET 0
#define I2C_BIT_SET	1

#define I2C_OPERATION_WRITE 0
#define I2C_OPERATION_READ 1

/* Get the status of a flag from SPI status register */
#define I2C_GetFlagStatus(pI2Cx, flag) 		((pI2Cx->SR1 & (1 << flag) ? I2C_BIT_SET :	I2C_BIT_RESET))

/* I2C Driver APIs */

/**
 * @brief  I2C initialization API
 *
 * @param  pI2CHandle - I2C handle
 *
 */
void I2C_Init(I2C_Handle_t* pI2CHandle);

/**
 * @brief  I2C peripheral control API
 *
 * @param  pI2Cx  - I2C peripheral ID
 * @param  enable - if 0 then disable I2C
 *				  	if 1 then enable I2C 
 *
 */
void I2C_Periph_Ctrl(I2C_TypeDef* pI2Cx, bool enable);

/**
 * @brief  I2C de-initialization API
 *
 * @param  pI2Cx - I2C peripheral ID
 *
 */
void I2C_Deinit(I2C_TypeDef* pI2Cx);

/**
 * @brief  I2C clock control API
 *
 * @param  pI2Cx  - I2C peripheral ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void I2C_Clock_Ctrl(I2C_TypeDef* pI2Cx, bool enable);

/**
 * @brief  I2C Master Transmit API (Blocking mode)
 *
 * @param  pI2CHandle - I2C handle
 * @param  TxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @param  SlaveAddr  - slave address to which data needs to be transmitted
 *
 */
void I2C_Master_Tx_Blocking(I2C_Handle_t* pI2CHandle, uint8_t* TxBuffer, uint32_t len, uint8_t SlaveAddr);

/**
 * @brief  I2C Master Receive API (Blocking mode)
 *
 * @param  pI2CHandle - I2C handle
 * @param  RxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @param  SlaveAddr  - slave address to which data needs to be transmitted
 *
 */
void I2C_Master_Rx_Blocking(I2C_Handle_t* pI2CHandle, uint8_t* RxBuffer, uint32_t len, uint8_t SlaveAddr);

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
uint8_t I2C_Master_Tx_NonBlocking(I2C_Handle_t* pI2CHandle, uint8_t* TxBuffer, uint32_t len, uint8_t SlaveAddr);

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
uint8_t I2C_Master_Rx_NonBlocking(I2C_Handle_t* pI2CHandle, uint8_t* RxBuffer, uint32_t len, uint8_t SlaveAddr);

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
void I2C_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable);

/**
 * @brief  API for I2C interrupt handler
 *
 * @param  pI2CHandle - I2C handle structure variable
 *
 */
void I2C_IRQHandler(I2C_Handle_t* pI2CHandle);

#endif /* INCLUDE_STM32F407XX_I2C_H_ */
