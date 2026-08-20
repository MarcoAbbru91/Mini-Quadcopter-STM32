/*
 * LIS2MDL.c
 *
 *  Created on: Jul 30, 2026
 *      Author: marco91
 */


#include "LIS2MDL.h"



/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/
Magnetom_raw_t Magnetom_raw = {0};




uint8_t LIS2MDL_Magnetom_Init()
{
	uint8_t retVal = I2C_ERROR; // I2C, differently from SPI, has several "buil-in" error status in its protocol

	retVal = I2C_Write(LIS2MDL_I2C_ADDR, CFG_REG_A, CFG_REG_A_VAL); // Configure register REGA
	retVal |= I2C_Write(LIS2MDL_I2C_ADDR, CFG_REG_B, CFG_REG_B_VAL); // Configure register REGB
	retVal |= I2C_Write(LIS2MDL_I2C_ADDR, CFG_REG_C, CFG_REG_C_VAL); // Configure register REGC

	return (retVal);
}


void LIS2MDL_Magnetom_Task(Magnetom_raw_t *pMagnetom_raw)
{
	uint8_t retVal = I2C_ERROR;
	uint8_t Magnetic_Val[NUM_OUTPUT_REG];


	/* Burst read of magnetometer output registers */
	retVal = I2C_ReadBurst(LIS2MDL_I2C_ADDR, OUTX_L_REG, Magnetic_Val, NUM_OUTPUT_REG); // Read first byte of the 6-bytes representing the magnetic value. Burst read will be started from here
  //retVal = I2C_Read(LIS2MDL_I2C_ADDR, OUTX_H_REG, &Magnetic_Val); // Read second byte of the 6-bytes representing the magnetic value
  //retVal = I2C_Read(LIS2MDL_I2C_ADDR, OUTY_L_REG, &Magnetic_Val); // Read third byte of the 6-bytes representing the magnetic value
  //retVal = I2C_Read(LIS2MDL_I2C_ADDR, OUTY_H_REG, &Magnetic_Val); // Read fourth byte of the 6-bytes representing the magnetic value
  //retVal = I2C_Read(LIS2MDL_I2C_ADDR, OUTZ_L_REG, &Magnetic_Val); // Read fifth byte of the 6-bytes representing the magnetic value
  //retVal = I2C_Read(LIS2MDL_I2C_ADDR, OUTZ_H_REG, &Magnetic_Val); // Read sixth byte of the 6-bytes representing the magnetic value
	if(retVal != I2C_OK)
	{
		// Keep the previous valid sample instead of using garbage
	}
	else
	{
		pMagnetom_raw->Magnetic_x_Gauss_raw = (Magnetic_Val[1] << 8) | Magnetic_Val[0];
		pMagnetom_raw->Magnetic_y_Gauss_raw = (Magnetic_Val[3] << 8) | Magnetic_Val[2];
		pMagnetom_raw->Magnetic_z_Gauss_raw = (Magnetic_Val[5] << 8) | Magnetic_Val[4];
	}

	//float Magnetom_x_Gauss = mx_raw * MAGNETIC_SENSITIVITY; // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Magnetom_y_Gauss = my_raw * MAGNETIC_SENSITIVITY; // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Magnetom_z_Gauss = mz_raw * MAGNETIC_SENSITIVITY; // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
}
