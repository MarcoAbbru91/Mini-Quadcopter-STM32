/*
 * SysCfg.c
 *
 *  Created on: 14 apr 2026
 *      Author: marco91
 */

#include "SysCfg.h"



void SysCfg_Init()
{
	/* Enable Clock for EXTI peripheral */
	RCC_APB2 |= (1UL << RCC_APB2_SYSCFGEN_EN);

	/* Select PA[x] pin as source input for the EXTI4 */
	SYSCFG_EXTICR2 |= (0UL << SYSCFG_EXTICR2_EXTI4_OFFSET);
}
