/*
 * LSM6DSL.h
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */

#ifndef DEVICES_LSM6DSL_ACCEL_IMU_INC_LSM6DSL_H_
#define DEVICES_LSM6DSL_ACCEL_IMU_INC_LSM6DSL_H_


#include "GPIO.h"
#include "SPI.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* Registers definition */
#define CTRL1_XL      (0x10U)   // Accelerometer configuration
#define CTRL1_XL_VAL  (0x68U)   // 01100000 -> ODR = 416 Hz, FS = ±4g, LowPass Filter enabled

#define CTRL2_G       (0x11U)   // Gyroscope configuration
#define CTRL2_G_VAL   (0x6CU)    // ODR = 416 Hz, FS = ±2000 dps

#define CTRL3_C       (0x12U)   // Interface settings
#define CTRL3_C_VAL   (0x44U)   // BDU=1 (avoid partial register update), IF_INC=1 (allows multi-bytes read)

#define CTRL4_C       (0x13U)   // Filter / SPI configuration
#define CTRL4_C_VAL   (0x04U)   // i2c disabled

#define CTRL8_XL      (0x17U)   // Accelerometer filtering
#define CTRL8_XL_VAL  (0x09U)   // Low-pass filtering enabled


#define GYRO_OUTX_L_G  (0x22U) // Output register - L=Low byte
#define GYRO_OUTX_H_G  (0x23U) // Output register - H=High byte
#define GYRO_OUTY_L_G  (0x24U) // Output register
#define GYRO_OUTY_H_G  (0x25U) // Output register
#define GYRO_OUTZ_L_G  (0x26U) // Output register
#define GYRO_OUTZ_H_G  (0x27U) // Output register
#define ACCE_OUTX_L_XL (0x28U) // Output register
#define ACCE_OUTX_H_XL (0x29U) // Output register
#define ACCE_OUTY_L_XL (0x2AU) // Output register
#define ACCE_OUTY_H_XL (0x2BU) // Output register
#define ACCE_OUTZ_L_XL (0x2CU) // Output register
#define ACCE_OUTZ_H_XL (0x2DU) // Output register


#define LSM6DSL_CS_HIGH()  (GPIOA_BSRR |= (1UL << GPIOA_BSRR_BS_8_OFFSET))/* Set CS pin high */
#define LSM6DSL_CS_LOW()   (GPIOA_BSRR |= (1UL << GPIOA_BSRR_BR_8_OFFSET))/* Set CS pin low */


//#define IMU_SENSITIVITY  (0.07f) // Sensitivity is 70 mdps/LSB according to the datasheet, where dps=degree per second


/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/

//#define NULL         ((void *)0)
typedef struct
{
	int16_t Gyroscope_x_dps_raw;
	int16_t Gyroscope_y_dps_raw;
	int16_t Gyroscope_z_dps_raw;
	int16_t Accelerom_x_dps_raw;
	int16_t Accelerom_y_dps_raw;
	int16_t Accelerom_z_dps_raw;
} IMU_raw_t;

IMU_raw_t IMU_raw;


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/


/* IMU Init function */
void LSM6DSL_Imu_Init(void);

/* IMU periodic task */
void LSM6DSL_Imu_Task(void);



#endif /* DEVICES_LSM6DSL_ACCEL_IMU_INC_LSM6DSL_H_ */
