/*
 * FlashMem_IF.c
 *
 *  Created on: 14 mar 2026
 *      Author: marco91
 */

#include "FlashMem_IF.h"


/* Initialize Flash Memory Interface peripherals */
void Flash_Mem_IF_Init()
{
	/* Clears and sets latency value - ratio of the CPU clock period to the Flash memory access time*/
	FLASH_MEM_IF_ACR &= ~(0xFFU << FLASH_MEM_IF_ACR_LATENCY_OFFSET);
	FLASH_MEM_IF_ACR |=  (0x02U << FLASH_MEM_IF_ACR_LATENCY_OFFSET); // Two wait states, since flash access time is around 35ns, but ClockFreq=1/84MHz=11.9ns.
																	 // Therefore flash access needs 1 cycle + 2 wait states.
																	 // The datasheet specifies that for a clock frequency >=60MHz and <=90MHz two (extra) wait states are required.
	/* Sets Prefetch Enable*/
	FLASH_MEM_IF_ACR |= (0x1U << FLASH_MEM_IF_ACR_PRFTEN_OFFSET); // Enables prefetch
	/* Sets Instruction Cache Enable*/
	FLASH_MEM_IF_ACR |= (0x1U << FLASH_MEM_IF_ACR_ICEN_OFFSET); // Enables Instruction cache
	/* Sets Data Cache Enable*/
	FLASH_MEM_IF_ACR |= (0x1U << FLASH_MEM_IF_ACR_DCEN_OFFSET); // Enables Data cache
}
