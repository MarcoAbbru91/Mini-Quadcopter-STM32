/*
 * LIS2MDL.c
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */


#include "LIS2MDL.h"




void LIS2MDL_Magnetom_Init()
{
	Magnetom_raw.Magnetic_x_Gauss_raw = 0U;
	Magnetom_raw.Magnetic_y_Gauss_raw = 0U;
	Magnetom_raw.Magnetic_z_Gauss_raw = 0U;

	LIS2MDL_CS_Low(); // Set Control Select pin high to start a transaction
	SPI_Write(CFG_REG_A, CFG_REG_A_VAL); // Configure register REGA
	LIS2MDL_CS_High(); // Set Control Select pin low to finish a transaction

	LIS2MDL_CS_Low(); // Set Control Select pin high to start a transaction
	SPI_Write(CFG_REG_B, CFG_REG_B_VAL); // Configure register REGB
	LIS2MDL_CS_High(); // Set Control Select pin low to finish a transaction

	LIS2MDL_CS_Low(); // Set Control Select pin high to start a transaction
	SPI_Write(CFG_REG_C, CFG_REG_C_VAL); // Configure register REGC
	LIS2MDL_CS_High(); // Set Control Select pin low to finish a transaction

}


void LIS2MDL_Magnetom_Task()
{
	uint8_t Magnetic_Val[6];

	LIS2MDL_CS_Low(); // Set Control Select pin high to start a transaction

	/* Currently the multi-byte read (auto-increment) feature is not used. Possible ToDo */
	Magnetic_Val[0] = SPI_Read((uint8_t)OUTX_L_REG); // Read first byte of the 6-bytes representing the magnetic value
	Magnetic_Val[1] = SPI_Read((uint8_t)OUTX_H_REG); // Read second byte of the 6-bytes representing the magnetic value
	Magnetic_Val[2] = SPI_Read((uint8_t)OUTY_L_REG); // Read third byte of the 6-bytes representing the magnetic value
	Magnetic_Val[3] = SPI_Read((uint8_t)OUTY_H_REG); // Read fourth byte of the 6-bytes representing the magnetic value
	Magnetic_Val[4] = SPI_Read((uint8_t)OUTZ_L_REG); // Read fifth byte of the 6-bytes representing the magnetic value
	Magnetic_Val[5] = SPI_Read((uint8_t)OUTZ_H_REG); // Read sixth byte of the 6-bytes representing the magnetic value

	LIS2MDL_CS_High(); // Set Control Select pin low to finish a transaction

	Magnetom_raw.Magnetic_x_Gauss_raw = (Magnetic_Val[1] << 8) | Magnetic_Val[0];
	Magnetom_raw.Magnetic_y_Gauss_raw = (Magnetic_Val[3] << 8) | Magnetic_Val[2];
	Magnetom_raw.Magnetic_z_Gauss_raw = (Magnetic_Val[5] << 8) | Magnetic_Val[4];

	//float Magnetom_x_Gauss = mx_raw * MAGNETIC_SENSITIVITY; // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Magnetom_y_Gauss = my_raw * MAGNETIC_SENSITIVITY; // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Magnetom_z_Gauss = mz_raw * MAGNETIC_SENSITIVITY; // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
}
