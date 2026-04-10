/*
 * BLE_Protocol.h
 *
 *  Created on: 10 apr 2026
 *      Author: marco91
 */

#ifndef BLE_APPL_H_
#define BLE_APPL_H_


#include <stdint.h>
#include <string.h>

/* =========================
 * COMMAND IDs
 * ========================= */

#define BLE_CMD_SET_THROTTLE      0x01
#define BLE_CMD_SET_ATTITUDE      0x02
#define BLE_CMD_SET_PID           0x03
#define BLE_CMD_ARM               0x04
#define BLE_CMD_DISARM            0x05

/* =========================
 * STRUCTURES
 * ========================= */

/* Roll / Pitch / Yaw */
typedef struct
{
	int8_t roll;   // -100 .. 100
	int8_t pitch;  // -100 .. 100
	int8_t yaw;    // -100 .. 100
} BLE_AttitudeCmd_t;

/* PID tuning */
typedef struct
{
	uint8_t axis;   // 0=roll,1=pitch,2=yaw
	float kp;
	float ki;
	float kd;
} BLE_PIDCmd_t;


#endif /* BLE_APPL_H_ */
