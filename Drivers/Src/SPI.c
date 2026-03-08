/*
 * SPI.c
 *
 *  Created on: Mar 5, 2026
 *      Author: marco91
 */


#include "SPI.h"
#include "RCC.h"


void SPI_Init()
{
	/* Enable Clock for SPI1 peripheral */
	RCC_APB2 |= (0x01UL << RCC_APB2_SPI1_EN);

	/* Clears/sets SPI Clock Phase */
	SPI1_CR1 &= ~(1UL << SPI_CR1_CPHA_OFFSET); // first clock transition is the first data capture edge
	/* Clears/sets SPI Clock Polarity */
	SPI1_CR1 &= ~(1UL << SPI_CR1_CPOL_OFFSET); // CLK=0 when idle
	/* Sets SPI Master Selection */
	SPI1_CR1 |= (1UL << SPI_CR1_MSTR_OFFSET);
	/* Clears and sets SPI Baud Rate */
	SPI1_CR1 &= ~(7UL << SPI_CR1_BR_OFFSET); // Clear 3 bits
	SPI1_CR1 |= (2UL << SPI_CR1_BR_OFFSET); // SPI Baud Rate reduced to (Clock Freq)/8
	/* Clears/sets SPI LSB/MSB first */
	SPI1_CR1 &= ~(1UL << SPI_CR1_LSBFIRST_OFFSET); // MSB first
	/* SPI Software SSN Management */
	SPI1_CR1 |= (1UL << SPI_CR1_SSM_OFFSET); // Software slave select (not Hardware)
	/* Set SPI SSN (Slave Select) */
	SPI1_CR1 |= (1UL << SPI_CR1_SSI_OFFSET);
	/* Clears/sets SPI Half/Full-duplex communication */
	SPI1_CR1 &= ~(1UL << SPI_CR1_RXONLY_OFFSET); // Full Duplex communication
	/* Sets SPI Data Frame Format */
	SPI1_CR1 &= ~(1UL << SPI_CR1_DFF_OFFSET); // Sensors use 8-bit data
	/* Clears/sets SPI uni/bi-directional communication */
	SPI1_CR1 &= ~(1UL << SPI_CR1_BIDIMODE_OFFSET); // 2-lines unidirectional mode

	/* Enables SPI Interrupt on data reception completion */
	SPI1_CR2 |= (1UL << SPI_CR2_RXNEIE_OFFSET);

	/* Enables SPI */
	SPI1_CR1 |= (1UL << SPI_CR1_SPE_OFFSET);
}



void SPI_Transmit()
{

}

void SPI_Receive()
{

}
