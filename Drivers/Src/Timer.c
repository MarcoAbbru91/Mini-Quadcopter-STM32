/*
 * Timer.c
 *
 *  Created on: 23 feb 2026
 *      Author: marco91
 */

#include "Timer.h"


extern volatile uint32_t SysTick_Counter;



void Delay_ms(uint32_t ms)
{
	uint8_t Timer_Delay_Start = SysTick_Counter;
	while((SysTick_Counter - Timer_Delay_Start) < ms); // Wait inside this function until the expected number of ms are elapsed
}


void Timer_Init(void)
{
	/* SysTick initialization sequence as per Cortex Tech Ref */
	/* SysTick Counter Reload Value */
	SYSTICK_LOAD = ((PLL_CLOCK_SYS_FREQ / 1000)  - 1UL); // To get Reload value of the timer equal to 1ms, considering that SysTick runs at frequency of AHB/8 (= 84MHz/8)
	/* SysTick Counter Current Value */
	SYSTICK_VAL  = 0UL; // Reset it
	/* SysTick Counter Enable */
	SYSTICK_CTRL |= (0x01UL << SYSTICK_CTRL_ENABLE_OFFSET); // Enable counter
	/* SysTick Exception Request Enable */
	SYSTICK_CTRL |= (0x01UL << SYSTICK_CTRL_TICKINT_OFFSET); // Assert the SysTick exception request

	SYSTICK_CTRL |= (0x01UL << SYSTICK_CTRL_CLKSOURCE_OFFSET);

	/* Timer4 peripheral initialization */
	/* Enable Clock for Timer4 peripheral */
	RCC_APB1 |= (0x01UL << RCC_APB1_TIM4_EN);


	/* Enables/disables auto-reaload preload */
	TIM4_CR1 |= (0x01UL << TIM4_CR1_ARPE_OFFSET); // Currently use preloaded value (not updated at runtime)
	/* Enables/disables Update Event generation */
	TIM4_CR1 &= ~(0x01UL << TIM4_CR1_UDIS_OFFSET); // Enables Update Event generation, currently after overflow
	/* Sets counter direction */
	//TIM4_CR1 &= ~(0x01UL << TIM4_CR1_DIR_OFFSET); // Upcounter. Not required anymore for center-aligned mode
	/* Sets alignment mode */
	TIM4_CR1 &= ~(0x03UL << TIM4_CR1_CMS_OFFSET);
	TIM4_CR1 |=  (0x01UL << TIM4_CR1_CMS_OFFSET); // Center-aligned

	/* Sets TIM4 CCMR1 register for CH1 */
	TIM4_CCMR1 &= ~(0x03UL << TIM4_CCMR1_CC1S_OFFSET); // Outpure Compare mode.  CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIMx_CCER)
	/* Enables/Disables preload register for CH1 */
	TIM4_CCMR1 |= (0x01UL << TIM4_CCMR1_OC1PE_OFFSET); // Enabled
	/* Sets TIM4 CCMR1 register for CH2 */
	TIM4_CCMR1 &= ~(0x03UL << TIM4_CCMR1_CC2S_OFFSET); // Outpure Compare mode.  CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIMx_CCER)
	/* Enables/Disables preload register for CH2 */
	TIM4_CCMR1 |= (0x01UL << TIM4_CCMR1_OC2PE_OFFSET); // Enabled

	/* Sets TIM4 CCMR2 register for CH3 */
	TIM4_CCMR2 &= ~(0x03UL << TIM4_CCMR2_CC3S_OFFSET); // Outpure Compare mode.  CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIMx_CCER)
	/* Enables/Disables preload register for CH3 */
	TIM4_CCMR2 |= (0x01UL << TIM4_CCMR2_OC3PE_OFFSET); // Enabled
	/* Sets TIM4 CCMR2 register for CH4 */
	TIM4_CCMR2 &= ~(0x03UL << TIM4_CCMR2_CC4S_OFFSET); // Outpure Compare mode.  CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIMx_CCER)
	/* Enables/Disables preload register for CH4 */
	TIM4_CCMR2 |= (0x01UL << TIM4_CCMR2_OC4PE_OFFSET); // Enabled

	/* Sets Polarity */
	TIM4_CCER &= ~(0x01UL << TIM4_CCER_OC1P_OFFSET); // Active High
	/* Configures Channel1 on TIM4 */
	TIM4_CCER |= (0x01UL << TIM4_CCER_CC1E_OFFSET); // Output mode
	/* Sets Polarity */
	TIM4_CCER &= ~(0x01UL << TIM4_CCER_OC2P_OFFSET); // Active High
	/* Configures Channel2 on TIM4 */
	TIM4_CCER |= (0x01UL << TIM4_CCER_CC2E_OFFSET); // Output mode
	/* Sets Polarity */
	TIM4_CCER &= ~(0x01UL << TIM4_CCER_OC3P_OFFSET); // Active High
	/* Configures Channel3 on TIM4 */
	TIM4_CCER |= (0x01UL << TIM4_CCER_CC3E_OFFSET); // Output mode
	/* Sets Polarity */
	TIM4_CCER &= ~(0x01UL << TIM4_CCER_OC4P_OFFSET); // Active High
	/* Configures Channel4 on TIM4 */
	TIM4_CCER |= (0x01UL << TIM4_CCER_CC4E_OFFSET); // Output mode

	/* Sets TIM4 Prescaler */
	TIM4_PSC = 0;

	/* Interrupt Enable */
	TIM4_DIER |= (1 << TIM4_DIER_UIE_OFFSET);
}


void PWM_Init()
{
	/* Clear and Sets Output Compare Mode for CH1 */
	TIM4_CCMR1 &= ~(0x07UL << TIM4_CCMR1_OC1M_OFFSET);
	TIM4_CCMR1 |=  (0x06UL << TIM4_CCMR1_OC1M_OFFSET); // PWM Mode 1
	/* Clear and Sets Output Compare Mode for CH2 */
	TIM4_CCMR1 &= ~(0x07UL << TIM4_CCMR1_OC2M_OFFSET);
	TIM4_CCMR1 |=  (0x06UL << TIM4_CCMR1_OC2M_OFFSET); // PWM Mode 1
	/* Clear and Sets Output Compare Mode for CH3 */
	TIM4_CCMR2 &= ~(0x07UL << TIM4_CCMR2_OC3M_OFFSET);
	TIM4_CCMR2 |=  (0x06UL << TIM4_CCMR2_OC3M_OFFSET); // PWM Mode 1
	/* Clear and Sets Output Compare Mode for CH4 */
	TIM4_CCMR2 &= ~(0x07UL << TIM4_CCMR2_OC4M_OFFSET);
	TIM4_CCMR2 |=  (0x06UL << TIM4_CCMR2_OC4M_OFFSET); // PWM Mode 1

	/* Sets Autoreload register on TIM4. DETERMINES FREQUENCY OF PWM */
	TIM4_ARR = (MAX_DC-1); // To have PWM of 20 KHz with Clock freq of 84MHz, considering Prescaler=0
	/* Sets Preload value for TIM4 CH1. DETERMINES DUTY CYCLE OF PWM */
	TIM4_CCR1 = (0); // 0% Duty Cycle
	/* Sets Preload value for TIM4 CH2. DETERMINES DUTY CYCLE OF PWM */
	TIM4_CCR2 = (0); // 0% Duty Cycle
	/* Sets Preload value for TIM4 CH3. DETERMINES DUTY CYCLE OF PWM */
	TIM4_CCR3 = (0); // 0% Duty Cycle
	/* Sets Preload value for TIM4 CH4. DETERMINES DUTY CYCLE OF PWM */
	TIM4_CCR4 = (0); // 0% Duty Cycle
	/* Manually set the UG bit inside the EGR register to load the "preload value" */
	TIM4_EGR |= (0x01UL << TIM4_EGR_UG_OFFSET);
	/* Enables/Disables TIM4 counter */
	TIM4_CR1 |= (0x01UL << TIM4_CR1_CEN_OFFSET); // Enabled
}

void PWM_Set(float PWM_Mot1, float PWM_Mot2, float PWM_Mot3, float PWM_Mot4)
{
	if(PWM_Mot1 > 100.0f) PWM_Mot1 = 100.0f;
	if(PWM_Mot1 < 0.0f)   PWM_Mot1 = 0.0f;
	if(PWM_Mot2 > 100.0f) PWM_Mot2 = 100.0f;
	if(PWM_Mot2 < 0.0f)   PWM_Mot2 = 0.0f;
	if(PWM_Mot3 > 100.0f) PWM_Mot3 = 100.0f;
	if(PWM_Mot3 < 0.0f)   PWM_Mot3 = 0.0f;
	if(PWM_Mot4 > 100.0f) PWM_Mot4 = 100.0f;
	if(PWM_Mot4 < 0.0f)   PWM_Mot4 = 0.0f;

	TIM4_CCR1 = (uint16_t)((MAX_DC * PWM_Mot1) / 100.0f); // PWM_Mot : 100 = DC(CCRx) : MAX_DC
	TIM4_CCR2 = (uint16_t)((MAX_DC * PWM_Mot2) / 100.0f); // PWM_Mot : 100 = DC(CCRx) : MAX_DC
	TIM4_CCR3 = (uint16_t)((MAX_DC * PWM_Mot3) / 100.0f); // PWM_Mot : 100 = DC(CCRx) : MAX_DC
	TIM4_CCR4 = (uint16_t)((MAX_DC * PWM_Mot4) / 100.0f); // PWM_Mot : 100 = DC(CCRx) : MAX_DC

}
