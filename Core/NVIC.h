/*
 * IRQ.h
 *
 *  Created on: Mar 6, 2026
 *      Author: marco91
 */

#ifndef NVIC_H_
#define NVIC_H_


#include <stdint.h>


/* Cortex-M4 Processor Exception's and Interrupt's IRQs */
#define SPI1_IRQn 35U // SPI1 global Interrupt


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


uint32_t *pNVIC_ISER_BaseAddr = (uint32_t *)0xE000E200; // Interrupt Set-Enable Register base address
uint32_t *pNVIC_IPR_BaseAddr  = (uint32_t *)0xE000E400; // Interrupt Priority Register base address


void Priority_IRQ(uint8_t Irq_n, uint8_t Irq_val);

void NVIC_Init();


#endif /* NVIC_H_ */
