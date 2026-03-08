/*
 * RCC.h
 *
 *  Created on: 5 feb 2026
 *      Author: marco91
 */

#ifndef FIRMWARE_INC_RCC_H_
#define FIRMWARE_INC_RCC_H_


#include <stdint.h>
#include "RCC_Cfg.h"

/****************************************************************************
DEFINES
****************************************************************************/

/* Initialize RCC (Reset and Clock Control) peripheral.
   Required for configuring reset and clock sources. */

/* Define base address */
#define RCC_BASE_ADDRESS      (0x40023800UL)


/* RCC Control Register offset */
#define RCC_CR_OFFSET         (0UL) /* RCC Control Register address */
#define RCC_CR_ADDRESS        (RCC_BASE_ADDRESS + RCC_CR_OFFSET) /* alternative writing: (* (volatile uint32_t *) (RCC_BASE_ADDRESS + RCC_CR_OFFSET)). In this way we already typecast and dereference, and then we can directly write the new register value by directly using RCC_CR_ADDRESS |= 0xYY */
#define RCC_CR                (* (volatile uint32_t *)(RCC_CR_ADDRESS)) // typecast and dereference
/* RCC Control Register Main PLL Enable */
#define RCC_CR_PLLON_OFFSET  (24UL)
/* RCC Control Register Main PLL Clock Ready Flag */
#define RCC_CR_PLLRDY_OFFSET (25UL)

/* RCC PLL Configuration Register offset */
#define RCC_PLLCFGR_OFFSET    (4UL)
/* RCC PLL Configuration Register address */
#define RCC_PLLCFGR_ADDRESS   (RCC_BASE_ADDRESS + RCC_PLLCFGR_OFFSET)
#define RCC_PLLCFGR           (* (volatile uint32_t *)(RCC_PLLCFGR_ADDRESS)) // typecast and dereference // @suppress("Line comments")
/* RCC PLL Source */
#define RCC_PLLCFGR_PLLSRC_OFFSET    (22UL)

/* RCC Configuration Register */
#define RCC_CFGR_OFFSET       (8UL)
/* RCC Control Register address */
#define RCC_CFGR_ADDRESS      (RCC_BASE_ADDRESS + RCC_CFGR_OFFSET)
#define RCC_CFGR              (* (volatile uint32_t *)(RCC_CFGR_ADDRESS)) // typecast and dereference // @suppress("Line comments")
/* MCO1 - Microcontroller Clock Output 1 */
#define RCC_CFGR_MCO1_OFFSET  (21UL) // 2 bits
/* MCO1PRE - Microcontroller Clock Output 1 Prescaler */
#define RCC_CFGR_MCO1PRE_OFFSET  (24UL) // 3 bits


/*****************************
 * Peripherals Clock Enable
 ****************************/
 
/* APB1 Clock Enable Register offset */
#define RCC_APB1_ENR_OFFSET   (0x40UL)
/* APB1 Clock Enable Register address */
#define RCC_APB1_ENR_ADDRESS    (RCC_BASE_ADDRESS + RCC_APB1_ENR_OFFSET)
#define RCC_APB1                (* (volatile uint32_t *)(RCC_APB1_ENR_ADDRESS)) // typecast and dereference
/* APB1 Tim4 Clock Enable bit (PWM for motors control) */
#define RCC_APB1_TIM4_EN       (2UL)
/* APB1 SPI2 Clock Enable bit (BLE) */
#define RCC_APB1_SPI2_EN       (14UL)


/* APB2 Clock Enable Register offset */
#define RCC_APB2_ENR_OFFSET   (0x44UL)
/* APB2 Clock Enable Register address */
#define RCC_APB2_ENR_ADDRESS    (RCC_BASE_ADDRESS + RCC_APB2_ENR_OFFSET)
#define RCC_APB2                (* (volatile uint32_t *)(RCC_APB2_ENR_ADDRESS)) // typecast and dereference
/* APB2 SPI1 Clock Enable bit (Sensors) */
#define RCC_APB2_SPI1_EN       (12UL)


/* AHB1 Clock Enable Register offset */
#define RCC_AHB1_ENR_OFFSET   (0x30UL)
/* AHB1 Clock Enable Register address */
#define RCC_AHB1_ENR_ADDRESS    (RCC_BASE_ADDRESS + RCC_AHB1_ENR_OFFSET)
#define RCC_AHB1                (* (volatile uint32_t *)(RCC_AHB1_ENR_ADDRESS)) // typecast and dereference // @suppress("Line comments")
/* AHB1 GPIOB Clock Enable bit */
#define RCC_AHB1_GPIOB_EN       (1UL)
/* AHB1 GPIOB Clock Enable bit */
#define RCC_AHB1_GPIOA_EN       (0UL)


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Initialize RCC peripheral */
void RCC_Init();

#endif /* FIRMWARE_INC_RCC_H_ */
