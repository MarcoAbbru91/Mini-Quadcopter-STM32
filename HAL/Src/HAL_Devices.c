/*
 * HAL_Devices.c
 *
 *  Created on: 12 mar 2026
 *      Author: marco91
 */

#include "HAL_Devices.h"


/*void Devices_Init()
{
	LPS22HH_Pressure_Init();

	LIS2MDL_Magnetom_Init();

	LSSM6DSL_Acc_Imu_Init();
}*/


void Update_Controller_Inputs(const IMU_raw_t *IMU_raw, const uint32_t *Pressure_raw)
{
	IMU_Data_Conversion(IMU_raw, &IMU_converted); // no need for "&" for IMU_raw: from function input parameter is already received as "address"

	LPS22HH_Data_Conversion(Pressure_raw, &Pressure_hPa); // no need for "&" for Pressure_raw: from function input parameter is already received as "address"

	/* assign BLE commands to controller "input desired" data */
	//Assign_Controller_Inputs();

}

void Update_Motor_Inputs()
{
	/* Convert controller outputs (thrust, roll, pitch, yaw) into PWM values */
	//PWM_Mot1 = ;
	//PWM_Mot2 = ;
	//PWM_Mot3 = ;
	//PWM_Mot4 = ;
}
