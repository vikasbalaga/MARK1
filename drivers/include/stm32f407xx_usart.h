/*
 * stm32f407xx_usart.h
 *
 *  Created on: Nov 15, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_STM32F407XX_USART_H_
#define INCLUDE_STM32F407XX_USART_H_

#include "stm32f407xx.h"

typedef struct {
    uint8_t Mode;                       /*!< possible values from @USART_MODES>*/
    uint32_t BaudRate;                      /*!< possible values from @USART_BAUD_RATES>*/
    uint8_t StopBits;                   /*!< possible values from @USART_STOP_BITS>*/
    uint8_t WordLength;                 /*!< possible values from @USART_WORD_LEN>*/
    uint8_t ParityCTRL;                 /*!< possible values from @USART_PARITY_CTRL>*/
    uint8_t HwFlowControl;              /*!< possible values from @USART_HW_CTRL>*/
    uint8_t OversamplingMode;           /*!< possible values from @USART_OVERSAMPLE_MODES>*/
} USART_Config_t;

typedef struct {
    USART_TypeDef* pUSARTx;             /*!< USART/UART peripheral device ID>*/
    USART_Config_t USART_Config;         /*!< USART/UART config>*/
} USART_Handle_t;

/*
 * @USART_MODES
 * USART modes
 */
#define USART_MODE_TX_ONLY              0 /*!< USART transmit only>*/
#define USART_MODE_RX_ONLY              1 /*!< USART receive only>*/
#define USART_MODE_TX_RX                 2 /*!< USART transmit and receive>*/

/*
 * @USART_BAUD_RATES
 * USART baud rates
 */
#define USART_BAUD_1200                 1200U
#define USART_BAUD_2400                 2400U
#define USART_BAUD_9600                 9600U
#define USART_BAUD_19200                19200U
#define USART_BAUD_38400                38400U
#define USART_BAUD_57600                57600U
#define USART_BAUD_115200               115200U
#define USART_BAUD_230400               230400U
#define USART_BAUD_460800               460800U
#define USART_BAUD_921600               921600U
#define USART_BAUD_2M                   2000000U
#define USART_BAUD_3M                   3000000U

/*
 * @USART_STOP_BITS
 * USART possible stop bits lengths
 */
#define USART_STOPBITS_1                0 /*!< USART no of stop bits 1>*/
#define USART_STOPBITS_0_5              1 /*!< USART no of stop bits 0.5>*/
#define USART_STOPBITS_2                2 /*!< USART no of stop bits 2>*/
#define USART_STOPBITS_1_5              3 /*!< USART no of stop bits 1.5>*/

/*
 * @USART_WORD_LEN
 * USART possible word lengths
 */
#define USART_WORD_LEN_8                0 /*!< USART word length 8 bits>*/
#define USART_WORD_LEN_9                1 /*!< USART word length 9 bits>*/

/*
 * @USART_PARITY_CTRL
 * USART parity control modes
 */
#define USART_PARITY_NONE               0 /*!< USART parity NONE>*/
#define USART_PARITY_ODD                1 /*!< USART parity ODD>*/
#define USART_PARITY_EVEN               2 /*!< USART parity EVEN>*/

/*
 * @USART_HW_CTRL
 * USART possible hardware flow control modes
 */
#define USART_HW_CTRL_NONE              0 /*!< USART HW flow control NONE>*/
#define USART_HW_CTRL_CTS               1 /*!< USART HW flow control clear to send (CTS)>*/
#define USART_HW_CTRL_RTS               2 /*!< USART HW flow control request to send (RTS)>*/
#define USART_HW_CTRL_CTS_RTS           3 /*!< USART HW flow control CTS and RTS>*/

/*
 * @USART_OVERSAMPLE_MODES
 * USART possible oversampling modes
 */
#define OVERSAMPLING_MODE_16            0 /*!< oversampling mode 16>*/
#define OVERSAMPLING_MODE_8             1 /*!< oversampling mode 8>*/

/* USART peripheral enable and disable macros */
#define USART_DEVICE_ENABLE 1
#define USART_DEVICE_DISABLE 0

/* USART clock enable and disable macros */
#define USART_CLK_ENABLE 1
#define USART_CLK_DISABLE 0

/******************************************************************************************
USART register bit position definitions
******************************************************************************************/

/* USART SR register bit position definitions */
#define USART_SR_PE_Bit 0
#define USART_SR_FE_Bit 1
#define USART_SR_NF_Bit 2
#define USART_SR_ORE_Bit 3
#define USART_SR_IDLE_Bit 4
#define USART_SR_RXNE_Bit 5
#define USART_SR_TC_Bit 6
#define USART_SR_TXE_Bit 7
#define USART_SR_LBD_Bit 8
#define USART_SR_CTS_Bit 9

/* USART CR1 register bit position definitions */
#define USART_CR1_SBK_Bit 0
#define USART_CR1_RWU_Bit 1
#define USART_CR1_RE_Bit 2
#define USART_CR1_TE_Bit 3
#define USART_CR1_IDLEIE_Bit 4
#define USART_CR1_RXNEIE_Bit 5
#define USART_CR1_TCIE_Bit 6
#define USART_CR1_TXEIE_Bit 7
#define USART_CR1_PEIE_Bit 8
#define USART_CR1_PS_Bit 9
#define USART_CR1_PCE_Bit 10
#define USART_CR1_WAKE_Bit 11
#define USART_CR1_M_Bit 12
#define USART_CR1_UE_Bit 13
#define USART_CR1_OVER8_Bit 15

/* USART CR2 register bit position definitions */
#define USART_CR2_ADD_Bit 0
#define USART_CR2_LBDL_Bit 5
#define USART_CR2_LBDIE_Bit 6
#define USART_CR2_LBCL_Bit 8
#define USART_CR2_CPHA_Bit 9
#define USART_CR2_CPOL_Bit 10
#define USART_CR2_CLKEN_Bit 11
#define USART_CR2_STOP_Bit 12
#define USART_CR2_LINEN_Bit 14

/* USART CR3 register bit position definitions */
#define USART_CR3_EIE_Bit 0
#define USART_CR3_IREN_Bit 1
#define USART_CR3_IRLP_Bit 2
#define USART_CR3_HDSEL_Bit 3
#define USART_CR3_NACK_Bit 4
#define USART_CR3_SCEN_Bit 5
#define USART_CR3_DMAR_Bit 6
#define USART_CR3_DMAT_Bit 7
#define USART_CR3_RTSE_Bit 8
#define USART_CR3_CTSE_Bit 9
#define USART_CR3_CTSIE_Bit 10
#define USART_CR3_ONEBIT_Bit 11

/* USART register reset macros */
#define USART1_REG_RESET()		do {\
									RCC->APB2RSTR |= (1 << 4);\
									RCC->APB2RSTR &= ~(1 << 4);\
									}while(0)
#define USART2_REG_RESET()		do {\
									RCC->APB1RSTR |= (1 << 17);\
									RCC->APB1RSTR &= ~(1 << 17);\
									}while(0)
#define USART3_REG_RESET()		do {\
									RCC->APB1RSTR |= (1 << 18);\
									RCC->APB1RSTR &= ~(1 << 18);\
									}while(0)
#define USART6_REG_RESET()		do {\
									RCC->APB2RSTR |= (1 << 5);\
									RCC->APB2RSTR &= ~(1 << 5);\
									}while(0)
#define USART_BIT_RESET 0
#define USART_BIT_SET	1

/* Get the status of a flag from USART status register */
#define USART_GetFlagStatus(pUSARTx, flag) 		((pUSARTx->SR & (1 << flag) ? USART_BIT_SET :	USART_BIT_RESET))

/* USART Driver APIs */

/**
 * @brief  USART initialization API
 *
 * @param  pUSARTHandle - USART handle
 *
 */
void USART_Init(USART_Handle_t* pUSARTHandle);

/**
 * @brief  USART peripheral control API
 *
 * @param  pUSARTx  - USART peripheral ID
 * @param  enable - if 0 then disable USART
 *				  	if 1 then enable USART 
 *
 */
void USART_Periph_Ctrl(USART_TypeDef* pUSARTx, bool enable);

/**
 * @brief  USART de-initialization API
 *
 * @param  pUSARTx - USART peripheral ID
 *
 */
void USART_Deinit(USART_TypeDef* pUSARTx);

/**
 * @brief  USART clock control API
 *
 * @param  pUSARTx  - USART peripheral ID
 * @param  enable - if 0 then disable clock
 *				  	if 1 then enable clock 
 *
 */
void USART_Clock_Ctrl(USART_TypeDef* pUSARTx, bool enable);

/**
 * @brief  USART Baud rate configuration API
 *
 * @param  pUSARTHandle - USART handle
 *
 */
void USART_SetBaudRate(USART_Handle_t* pUSARTHandle);

/**
 * @brief  USART Transmit API (Blocking mode)
 *
 * @param  pUSARTHandle - USART handle
 * @param  pTxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 *
 */
void USART_Tx_Blocking(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t len);

/**
 * @brief  USART Receive API (Blocking mode)
 *
 * @param  pUSARTHandle - USART handle
 * @param  pRxBuffer   - buffer holding data to be transmitted
 * @param  len        - lenght of the data to be transmitted
 *
 */
void USART_Rx_Blocking(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t len);

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
void USART_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, bool enable);

/**
 * @brief  API for USART interrupt handler
 *
 * @param  pUSARTHandle - USART handle structure variable
 *
 */
void USART_IRQHandler(USART_Handle_t* pUSARTHandle);

#endif /* INCLUDE_STM32F407XX_USART_H_ */
