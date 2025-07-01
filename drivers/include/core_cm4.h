/*
 * core_cm4.h
 *
 *  Created on: Jun 30, 2025
 *      Author: vbalaga
 */

#ifndef INCLUDE_CORE_CM4_H_
#define INCLUDE_CORE_CM4_H_

/* Structure definiton for NVIC */
typedef struct {
	uint32_t ISER[8];			/*!< NVIC interrupt set enable register, Address offset: 0x00 */
	uint32_t RESERVED0[24];		/*!< Reserved, Address offset: 0x20 */
	uint32_t ICER[8];			/*!< NVIC interrupt clear enable register, Address offset: 0x80 */
	uint32_t RESERVED1[24];		/*!< Reserved, Address offset: 0x00 */
	uint32_t ISPR[8];			/*!< NVIC interrupt set pending register, Address offset: 0x00 */
	uint32_t RESERVED2[24];		/*!< Reserved, Address offset: 0x00 */
	uint32_t ICPR[8];			/*!< NVIC interrupt clear pending register, Address offset: 0x00 */
	uint32_t RESERVED3[24];		/*!< Reserved, Address offset: 0x00 */
	uint32_t IABR[8];			/*!< NVIC active bit register, Address offset: 0x00 */
	uint32_t RESERVED4[56];		/*!< Reserved, Address offset: 0x00 */
	uint8_t IPR[240];			/*!< NVIC interrupt priority register, Address offset: 0x00 */
	uint32_t RESERVED5[644];	/*!< Reserved, Address offset: 0x00 */
	uint32_t STIR;				/*!< NVIC software trigger interrupt register, Address offset: 0x00 */
} NVIC_TypeDef;

#define NVIC						((NVIC_TypeDef*)0xE000E100)

/*
 * @NVIC_IRQ_PRIORITY_NUM
 * NVIC possible interrupt priority numbers
 */
#define NVIC_IRQ_PRIORITY_0 0
#define NVIC_IRQ_PRIORITY_1 1
#define NVIC_IRQ_PRIORITY_2 2
#define NVIC_IRQ_PRIORITY_3 3
#define NVIC_IRQ_PRIORITY_4 4
#define NVIC_IRQ_PRIORITY_5 5
#define NVIC_IRQ_PRIORITY_6 6
#define NVIC_IRQ_PRIORITY_7 7
#define NVIC_IRQ_PRIORITY_8 8
#define NVIC_IRQ_PRIORITY_9 9
#define NVIC_IRQ_PRIORITY_10 10
#define NVIC_IRQ_PRIORITY_11 11
#define NVIC_IRQ_PRIORITY_12 12
#define NVIC_IRQ_PRIORITY_13 13
#define NVIC_IRQ_PRIORITY_14 14
#define NVIC_IRQ_PRIORITY_15 15

#endif /* INCLUDE_CORE_CM4_H_ */
