/*
 * LPS22HH.h
 *
 *  Created on: Mar 10, 2026
 *      Author: marco91
 */

#ifndef DEVICES_LPS22HH_PRESSURE_INC_LPS22HH_H_
#define DEVICES_LPS22HH_PRESSURE_INC_LPS22HH_H_


#include "GPIO.h"
#include "Timer.h"
#include "SPI.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* Registers definition */
#define CTRL_REG1     (0x10U)
#define CTRL_REG1_VAL (0x5CU)  // 01010000  -> ODR1=50Hz, EN_LPFP=1 and LPFP_CFG=1 for bandwidth==ODR/20

#define CTRL_REG2     (0x11U)
#define CTRL_REG2_VAL (0x00U)  // No reboot, no one-shot

#define CTRL_REG3     (0x12U)
#define CTRL_REG3_VAL (0x00U)  // No sensor's interrupts used. Read pressure periodically every 100ms using SysTick timer

#define FIFO_CTRL     (0x13U)
#define FIFO_CTRL_VAL (0x00)  // FIFO not used/bypassed. Pressure read periodically

#define PRESSURE_OUT_XL (0x28U) // Output register
#define PRESSURE_OUT_L  (0x29U) // Output register
#define PRESSURE_OUT_H  (0x2AU) // Output register


#define LPS22HH_CS_HIGH()  (GPIOC_BSRR = (1UL << GPIOC_BSRR_BS_13_OFFSET))/* Set CS pin high */
#define LPS22HH_CS_LOW()   (GPIOC_BSRR = (1UL << GPIOC_BSRR_BR_13_OFFSET))/* Set CS pin low */


#define PRESSURE_SENSITIVITY  (4096.0f)  // Sensitivity value (in LSB/hPa) according to the datasheet



/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/

uint32_t Pressure_hPa_raw;



/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Pressure Init function */
void LPS22HH_Pressure_Init(void);

/* Pressure periodic task */
void LPS22HH_Pressure_Task(void);


#endif /* DEVICES_LPS22HH_PRESSURE_INC_LPS22HH_H_ */
