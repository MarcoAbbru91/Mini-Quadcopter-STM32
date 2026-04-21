/*
 * Timer.h
 *
 *  Created on: 23 feb 2026
 *      Author: marco91
 */

#ifndef FIRMWARE_INC_TIMER_H_
#define FIRMWARE_INC_TIMER_H_

#include <NVIC_EXTI.h>
#include <stdint.h>
#include "RCC.h"

/****************************************************************************
DEFINES
****************************************************************************/

/* Initialize CPU SysTick Timer */

/* SysTick Timer Control and Status register base address */
#define SYSTICK_CTRL  (* (volatile uint32_t *)(0xE000E010UL))
/* SysTick Timer Control and Status register Counter Enable offset */
#define SYSTICK_CTRL_ENABLE_OFFSET  (0UL)
/* SysTick Timer Control and Status register Systick Exception Request Enable offset */
#define SYSTICK_CTRL_TICKINT_OFFSET  (1UL)
/* SysTick Timer Reload value register base address */
#define SYSTICK_LOAD  (* (volatile uint32_t *)(0xE000E014UL))
/* SysTick Timer Value register base address */
#define SYSTICK_VAL   (* (volatile uint32_t *)(0xE000E018UL))



/* Initialize Timer4 peripheral in Output Compare mode.
   Initializes as well the PWM mode for this timer. */

/* Define base address specific to Timer4 */
#define TIM4_BASE_ADDRESS    (0x40000800UL)


/* Timer4 CR1 - Control Register 1 */
#define TIM4_CR1_OFFSET         (0x00UL)/* TIM4 Control Register 1 address */
#define TIM4_CR1_BASE_ADDRESS   (TIM4_BASE_ADDRESS + TIM4_CR1_OFFSET)
#define TIM4_CR1           (* (volatile uint32_t *)(TIM4_CR1_BASE_ADDRESS)) // typecast and dereference
/* Timer4 Control Register 1 ARPE Register offset */
#define TIM4_CR1_ARPE_OFFSET  (7UL)
/* Timer4 Control Register 1 CMS Register offset */
#define TIM4_CR1_CMS_OFFSET  (5UL) // 2 bits
/* Timer4 Control Register 1 DIR Register offset */
#define TIM4_CR1_DIR_OFFSET  (4UL)
/* Timer4 Control Register 1 UDIS Register offset */
#define TIM4_CR1_UDIS_OFFSET  (1UL)
/* Timer4 Control Register 1 CEN Register offset */
#define TIM4_CR1_CEN_OFFSET  (0UL)

/* Timer4 DIER - Interrupt Enable Register 1 */
#define TIM4_DIER_OFFSET         (0x0CUL)/* TIM4 Control Register 1 address */
#define TIM4_DIER_BASE_ADDRESS   (TIM4_BASE_ADDRESS + TIM4_DIER_OFFSET)
#define TIM4_DIER           (* (volatile uint32_t *)(TIM4_DIER_BASE_ADDRESS)) // typecast and dereference
/* Timer4 DIER Update Interrupt Enable offset */
#define TIM4_DIER_UIE_OFFSET  (0UL)

/* Timer4 SR - Status Register */
#define TIM4_SR_OFFSET        (0x10UL)
#define TIM4_SR_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_SR_OFFSET)
#define TIM4_SR          (* (volatile uint32_t *)(TIM4_SR_BASE_ADDRESS)) // typecast and dereference
/* Timer4 SR UIF - Update Interrupt Flag */
#define TIM4_SR_UIF_OFFSET  (0UL)

/* Timer4 EGR - Event Generation Register */
#define TIM4_EGR_OFFSET        (0x14UL)
#define TIM4_EGR_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_EGR_OFFSET)
#define TIM4_EGR          (* (volatile uint32_t *)(TIM4_EGR_BASE_ADDRESS)) // typecast and dereference
/* Timer4 EGR UG - Update Generation bit */
#define TIM4_EGR_UG_OFFSET  (0UL)

/* Timer4 CCMR1 - Capture Compare Mode Register 1 */
#define TIM4_CCMR1_OFFSET        (0x18UL)
#define TIM4_CCMR1_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CCMR1_OFFSET)
#define TIM4_CCMR1          (* (volatile uint32_t *)(TIM4_CCMR1_BASE_ADDRESS)) // typecast and dereference
/* Timer4 CCMR1 CC1S - Capture/Compare 1 Selection offset */
#define TIM4_CCMR1_CC1S_OFFSET  (0UL) // 2 bits
/* Timer4 CCMR1 OC1PE - Output Compare 1 Preload Enable offset */
#define TIM4_CCMR1_OC1PE_OFFSET  (3UL)
/* Timer4 CCMR1 OC1M - Output Compare 1 Mode: These bits define the behavior of the output reference signal OC1REF */
#define TIM4_CCMR1_OC1M_OFFSET  (4UL) // 3 bits
/* Timer4 CCMR1 CC2S - Capture/Compare 2 Selection offset */
#define TIM4_CCMR1_CC2S_OFFSET  (8UL) // 2 bits
/* Timer4 CCMR1 OC2PE - Output Compare 2 Preload Enable offset */
#define TIM4_CCMR1_OC2PE_OFFSET  (11UL)
/* Timer4 CCMR1 OC2M - Output Compare 2 Mode: These bits define the behavior of the output reference signal OC1REF */
#define TIM4_CCMR1_OC2M_OFFSET  (12UL) // 3 bits

/* Timer4 CCMR2 - Capture Compare Mode Register 2 */
#define TIM4_CCMR2_OFFSET        (0x1CUL)
#define TIM4_CCMR2_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CCMR2_OFFSET)
#define TIM4_CCMR2          (* (volatile uint32_t *)(TIM4_CCMR2_BASE_ADDRESS)) // typecast and dereference
/* Timer4 CCMR2 CC3S - Capture/Compare 3 Selection offset */
#define TIM4_CCMR2_CC3S_OFFSET  (0UL) // 2 bits
/* Timer4 CCMR2 OC3PE - Output Compare 3 Preload Enable offset */
#define TIM4_CCMR2_OC3PE_OFFSET  (3UL)
/* Timer4 CCMR2 OC3M - Output Compare 3 Mode: These bits define the behavior of the output reference signal OC1REF */
#define TIM4_CCMR2_OC3M_OFFSET  (4UL) // 3 bits
/* Timer4 CCMR2 CC4S - Capture/Compare 4 Selection offset */
#define TIM4_CCMR2_CC4S_OFFSET  (8UL) // 2 bits
/* Timer4 CCMR2 OC4PE - Output Compare 4 Preload Enable offset */
#define TIM4_CCMR2_OC4PE_OFFSET  (11UL)
/* Timer4 CCMR2 OC4M - Output Compare 4 Mode: These bits define the behavior of the output reference signal OC1REF */
#define TIM4_CCMR2_OC4M_OFFSET  (12UL) // 3 bits

/* Timer4 CCER - Capture Compare Enable Register */
#define TIM4_CCER_OFFSET        (0x20UL)
#define TIM4_CCER_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CCER_OFFSET)
#define TIM4_CCER          (* (volatile uint32_t *)(TIM4_CCER_BASE_ADDRESS)) // typecast and dereference
/* Timer4 CCER CC1E - Capture/Compare 1 Output Enable offset */
#define TIM4_CCER_CC1E_OFFSET  (0UL)
/* Timer4 CCER OC1P - Capture/Compare 1 Output Polarity offset */
#define TIM4_CCER_OC1P_OFFSET  (1UL)
/* Timer4 CCER CC2E - Capture/Compare 2 Output Enable offset */
#define TIM4_CCER_CC2E_OFFSET  (4UL)
/* Timer4 CCER OC2P - Capture/Compare 2 Output Polarity offset */
#define TIM4_CCER_OC2P_OFFSET  (5UL)
/* Timer4 CCER CC3E - Capture/Compare 3 Output Enable offset */
#define TIM4_CCER_CC3E_OFFSET  (8UL)
/* Timer4 CCER OC3P - Capture/Compare 3 Output Polarity offset */
#define TIM4_CCER_OC3P_OFFSET  (9UL)
/* Timer4 CCER CC4E - Capture/Compare 4 Output Enable offset */
#define TIM4_CCER_CC4E_OFFSET  (12UL)
/* Timer4 CCER OC4P - Capture/Compare 4 Output Polarity offset */
#define TIM4_CCER_OC4P_OFFSET  (13UL)

/* Timer4 CNT - Timer4 Counter Register */
#define TIM4_CNT_OFFSET        (0x24UL)
#define TIM4_CNT_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CNT_OFFSET) // typecast and dereference
#define TIM4_CNT          (* (volatile uint32_t *)(TIM4_CNT_BASE_ADDRESS)) // typecast and dereference

/* Timer4 PSC - Timer4 Prescaler Register */
#define TIM4_PSC_OFFSET        (0x28UL)
#define TIM4_PSC_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_PSC_OFFSET) // typecast and dereference
#define TIM4_PSC          (* (volatile uint32_t *)(TIM4_PSC_BASE_ADDRESS)) // typecast and dereference

/* Timer4 ARR - Timer4 Autoreload Register */
#define TIM4_ARR_OFFSET        (0x2CUL)
#define TIM4_ARR_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_ARR_OFFSET) // typecast and dereference
#define TIM4_ARR          (* (volatile uint32_t *)(TIM4_ARR_BASE_ADDRESS)) // typecast and dereference

/* Timer4 CCR1 - Timer4 Capture Compare Register 1 (CH1) */
#define TIM4_CCR1_OFFSET        (0x34UL)
#define TIM4_CCR1_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CCR1_OFFSET) // typecast and dereference
#define TIM4_CCR1          (* (volatile uint32_t *)(TIM4_CCR1_BASE_ADDRESS)) // typecast and dereference

/* Timer4 CCR2 - Timer4 Capture Compare Register 2 (CH2) */
#define TIM4_CCR2_OFFSET        (0x38UL)
#define TIM4_CCR2_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CCR2_OFFSET) // typecast and dereference
#define TIM4_CCR2          (* (volatile uint32_t *)(TIM4_CCR2_BASE_ADDRESS)) // typecast and dereference

/* Timer4 CCR3 - Timer4 Capture Compare Register 3 (CH3) */
#define TIM4_CCR3_OFFSET        (0x3CUL)
#define TIM4_CCR3_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CCR3_OFFSET) // typecast and dereference
#define TIM4_CCR3          (* (volatile uint32_t *)(TIM4_CCR3_BASE_ADDRESS)) // typecast and dereference

/* Timer4 CCR1 - Timer4 Capture Compare Register 4 (CH4) */
#define TIM4_CCR4_OFFSET        (0x40UL)
#define TIM4_CCR4_BASE_ADDRESS  (TIM4_BASE_ADDRESS + TIM4_CCR4_OFFSET) // typecast and dereference
#define TIM4_CCR4          (* (volatile uint32_t *)(TIM4_CCR4_BASE_ADDRESS)) // typecast and dereference





/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Create delay in milliseconds */
void Delay_ms(uint32_t ms);

/* Initialize CPU SysTick and General-Purpose Timer peripheral */
void Timer_Init();

/* Initialize PWM */
void PWM_Init();

#endif /* FIRMWARE_INC_TIMER_H_ */
