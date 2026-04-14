/*
 * LSM6DSL.c
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */


#include "LSM6DSL.h"



void LSM6DSL_Imu_Init()
{
	IMU_raw.Gyroscope_x_dps_raw = 0U;
	IMU_raw.Gyroscope_y_dps_raw = 0U;
	IMU_raw.Gyroscope_z_dps_raw = 0U;
	IMU_raw.Accelerom_x_dps_raw = 0U;
	IMU_raw.Accelerom_y_dps_raw = 0U;
	IMU_raw.Accelerom_z_dps_raw = 0U;

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_FD_Write(CTRL1_XL, CTRL1_XL_VAL); // Configure register CTRL1_XL
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_FD_Write(CTRL2_G, CTRL2_G_VAL); // Configure register CTRL2_G
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_FD_Write(CTRL3_C, CTRL3_C_VAL); // Configure register CTRL3_C
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_FD_Write(CTRL4_C, CTRL4_C_VAL); // Configure register CTRL4_C
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_FD_Write(CTRL8_XL, CTRL8_XL_VAL); // Configure register CTRL8_XL
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction
}


void LSM6DSL_Imu_Task()
{
	uint8_t IMU_Val[12];


	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction

	/* Multi-byte read (auto-increment) feature */
	SPI_FD_Transmit((uint8_t)GYRO_OUTX_L_G | SPI_Read_Operation | SPI_Read_Burst); // Read first byte of the 12-bytes representing the gyroscope and accelerometer values. Read operation will be performed in burst mode.
	// The first command only sends register address, we can ignore the Rx since it does not return valid sensor data.

	for(uint8_t i=0; i<12; i++)
	{
		SPI_FD_Transmit((uint8_t)Dummy_Write); // Dummy write operation for generating clock
		IMU_Val[i] = SPI_FD_Receive((uint8_t)0U); // Store next bytes
	}

	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	IMU_raw.Gyroscope_x_dps_raw = (IMU_Val[1] << 8) | IMU_Val[0];
	IMU_raw.Gyroscope_y_dps_raw = (IMU_Val[3] << 8) | IMU_Val[2];
	IMU_raw.Gyroscope_z_dps_raw = (IMU_Val[5] << 8) | IMU_Val[4];
	IMU_raw.Accelerom_x_dps_raw = (IMU_Val[1] << 8) | IMU_Val[0];
	IMU_raw.Accelerom_y_dps_raw = (IMU_Val[3] << 8) | IMU_Val[2];
	IMU_raw.Accelerom_z_dps_raw = (IMU_Val[5] << 8) | IMU_Val[4];

	//float Gyroscope_x_dps = (Gyroscope_x_raw * IMU_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Gyroscope_y_dps = (Gyroscope_y_raw * IMU_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Gyroscope_z_dps = (Gyroscope_z_raw * IMU_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
}
