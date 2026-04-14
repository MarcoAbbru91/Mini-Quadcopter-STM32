/*
 * BLE_Appl.c
 *
 *  Created on: 10 apr 2026
 *      Author: marco91
 */

#include "BLE_Appl.h"
//#include "Drone.h"
//#include "motors.h"

/* =========================
 * CONNECTION CALLBACKS
 * ========================= */

void BLE_OnConnected(void)
{
	//Drone.Connected = true;
	//Drone.mode = DRONE_MODE_REMOTE_CONTROL;
}

void BLE_OnDisconnected(void)
{
	//Drone.Connected = false;

	/* FAILSAFE */
	//Stop_Motors();

	//Drone.mode = DRONE_MODE_FAILSAFE;
}

/* =========================
 * COMMAND PARSER
 * ========================= */

void BLE_OnDataReceived(uint8_t *data, uint8_t len)
{
	//if (len < 2) return;

	//uint8_t cmd  = data[0];
	//uint8_t plen = data[1];

	/* Basic validation */
	//if (plen != (len - 2)) return;

	//uint8_t *payload = &data[2];

	//switch (cmd)
	//{
	/* =========================
	 * * THROTTLE
	 * ========================= */
		//case BLE_CMD_SET_THROTTLE:
		//{
			//if (plen != 1) return;

			//uint8_t throttle = payload[0]; // 0..255

			//drone_set_throttle(throttle);
		//}
		//break;

		/* =========================
		 * * ATTITUDE (ROLL/PITCH/YAW)
		 * ========================= */
		//case BLE_CMD_SET_ATTITUDE:
		//{
			//if (plen != 3) return;

			//BLE_AttitudeCmd_t att;

			//att.roll  = (int8_t)payload[0];
			//att.pitch = (int8_t)payload[1];
			//att.yaw   = (int8_t)payload[2];

			//drone_set_attitude(att.roll, att.pitch, att.yaw);
		//}
		//break;

		/* =========================
		 * * PID TUNING
		 * ========================= */
		//case BLE_CMD_SET_PID:
		//{
			//if (plen != 13) return;

			//BLE_PIDCmd_t pid;

			//pid.axis = payload[0];

			/* Extract floats (little-endian) */
			//memcpy(&pid.kp, &payload[1], 4);
			//memcpy(&pid.ki, &payload[5], 4);
			//memcpy(&pid.kd, &payload[9], 4);

			//drone_set_pid(pid.axis, pid.kp, pid.ki, pid.kd);
		//}
		//break;

		/* =========================
		 * * ARM
		 * ========================= */
		//case BLE_CMD_ARM:
		//{
			//drone_arm();
		//}
		//break;

		/* =========================
		 * * DISARM
		 * ========================= */
		//case BLE_CMD_DISARM:
		//{
			//drone_disarm();
		//}
		//break;

		//default:
			//break;
	//}
}
