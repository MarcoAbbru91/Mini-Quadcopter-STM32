/*
 * LSM6DSL.c
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */


#include "LSM6DSR.h"



void LSM6DSR_Imu_Init()
{
	/* Set CS to HIGH (idle) before starting any SPI transaction */
	LSM6DSR_CS_HIGH();
	/* Small delay to be sure all pins are now really high */
	Delay_ms(1);

	IMU_raw.Gyroscope_x_dps_raw = 0U;
	IMU_raw.Gyroscope_y_dps_raw = 0U;
	IMU_raw.Gyroscope_z_dps_raw = 0U;
	IMU_raw.Accelerom_x_dps_raw = 0U;
	IMU_raw.Accelerom_y_dps_raw = 0U;
	IMU_raw.Accelerom_z_dps_raw = 0U;

	LSM6DSR_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL1_XL, CTRL1_XL_VAL); // Configure register CTRL1_XL
	LSM6DSR_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSR_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL2_G, CTRL2_G_VAL); // Configure register CTRL2_G
	LSM6DSR_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSR_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL3_C, CTRL3_C_VAL); // Configure register CTRL3_C
	LSM6DSR_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSR_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL4_C, CTRL4_C_VAL); // Configure register CTRL4_C
	LSM6DSR_CS_HIGH(); // Set Control Select pin low to finish a transaction

	LSM6DSR_CS_LOW(); // Set Control Select pin high to start a transaction
	SPI2_FlushRX(); // Flush previously used/written Rx buffer
	SPI2_Write(CTRL8_XL, CTRL8_XL_VAL); // Configure register CTRL8_XL
	LSM6DSR_CS_HIGH(); // Set Control Select pin low to finish a transaction
}


void LSM6DSR_Imu_Task()
{
	uint8_t IMU_Val[12];


	LSM6DSR_CS_LOW(); // Set Control Select pin low to start a transaction

	SPI2_FlushRX(); // Flush previously used/written Rx buffer

	/* Multi-byte read (auto-increment) feature */
	SPI2_Transmit((uint8_t)GYRO_OUTX_L_G | SPI_Read_Operation); // Write first address to start afterwards a burst read. Read operation will be performed in burst mode.
	SPI2_Receive((uint8_t)1U);  // Dummy read operation (discarded) to empty RX buffer. The first command only sends register address, we can ignore the Rx since it does not return valid sensor data.

	for(uint8_t i=0; i<12; i++)
	{
		SPI2_Transmit((uint8_t)Dummy_Write); // Dummy write operation for generating clock
		IMU_Val[i] = SPI2_Receive((uint8_t)0U); // Store next bytes
	}

	while(SPI2_SR & (1 << SPI_SR_BSY_OFFSET));

	LSM6DSR_CS_HIGH(); // Set Control Select pin high to finish a transaction

	IMU_raw.Gyroscope_x_dps_raw = (IMU_Val[1] << 8)  | IMU_Val[0];
	IMU_raw.Gyroscope_y_dps_raw = (IMU_Val[3] << 8)  | IMU_Val[2];
	IMU_raw.Gyroscope_z_dps_raw = (IMU_Val[5] << 8)  | IMU_Val[4];
	IMU_raw.Accelerom_x_dps_raw = (IMU_Val[7] << 8)  | IMU_Val[6];
	IMU_raw.Accelerom_y_dps_raw = (IMU_Val[9] << 8)  | IMU_Val[8];
	IMU_raw.Accelerom_z_dps_raw = (IMU_Val[11] << 8) | IMU_Val[10];

	//float Gyroscope_x_dps = (IMU_raw.Gyroscope_x_dps_raw * GYRO_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed, to reduce CPU load
	//float Gyroscope_y_dps = (IMU_raw.Gyroscope_y_dps_raw * GYRO_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed, to reduce CPU load
	//float Gyroscope_z_dps = (IMU_raw.Gyroscope_z_dps_raw * GYRO_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed, to reduce CPU load
	//float Accelerom_x_dps = (IMU_raw.Accelerom_x_dps_raw * ACCEL_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed, to reduce CPU load
	//float Accelerom_y_dps = (IMU_raw.Accelerom_y_dps_raw * ACCEL_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed, to reduce CPU load
	//float Accelerom_z_dps = (IMU_raw.Accelerom_z_dps_raw * ACCEL_SENSITIVITY);  // Keep result currently in "raw form". Convert to float only when needed, to reduce CPU load
}
