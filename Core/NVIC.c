/*
 * ISR.c
 *
 *  Created on: 24 feb 2026
 *      Author: marco91
 */

#include <stdint.h>
#include "NVIC.h"
#include "Timer.h"
#include "SPI.h"


/****************************************************************************
Global variables
****************************************************************************/
extern uint32_t *pSPI_ReadBuffer;


void Priority_IRQ(uint8_t Irq_n, uint8_t Irq_val)
{
	/* Calculate required Interrupt Priority Register address and offset */
	uint8_t  IPR_Numb = (Irq_n / 4); // For each IPRx we can configure up to 3 IRQs. For this reason the division by 4
	uint32_t *pIPR_Addr = (pNVIC_IPR_BaseAddr + IPR_Numb);// Address of IPRx related to our IRQ number
	uint8_t  IPR_AddrOffset = (Irq_n % 4) * 8; // Calculates the offset inside the IPRx in which we can write our IRQ value

	/* Sets IRQ value */
	*pIPR_Addr &= ~(0xFF << IPR_AddrOffset); // Clears current value
	*pIPR_Addr |=  (Irq_val << IPR_AddrOffset); // Clears current value
}

void NVIC_Init()
{
	uint8_t  ISER_Numb;
	uint32_t *pISER_Addr;
	uint8_t  ISER_AddrOffset;

	/* Sensors */
	Priority_IRQ(SPI1_IRQn, Prio_Lev_4); // 16 different priority levels are allowed by the ARM CPU. The lower the higher.

	/****** Enable IRQs in NVIC ******/

	/* Calculate required address and offset for SPI1 */
	ISER_Numb = (SPI1_IRQn / 32); // For each ISERx we can configure up to 32 IRQs. For this reason the division by 32
	pISER_Addr = (pNVIC_ISER_BaseAddr + ISER_Numb);// Address of IPRx related to our IRQ number
	ISER_AddrOffset = (SPI1_IRQn % 32); // Calculates the offset inside the IPRx in which we can write our IRQ value
	*pISER_Addr |= (1 << ISER_AddrOffset);
}



void TIM4_IRQHandler(void)
{
    if(TIM4_SR & (1UL << TIM4_SR_UIF_OFFSET))
    {
        TIM4_SR &= ~(1UL << TIM4_SR_UIF_OFFSET); // Reset the "Update Interrupt Flag" which was set due to overflow on Timer4
    }
}


void SPI1_IRQHandler(void)
{
    if(SPI1_SR & (1UL << SPI_SR_RXNE_OFFSET))
    {
    	*pSPI_ReadBuffer = (0UL || SPI1_DR); // Reads the SPI Data Buffer (sets 16 MSbits to 0). RXNE bit is automatically cleared.
    }

}
