/*
 * Sensor_service.c
 *
 *  Created on: 25 giu 2026
 *      Author: marco91
 */


#include "Sensor_service.h"


/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/
static uint16_t HWServW2STHandle;
static uint16_t EnvironmentalCharHandle;
static uint16_t AccGyroMagCharHandle;
static uint16_t AccEventCharHandle;
static uint16_t ArmingCharHandle;
static uint16_t MaxCharHandle;

static uint16_t ConfigServW2STHandle;
static uint16_t ConfigCharHandle;

static uint16_t ConsoleW2STHandle;
static uint16_t TermCharHandle;
static uint16_t StdErrCharHandle;


volatile uint32_t HCI_ProcessEvent = 0UL;
uint8_t bdaddr[6] = {0};


int connected = FALSE;
uint8_t set_connectable = TRUE;



/****************************************************************************
FUNCTION PROTOTYPES
****************************************************************************/
static void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
static void GAP_DisconnectionComplete_CB(void);
static void Attribute_Modified_CB(uint16_t attr_handle, uint8_t * att_data, uint8_t data_length);





/* Add the Config service using a vendor specific profile */
tBleStatus Add_ConfigW2ST_Service(void)
{
	tBleStatus ret;

	uint8_t uuid[16];

	COPY_CONFIG_SERVICE_UUID(uuid);
	ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 1+3,&ConfigServW2STHandle);

	if (ret != BLE_STATUS_SUCCESS)
		return BLE_STATUS_ERROR;

	COPY_CONFIG_W2ST_CHAR_UUID(uuid);
	ret =  aci_gatt_add_char(ConfigServW2STHandle, UUID_TYPE_128, uuid, 20 /* Max Dimension */,
							CHAR_PROP_NOTIFY| CHAR_PROP_WRITE_WITHOUT_RESP,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
							16, 1, &ConfigCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	return BLE_STATUS_SUCCESS;
}

/* Add the Console service using a vendor specific profile */
tBleStatus Add_ConsoleW2ST_Service(void)
{
	tBleStatus ret;

	uint8_t uuid[16];

	COPY_CONSOLE_SERVICE_UUID(uuid);
	ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 1+3*2,&ConsoleW2STHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	COPY_TERM_CHAR_UUID(uuid);
	ret =  aci_gatt_add_char(ConsoleW2STHandle, UUID_TYPE_128, uuid, W2ST_CONSOLE_MAX_CHAR_LEN,
							CHAR_PROP_NOTIFY| CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_WRITE | CHAR_PROP_READ ,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
							16, 1, &TermCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	COPY_STDERR_CHAR_UUID(uuid);
	ret =  aci_gatt_add_char(ConsoleW2STHandle, UUID_TYPE_128, uuid, W2ST_CONSOLE_MAX_CHAR_LEN,
							CHAR_PROP_NOTIFY | CHAR_PROP_READ,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
							16, 1, &StdErrCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	return BLE_STATUS_SUCCESS;
}

/* Add the HW Features service using a vendor specific profile */
tBleStatus Add_HWServW2ST_Service(void)
{
	tBleStatus ret;
	int32_t NumberChars = 5;

	uint8_t uuid[16];

	COPY_HW_SENS_W2ST_SERVICE_UUID(uuid);
	ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE,
							1+3 * NumberChars,
							&HWServW2STHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	/* Fill the Environmental BLE Characteristc */
	COPY_ENVIRONMENTAL_W2ST_CHAR_UUID(uuid);
//  /* Fill the Battery and Environmental BLE Characteristc */
//  //COPY_BATT_ENV_W2ST_CHAR_UUID(uuid);
//  if(TargetBoardFeatures.NumTempSensors==2) {
//    uuid[14] |= 0x05; /* Two Temperature values*/
//  } else if(TargetBoardFeatures.NumTempSensors==1) {
//    uuid[14] |= 0x04; /* One Temperature value*/
//  }

	uuid[14] |= 0x05; /* Two Temperature values*/

	uuid[14] |= 0x08; /* Battery level (percentage of full battery) */

	uuid[14] |= 0x10; /* Pressure value*/

	ret =  aci_gatt_add_char(HWServW2STHandle, UUID_TYPE_128, uuid, 2+4+2+2+2,
							CHAR_PROP_NOTIFY|CHAR_PROP_READ,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
							16, 0, &EnvironmentalCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	COPY_ACC_GYRO_MAG_W2ST_CHAR_UUID(uuid);
	ret =  aci_gatt_add_char(HWServW2STHandle, UUID_TYPE_128, uuid, 2+3*3*2,
							CHAR_PROP_NOTIFY,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
							16, 0, &AccGyroMagCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	COPY_ACC_EVENT_W2ST_CHAR_UUID(uuid);
	ret =  aci_gatt_add_char(HWServW2STHandle, UUID_TYPE_128, uuid, 2+2,
							CHAR_PROP_NOTIFY | CHAR_PROP_READ,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
							16, 0, &AccEventCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	COPY_ARMING_W2ST_CHAR_UUID(uuid);
	ret =  aci_gatt_add_char(HWServW2STHandle, UUID_TYPE_128, uuid, 2+1,
							CHAR_PROP_NOTIFY | CHAR_PROP_READ,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
							16, 0, &ArmingCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}

	/* MAX charecteristic */
	COPY_MAX_W2ST_CHAR_UUID(uuid);
	ret =  aci_gatt_add_char(HWServW2STHandle, UUID_TYPE_128, uuid, 7,
							CHAR_PROP_WRITE_WITHOUT_RESP | CHAR_PROP_WRITE,
							ATTR_PERMISSION_NONE,
							GATT_NOTIFY_ATTRIBUTE_WRITE,
							16, 0, &MaxCharHandle);

	if (ret != BLE_STATUS_SUCCESS) {
		return BLE_STATUS_ERROR;
	}


	return BLE_STATUS_SUCCESS;
}

/* Puts the device in connectable mode */
void setConnectable(void)
{
	char local_name[8] = {AD_TYPE_COMPLETE_LOCAL_NAME, NAME_DRN};
	uint8_t manuf_data[26] = {
		2, 0x0A, 0x00 /* 0 dBm */, // Trasmission Power
		8, 0x09, NAME_DRN, // Complete Name
		13, 0xFF, 0x01/*SKD version */,
		0x80,
		0x00, /* */
		0xE0, /* ACC+Gyro+Mag*/
		0x00, /*  */
		0x00, /*  */
		0x00, /* BLE MAC start */
		0x00,
		0x00,
		0x00,
		0x00,
		0x00, /* BLE MAC stop */
		};

	/* BLE MAC */
	manuf_data[20] = bdaddr[5];
	manuf_data[21] = bdaddr[4];
	manuf_data[22] = bdaddr[3];
	manuf_data[23] = bdaddr[2];
	manuf_data[24] = bdaddr[1];
	manuf_data[25] = bdaddr[0];

	manuf_data[16] |= 0x20; /* Led */
	manuf_data[17] |= 0x05; /* Temperature and RSSI values*/
	manuf_data[17] |= 0x08; /* Battery level (percentage of full battery) */
	manuf_data[17] |= 0x10; /* Pressure value*/

	/* Max Char */
	manuf_data[18] |=0x80;

	/* disable scan response */
	hci_le_set_scan_resp_data(0,NULL);
	aci_gap_set_discoverable(ADV_IND, 0, 0,
							STATIC_RANDOM_ADDR,
							NO_WHITE_LIST_USE,
							sizeof(local_name), local_name, 0, NULL, 0, 0);

	/* Send Advertising data */
	aci_gap_update_adv_data(26, manuf_data);
}

/* This function is called when there is a LE Connection Complete event */
static void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle)
{
	(void)addr;
	(void)handle;

	connected = TRUE;
}

/* This function is called when the peer device gets disconnected */
static void GAP_DisconnectionComplete_CB(void)
{
	connected = FALSE;
	/* Make the device connectable again. */
	set_connectable = TRUE;

	/* TODO stop PWM/Motors */

}

/* This function is called when there is a change on the gatt attribute */
static void Attribute_Modified_CB(uint16_t attr_handle, uint8_t * att_data, uint8_t data_length)
{

}

/* This function is called whenever there is an ACI event to be processed */
void HCI_Event_CB(void *pckt)
{
	hci_uart_pckt *hci_pckt = pckt;
	hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;

	if(hci_pckt->type != HCI_EVENT_PKT) return;

	switch(event_pckt->evt)
	{
		case EVT_DISCONN_COMPLETE:
		{
			GAP_DisconnectionComplete_CB();
		}
		break;

		case EVT_LE_META_EVENT:
		{
			evt_le_meta_event *evt = (void *)event_pckt->data;
			if(evt->subevent == EVT_LE_CONN_COMPLETE)
			{
				evt_le_connection_complete *cc = (void *)evt->data;
				GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
			}
		}
		break;

		case EVT_VENDOR:
		{
			evt_blue_aci *blue_evt = (void*)event_pckt->data;
			if(blue_evt->ecode == EVT_BLUE_GATT_ATTRIBUTE_MODIFIED)
			{
				/* Drone commands from BLE app */
				evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*)blue_evt->data;
				Attribute_Modified_CB(evt->attr_handle, evt->att_data,evt->data_length);
			}
		}
		break;

		default:
			// added to avoid warning
			break;
	}
}
