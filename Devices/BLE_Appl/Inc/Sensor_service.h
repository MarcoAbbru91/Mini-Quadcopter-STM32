/*
 * Sensor_service.h
 *
 *  Created on: 25 giu 2026
 *      Author: marco91
 */


#ifndef SENSOR_SERVICE_H_
#define SENSOR_SERVICE_H_



#include "link_layer.h"
#include "hci_const.h"
#include "bluenrg_aci_const.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_hal_aci.h"



/****************************************************************************
Defines
****************************************************************************/
#define W2ST_CONSOLE_MAX_CHAR_LEN 20 // Define the Max dimesion of the Bluetooth characteristics for each packet used for Console Service

/* Generic helper: copies 16 UUID bytes (uuid_15..uuid_0) into uuid_struct[0..15] */
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, \
						uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
	uuid_struct[0]  = uuid_0;  uuid_struct[1]  = uuid_1;  uuid_struct[2]  = uuid_2;  uuid_struct[3]  = uuid_3;  \
	uuid_struct[4]  = uuid_4;  uuid_struct[5]  = uuid_5;  uuid_struct[6]  = uuid_6;  uuid_struct[7]  = uuid_7;  \
	uuid_struct[8]  = uuid_8;  uuid_struct[9]  = uuid_9;  uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
	uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
#define COPY_HW_SENS_W2ST_SERVICE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x00,0x01,0x11,0xe1,0x9a,0xb4,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_ENVIRONMENTAL_W2ST_CHAR_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x00,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)

#define COPY_ACC_GYRO_MAG_W2ST_CHAR_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x00,0xE0,0x00,0x00,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_ACC_EVENT_W2ST_CHAR_UUID(uuid_struct)     COPY_UUID_128(uuid_struct,0x00,0x00,0x04,0x00,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)

#define COPY_ARMING_W2ST_CHAR_UUID(uuid_struct)           COPY_UUID_128(uuid_struct,0x20,0x00,0x00,0x00,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_MAX_W2ST_CHAR_UUID(uuid_struct)           COPY_UUID_128(uuid_struct,0x00,0x00,0x80,0x00,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)

#define COPY_CONSOLE_SERVICE_UUID(uuid_struct)         COPY_UUID_128(uuid_struct,0x00,0x00,0x01,0x00,0x00,0x01,0x11,0xe1,0x9a,0xb4,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_TERM_CHAR_UUID(uuid_struct)               COPY_UUID_128(uuid_struct,0x00,0x00,0x01,0x01,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_STDERR_CHAR_UUID(uuid_struct)             COPY_UUID_128(uuid_struct,0x00,0x00,0x01,0x02,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_CONFIG_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x02,0x00,0x00,0x01,0x11,0xe1,0x9a,0xb4,0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_CONFIG_W2ST_CHAR_UUID(uuid_struct)        COPY_UUID_128(uuid_struct,0x00,0x00,0x02,0x01,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b)


#define AD_TYPE_COMPLETE_LOCAL_NAME			    (0x09)

#define NAME_DRN 'M','A','R','C', 'O', '9', '1'



/****************************************************************************
Global variables
****************************************************************************/
extern uint8_t bdaddr[6];

typedef struct
{
	int8_t roll;   // -100 .. 100
	int8_t pitch;  // -100 .. 100
	int8_t yaw;    // -100 .. 100
} BLE_AttitudeCmd_t;



/****************************************************************************
Function prototypes
****************************************************************************/
extern tBleStatus Add_ConfigW2ST_Service(void);
extern tBleStatus Add_ConsoleW2ST_Service(void);
extern tBleStatus Add_HWServW2ST_Service(void);
extern void setConnectable(void);




#endif /* SENSOR_SERVICE_H_ */
