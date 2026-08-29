/*
 * LIS2MDL.h
 *
 *  Created on: Jul 30, 2026
 *      Author: marco91
 */

#ifndef DEVICES_LIS2MDL_MAGNETOM_INC_LIS2MDL_H_
#define DEVICES_LIS2MDL_MAGNETOM_INC_LIS2MDL_H_


#include "GPIO.h"
#include "I2C.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* I2C magnetometer (slave) default address */
#define LIS2MDL_I2C_ADDR  (0x1EU) // This value can be found on the back of the sensor's PCB


/* Registers definition */
#define OFFSET_X_REG_L (0x45U) // Hard-iron register
#define OFFSET_X_REG_H (0x46U) // Hard-iron register
#define OFFSET_Y_REG_L (0x47U) // Hard-iron register
#define OFFSET_Y_REG_H (0x48U) // Hard-iron register
#define OFFSET_Z_REG_L (0x49U) // Hard-iron register
#define OFFSET_Z_REG_H (0x4AU) // Hard-iron register

#define CFG_REG_A      (0x60U)
#define CFG_REG_A_VAL  (0x88U) // 10001000 -> ODR=50Hz, COMP_TEMP_EN=1 (Temp compensation enabled), Continuous mode

#define CFG_REG_B      (0x61U)
#define CFG_REG_B_VAL  (0x03U) // 00000011 -> OFF_CANC=1 (Offset cancellation uses the internal set/reset pulse to null out sensor offset drift over temperature - recommended by ST for continuous mode), Digital filter enabled

#define CFG_REG_C      (0x62U)
#define CFG_REG_C_VAL  (0x10U) // BDU=1 (reading incorrect data is avoided when the user reads asynchronously)

#define OUTX_L_REG     (0x68U) // Output register
#define OUTX_H_REG     (0x69U) // Output register
#define OUTY_L_REG     (0x6AU) // Output register
#define OUTY_H_REG     (0x6BU) // Output register
#define OUTZ_L_REG     (0x6CU) // Output register
#define OUTZ_H_REG     (0x6DU) // Output register



#define NUM_OUTPUT_REG   6U



//#define MAGNETIC_SENSITIVITY  (0.0015f) // Sensitivity value is 1.5 mGauss/LSB according to the datasheet


/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/

//#define NULL         ((void *)0)
typedef struct
{
	int16_t Magnetic_x_Gauss_raw;
	int16_t Magnetic_y_Gauss_raw;
	int16_t Magnetic_z_Gauss_raw;
} Magnetom_raw_t;

extern Magnetom_raw_t Magnetom_raw;



/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Magnetometer Init function */
uint8_t LIS2MDL_Magnetom_Init();

/* Magnetometer periodic task */
void LIS2MDL_Magnetom_Task(Magnetom_raw_t *pMagnetom_raw);


#endif /* DEVICES_LIS2MDL_MAGNETOM_INC_LIS2MDL_H_ */
