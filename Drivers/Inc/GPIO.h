/*
 * GPIO.h
 *
 *  Created on: 1 mar 2026
 *      Author: marco91
 */

#ifndef DRIVERS_INC_GPIO_H_
#define DRIVERS_INC_GPIO_H_

#include <stdint.h>

/****************************************************************************
DEFINES
****************************************************************************/

/* Initialize GPIOA peripheral.
   Required for configuring GPIO for SCK, MISO, MOSI SPI pins for the three sensors. */

/* Define base address */
#define GPIOA_BASE_ADDRESS      (0x40020000UL)


/* GPIOA Mode Register offset */
#define GPIOA_MODER_OFFSET         (0UL) /* GPIOA Mode Register address */
#define GPIOA_MODER_ADDRESS        (GPIOA_BASE_ADDRESS + GPIOA_MODER_OFFSET)
#define GPIOA_MODER                (* (volatile uint32_t *)(GPIOA_MODER_ADDRESS)) // typecast and dereference
/* GPIOA Mode Register PA5 bits */
#define GPIOA_MODER_5_OFFSET  (10UL) // 2 bits
/* GPIOA Mode Register PA6 bits */
#define GPIOA_MODER_6_OFFSET  (12UL) // 2 bits
/* GPIOA Mode Register PA7 bits */
#define GPIOA_MODER_7_OFFSET  (14UL) // 2 bits

/* GPIOA Output Speed Register offset */
#define GPIOA_OSPEEDR_OFFSET         (8UL) /* GPIOA Output Speed Register address */
#define GPIOA_OSPEEDR_ADDRESS        (GPIOA_BASE_ADDRESS + GPIOA_OSPEEDR_OFFSET)
#define GPIOA_OSPEEDR                (* (volatile uint32_t *)(GPIOA_OSPEEDR_ADDRESS)) // typecast and dereference
/* GPIOA Output Speed Register PA5 bits */
#define GPIOA_OSPEEDR_5_OFFSET  (10UL) // 2 bits
/* GPIOA Output Speed Register PA6 bits */
#define GPIOA_OSPEEDR_6_OFFSET  (12UL) // 2 bits
/* GPIOA Output Speed Register PA7 bits */
#define GPIOA_OSPEEDR_7_OFFSET  (14UL) // 2 bits

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


/* Initialize GPIOB peripheral.
   Required for configuring GPIO for exploiting it to drive the PWM and to use the BLE peripheral. */

/* Define base address */
#define GPIOB_BASE_ADDRESS      (0x40020400UL)


/* GPIOB Mode Register offset */
#define GPIOB_MODER_OFFSET         (0UL) /* GPIOB Mode Register address */
#define GPIOB_MODER_ADDRESS        (GPIOB_BASE_ADDRESS + GPIOB_MODER_OFFSET)
#define GPIOB_MODER                (* (volatile uint32_t *)(GPIOB_MODER_ADDRESS)) // typecast and dereference
/* GPIOB Mode Register PB6 bits */
#define GPIOB_MODER_6_OFFSET  (12UL) // 2 bits

/* GPIOB AFRL - Alternate Function Low Register offset */
#define GPIOB_AFRL_OFFSET         (0x20UL) /* GPIOB Alternate Function Register address */
#define GPIOB_AFRL_ADDRESS        (GPIOB_BASE_ADDRESS + GPIOB_AFRL_OFFSET)
#define GPIOB_AFRL                (* (volatile uint32_t *)(GPIOB_AFRL_ADDRESS)) // typecast and dereference
/* GPIOB Alternate Function Register PB6 offset (AFRL contains bits from 0 to 7, of port X) */
#define GPIOB_AFRL_6_OFFSET  (24UL) // 4 bits

/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Initialize RCC peripheral */
void GPIO_Init();

#endif /* DRIVERS_INC_GPIO_H_ */
