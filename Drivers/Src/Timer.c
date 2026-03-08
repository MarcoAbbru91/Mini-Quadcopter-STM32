/*
 * Timer.c
 *
 *  Created on: 23 feb 2026
 *      Author: marco91
 */

#include "NVIC.h"
#include "Timer.h"
#include "RCC.h"


void Timer_Init(void)
{
	/* Enable Clock for Timer4 peripheral */
	RCC_APB1 |= (0x01UL << RCC_APB1_TIM4_EN);


	/* Enables/disables auto-reaload preload */
	TIM4_CR1 |= (0x01UL << TIM4_CR1_ARPE_OFFSET); // Currently use preloaded value (not updated at runtime)
	/* Enables/disables Update Event generation */
	TIM4_CR1 &= ~(0x01UL << TIM4_CR1_UDIS_OFFSET); // Enables Update Event generation, currently after overflow
	/* Sets counter direction */
	TIM4_CR1 &= ~(0x01UL << TIM4_CR1_DIR_OFFSET); // Upcounter
	/* Sets alignment mode */
	TIM4_CR1 &= ~(0x03UL << TIM4_CR1_CMS_OFFSET); // Edge-aligned

	/* Sets TIM4 CCMR1 register */
	TIM4_CCMR1 &= ~(0x03UL << TIM4_CCMR1_CC1S_OFFSET); // Outpure Compare mode.  CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIMx_CCER)
	/* Enables/Disables preload register */
	TIM4_CCMR1 |= (0x01UL << TIM4_CCMR1_OC1PE_OFFSET); // Enabled

	/* Sets TIM4 Prescaler */
	TIM4_PSC = 0;
	/* Sets Polarity */
	TIM4_CCER &= ~(0x01UL << TIM4_CCER_OC1P_OFFSET); // Active High
	/* Configures Channel1 on TIM4 */
	TIM4_CCER |= (0x01UL << TIM4_CCER_CC1E_OFFSET); // Output mode

	/* Interrupt Enable */
	TIM4_DIER |= (1 << TIM4_DIER_UIE_OFFSET);
}


void PWM_Init()
{
	/* Clear and Sets Output Compare Mode */
	TIM4_CCMR1 &= ~(0x07UL << TIM4_CCMR1_OC1M_OFFSET);
	TIM4_CCMR1 |=  (0x06UL << TIM4_CCMR1_OC1M_OFFSET); // PWM Mode 1
	/* Sets Autoreload register on TIM4. DETERMINES FREQUENCY OF PWM */
	TIM4_ARR = (799); // To have PWM of 20 KHz, considering Prescaler=0
	/* Sets Preload value. DETERMINES DUTY CYCLE OF PWM */
	TIM4_CCR1 = (400); // 50% Duty Cycle
	/* Manually set the UG bit inside the EGR register to load the "preload value" */
	TIM4_EGR |= (0x01UL << TIM4_EGR_UG_OFFSET);
	/* Enables/Disables TIM4 counter */
	TIM4_CR1 |= (0x01UL << TIM4_CR1_CEN_OFFSET); // Enabled
}
