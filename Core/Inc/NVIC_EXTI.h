/*
 * IRQ.h
 *
 *  Created on: Mar 6, 2026
 *      Author: marco91
 */

#ifndef NVIC_EXTI_H_
#define NVIC_EXTI_H_


#include <stdint.h>
//#include "Timer.h"
#include "SPI.h"
#include "BLE.h"



/****************************************************************************
DEFINES
****************************************************************************/

/* Cortex-M4 Processor Exception's and Interrupt's IRQs */
#define EXTI4_IRQn 10U // EXTI4 global Interrupt - all derived from "Vector table for STM32F401xB/CSTM32F401xD/E"
#define TIM4_IRQn  30U // TIM4 global Interrupt
#define SPI1_IRQn  35U // SPI1 global Interrupt
#define SPI2_IRQn  36U // SPI2 global Interrupt


#define Prio_Lev_0   0x00U
#define Prio_Lev_1   0x10U
#define Prio_Lev_2   0x20U
#define Prio_Lev_3   0x30U
#define Prio_Lev_4   0x40U
#define Prio_Lev_5   0x50U
#define Prio_Lev_6   0x60U
#define Prio_Lev_7   0x70U
#define Prio_Lev_8   0x80U
#define Prio_Lev_9   0x90U
#define Prio_Lev_10  0xA0U
#define Prio_Lev_11  0xB0U
#define Prio_Lev_12  0xC0U
#define Prio_Lev_13  0xD0U
#define Prio_Lev_14  0xE0U
#define Prio_Lev_15  0xF0U


#define NVIC_ISER_BASEADDR  ((volatile uint32_t *)(0xE000E200)) // Interrupt Set-Enable Register base address
#define NVIC_IPR_BASEADDR   ((volatile uint32_t *)(0xE000E400)) // Interrupt Priority Register base address


/* Initialize EXTI (External Interrupt/Event Controller) peripheral */

/* Define base address */
#define EXTI_BASE_ADDRESS      (0x40013C00UL)


/* EXTI IMR - Interrupt Mask Register offset */
#define EXTI_IMR_OFFSET        (0UL)
/* EXTI IMR Interrupt Mask Register address */
#define EXTI_IMR_ADDRESS       (EXTI_BASE_ADDRESS + EXTI_IMR_OFFSET)
#define EXTI_IMR               (* (volatile uint32_t *)(EXTI_IMR_ADDRESS)) // typecast and dereference
/* EXTI IMR MR4 offset (line 4 cause we use PIN 4) */
#define EXTI_IMR_MR4_OFFSET  (4UL)

/* EXTI RTSR - Rising Trigger Selection Register offset */
#define EXTI_RTSR_OFFSET        (0x08UL)
/* EXTI RTSR - Rising Trigger Selection Register address */
#define EXTI_RTSR_ADDRESS       (EXTI_BASE_ADDRESS + EXTI_RTSR_OFFSET)
#define EXTI_RTSR               (* (volatile uint32_t *)(EXTI_RTSR_ADDRESS)) // typecast and dereference
/* EXTI RTSR TR4 offset (line 4 cause we use PIN 4) */
#define EXTI_RTSR_TR4_OFFSET  (4UL)

/* EXTI PR - Pending Register offset */
#define EXTI_PR_OFFSET        (0x14UL)
/* EXTI PR - Pending Register address */
#define EXTI_PR_ADDRESS       (EXTI_BASE_ADDRESS + EXTI_PR_OFFSET)
#define EXTI_PR               (* (volatile uint32_t *)(EXTI_PR_ADDRESS)) // typecast and dereference
/* EXTI PR4 offset */
#define EXTI_PR_PR4_OFFSET  (4UL)


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

void Priority_IRQ(uint8_t Irq_n, uint8_t Irq_val);

void NVIC_EXTI_Init();


#endif /* NVIC_EXTI_H_ */
