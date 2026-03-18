/*
 * LPS22HH.c
 *
 *  Created on: Mar 10, 2026
 *      Author: marco91
 */


#include "LPS22HH.h"



void LPS22HH_Pressure_Init()
{
	Pressure_hPa_raw = 0UL;

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(CTRL_REG1, CTRL_REG1_VAL); // Configure register REG1
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(CTRL_REG2, CTRL_REG2_VAL); // Configure register REG2
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(CTRL_REG3, CTRL_REG3_VAL); // Configure register REG3
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(FIFO_CTRL, FIFO_CTRL_VAL); // Configure FIFO register
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction
}


void LPS22HH_Pressure_Task()
{
	uint8_t Pressure_Val[3];

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction

	/* Currently the multi-byte read (auto-increment) feature is not used. Possible ToDo */
	Pressure_Val[0] = SPI_Read((uint8_t)PRESSURE_OUT_XL); // Read first byte of the 3-bytes representing the pressure value
	Pressure_Val[1] = SPI_Read((uint8_t)PRESSURE_OUT_L); // Read second byte of the 3-bytes representing the pressure value
	Pressure_Val[2] = SPI_Read((uint8_t)PRESSURE_OUT_H); // Read third byte of the 3-bytes representing the pressure value

	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction

	Pressure_hPa_raw = Pressure_Val[2]<<16 | Pressure_Val[1]<<8 | Pressure_Val[0];
	//float Pressure_hPa = (Pressure_raw / PRESSURE_SENSITIVITY); // 4096 is the sensitivity according to the datasheet
}
