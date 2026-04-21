/*
 * GPIO.c
 *
 *  Created on: 1 mar 2026
 *      Author: marco91
 */

#include "GPIO.h"


void GPIO_Init(void)
{
	/* Enable Clock for GPIOB peripheral - Needed for PWM on TIM4 (Ch1 to Ch4) as well as for the sensors*/
	RCC_AHB1 |= (0x01UL << RCC_AHB1_GPIOB_EN);
	/* Clear and Set port to Alternate Function mode */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_6_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_6_OFFSET); // 2 = Alternate Function Mode
	/* Clear and Set Alternate Function 2 for PB6 port */
	GPIOB_AFRL &= ~(0xFUL << GPIOB_AFRL_6_OFFSET);
	GPIOB_AFRL |=  (0x2UL << GPIOB_AFRL_6_OFFSET);
	/* Clear and Set port to Alternate Function mode */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_7_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_7_OFFSET); // 2 = Alternate Function Mode
	/* Clear and Set Alternate Function 2 for PB7 port */
	GPIOB_AFRL &= ~(0xFUL << GPIOB_AFRL_7_OFFSET);
	GPIOB_AFRL |=  (0x2UL << GPIOB_AFRL_7_OFFSET);
	/* Clear and Set port to Alternate Function mode */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_8_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_8_OFFSET); // 2 = Alternate Function Mode
	/* Clear and Set Alternate Function 2 for PB8 port */
	GPIOB_AFRH &= ~(0xFUL << GPIOB_AFRH_8_OFFSET);
	GPIOB_AFRH |=  (0x2UL << GPIOB_AFRH_8_OFFSET);
	/* Clear and Set port to Alternate Function mode */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_9_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_9_OFFSET); // 2 = Alternate Function Mode
	/* Clear and Set Alternate Function 2 for PB9 port */
	GPIOB_AFRH &= ~(0xFUL << GPIOB_AFRH_9_OFFSET);
	GPIOB_AFRH |=  (0x2UL << GPIOB_AFRH_9_OFFSET);
	/* CS SPI pin for the magnetometer sensor */
	/* Clear and Set port to digital output mode */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_12_OFFSET);
	GPIOB_MODER |=  (0x1UL << GPIOB_MODER_12_OFFSET); // General Purpose Output mode
	/* Sets speed for PB12 */
	GPIOB_OSPEEDR |=  (0x3UL << GPIOB_OSPEEDR_12_OFFSET); // Very High Speed
	/* CLK SPI pin for the sensors */
	/* Clear and Set port 13 to Alternate Function mode 13 */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_13_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_13_OFFSET); // 2 = Alternate Function Mode
	/* Sets Speed for PA13 */
	GPIOB_OSPEEDR |=  (0x3UL << GPIOB_OSPEEDR_13_OFFSET); // Very High Speed
	/* Clear and Set Alternate Function 13 for PB13 port */
	GPIOB_AFRH &= ~(0xFUL << GPIOB_AFRH_13_OFFSET);
	GPIOB_AFRH |=  (0x5UL << GPIOB_AFRH_13_OFFSET);
	/* SDO SPI pin for the sensors */
	/* Clear and Set port 14 to Alternate Function mode 14 */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_14_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_14_OFFSET); // 2 = Alternate Function Mode
	/* Sets Speed for PB14 */
	GPIOB_OSPEEDR |=  (0x3UL << GPIOB_OSPEEDR_14_OFFSET); // Very High Speed
	/* Clear and Set Alternate Function 14 for PB14 port */
	GPIOB_AFRH &= ~(0xFUL << GPIOB_AFRH_14_OFFSET);
	GPIOB_AFRH |=  (0x5UL << GPIOB_AFRH_14_OFFSET);
	/* Clear bits for PB14 port */
	GPIOB_PUPDR &= ~(0x3UL << GPIOB_PUPDR_14_OFFSET); // no pull-up/down
	/* SDI SPI pin for the sensors */
	/* Clear and Set port 15 to Alternate Function mode 15 */
	GPIOB_MODER &= ~(0x3UL << GPIOB_MODER_15_OFFSET);
	GPIOB_MODER |=  (0x2UL << GPIOB_MODER_15_OFFSET); // 2 = Alternate Function Mode
	/* Sets Speed for PB15 */
	GPIOB_OSPEEDR |=  (0x3UL << GPIOB_OSPEEDR_15_OFFSET); // Very High Speed
	/* Clear and Set Alternate Function 15 for PB15 port */
	GPIOB_AFRH &= ~(0xFUL << GPIOB_AFRH_15_OFFSET);
	GPIOB_AFRH |=  (0x5UL << GPIOB_AFRH_15_OFFSET);

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

