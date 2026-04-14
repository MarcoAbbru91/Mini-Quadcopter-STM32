/*
 * ISR.c
 *
 *  Created on: 24 feb 2026
 *      Author: marco91
 */

#include <NVIC_EXTI.h>


/****************************************************************************
Global variables
****************************************************************************/
//#define NULL         ((void *)0)
volatile uint32_t SysTick_Counter = 0UL;




void Priority_IRQ(uint8_t Irq_n, uint8_t Irq_val)
{
	/* Calculate required Interrupt Priority Register address and offset */
	uint8_t  IPR_Numb = (Irq_n / 4); // For each IPRx we can configure up to 3 IRQs. For this reason the division by 4
	volatile uint32_t *pIPR_Addr = (NVIC_IPR_BASEADDR + IPR_Numb);// Address of IPRx related to our IRQ number
	uint8_t  IPR_AddrOffset = (Irq_n % 4) * 8; // Calculates the offset inside the IPRx in which we can write our IRQ value

	/* Sets IRQ value */
	*pIPR_Addr &= ~(0xFF << IPR_AddrOffset); // Clears current value
	*pIPR_Addr |=  (Irq_val << IPR_AddrOffset); // Clears current value
}

void NVIC_EXTI_Init()
{
	uint8_t  ISER_Numb;
	volatile uint32_t *pISER_Addr;
	uint8_t  ISER_AddrOffset;

	/****** Set priority for IRQs ******/
	/* PWM for motors */
	Priority_IRQ((uint8_t)TIM4_IRQn, (uint8_t)Prio_Lev_1); // 16 different priority levels are allowed by the ARM CPU. The lower the higher.
	/* BLE */
	Priority_IRQ((uint8_t)EXTI4_IRQn, (uint8_t)Prio_Lev_3); // 16 different priority levels are allowed by the ARM CPU. The lower the higher.
	/* Sensors */ ////* TODO: Re-enable if required to be read via interrupt *////
	//Priority_IRQ((uint8_t)SPI1_IRQn, (uint8_t)Prio_Lev_4); // 16 different priority levels are allowed by the ARM CPU. The lower the higher.



	/****** Enable IRQs in NVIC ******/
	/* For TIM4 no need for any address/offset calculation */
	*NVIC_ISER_BASEADDR |= (1 << TIM4_IRQn);
	/* For EXTI4 (BLE) calculate required address and offset */
	ISER_Numb = (EXTI4_IRQn / 32); // For each ISERx we can configure up to 32 IRQs. For this reason the division by 32
	pISER_Addr = (NVIC_ISER_BASEADDR + ISER_Numb);// Address of IPRx related to our IRQ number
	ISER_AddrOffset = (EXTI4_IRQn % 32); // Calculates the offset inside the IPRx in which we can write our IRQ value
	*pISER_Addr |= (1UL << ISER_AddrOffset);
	/* For SPI1 calculate required address and offset */ ////* TODO: Re-enable if required to be read via interrupt *////
	//ISER_Numb = (SPI1_IRQn / 32); // For each ISERx we can configure up to 32 IRQs. For this reason the division by 32
	//pISER_Addr = (pNVIC_ISER_BaseAddr + ISER_Numb);// Address of IPRx related to our IRQ number
	//ISER_AddrOffset = (SPI1_IRQn % 32); // Calculates the offset inside the IPRx in which we can write our IRQ value
	//*pISER_Addr |= (1 << ISER_AddrOffset);



	/****** General Enables for NVIC and EXTI ******/
	/* SysTick Exception Request Enable */
	SYSTICK_CTRL |= (0x01UL << SYSTICK_CTRL_TICKINT_OFFSET); // Enable Exception request

	/* EXTI Interrupt Mask Register */
	EXTI_IMR |= (1UL << EXTI_IMR_MR4_OFFSET); // Set line 4
	/* EXTI Rising Trigger Selection */
	EXTI_RTSR |= (1UL << EXTI_RTSR_TR4_OFFSET); // Set line 4
}



void SysTick_Handler(void)
{
	SysTick_Counter++;
}

void TIM4_IRQHandler(void)
{
	if(TIM4_SR & (1UL << TIM4_SR_UIF_OFFSET))
	{
		TIM4_SR &= ~(1UL << TIM4_SR_UIF_OFFSET); // Reset the "Update Interrupt Flag" which was set due to overflow on Timer4
	}
}

void EXTI4_IRQHandler(void)
{
	if (EXTI_PR & (1UL << EXTI_PR_PR4_OFFSET)) // Pending interrupt
	{
		EXTI_PR = (1UL << EXTI_PR_PR4_OFFSET); // Clear flag

		BLE_EXTI_IRQHandler();
	}
}


////* TODO: Re-enable if required to be read via interrupt *////
//void SPI1_IRQHandler(void)
//{
//    if(SPI1_SR & (1UL << SPI_SR_RXNE_OFFSET))
//    {
//    	SPI_ReadBuffer = (0UL || SPI1_DR); // Reads the SPI Data Buffer (sets 16 MSbits to 0). RXNE bit is automatically cleared.
//    }

//}
