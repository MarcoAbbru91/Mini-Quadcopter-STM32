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
	SPI_Write(CTRL1_XL, CTRL1_XL_VAL); // Configure register CTRL1_XL
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(CTRL2_G, CTRL2_G_VAL); // Configure register CTRL2_G
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(CTRL3_C, CTRL3_C_VAL); // Configure register CTRL3_C
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(CTRL4_C, CTRL4_C_VAL); // Configure register CTRL4_C
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI_Write(CTRL8_XL, CTRL8_XL_VAL); // Configure register CTRL8_XL
	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction
}


void LSM6DSL_Imu_Task()
{
	uint8_t Gyroscope_Val[6];
	uint8_t Accelerom_Val[6];


	LSM6DSL_CS_LOW(); // Set Control Select pin high to start a transaction

	/* Currently the multi-byte read (auto-increment) feature is not used. Possible ToDo */
	Gyroscope_Val[0] = SPI_Read((uint8_t)GYRO_OUTX_L_G);   // Read first byte of the 6-bytes representing the gyroscope value
	Gyroscope_Val[1] = SPI_Read((uint8_t)GYRO_OUTX_H_G);   // Read second byte of the 6-bytes representing the gyroscope value
	Gyroscope_Val[2] = SPI_Read((uint8_t)GYRO_OUTY_L_G);   // Read third byte of the 6-bytes representing the gyroscope value
	Gyroscope_Val[3] = SPI_Read((uint8_t)GYRO_OUTY_H_G);   // Read fourth byte of the 6-bytes representing the gyroscope value
	Gyroscope_Val[4] = SPI_Read((uint8_t)GYRO_OUTZ_L_G);   // Read fifth byte of the 6-bytes representing the gyroscope value
	Gyroscope_Val[5] = SPI_Read((uint8_t)GYRO_OUTZ_H_G);   // Read sixth byte of the 6-bytes representing the gyroscope value
	Accelerom_Val[0] = SPI_Read((uint8_t)ACCE_OUTX_L_XL);  // Read first byte of the 6-bytes representing the accelerometer value
	Accelerom_Val[1] = SPI_Read((uint8_t)ACCE_OUTX_H_XL);  // Read second byte of the 6-bytes representing the accelerometer value
	Accelerom_Val[2] = SPI_Read((uint8_t)ACCE_OUTY_L_XL);  // Read third byte of the 6-bytes representing the accelerometer value
	Accelerom_Val[3] = SPI_Read((uint8_t)ACCE_OUTY_H_XL);  // Read fourth byte of the 6-bytes representing the accelerometer value
	Accelerom_Val[4] = SPI_Read((uint8_t)ACCE_OUTZ_L_XL);  // Read fifth byte of the 6-bytes representing the accelerometer value
	Accelerom_Val[5] = SPI_Read((uint8_t)ACCE_OUTZ_H_XL);  // Read sixth byte of the 6-bytes representing the accelerometer value

	LSM6DSL_CS_HIGH(); // Set Control Select pin low to finish a transaction

	IMU_raw.Gyroscope_x_dps_raw = (Gyroscope_Val[1] << 8) | Gyroscope_Val[0];
	IMU_raw.Gyroscope_y_dps_raw = (Gyroscope_Val[3] << 8) | Gyroscope_Val[2];
	IMU_raw.Gyroscope_z_dps_raw = (Gyroscope_Val[5] << 8) | Gyroscope_Val[4];
	IMU_raw.Accelerom_x_dps_raw = (Accelerom_Val[1] << 8) | Accelerom_Val[0];
	IMU_raw.Accelerom_y_dps_raw = (Accelerom_Val[3] << 8) | Accelerom_Val[2];
	IMU_raw.Accelerom_z_dps_raw = (Accelerom_Val[5] << 8) | Accelerom_Val[4];

	//float Gyroscope_x_dps = (Gyroscope_x_raw * IMU_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Gyroscope_y_dps = (Gyroscope_y_raw * IMU_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
	//float Gyroscope_z_dps = (Gyroscope_z_raw * IMU_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed to reduce CPU load
}
