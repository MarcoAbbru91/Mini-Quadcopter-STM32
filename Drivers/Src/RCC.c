/*
 * RCC.c
 *
 *  Created on: 5 feb 2026
 *      Author: marco91
 */
#include "RCC.h"


void RCC_Init()
{
	/* Flash Memory Interface configuration - Needed for PLL
	 * Configure Flash latency before increasing clock frequency */
	Flash_Mem_IF_Init();

	/* Clock Source configuration */
#if CLOCK_SOURCE == PLL_CLOCK
	/* Clears and sets PLLM register */
	RCC_PLLCFGR &= ~(0x3FU << RCC_PLLCFGR_PLLM_OFFSET); // Clears 6 bits
	RCC_PLLCFGR |= (16U << RCC_PLLCFGR_PLLM_OFFSET); // to get VCOin = HSI / PLLM = 1MHz
	/* Clears and sets PLLN register */
	RCC_PLLCFGR &= ~(0x1FFU << RCC_PLLCFGR_PLLN_OFFSET); // Clears 9 bits
	RCC_PLLCFGR |= (168U << RCC_PLLCFGR_PLLN_OFFSET); // VCOout = VCOin * PLLN
	/* Clears and sets PLLP register */
	RCC_PLLCFGR &= ~(0x3U << RCC_PLLCFGR_PLLP_OFFSET); // Clears 2 bits to have PLLP=2 (to have SysClock = VCOout / PLLP = 84MHz)
	/* Clears and sets PLLQ register */
	RCC_PLLCFGR &= ~(0xFU << RCC_PLLCFGR_PLLQ_OFFSET); // Clears 4 bits
	RCC_PLLCFGR |= (7U << RCC_PLLCFGR_PLLQ_OFFSET); // Actually not used in SW
	/* Initialize PLL Source */
	RCC_PLLCFGR &= ~(0x01UL << RCC_PLLCFGR_PLLSRC_OFFSET); // Clear bit 22 to set HSI as source

	/* Enable Main PLL */
	RCC_CR     |=  (0x01UL << RCC_CR_PLLON_OFFSET); // Sets PLL ON bit
	while(!(RCC_CR & (1UL << RCC_CR_PLLRDY_OFFSET))); // Wait for PLL HW to be ready

	/* Clears and sets PLL CFGR PPRE1 bits */
	RCC_CFGR &= ~(7UL << RCC_CFGR_PPRE1_OFFSET); // Clear bits
	RCC_CFGR |=  (4UL << RCC_CFGR_PPRE1_OFFSET); // Selects AHB clock divided by 2 -> 42MHz
	/* Clears and sets PLL CFGR SW bits */
	RCC_CFGR &= ~(3UL << RCC_CFGR_SW_OFFSET); // Clear bits
	RCC_CFGR |=  (2UL << RCC_CFGR_SW_OFFSET); // Selects PLL as system clock
	while((RCC_CFGR & (3 << RCC_CFGR_SWS_OFFSET)) != (2 << RCC_CFGR_SWS_OFFSET));

#elif CLOCK_SOURCE == HSI_CLOCK
/* The HSI configuration is actually not required since the HSI clock is automatically selected after system reset */
	/* Initialize MCO1 (Microcontroller Clock Output) */
	RCC_CFGR &= ~(0x03UL << RCC_CFGR_MCO1_OFFSET); // Clear bits 21 and 22 to select HSI clock source
	/* Initialize MCO1PRE (Microcontroller Clock Output Prescaler) */
	RCC_CFGR &= ~(0x07UL << RCC_CFGR_MCO1PRE_OFFSET); // Clear bits 24, 25 and 26 to have no division done by the prescaler

#endif
}
