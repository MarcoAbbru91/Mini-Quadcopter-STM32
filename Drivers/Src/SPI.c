/*
 * SPI.c
 *
 *  Created on: Mar 5, 2026
 *      Author: marco91
 */


#include "SPI.h"


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
	//SPI1_CR2 |= (1UL << SPI_CR2_RXNEIE_OFFSET); ////* TODO: Re-enable if required to be read via interrupt *////

	/* Enables SPI */
	SPI1_CR1 |= (1UL << SPI_CR1_SPE_OFFSET);
}



void SPI_Transmit(uint8_t Val)
{
	/* Wait until TX buffer is empty */
	while(!(SPI1_SR & (1UL << SPI_SR_TXE_OFFSET)));
	/* Write to SPI DR register to initiate a write operation */
	SPI1_DR = Val; // Writes the SPI Data buffer
}

uint8_t SPI_Receive(uint8_t DummyRead)
{
	uint32_t Pressure_Val;

	if(DummyRead == 1U) // The first received data (during MOSI transfer) contains no useful data
	{
		/* Wait until RX buffer is empty */
		while(!(SPI1_SR & (1UL << SPI_SR_RXNE_OFFSET)));
		/* Read SPI DR register to clear RXNE flag as well as RX buffer */
		Pressure_Val = SPI1_DR; // RXNE bit is automatically cleared.
		/* Always read SPI_DR register to avoid Overrun Flag (OVR) is set */
		return (0); // This read will be discarded since useless
	}
	else{
		/* Wait until RX buffer is empty */
		while(!(SPI1_SR & (1UL << SPI_SR_RXNE_OFFSET)));
		/* Read SPI DR register to clear RXNE flag as well as RX buffer */
		Pressure_Val = SPI1_DR; // Reads the SPI Data Buffer (8 MSbits are automatically forced to 0 in SPI DR). RXNE bit is automatically cleared.
		/* Always read SPI_DR register to avoid Overrun Flag (OVR) is set */
		return((uint8_t)Pressure_Val);
	}
}

/* SPI Write operation to configure slave's registers */
void SPI_Write(uint8_t Addr, uint8_t Data)
{
	SPI_Transmit(Addr & SPI_Write_Operation); // Send register's address, keeping MSB=0 (write operation)
	SPI_Receive((uint8_t)1U);  // Dummy read operation (discarded) to empty RX buffer

	SPI_Transmit(Data); // Data to be written in the sensor's register
	SPI_Receive((uint8_t)1U); // Dummy read operation (discarded) to empty RX buffer
}

/* Reads data from slave */
uint8_t SPI_Read(uint8_t SPI_Data_Read)
{
	uint8_t RetVal_Pressure;

	SPI_Transmit(SPI_Data_Read | SPI_Read_Operation);
	SPI_Receive((uint8_t)1U);  // Dummy read operation (discarded) to empty RX buffer

	SPI_Transmit((uint8_t)Dummy_Write); // Dummy write operation just to generate clock
	RetVal_Pressure = SPI_Receive((uint8_t)0U);

	return (RetVal_Pressure);
}
