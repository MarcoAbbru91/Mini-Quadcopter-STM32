/*
 * FlashMem_IF.h
 *
 *  Created on: 14 mar 2026
 *      Author: marco91
 */

#ifndef FLASHMEM_IF_H_
#define FLASHMEM_IF_H_


#include <stdint.h>
#include "RCC_Cfg.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* Initialize the Flash Memory Interface.
 * Manages CPU AHB I-Code and D-Code accesses to the flash memory. */

/* Define base address */
#define FLASH_MEM_IF_BASE_ADDRESS      (0x40023C00UL)


/* Flash Memory IF Control Register */
#define FLASH_MEM_IF_ACR_OFFSET         (0UL) /* Flash Memory IF Control Register offset */
#define FLASH_MEM_IF_ACR_ADDRESS        (FLASH_MEM_IF_BASE_ADDRESS + FLASH_MEM_IF_ACR_OFFSET)
#define FLASH_MEM_IF_ACR                (* (volatile uint32_t *)(FLASH_MEM_IF_ACR_ADDRESS)) // typecast and dereference
/* Flash Memory IF ACR Latency offset */
#define FLASH_MEM_IF_ACR_LATENCY_OFFSET  (0UL) // 4 bits
/* Flash Memory IF ACR Prefetch Enable offset */
#define FLASH_MEM_IF_ACR_PRFTEN_OFFSET  (8UL)
/* Flash Memory IF ACR I-Cache Enable offset */
#define FLASH_MEM_IF_ACR_ICEN_OFFSET  (9UL)
/* Flash Memory IF ACR D-Cache Enable offset */
#define FLASH_MEM_IF_ACR_DCEN_OFFSET  (10UL)



/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Initialize Flash Memory Interface peripherals */
void Flash_Mem_IF_Init();



#endif /* FLASHMEM_IF_H_ */
