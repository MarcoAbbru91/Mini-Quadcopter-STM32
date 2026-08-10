/*
 * LPS22HH.c
 *
 *  Created on: Mar 10, 2026
 *      Author: marco91
 */


#include "LPS22HH.h"


uint32_t Pressure_raw = 0UL;
float Pressure_hPa = 0.0f;



void LPS22HH_Pressure_Init()
{
	/* Set CS to HIGH (idle) before starting any SPI transaction */
	LPS22HH_CS_HIGH();
	/* Small delay to be sure all pins are now really high */
	Delay_ms(1.0f);

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL_REG1, CTRL_REG1_VAL); // Configure register REG1
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL_REG2, CTRL_REG2_VAL); // Configure register REG2
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL_REG3, CTRL_REG3_VAL); // Configure register REG3
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LPS22HH_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(FIFO_CTRL, FIFO_CTRL_VAL); // Configure FIFO register
	LPS22HH_CS_HIGH(); // Set Control Select pin low to finish a transaction
}


void LPS22HH_Pressure_Task(uint32_t *Pressure_raw)
{
	uint8_t Pressure_Val[3];

	//***** Apparently the read burst operation is not working well for this sensor (at least with SPI Mode 3 configuration - CPHA & CPOL). *****//
	//*****	For this reason currently this basic read operation is performed. However, since it is executed every 50ms it does not create problems to the scheduler. *****//

	LPS22HH_CS_LOW();
	SPI2_FlushRX();
	Pressure_Val[0] = SPI2_Read(PRESSURE_OUT_XL);
	LPS22HH_CS_HIGH();

	LPS22HH_CS_LOW();
	SPI2_FlushRX();
	Pressure_Val[1] = SPI2_Read(PRESSURE_OUT_L);
	LPS22HH_CS_HIGH();

	LPS22HH_CS_LOW();
	SPI2_FlushRX();
	Pressure_Val[2] = SPI2_Read(PRESSURE_OUT_H);
	LPS22HH_CS_HIGH();

	*Pressure_raw = (uint32_t)Pressure_Val[2]<<16 | (uint32_t)Pressure_Val[1]<<8 | (uint32_t)Pressure_Val[0];
	(void)Pressure_raw; // only for debug purposes to set breakpoint here
	/* Keep result currently in "raw form". Convert to float only when needed, to reduce CPU load */
}


inline void LPS22HH_Data_Conversion(const uint32_t *Pressure_raw, float *Pressure_hPa)
{
	/* Convert raw pressure value into ......   measurement unit */
	*Pressure_hPa = ((float)*Pressure_raw / PRESSURE_SENSITIVITY); // By-reference update of output parameter
}
