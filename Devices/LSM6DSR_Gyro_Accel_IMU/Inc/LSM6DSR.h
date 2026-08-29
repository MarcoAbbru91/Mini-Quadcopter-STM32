/*
 * LSM6DSR.h
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */

#ifndef DEVICES_LSM6DSR_ACCEL_IMU_INC_LSM6DSR_H_
#define DEVICES_LSM6DSR_ACCEL_IMU_INC_LSM6DSR_H_


#include "GPIO.h"
#include "Timer.h"
#include "SPI.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* Registers definition */
#define CTRL1_XL      (0x10U)   // Accelerometer configuration
#define CTRL1_XL_VAL  (0x6AU)   // 01101010 -> ODR = 416Hz, FS = ±4g, LowPass Filter LPF2 enabled

#define CTRL2_G       (0x11U)   // Gyroscope configuration
#define CTRL2_G_VAL   (0x68U)    // 01101000 -> ODR = 416Hz, FS = ±2000 dps

#define CTRL3_C       (0x12U)   // Interface settings
#define CTRL3_C_VAL   (0x44U)   // 01000100 -> BDU=1 (avoid partial register update), IF_INC=1 (allows multi-bytes read)

#define CTRL4_C       (0x13U)   // Filter / SPI configuration
#define CTRL4_C_VAL   (0x04U)   // 00000100 -> i2c disabled

#define CTRL8_XL      (0x17U)   // Accelerometer filtering
#define CTRL8_XL_VAL  (0xC0U)   // 11000000 -> Low-pass filtering enabled. LPF2 cutoff = ODR/100 = 4Hz


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


#define LSM6DSR_CS_HIGH()  (GPIOA_BSRR = (1UL << GPIOA_BSRR_BS_8_OFFSET))/* Set CS pin high */
#define LSM6DSR_CS_LOW()   (GPIOA_BSRR = (1UL << GPIOA_BSRR_BR_8_OFFSET))/* Set CS pin low */


#define GYRO_SENSITIVITY   (0.07f)     // Sensitivity is 70 mdps/LSB according to the datasheet, where dps=degree per second, to measure angular velocity
#define ACCEL_SENSITIVITY  (0.000122f) // Sensitivity is 0.122 mg/LSB according to the datasheet


/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/
typedef struct
{
	int16_t Gyroscope_x_raw;
	int16_t Gyroscope_y_raw;
	int16_t Gyroscope_z_raw;
	int16_t Accelerom_x_raw;
	int16_t Accelerom_y_raw;
	int16_t Accelerom_z_raw;
} IMU_raw_t;

extern IMU_raw_t IMU_raw;

typedef struct
{
	float Gyroscope_x_radps;
	float Gyroscope_y_radps;
	float Gyroscope_z_radps;
	float Accelerom_x_mps2;
	float Accelerom_y_mps2;
	float Accelerom_z_mps2;
} IMU_data_conv_t;

extern IMU_data_conv_t IMU_converted;


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/


/* IMU Init function */
void LSM6DSR_IMU_Init(void);

/* IMU periodic task */
void LSM6DSR_IMU_Task(IMU_raw_t *pIMU_raw);


void IMU_Data_Conversion(const IMU_raw_t *pIMU_raw, IMU_data_conv_t *pIMU_converted); // "inline" keyword kept only in .c file, since compilation uses "-std=gnu11", and consequently the C99/C11 inline rules apply


#endif /* DEVICES_LSM6DSR_ACCEL_IMU_INC_LSM6DSR_H_ */
