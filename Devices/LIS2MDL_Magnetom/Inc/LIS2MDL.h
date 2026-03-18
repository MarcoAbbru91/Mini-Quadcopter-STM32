/*
 * LIS2MDL.h
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */

#ifndef DEVICES_LIS2MDL_MAGNETOM_INC_LIS2MDL_H_
#define DEVICES_LIS2MDL_MAGNETOM_INC_LIS2MDL_H_


#include "GPIO.h"
#include "SPI.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* Registers definition */
#define CFG_REG_A      (0x60U)
#define CFG_REG_A_VAL  (0x8CU) // 10001100 -> ODR=50Hz, COMP_TEMP_EN=1 (Temp compensation enabled), Continuous mode

#define CFG_REG_B      (0x61U)
#define CFG_REG_B_VAL  (0x01U) // Digital filter enabled

#define CFG_REG_C      (0x62U)
#define CFG_REG_C_VAL  (0x10U) // BDU=1 (reading incorrect data is avoided when the user reads asynchronously)

#define OUTX_L_REG     (0x68U) // Output register
#define OUTX_H_REG     (0x69U) // Output register
#define OUTY_L_REG     (0x6AU) // Output register
#define OUTY_H_REG     (0x6BU) // Output register
#define OUTZ_L_REG     (0x6CU) // Output register
#define OUTZ_H_REG     (0x6DU) // Output register


#define LIS2MDL_CS_High()  (GPIOB_BSRR |= (1UL << GPIOB_BSRR_BS_12_OFFSET))/* Set CS pin high */
#define LIS2MDL_CS_Low()   (GPIOB_BSRR |= (1UL << GPIOB_BSRR_BR_12_OFFSET))/* Set CS pin low */


//#define MAGNETIC_SENSITIVITY  (0.15f) // Sensitivity value is 1.5 mGauss/LSB according to the datasheet


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

Magnetom_raw_t Magnetom_raw;


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Magnetometer Init function */
void LIS2MDL_Magnetom_Init(void);

/* Magnetometer periodic task */
void LIS2MDL_Magnetom_Task(void);



#endif /* DEVICES_LIS2MDL_MAGNETOM_INC_LIS2MDL_H_ */
