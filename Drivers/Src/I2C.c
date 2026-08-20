/*
 * I2C.c
 *
 *  Created on: Jul 30, 2026
 *      Author: marco91
 */


#include "I2C.h"


/* Waits until the requested flag of the Status Register 1 is set.
   Aborts as soon as a NACK (AF), a bus error (BERR) or an arbitration loss (ARLO) is detected,
   as well as when the timeout expires (to avoid infinite loops in case of e.g. HW fail) */
static uint8_t I2C_WaitFlagSR1(uint32_t RegFlagOffset)
{
	uint32_t Timeout = I2C_TIMEOUT;

	while(!(I2C_SR1 & (1UL << RegFlagOffset)))
	{
		/* An error aborts the transfer: the flags are cleared by the caller through I2C_Abort() */
		if(I2C_SR1 & ((1UL << I2C_SR1_AF_OFFSET) | (1UL << I2C_SR1_BERR_OFFSET) | (1UL << I2C_SR1_ARLO_OFFSET)))
		{
			return (I2C_ERROR);
		}
		if(--Timeout == 0UL)
		{
			return (I2C_ERROR);
		}
	}

	return (I2C_OK);
}


void I2C_Init()
{
	/* Enable Clock for I2C peripheral */
	RCC_APB1 |= (0x01UL << RCC_APB1_I2C_EN);

	/* Disable I2C */
	I2C_CR1 &= ~(1UL << I2C_CR1_PE_OFFSET);

	I2C_CR1 = 0x0000; // Reset register (I2C mode, no SMBus, no PEC, clock stretching enabled)
	I2C_CR2 = 0x0000; // Reset register

	/* Clears and sets the peripheral input clock frequency, needed by the HW to generate the timings */
	I2C_CR2 &= ~(0x3FUL << I2C_CR2_FREQ_OFFSET); // Clear 6 bits
	I2C_CR2 |=  (I2C_CR2_FREQ_VAL << I2C_CR2_FREQ_OFFSET); // PCLK1 = 42MHz

	/* Clears/sets I2C Standard/Fast Mode selection and the clock divider */
	I2C_CCR = 0x0000; // Reset register
#if (I2C_SCL_FREQ_HZ > 100000UL)
	I2C_CCR |=  (1UL << I2C_CCR_FS_OFFSET); // Fast Mode (SCL up to 400kHz)
	I2C_CCR &= ~(1UL << I2C_CCR_DUTY_OFFSET); // Tlow/Thigh = 2
#else
	I2C_CCR &= ~(1UL << I2C_CCR_FS_OFFSET); // Standard Mode (SCL up to 100kHz)
#endif
	I2C_CCR &= ~(0xFFFUL << I2C_CCR_CCR_OFFSET); // Clear 12 bits
	I2C_CCR |=  (I2C_CCR_VAL << I2C_CCR_CCR_OFFSET); // SCL frequency = I2C_SCL_FREQ_HZ

	/* Clears and sets the maximum SCL rise time */
	I2C_TRISE &= ~(0x3FUL << I2C_TRISE_TRISE_OFFSET); // Clear 6 bits
	I2C_TRISE |=  (I2C_TRISE_VAL << I2C_TRISE_TRISE_OFFSET);

	/* Noise filters configuration */
	I2C_FLTR = 0x0000; // Analog filter enabled (ANOFF=0) and digital filter disabled (DNF=0)

	/* Own address configuration: not used since the MCU always acts as master, but bit 14 of OAR1
	   must always be kept at 1 by software */
	I2C_OAR1 = 0x0000; // Reset register
	I2C_OAR1 &= ~(1UL << I2C_OAR1_ADDMODE_OFFSET); // 7-bit slave addressing mode
	I2C_OAR1 |=  (1UL << I2C_OAR1_KEEPAT1_OFFSET);
	I2C_OAR2 = 0x0000; // Dual addressing mode disabled

	/* Enables I2C Interrupts on event/error */
	//I2C_CR2 |= (1UL << I2C_CR2_ITEVTEN_OFFSET); ////* TODO: Re-enable if required to be handled via interrupt *////
	//I2C_CR2 |= (1UL << I2C_CR2_ITERREN_OFFSET); ////* TODO: Re-enable if required to be handled via interrupt *////

	/* Enables I2C */
	I2C_CR1 |= (1UL << I2C_CR1_PE_OFFSET);

	/* Sets I2C Acknowledge generation, so that the received bytes are acknowledged by default (only valid once PE=1) */
	I2C_CR1 |= (1UL << I2C_CR1_ACK_OFFSET);
}


/* Generates a START condition */
uint8_t I2C_Start(void)
{
	uint8_t retVal;
	//uint32_t Timeout = I2C_TIMEOUT;

	/* By default, the I2C device operates in target mode; it switches then from target to controller AFTER
       it generates a START condition. Therefore at this point we are in target mode, and when setting the
	   START bit we should check that the bus is free. However in our case we have only one master (the STM32
	   micro) and different slaves (the sensors), therefore we assume the bus was left free after the previous transfer */

	/* Generate the START condition */
	I2C_CR1 |= (1UL << I2C_CR1_START_OFFSET);

	/* Wait until the START condition is generated and the master mode is entered.
	   The SB flag is then cleared by reading SR1 (done here) followed by writing DR (done by I2C_SendAddress()) */
	retVal = I2C_WaitFlagSR1(I2C_SR1_SB_OFFSET);
	
	return (retVal);
}

/* Generates a STOP condition to release the bus */
inline void I2C_Stop(void)
{
	I2C_CR1 |= (1UL << I2C_CR1_STOP_OFFSET);
}

/* Sends the address byte, built from the 7-bit slave address (right aligned) and from the direction bit */
uint8_t I2C_SendAddress(uint8_t SlaveAddr, uint8_t Direction)
{
	if(Direction == I2C_Direction_Read)
	{
		I2C_DR = (uint32_t)((uint8_t)(SlaveAddr << 1U) | I2C_Read_Operation); // LSB=1 (read operation)
	}
	else
	{
		I2C_DR = (uint32_t)((uint8_t)(SlaveAddr << 1U) & I2C_Write_Operation); // LSB=0 (write operation)
	}

	/* Wait until the address is sent and acknowledged by the slave.
	   The ADDR flag is left set on purpose: it must be cleared by the caller through I2C_ClearADDR() */
	return (I2C_WaitFlagSR1(I2C_SR1_ADDR_OFFSET));
}

/* Clears the ADDR flag. The HW requires SR1 to be read first and SR2 afterwards */
void I2C_ClearADDR(void)
{
	volatile uint32_t dummy;

	/* Reads in sequence SR1 and SR2 registerd to clear the ADDR flag, as mentioned in the I2C datasheet*/
	dummy = I2C_SR1;
	dummy = I2C_SR2;
	(void)dummy; // to avoid warning of variable set but not used
}

/* Clears the error flags and releases the bus. It is called on every failing transfer */
void I2C_Abort(void)
{
	/* The error flags are cleared by writing 0 (the other flags of SR1 are read-only, so they are not affected) */
	I2C_SR1 &= ~((1UL << I2C_SR1_AF_OFFSET) | (1UL << I2C_SR1_BERR_OFFSET) | (1UL << I2C_SR1_ARLO_OFFSET) | (1UL << I2C_SR1_OVR_OFFSET));

	/* Restore the default acknowledge configuration for the next transfer */
	I2C_CR1 &= ~(1UL << I2C_CR1_POS_OFFSET);
	I2C_CR1 |=  (1UL << I2C_CR1_ACK_OFFSET);

	/* IMPO: Release the bus by generating a STOP condition */
	I2C_CR1 |= (1UL << I2C_CR1_STOP_OFFSET);
}

/* Transmit function for the external devices (magnetometer sensor in this case) */
uint8_t I2C_Transmit(uint8_t Val)
{
	/* Wait until I2C TX buffer is empty */
	if(I2C_WaitFlagSR1(I2C_SR1_TXE_OFFSET) != I2C_OK)
	{
		return (I2C_ERROR);
	}

	/* Write to I2C DR register to initiate a write operation */
	I2C_DR = (uint32_t)Val; // Writes the I2C Data Register

	return (I2C_OK);
}

/* I2C Write operation, usually called only during init phase to configure slave's registers.
   I2C Write operation of a whole buffer: the slave increments its register pointer at every byte,
   so a single frame "START - Addr(W) - RegAddr - Data0 .. DataN - STOP" is used */
uint8_t I2C_Write(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t Data)
{
	/* START condition */
	if(I2C_Start() != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}
	/* Slave (magnetometer) address with write direction */
	if(I2C_SendAddress(SlaveAddr, I2C_Direction_Write) != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}
	I2C_ClearADDR();

	/* Address of the first register to be written */
	if(I2C_Transmit(RegAddr) != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}

	/* Data to be written in the slave's registers */
	/* The I2C protocol is used only by magnetometer sensor, which has 8-bits registers. Therefore 1 write operation is enough */
	if(I2C_Transmit(Data) != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}

	/* Wait until the last byte is actually shifted out on the bus, before releasing it */
	if(I2C_WaitFlagSR1(I2C_SR1_BTF_OFFSET) != I2C_OK) // BTF - Byte Transfer Finished
	{
		I2C_Abort();
		return (I2C_ERROR);
	}

	/* STOP condition */
	I2C_Stop();

	return (I2C_OK);
}

/* I2C Read operation for the external devices, to read runtime data from slave */
uint8_t I2C_Read(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *Data)
{
	return (I2C_ReadBurst(SlaveAddr, RegAddr, Data, (uint8_t)1U));
}

/* I2C Read operation of Len consecutive registers. The frame is
   "START - Addr(W) - RegAddr - repeated START - Addr(R) - Data0 .. DataN - NACK - STOP".
   The closing of the reception is handled differently for 1, 2 and 3 or more bytes, as required by the HW */
uint8_t I2C_ReadBurst(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *Buffer, uint8_t Len)
{
	uint32_t Timeout = I2C_TIMEOUT;
	uint16_t Remaining = Len;
	uint8_t *pData = Buffer;

	if((Buffer == 0) || (Len == 0U))
	{
		return (I2C_ERROR);
	}

	/* Step 1: write the address of the first register to be read */

	/* START condition */
	if(I2C_Start() != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}
	/* Slave (magnetometer) address with write direction */
	if(I2C_SendAddress(SlaveAddr, I2C_Direction_Write) != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}
	I2C_ClearADDR();

	/* Address of the first register to be read */
	if(I2C_Transmit(RegAddr) != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}
	/* Wait until the register address is fully shifted out, before the repeated START */
	if(I2C_WaitFlagSR1(I2C_SR1_BTF_OFFSET) != I2C_OK) // BTF - Byte Transfer Finished
	{
		I2C_Abort();
		return (I2C_ERROR);
	}

	/* Step 2: "New" START and consequent reception of the data */

	/* Default acknowledge configuration: every received byte is acknowledged (therefore, ACK Enable bit set to 1).
	   Reason: every read operation clears ACK, because the last byte of an I2C read "must set" NACK to tell the slave to stop driving the bus.
	   So after any completed read, the HW sets ACK=0. If an ACK is not set again, the next read would set NACK and return garbage */
	I2C_CR1 &= ~(1UL << I2C_CR1_POS_OFFSET);
	I2C_CR1 |=  (1UL << I2C_CR1_ACK_OFFSET);

	/* "New" START condition */
	if(I2C_Start() != I2C_OK)
	{
		I2C_Abort();
		return (I2C_ERROR);
	}

	if(Remaining == 1U) // Single byte reception
	{
		if(I2C_SendAddress(SlaveAddr, I2C_Direction_Read) != I2C_OK)
		{
			I2C_Abort();
			return (I2C_ERROR);
		}
		/* IMPO: To close this "1-byte communication", a NACK pulse has to be generated after the last received data byte:
		   In order to do that, (as per the datasheet) the ACK Enable bit must be cleared just after reading the second last data byte (after second last RxNE event).
		   In this way the ACK bit is set low on time before the end of the last data reception. */
		I2C_CR1 &= ~(1UL << I2C_CR1_ACK_OFFSET);
		I2C_ClearADDR();
		/* IMPO: In order to generate the Stop/Restart condition, software must set the STOP/START bit after reading the second last data byte (after the second last RxNE event) */
		I2C_Stop();

		/* Wait until RX buffer is not empty */
		if(I2C_WaitFlagSR1(I2C_SR1_RXNE_OFFSET) != I2C_OK)
		{
			I2C_Abort();
			return (I2C_ERROR);
		}
		/* Read I2C DR register to clear RXNE flag as well as RX buffer */
		*pData = (uint8_t)I2C_DR;
	}
	else if(Remaining == 2U) // Two bytes reception
	{
		/* POS must be set before the ADDR flag is cleared, so that the
		   acknowledge configuration applies to the next byte (i.e. the NACK is sent on the second one) */
		I2C_CR1 |= (1UL << I2C_CR1_POS_OFFSET);

		if(I2C_SendAddress(SlaveAddr, I2C_Direction_Read) != I2C_OK)
		{
			I2C_Abort();
			return (I2C_ERROR);
		}
		I2C_ClearADDR(); // First step for "closing 2-bytes communication", as per datasheet
		/* The second byte (i.e. the last one) must not be acknowledged, to signal that that will be the last byte we want to receive */
		I2C_CR1 &= ~(1UL << I2C_CR1_ACK_OFFSET);

		/* Wait for BTF: Data1 is in DR and Data2 is in the shift register */
		if(I2C_WaitFlagSR1(I2C_SR1_BTF_OFFSET) != I2C_OK) // BTF - Byte Transfer Finished
		{
			I2C_Abort();
			return (I2C_ERROR);
		}
		I2C_Stop();

		*pData = (uint8_t)I2C_DR; // First read empties the DR register
		pData++;
		*pData = (uint8_t)I2C_DR; // Second read empties the shift register
	}
	else // Three or more bytes reception
	{
		if(I2C_SendAddress(SlaveAddr, I2C_Direction_Read) != I2C_OK)
		{
			I2C_Abort();
			return (I2C_ERROR);
		}
		I2C_ClearADDR();

		/* All the bytes up to DataN-3 are simply read as soon as they are available (and acknowledged, since ACK=1) */
		while(Remaining > 3U)
		{
			/* Wait until RX buffer is not empty */
			if(I2C_WaitFlagSR1(I2C_SR1_RXNE_OFFSET) != I2C_OK)
			{
				I2C_Abort();
				return (I2C_ERROR);
			}
			/* Read I2C DR register to clear RXNE flag as well as RX buffer */
			*pData = (uint8_t)I2C_DR;
			pData++;
			Remaining--;
		}

		/* Three bytes left: wait for BTF, i.e. DataN-2 in DR and DataN-1 in the shift register */
		if(I2C_WaitFlagSR1(I2C_SR1_BTF_OFFSET) != I2C_OK) // BTF - Byte Transfer Finished
		{
			I2C_Abort();
			return (I2C_ERROR);
		}

		/* IMPO: The sequence below must not be interrupted, otherwise the NACK and the STOP conditions
		   would be sent on the wrong byte */

		/* DataN (last one) must not be acknowledged */
		I2C_CR1 &= ~(1UL << I2C_CR1_ACK_OFFSET);
		/* Read DataN-2, which shifts DataN-1 into DR and starts the reception of DataN */
		*pData = (uint8_t)I2C_DR;
		pData++;
		/* The STOP condition is programmed right after having read DataN-2 */
		I2C_Stop();
		/* Read DataN-1 */
		*pData = (uint8_t)I2C_DR;
		pData++;

		/* Wait until the last byte is available */
		if(I2C_WaitFlagSR1(I2C_SR1_RXNE_OFFSET) != I2C_OK)
		{
			I2C_Abort();
			return (I2C_ERROR);
		}
		/* Read DataN */
		*pData = (uint8_t)I2C_DR;
	}

	/* Wait until the STOP condition is actually generated: the STOP bit is cleared by the HW */
	while(I2C_CR1 & (1UL << I2C_CR1_STOP_OFFSET))
	{
		if(--Timeout == 0UL)
		{
			return (I2C_ERROR);
		}
	}

	/* Restore the default acknowledge configuration for the next transfer */
	I2C_CR1 &= ~(1UL << I2C_CR1_POS_OFFSET);
	I2C_CR1 |=  (1UL << I2C_CR1_ACK_OFFSET);

	return (I2C_OK);
}
