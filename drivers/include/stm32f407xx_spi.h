/*
 * stm32f407xx_spi.h
 *
 *  Created on: Jul 19, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_STM32F407XX_SPI_H_
#define INCLUDE_STM32F407XX_SPI_H_

#include <stddef.h>
#include <stdbool.h>
#include "stm32f407xx.h"

typedef struct {
	uint8_t SPI_DeviceMode;				/*!< possible values from @SPI_DEVICE_MODES>*/
	uint8_t SPI_BusConfig;				/*!< possible values from @SPI_BUS_CONFIG_MODES>*/
	uint8_t SPI_SclkSpeed;				/*!< possible values from @SPI_SCLK_SPEED_DIV>*/
	uint8_t SPI_DFF;					/*!< possible values from @SPI_DFF_TYPES>*/
	uint8_t SPI_CPOL;					/*!< possible values from @SPI_CPOL_TYPES>*/
	uint8_t SPI_CPHA;					/*!< possible values from @SPI_CPHA_TYPES>*/
	uint8_t SPI_SSM;					/*!< possible values from @SPI_SSM_FLAGS>*/
} SPI_Config_t;

typedef void (*spi_callback_t)(void*, uint8_t);

typedef struct {
	SPI_TypeDef* pSPIx;					/*!< SPI peripheral device ID>*/
	SPI_Config_t SPI_Config;			/*!< SPI Config>*/
	uint8_t* TxBuffer;					/*!< TX buffer pointer used in non blocking mode>*/
	uint8_t* RxBuffer;					/*!< RX buffer pointer used in non blocking mode>*/
	uint32_t TxLen;						/*!< TX buffer pointer length used in non blocking mode>*/
	uint32_t RxLen;						/*!< RX buffer pointer length used in non blocking mode>*/
	uint8_t Tx_State;					/*!< SPI transmission state used in non blocking mode>*/
	uint8_t Rx_State;					/*!< SPI receive state used in non blocking mode>*/
	spi_callback_t cb_fn;				/*!< SPI call back function pointer>*/
	void* cb_params;					/*!< SPI call back function params>*/
} SPI_Handle_t;

/* SPI peripheral enable and disable macros */
#define SPI_DEVICE_ENABLE 1
#define SPI_DEVICE_DISABLE 0

/* SPI clock enable and disable macros */
#define SPI_CLK_ENABLE 1
#define SPI_CLK_DISABLE 0

/*
 * @SPI_DEVICE_MODES
 * SPI device possible modes
 */
#define SPI_MODE_SLAVE 0
#define SPI_MODE_MASTER 1

/*
 * @SPI_BUS_CONFIG_MODES
 * SPI bus config modes
 */
#define SPI_BUS_MODE_FULL_DUPLEX 0
#define SPI_BUS_MODE_HALF_DUPLEX 1
#define SPI_BUS_MODE_SIMPLEX_RX 2

/*
 * @SPI_SCLK_SPEED_DIV
 * SPI SCLK speed divisors
 */
#define SPI_SCLK_SPEED_DIV2 0
#define SPI_SCLK_SPEED_DIV4 1
#define SPI_SCLK_SPEED_DIV8 2
#define SPI_SCLK_SPEED_DIV16 3
#define SPI_SCLK_SPEED_DIV32 4
#define SPI_SCLK_SPEED_DIV64 5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7

/*
 * @SPI_DFF_TYPES
 * SPI data frame format types
 */
#define SPI_DFF_SIZE_8 0
#define SPI_DFF_SIZE_16 1

/*
 * @SPI_CPOL_TYPES
 * SPI clock polarity types
 */
#define SPI_CPOL_LOW 0
#define SPI_CPOL_HIGH 1

/*
 * @SPI_CPHA_TYPES
 * SPI clock phase types
 */
#define SPI_CPHA_FIRST_EDGE 0
#define SPI_CPHA_SECOND_EDGE 1

/*
 * @SPI_SSM_FLAGS
 * SPI software slave management flags
 */
#define SPI_SSM_DISABLE 0   	/* Disable software slave management */
#define SPI_SSM_ENABLE 1		/* Enable software slave management */

/* SPI CR1 register bit position definiitons */
#define SPI_CR1_CPHA_BIT 0
#define SPI_CR1_CPOL_BIT 1
#define SPI_CR1_MSTR_BIT 2
#define SPI_CR1_BR_BIT 3		//Baud rate has 3 bits (3:5)
#define SPI_CR1_SPE_BIT 6
#define SPI_CR1_LSBFIRST_BIT 7
#define SPI_CR1_SSI_BIT 8
#define SPI_CR1_SSM_BIT 9
#define SPI_CR1_RXONLY_BIT 10
#define SPI_CR1_DFF_BIT 11
#define SPI_CR1_CRCNEXT_BIT 12
#define SPI_CR1_CRCEN_BIT 13
#define SPI_CR1_BIDIOE_BIT 14
#define SPI_CR1_BIDIMODE_BIT 15

/* SPI CR2 register bit position definiitons */
#define SPI_CR2_RXDMAEN_BIT 0
#define SPI_CR2_TXDMAEN_BIT 1
#define SPI_CR2_SSOE_BIT 2
#define SPI_CR2_FRF_BIT 4		//Bit 3 and bits (9-15) are reserved
#define SPI_CR2_ERRIE_BIT 5
#define SPI_CR2_RXNEIE_BIT 6
#define SPI_CR2_TXEIE_BIT 7

/* SPI SR register bit position definiitons */
#define SPI_SR_RXNE_BIT 0
#define SPI_SR_TXE_BIT 1
#define SPI_SR_CHSIDE_BIT 2
#define SPI_SR_UDR_BIT 3		// Bits (9-15) are reserved
#define SPI_SR_CRCERR_BIT 4
#define SPI_SR_MODF_BIT 5
#define SPI_SR_OVR_BIT 6
#define SPI_SR_BSY_BIT 7
#define SPI_SR_FRE_BIT 8

/*
 * SPI device state used in interrupt mode
 */
#define SPI_READY 0
#define SPI_BUSY_RX 1
#define SPI_BUSY_TX 2

/*
 * SPI event flags used in interrupt mode
 */
#define SPI_TX_COMPLETE 0
#define SPI_RX_COMPLETE 1
#define SPI_OVR_ERR 2

/* SPI register reset macros */
#define SPI1_REG_RESET()		do {\
									RCC->APB2RSTR |= (1 << 12);\
									RCC->APB2RSTR &= ~(1 << 12);\
									}while(0)
#define SPI2_REG_RESET()		do {\
									RCC->APB1RSTR |= (1 << 14);\
									RCC->APB1RSTR &= ~(1 << 14);\
									}while(0)
#define SPI3_REG_RESET()		do {\
									RCC->APB1RSTR |= (1 << 15);\
									RCC->APB1RSTR &= ~(1 << 15);\
									}while(0)

#define SPI_BIT_RESET 0
#define SPI_BIT_SET	1

/* Get the status of a flag from SPI status register */
#define SPI_GetFlagStatus(pSPIx, flag) 		((pSPIx->SR & (1 << flag) ? SPI_BIT_SET :	SPI_BIT_RESET))

/* SPI Driver APIs */

/**
 * @brief  SPI initialization API
 *
 * @param  pSPIHandle - SPI handle
 *
 */
void SPI_Init(SPI_Handle_t* pSPIHandle);

/**
 * @brief  SPI peripheral control API
 *
 * @param  pSPIx  - SPI peripheral ID
 * @param  enable - if 0 then disable SPI
 *				  	if 1 then enable SPI 
 *
 */
void SPI_Periph_Ctrl(SPI_TypeDef* pSPIx, bool enable);

/**
 * @brief  SPI de-initialization API
 *
 * @param  pSPIx - SPI peripheral ID
 *
 */
void SPI_Deinit(SPI_TypeDef* pSPIx);

/**
 * @brief  SPI clock control API
 *
 * @param  pSPIx  - SPI peripheral ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void SPI_Clock_Ctrl(SPI_TypeDef* pSPIx, bool enable);

/**
 * @brief  SPI data transmit API (Blocking mode)
 *
 * @param  pSPIx    - SPI peripheral ID
 * @param  TxBuffer - buffer holding data to be transmitted
 * @param  len      - lenght of the data to be transmitted
 * @return None
 *
 */
void SPI_Tx_Blocking(SPI_TypeDef* pSPIx, uint8_t* TxBuffer, uint32_t len);

/**
 * @brief  SPI data receive API (Blocking mode)
 *
 * @param  pSPIx    - SPI peripheral ID
 * @param  RxBuffer - buffer to hold the received data
 * @param  len      - lenght of the data to be received
 * @return None
 *
 */
void SPI_Rx_Blocking(SPI_TypeDef* pSPIx, uint8_t* RxBuffer, uint32_t len);

/**
 * @brief  SPI data transmit API (Non Blocking mode)
 *
 * @param  pSPIHandle - SPI handle
 * @param  TxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 * @return status     - SPI device state
 *
 */
uint8_t SPI_Tx_NonBlocking(SPI_Handle_t* pSPIHandle, uint8_t* TxBuffer, uint32_t len);

/**
 * @brief  SPI data receive API (Non Blocking mode)
 *
 * @param  pSPIHandle - SPI handle
 * @param  RxBuffer   - buffer to hold the received data
 * @param  len        - lenght of the data to be received
 * @return status     - SPI device state
 *
 */
uint8_t SPI_Rx_NonBlocking(SPI_Handle_t* pSPIHandle, uint8_t* RxBuffer, uint32_t len);

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
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable);

/**
 * @brief  API for SPI interrupt handler
 *
 * @param  pSPIHandle - SPI handle structure variable
 *
 */
void SPI_IRQHandler(SPI_Handle_t* pSPIHandle);

#endif /* INCLUDE_STM32F407XX_SPI_H_ */
