/*
 * SPI.c
 *
 *  Created on: Mar 5, 2026
 *      Author: marco91
 */


#include "SPI.h"





void SPI_Init()
{
	/******** SPI2 initialization ********/

	/* Enable Clock for SPI1 peripheral */
	RCC_APB1 |= (0x01UL << RCC_APB1_SPI2_EN);

	/* Disable SPI */
	SPI2_CR1 &= ~(1UL << SPI_CR1_SPE_OFFSET);

	/* Clears/sets SPI Clock Phase */
	SPI2_CR1 &= ~(1UL << SPI_CR1_CPHA_OFFSET); // first clock transition is the first data capture edge
	SPI2_CR1 |= (1 << SPI_CR1_CPHA_OFFSET); // CPHA and CPOL set to 1 to have the SPI2 in Mode 3 for the sensors
	/* Clears/sets SPI Clock Polarity */
	SPI2_CR1 &= ~(1UL << SPI_CR1_CPOL_OFFSET); // CLK=0 when idle
	SPI2_CR1 |= (1 << SPI_CR1_CPOL_OFFSET); // CPHA and CPOL set to 1 to have the SPI2 in Mode 3 for the sensors
	/* Sets SPI Master Selection */
	SPI2_CR1 |= (1UL << SPI_CR1_MSTR_OFFSET);
	/* Clears and sets SPI Baud Rate */
	SPI2_CR1 &= ~(7UL << SPI_CR1_BR_OFFSET); // Clear 3 bits
	SPI2_CR1 |= (3UL << SPI_CR1_BR_OFFSET); // SPI Baud Rate reduced to (Clock Freq)/16 (to have SPI clock freq lower than its maximum, i.e. 10 MHz)
	/* Clears/sets SPI LSB/MSB first */
	SPI2_CR1 &= ~(1UL << SPI_CR1_LSBFIRST_OFFSET); // MSB first
	/* SPI Software SSN Management */
	SPI2_CR1 |= (1UL << SPI_CR1_SSM_OFFSET); // Software slave select (not Hardware)
	/* Set SPI SSN (Slave Select) */
	SPI2_CR1 |= (1UL << SPI_CR1_SSI_OFFSET);
	/* Clears/sets SPI Half/Full-duplex communication */
	SPI2_CR1 &= ~(1UL << SPI_CR1_RXONLY_OFFSET); // Full Duplex communication
	/* Sets SPI Data Frame Format */
	SPI2_CR1 &= ~(1UL << SPI_CR1_DFF_OFFSET); // Sensors use 8-bit data
	/* Clears/sets SPI uni/bi-directional communication */
	SPI2_CR1 &= ~(1UL << SPI_CR1_BIDIMODE_OFFSET); // 2-lines unidirectional mode

	/* Enables SPI Interrupt on data reception completion */
	//SPI1_CR2 |= (1UL << SPI_CR2_RXNEIE_OFFSET); ////* TODO: Re-enable if required to be read via interrupt *////

	/* Enables SPI2 */
	SPI2_CR1 |= (1UL << SPI_CR1_SPE_OFFSET);


	/******** SPI1 initialization ********/

	/* Enable Clock for SPI1 peripheral */
	//RCC_APB2 |= (0x01UL << RCC_APB2_SPI1_EN);

	/* Clears/sets SPI Clock Phase */
	//SPI1_CR1 &= ~(1UL << SPI_CR1_CPHA_OFFSET); // first clock transition is the first data capture edge
	/* Clears/sets SPI Clock Polarity */
	//SPI1_CR1 &= ~(1UL << SPI_CR1_CPOL_OFFSET); // CLK=0 when idle
	/* Sets SPI Master Selection */
	//SPI1_CR1 |= (1UL << SPI_CR1_MSTR_OFFSET);
	/* Clears and sets SPI Baud Rate */
	//SPI1_CR1 &= ~(7UL << SPI_CR1_BR_OFFSET); // Clear 3 bits
	//SPI1_CR1 |= (3UL << SPI_CR1_BR_OFFSET);  // fPCLK/16 (safe < 10 MHz)
	/* Clears/sets SPI LSB/MSB first */
	//SPI1_CR1 &= ~(1UL << SPI_CR1_LSBFIRST_OFFSET); // MSB first
	/* SPI Software SSN Management */
	//SPI1_CR1 |= (1UL << SPI_CR1_SSM_OFFSET); // Software slave select
	//SPI1_CR1 |= (1UL << SPI_CR1_SSI_OFFSET); // Internal NSS high
	/* Clears RXONLY (must be 0 for bidirectional mode) */
	//SPI1_CR1 &= ~(1UL << SPI_CR1_RXONLY_OFFSET);
	/* Sets SPI Data Frame Format */
	//SPI1_CR1 &= ~(1UL << SPI_CR1_DFF_OFFSET); // 8-bit data

	/* Enable 1-line bidirectional mode */
	//SPI1_CR1 |= (1UL << SPI_CR1_BIDIMODE_OFFSET);
	/* Start in RX mode (BIDIOE = 0) */
	//SPI1_CR1 &= ~(1UL << SPI_CR1_BIDIOE_OFFSET);

	/* Enables SPI Interrupt */
	//SPI1_CR2 |= (1UL << SPI_CR2_RXNEIE_OFFSET); // TODO; Enable in future if needed

	/* Enable SPI1 */
	//SPI1_CR1 |= (1UL << SPI_CR1_SPE_OFFSET);
}




/* Flush any stale data from the Rx buffer and clear OVR flag.*/
inline void SPI2_FlushRX(void)
{
	volatile uint8_t dummy;
	while(SPI2_SR & (1UL << SPI_SR_RXNE_OFFSET))
	{
		dummy = *(volatile uint8_t *)&SPI2_DR;
	}
	/* Reading SR after DR clears the OVR flag if set */
	dummy = (volatile uint8_t)SPI2_SR;
	(void)dummy; // to avoid warning of unused variable
}

/* Full-Duplex Transmit function */
void SPI2_Transmit(uint8_t Val)
{
	//uint32_t Timeout = 10000; // TODO: To create a timeout timer of approx. 1ms, considering Clock=84MHz and below while-loop's iteration taking around 5-10 clock cycles.

	/* Wait until SPI TX buffer is empty */
	while(!(SPI2_SR & (1UL << SPI_SR_TXE_OFFSET)))
	{
		//TODO add return value in case of error
		//if (--Timeout == 0) return (-1); // To avoid infinite loop in case of e.g. HW fail
	}

	/* Write to SPI DR register to initiate a write operation */
	*(volatile uint8_t *)&SPI2_DR = Val; // Writes the SPI Data buffer
}

/* Full-Duplex Receive function */
uint8_t SPI2_Receive(uint8_t DummyRead)
{
	uint8_t RetVal_SPI;

	if(DummyRead == 1U) // The first received data (during MOSI transfer) contains no useful data
	{
		/* Wait until RX buffer is empty */
		while(!(SPI2_SR & (1UL << SPI_SR_RXNE_OFFSET)));
		/* Read SPI DR register to clear RXNE flag as well as RX buffer */
		RetVal_SPI = *(volatile uint8_t *)&SPI2_DR; // RXNE bit is automatically cleared.
		/* Always read SPI_DR register to avoid Overrun Flag (OVR) is set */

		return (0); // This read will be discarded since useless
	}
	else{
		/* Wait until RX buffer is empty */
		while(!(SPI2_SR & (1UL << SPI_SR_RXNE_OFFSET)));
		/* Read SPI DR register to clear RXNE flag as well as RX buffer */
		RetVal_SPI = *(volatile uint8_t *)&SPI2_DR; // Reads the SPI Data Buffer (8 MSbits are automatically forced to 0 in SPI DR). RXNE bit is automatically cleared.
		/* Always read SPI_DR register to avoid Overrun Flag (OVR) is set */

		return(RetVal_SPI);
	}
}


/* SPI Write operation usually called only during init phase to configure slave's registers */
void SPI2_Write(uint8_t Addr, uint8_t Data)
{
	SPI2_Transmit(Addr & SPI_Write_Operation); // Send register's address, keeping MSB=0 (write operation)
	SPI2_Receive((uint8_t)1U);  // Dummy read operation (discarded) to empty RX buffer

	SPI2_Transmit(Data); // Data to be written in the sensor's register
	SPI2_Receive((uint8_t)1U); // Dummy read operation (discarded) to empty RX buffer
}

/* SPI Read operation to read runtime data from slave */
uint8_t SPI2_Read(uint8_t SPI_Data_Read)
{
	uint8_t RetVal_SPI;

	SPI2_Transmit(SPI_Data_Read | SPI_Read_Operation);
	SPI2_Receive((uint8_t)1U);  // Dummy read operation (discarded) to empty RX buffer

	SPI2_Transmit((uint8_t)Dummy_Write); // Dummy write operation just to generate clock
	RetVal_SPI = SPI2_Receive((uint8_t)0U);

	return (RetVal_SPI);
}



/* SPI Half-duplex - Clear RX buffer (to avoid overrun) */
inline void SPI_ClearRX(void)
{
	volatile uint8_t dummy;
	dummy = (uint8_t)SPI2_DR;
	dummy = (uint8_t)SPI2_SR;
	(void)dummy;
}

/* SPI Half-duplex - Send buffer over SPI (1-line TX mode) */
void SPI_HD_Write(uint8_t *data, uint16_t len)
{
	/* Ensure SPI not busy before switching direction */
	while (SPI2_SR & (1UL << SPI_SR_BSY_OFFSET));

	/* Set TX mode (BIDIOE = 1) */
	SPI2_CR1 |= (1UL << SPI_CR1_BIDIOE_OFFSET);

	for (uint16_t i=0; i<len; i++)
	{
		while (!(SPI2_SR & (1UL << SPI_SR_TXE_OFFSET)));
		/* Write byte */
		*((volatile uint8_t*)&SPI2_DR) = data[i];

		// Wait transfer complete, RXNE, is not necessary in half-duplex communication. Since is one-line communication, if we transmit we can't expect to receive (valid) data back
	}

	/* Wait until SPI finished */
	///while (SPI2_SR & (1UL << SPI_SR_BSY_OFFSET));

	/* Clear possible RX overrun */
	SPI_ClearRX();
}

/* SPI Half-duplex - Receive buffer over SPI (1-line RX mode) */
void SPI_HD_Read(uint8_t *data, uint16_t len)
{
	/* Ensure SPI not busy */
	///while (SPI2_SR & (1UL << SPI_SR_BSY_OFFSET));

	/* Set RX mode (BIDIOE = 0) */
	SPI2_CR1 &= ~(1UL << SPI_CR1_BIDIOE_OFFSET);

	for (uint16_t i=0; i<len; i++)
	{
		while (!(SPI2_SR & (1UL << SPI_SR_TXE_OFFSET)));
		/* Generate clock by writing dummy */
		*((volatile uint8_t*)&SPI2_DR) = 0xFF;

		/* Wait data received */
		while (!(SPI2_SR & (1UL << SPI_SR_RXNE_OFFSET)));

		data[i] = *((volatile uint8_t*)&SPI2_DR);
	}

	/* Wait until SPI finished */
	while (SPI2_SR & (1UL << SPI_SR_BSY_OFFSET));
}
