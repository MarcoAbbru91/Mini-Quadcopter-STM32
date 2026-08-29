/*
 * SysCfg.h
 *
 *  Created on: 14 apr 2026
 *      Author: marco91
 */

#ifndef SYSCFG_H_
#define SYSCFG_H_


#include "RCC.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* Initialize SysCfg (System Configuration Controller) peripheral.
   Required to manage the external interrupt line connection to the GPIOs. */

/* Define base address */
#define SYSCFG_BASE_ADDRESS      (0x40013800UL)


/* SYSCFG EXTICR2 External Interrupt Configuration Register 2 offset */
#define SYSCFG_EXTICR2_OFFSET        (0x0CUL)
/* SYSCFG EXTICR2 Configuration Register address */
#define SYSCFG_EXTICR2_ADDRESS       (SYSCFG_BASE_ADDRESS + SYSCFG_EXTICR2_OFFSET)
#define SYSCFG_EXTICR2               (* (volatile uint32_t *)(SYSCFG_EXTICR2_ADDRESS)) // typecast and dereference
/* SYSCFG EXTICR2 EXTI4 offset */
#define SYSCFG_EXTICR2_EXTI4_OFFSET  (0UL) // 4 bits



/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Initialize SysCfg peripheral */
void SysCfg_Init();


#endif /* SYSCFG_H_ */
