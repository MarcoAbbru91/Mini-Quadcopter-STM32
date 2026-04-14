/*
 * GPIO.h
 *
 *  Created on: 1 mar 2026
 *      Author: marco91
 */

#ifndef DRIVERS_INC_GPIO_H_
#define DRIVERS_INC_GPIO_H_

#include <stdint.h>
#include <stdbool.h>
#include "RCC.h"

/****************************************************************************
DEFINES
****************************************************************************/

/* Initialize GPIOA peripheral.
   Required for configuring GPIO for SCK, MISO, MOSI, CS SPI pins for the three sensors. */

/* Define base address */
#define GPIOA_BASE_ADDRESS      (0x40020000UL)


/* GPIOA Mode Register offset */
#define GPIOA_MODER_OFFSET         (0UL) /* GPIOA Mode Register address */
#define GPIOA_MODER_ADDRESS        (GPIOA_BASE_ADDRESS + GPIOA_MODER_OFFSET)
#define GPIOA_MODER                (* (volatile uint32_t *)(GPIOA_MODER_ADDRESS)) // typecast and dereference
/* GPIOA Mode Register PA4  offset */
#define GPIOA_MODER_4_OFFSET  (8UL) // 2 bits
/* GPIOA Mode Register PA5  offset */
#define GPIOA_MODER_5_OFFSET  (10UL) // 2 bits
/* GPIOA Mode Register PA6  offset */
#define GPIOA_MODER_6_OFFSET  (12UL) // 2 bits
/* GPIOA Mode Register PA7  offset */
#define GPIOA_MODER_7_OFFSET  (14UL) // 2 bits
/* GPIOA Mode Register PA8  offset */
#define GPIOA_MODER_8_OFFSET  (16UL) // 2 bits

/* GPIOA Output Speed Register offset */
#define GPIOA_OSPEEDR_OFFSET         (8UL) /* GPIOA Output Speed Register address */
#define GPIOA_OSPEEDR_ADDRESS        (GPIOA_BASE_ADDRESS + GPIOA_OSPEEDR_OFFSET)
#define GPIOA_OSPEEDR                (* (volatile uint32_t *)(GPIOA_OSPEEDR_ADDRESS)) // typecast and dereference
/* GPIOA Output Speed Register PA4 offset */
#define GPIOA_OSPEEDR_4_OFFSET  (8UL) // 2 bits
/* GPIOA Output Speed Register PA5 offset */
#define GPIOA_OSPEEDR_5_OFFSET  (10UL) // 2 bits
/* GPIOA Output Speed Register PA6  offset */
#define GPIOA_OSPEEDR_6_OFFSET  (12UL) // 2 bits
/* GPIOA Output Speed Register PA7  offset */
#define GPIOA_OSPEEDR_7_OFFSET  (14UL) // 2 bits
/* GPIOA Output Speed Register PA8  offset */
#define GPIOA_OSPEEDR_8_OFFSET  (16UL) // 2 bits

/* GPIOA Input Data Register offset */
#define GPIOA_IDR_OFFSET         (0x10UL) /* GPIOA Input Data Register address */
#define GPIOA_IDR_ADDRESS        (GPIOA_BASE_ADDRESS + GPIOA_IDR_OFFSET)
#define GPIOA_IDR                (* (volatile uint32_t *)(GPIOA_IDR_ADDRESS)) // typecast and dereference
/* GPIOA Input Data Register PA4  offset */
#define GPIOA_IDR_4_OFFSET  (4UL)

/* GPIOA AFRL - Alternate Function Low Register offset */
#define GPIOA_AFRL_OFFSET         (0x20UL) /* GPIOA Alternate Function Register address */
#define GPIOA_AFRL_ADDRESS        (GPIOA_BASE_ADDRESS + GPIOA_AFRL_OFFSET)
#define GPIOA_AFRL                (* (volatile uint32_t *)(GPIOA_AFRL_ADDRESS)) // typecast and dereference
/* GPIOA Alternate Function Register PA5 offset (AFRL contains bits from 0 to 7, of port X) */
#define GPIOA_AFRL_5_OFFSET  (20UL) // 4 bits
/* GPIOA Alternate Function Register PA6 offset (AFRL contains bits from 0 to 7, of port X) */
#define GPIOA_AFRL_6_OFFSET  (24UL) // 4 bits
/* GPIOA Alternate Function Register PA7 offset (AFRL contains bits from 0 to 7, of port X) */
#define GPIOA_AFRL_7_OFFSET  (28UL) // 4 bits


/* GPIOA Set/Reset Register offset */
#define GPIOA_BSRR_OFFSET         (0x18UL) /* GPIOA Set/Reset Register address */
#define GPIOA_BSRR_ADDRESS        (GPIOA_BASE_ADDRESS + GPIOA_BSRR_OFFSET)
#define GPIOA_BSRR                (* (volatile uint32_t *)(GPIOA_BSRR_ADDRESS)) // typecast and dereference
/* GPIOA Set Register PA8 offset */
#define GPIOA_BSRR_BS_8_OFFSET  (8UL)
/* GPIOA Reset Register PA8 offset */
#define GPIOA_BSRR_BR_8_OFFSET  (8UL + 16UL)


/* Initialize GPIOB peripheral.
   Required for configuring GPIO for exploiting it to drive the PWM and to use the BLE peripheral. */

/* Define base address */
#define GPIOB_BASE_ADDRESS      (0x40020400UL)


/* GPIOB Mode Register offset */
#define GPIOB_MODER_OFFSET         (0UL) /* GPIOB Mode Register address */
#define GPIOB_MODER_ADDRESS        (GPIOB_BASE_ADDRESS + GPIOB_MODER_OFFSET)
#define GPIOB_MODER                (* (volatile uint32_t *)(GPIOB_MODER_ADDRESS)) // typecast and dereference
/* GPIOB Mode Register PB0 offset */
#define GPIOB_MODER_0_OFFSET  (0UL) // 2 bits
/* GPIOB Mode Register PB2 offset */
#define GPIOB_MODER_2_OFFSET  (4UL) // 2 bits
/* GPIOB Mode Register PB6 offset */
#define GPIOB_MODER_6_OFFSET  (12UL) // 2 bits
/* GPIOB Mode Register PB7 offset */
#define GPIOB_MODER_7_OFFSET  (14UL) // 2 bits
/* GPIOB Mode Register PB8 offset */
#define GPIOB_MODER_8_OFFSET  (16UL) // 2 bits
/* GPIOB Mode Register PB9 offset */
#define GPIOB_MODER_9_OFFSET  (18UL) // 2 bits
/* GPIOB Mode Register PB12 offset */
#define GPIOB_MODER_12_OFFSET (24U) // 2 bits
/* GPIOB Mode Register PB13 offset */
#define GPIOB_MODER_13_OFFSET (26U) // 2 bits
/* GPIOB Mode Register PB15 offset */
#define GPIOB_MODER_15_OFFSET (30U) // 2 bits

/* GPIOB AFRL - Alternate Function Low Register offset */
#define GPIOB_AFRL_OFFSET         (0x20UL) /* GPIOB Alternate Function Register address */
#define GPIOB_AFRL_ADDRESS        (GPIOB_BASE_ADDRESS + GPIOB_AFRL_OFFSET)
#define GPIOB_AFRL                (* (volatile uint32_t *)(GPIOB_AFRL_ADDRESS)) // typecast and dereference
/* GPIOB Alternate Function Register PB6 offset (AFRL contains bits from 0 to 7, of port X) */
#define GPIOB_AFRL_6_OFFSET  (24UL) // 4 bits
/* GPIOB Alternate Function Register PB7 offset (AFRL contains bits from 0 to 7, of port X) */
#define GPIOB_AFRL_7_OFFSET  (28UL) // 4 bits

/* GPIOB AFRH - Alternate Function High Register offset */
#define GPIOB_AFRH_OFFSET         (0x24UL) /* GPIOB Alternate Function Register address */
#define GPIOB_AFRH_ADDRESS        (GPIOB_BASE_ADDRESS + GPIOB_AFRH_OFFSET)
#define GPIOB_AFRH                (* (volatile uint32_t *)(GPIOB_AFRH_ADDRESS)) // typecast and dereference
/* GPIOB Alternate Function Register PB8 offset (AFRH contains bits from 8 to 15, of port X) */
#define GPIOB_AFRH_8_OFFSET  (0UL) // 4 bits
/* GPIOB Alternate Function Register PB9 offset (AFRH contains bits from 8 to 15, of port X) */
#define GPIOB_AFRH_9_OFFSET  (4UL) // 4 bits
/* GPIOB Alternate Function Register PB13 offset (AFRH contains bits from 8 to 15, of port X) */
#define GPIOB_AFRH_13_OFFSET  (20UL) // 4 bits
/* GPIOB Alternate Function Register PB15 offset (AFRH contains bits from 8 to 15, of port X) */
#define GPIOB_AFRH_15_OFFSET  (28UL) // 4 bits

/* GPIOB OSPEEDR - Output Speed Register offset */
#define GPIOB_OSPEEDR_OFFSET       (0x08UL) /* GPIOB Output Speed Register address */
#define GPIOB_OSPEEDR_ADDRESS      (GPIOB_BASE_ADDRESS + GPIOB_OSPEEDR_OFFSET)
#define GPIOB_OSPEEDR              (* (volatile uint32_t *)(GPIOB_OSPEEDR_ADDRESS)) // typecast and dereference
/* GPIOB Output Speed Register PB0 offset */
#define GPIOB_OSPEEDR_0_OFFSET  (0UL) // 2 bits
/* GPIOB Output Speed Register PB2 offset */
#define GPIOB_OSPEEDR_2_OFFSET  (4UL) // 2 bits
/* GPIOB Output Speed Register PB12 offset */
#define GPIOB_OSPEEDR_12_OFFSET  (24UL) // 2 bits
/* GPIOB Output Speed Register PB13 offset */
#define GPIOB_OSPEEDR_13_OFFSET  (26UL) // 2 bits
/* GPIOB Output Speed Register PB15 offset */
#define GPIOB_OSPEEDR_15_OFFSET  (30UL) // 2 bits


/* GPIOB Set/Reset Register offset */
#define GPIOB_BSRR_OFFSET         (0x18UL) /* GPIOB Set/Reset Register address */
#define GPIOB_BSRR_ADDRESS        (GPIOB_BASE_ADDRESS + GPIOB_BSRR_OFFSET)
#define GPIOB_BSRR                (* (volatile uint32_t *)(GPIOB_BSRR_ADDRESS)) // typecast and dereference
/* GPIOB Set Register PB0 */
#define GPIOB_BSRR_BS_0_OFFSET  (0UL)
/* GPIOB Reset Register PB0 */
#define GPIOB_BSRR_BR_0_OFFSET  (0UL + 16UL)
/* GPIOB Set Register PB12 */
#define GPIOB_BSRR_BS_12_OFFSET  (12UL)
/* GPIOB Reset Register PB12 */
#define GPIOB_BSRR_BR_12_OFFSET  (12UL + 16UL)


/* Initialize GPIOC peripheral.
   Required for configuring GPIO for CS pin for the pressure sensor. */

/* Define base address */
#define GPIOC_BASE_ADDRESS      (0x40020800UL)

/* GPIOC Mode Register offset */
#define GPIOC_MODER_OFFSET         (0UL) /* GPIOC Mode Register address */
#define GPIOC_MODER_ADDRESS        (GPIOC_BASE_ADDRESS + GPIOC_MODER_OFFSET)
#define GPIOC_MODER                (* (volatile uint32_t *)(GPIOC_MODER_ADDRESS)) // typecast and dereference
/* GPIOC Mode Register PC13 bits */
#define GPIOC_MODER_13_OFFSET  (26UL) // 2 bits

/* GPIOC Output Speed Register offset */
#define GPIOC_OSPEEDR_OFFSET         (8UL) /* GPIOC Output Speed Register address */
#define GPIOC_OSPEEDR_ADDRESS        (GPIOC_BASE_ADDRESS + GPIOC_OSPEEDR_OFFSET)
#define GPIOC_OSPEEDR                (* (volatile uint32_t *)(GPIOC_OSPEEDR_ADDRESS)) // typecast and dereference
/* GPIOC Output Speed Register PC13 */
#define GPIOC_OSPEEDR_13_OFFSET  (26UL) // 2 bits

/* GPIOC Set/Reset Register offset */
#define GPIOC_BSRR_OFFSET         (0x18UL) /* GPIOC Set/Reset Register address */
#define GPIOC_BSRR_ADDRESS        (GPIOC_BASE_ADDRESS + GPIOC_BSRR_OFFSET)
#define GPIOC_BSRR                (* (volatile uint32_t *)(GPIOC_BSRR_ADDRESS)) // typecast and dereference
/* GPIOC Set Register PC13 */
#define GPIOC_BSRR_BS_13_OFFSET  (13UL)
/* GPIOC Reset Register PC13 */
#define GPIOC_BSRR_BR_13_OFFSET  (13UL + 16UL)


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Initialize RCC peripheral */
void GPIO_Init();

/* Read IRQ pin (data ready) */
bool BLE_IRQ_ReadGPIO(void);


#endif /* DRIVERS_INC_GPIO_H_ */
