/*
 * GPIO.c
 *
 *  Created on: 1 mar 2026
 *      Author: marco91
 */

#include "GPIO.h"


void GPIO_Init(void)
{
	/* Enable Clock for GPIOB peripheral - Needed for PWM on TIM4 (Ch1 to Ch4) */
	RCC_AHB1 |= (0x01UL << RCC_AHB1_GPIOB_EN);
	/* Clear and Set port to Alternate Function mode */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_6_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_6_OFFSET);
	/* Clear and Set Alternate Function 2 for PB6 port */
	GPIOB_AFRL &= ~(0xFUL << GPIOB_AFRL_6_OFFSET);
	GPIOB_AFRL |=  (0x2UL << GPIOB_AFRL_6_OFFSET);

	/* CS SPI pin for the magnetometer sensor */
	/* Clear and Set port to digital output mode */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_12_OFFSET);
	GPIOB_MODER |=  (0x1UL << GPIOB_MODER_12_OFFSET); // General Purpose Output mode
	/* Sets speed for PC13 */
	GPIOB_OSPEEDR |=  (0x3UL << GPIOB_OSPEEDR_12_OFFSET); // Very High Speed


	/* Enable Clock for GPIOA peripheral - Needed for SCK, MOSI, MISO SPI pins for the three sensors */
	RCC_AHB1 |= (0x01UL << RCC_AHB1_GPIOA_EN);
	/* Clear and Set port 5 to Alternate Function mode 5 */
	GPIOA_MODER &= ~(0x3UL << GPIOA_MODER_5_OFFSET);
	GPIOA_MODER |=  (0x2UL << GPIOA_MODER_5_OFFSET);
	/* Clear and Set port 6 to Alternate Function mode 6 */
	GPIOA_MODER &= ~(0x3UL << GPIOA_MODER_6_OFFSET);
	GPIOA_MODER |=  (0x2UL << GPIOA_MODER_6_OFFSET);
	/* Clear and Set port 7 to Alternate Function mode 7 */
	GPIOA_MODER &= ~(0x3UL << GPIOA_MODER_7_OFFSET);
	GPIOA_MODER |=  (0x2UL << GPIOA_MODER_7_OFFSET);
	/* Clear and Set port 8 to digital output mode */
	GPIOA_MODER &= ~(0x3UL << GPIOA_MODER_8_OFFSET);
	GPIOA_MODER |=  (0x1UL << GPIOA_MODER_8_OFFSET); // General Purpose Output mode
	/* Sets Speed for PA5 */
	GPIOA_OSPEEDR |=  (0x3UL << GPIOA_OSPEEDR_5_OFFSET); // Very High Speed
	/* Sets Speed for PA6 */
	GPIOA_OSPEEDR |=  (0x3UL << GPIOA_OSPEEDR_6_OFFSET); // Very High Speed
	/* Sets Speed for PA7 */
	GPIOA_OSPEEDR |=  (0x3UL << GPIOA_OSPEEDR_7_OFFSET); // Very High Speed
	/* Sets speed for PA8 */
	GPIOA_OSPEEDR |=  (0x3UL << GPIOA_OSPEEDR_8_OFFSET); // Very High Speed
	/* Clear and Set Alternate Function 5 for PA5 port - SCK */
	GPIOA_AFRL &= ~(0xFUL << GPIOA_AFRL_5_OFFSET);
	GPIOA_AFRL |=  (0x5UL << GPIOA_AFRL_5_OFFSET);
	/* Clear and Set Alternate Function 5 for PA6 port - MISO */
	GPIOA_AFRL &= ~(0xFUL << GPIOA_AFRL_6_OFFSET);
	GPIOA_AFRL |=  (0x5UL << GPIOA_AFRL_6_OFFSET);
	/* Clear and Set Alternate Function 5 for PA7 port - MOSI */
	GPIOA_AFRL &= ~(0xFUL << GPIOA_AFRL_7_OFFSET);
	GPIOA_AFRL |=  (0x5UL << GPIOA_AFRL_7_OFFSET);



	/* Enable Clock for GPIOC peripheral - Needed for CS SPI pin for the pressure sensor */
	RCC_AHB1 |= (0x01UL << RCC_AHB1_GPIOC_EN);
	/* Clear and Set port to digital output mode */
	GPIOC_MODER &= ~(0x3UL << GPIOC_MODER_13_OFFSET);
	GPIOC_MODER |=  (0x1UL << GPIOC_MODER_13_OFFSET); // General Purpose Output mode
	/* Sets speed for PC13 */
	GPIOC_OSPEEDR |=  (0x3UL << GPIOC_OSPEEDR_13_OFFSET); // Very High Speed
}

