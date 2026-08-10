/*
 * HAL_Devices.h
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */

#ifndef HAL_INC_HAL_DEVICES_H_
#define HAL_INC_HAL_DEVICES_H_


#include "LSM6DSR.h"
#include "LPS22HH.h"


//void Devices_Init();

void Update_Controller_Inputs(const IMU_raw_t *Imu_raw, const uint32_t *Pressure_raw);
void Update_Motor_Inputs();


#endif /* HAL_INC_HAL_DEVICES_H_ */
