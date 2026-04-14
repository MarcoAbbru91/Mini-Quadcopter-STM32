/*
 * BLE.c
 *
 *  Created on: 9 apr 2026
 *      Author: marco91
 */

#include "BLE.h"


/* IRQ flag set from interrupt */
volatile uint32_t BLE_IRQ_Counter = 0;

/* Current BLE state */
static BLE_State_t BLE_State = BLE_STATE_IDLE;
/* Connection handle (assigned after connection) */
static uint16_t Connection_Handle = 0;



/* IRQ handler called by external interrupt (EXTI) */
void BLE_EXTI_IRQHandler(void)
{
	BLE_IRQ_Counter++;
}

/* SPI Write (MCU -> BLE) */
void BLE_SPI_Write(uint8_t *data, uint16_t len)
{
	uint8_t Header_TX[5] = {BLE_SPI_WRITE, 0,0,0,0};
	uint8_t Header_RX[5];

	BLE_CS_LOW();

	/* Send write header */
	SPI_HD_Write(Header_TX, 5);

	/* Read response */
	SPI_HD_Read(Header_RX, 5);

	/* Check if BLE is ready */
	if (Header_RX[0] != 0x02)
	{
		SPI_HD_Write(data, len);
	}

	BLE_CS_HIGH();
}

/* SPI Read (BLE -> MCU) */
uint16_t BLE_SPI_Read(uint8_t *buffer)
{
	uint8_t  Header_TX[5] = {BLE_SPI_READ,0,0,0,0};
	uint8_t  Header_RX[5];
	uint16_t len = 0U;

	BLE_CS_LOW();

	/* Send read header */
	SPI_HD_Write(Header_TX, 5);

	/* Read response */
	SPI_HD_Read(Header_RX, 5);

	if (Header_RX[0] == 0x02)
	{
		len = Header_RX[3] | (Header_RX[4] << 8);

		if (len > 0 && len < BLE_MAX_BUFFER)
		{
			SPI_HD_Read(buffer, len);
		}
	}

	BLE_CS_HIGH();

	return (len);
}


/* Reset BLE chip */
static void HCI_Reset(void)
{
	uint8_t cmd[] = {0x01U, 0x03U, 0x0CU, 0x00U};

	BLE_SPI_Write(cmd, sizeof(cmd));
}

/* Configure public address */
static void ACI_WriteConfig(void)
{
	uint8_t cmd[] =
	{
		0x01U, 0x0CU, 0xFCU, 0x07U,
		0x00U, 0x06U,
		0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U
	};

	BLE_SPI_Write(cmd, sizeof(cmd));
}

/* Initialize GATT layer */
static void ACI_GATT_Init(void)
{
	uint8_t cmd[] = {0x01U, 0x01U, 0xFDU, 0x00U};

	BLE_SPI_Write(cmd, sizeof(cmd));
}

/* Initialize GAP layer (peripheral role) */
static void ACI_GAP_Init(void)
{
	uint8_t cmd[] = {0x01U, 0x8AU, 0xFCU, 0x03U, 0x01U, 0x00U, 0x00U};

	BLE_SPI_Write(cmd, sizeof(cmd));
}

/* Service contains:
 *  - TX characteristic (notify): drone -> phone
 *  - RX characteristic (write):  phone -> drone
 */
static void BLE_AddService(void)
{
	/* Add service */
	uint8_t svc[] = {0x01U, 0x02U, 0xFDU, 0x05U, 0x01U, 0x34U, 0x12U, 0x07};
	BLE_SPI_Write(svc, sizeof(svc));

	/* TX characteristic (notify) */
	uint8_t tx[] = {0x01U, 0x04U, 0xFDU, 0x08U, 0U, 0U, 0x02U, 0x08U, 0x10U, 0U, 0U};
	BLE_SPI_Write(tx, sizeof(tx));

	/* RX characteristic (write) */
	uint8_t rx[] = {0x01U, 0x04U, 0xFDU, 0x08U, 0U, 0U, 0x02U, 0x08U, 0x08U, 0U, 0U};
	BLE_SPI_Write(rx, sizeof(rx));
}


static void BLE_StartAdvertising(void)
{
	uint8_t adv_params[] =
	{
		0x01, 0x06, 0x20, 0x0F,
		0xA0, 0x00,
		0xA0, 0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x07,
		0x00
	};
	BLE_SPI_Write(adv_params, sizeof(adv_params));

	uint8_t adv_enable[] = {0x01, 0x0A, 0x20, 0x01, 0x01};
	BLE_SPI_Write(adv_enable, sizeof(adv_enable));
}

/* Decode BLE events and trigger actions */
static void BLE_ParseEvent(uint8_t *buffer, uint16_t len)
{
	if (len < 3) return;

	/* Check packet type (0x04 = event) */
	if (buffer[0] != 0x04U) return;

	uint8_t evt = buffer[1];

	switch(evt)
	{
		/* Disconnection event */
		case 0x05U:
		{
			BLE_State = BLE_STATE_READY;
			BLE_OnDisconnected();
		} break;

		/* LE Meta Event */
		case 0x3EU:
		{
			uint8_t sub = buffer[3];

			/* Connection complete */
			if (sub == 0x01U)
			{
				Connection_Handle = buffer[4] | (buffer[5]<<8);
				BLE_State = BLE_STATE_CONNECTED;
				BLE_OnConnected();
			}
		} break;

		/* Vendor-specific event (data received) */
		case 0xFFU:
		{
			uint8_t data_len = buffer[2] - 3;
			BLE_OnDataReceived(&buffer[4], data_len);
		} break;

		default:
			break;
	}
}

/* Process function called inside infinite while loop */
void BLE_Process(void)
{
	/* Keep reading until IRQ is High, as per information from datasheet */
	while (BLE_IRQ_ReadGPIO())
	{
		uint8_t buffer[BLE_MAX_BUFFER];
		uint16_t len = BLE_SPI_Read(buffer);

		if (len > 0)
		{
			BLE_ParseEvent(buffer, len);
		}
	}
}

/* BLE Initialization */
void BLE_Init(void)
{
	Delay_ms(50);

	HCI_Reset();
	Delay_ms(100);

	ACI_WriteConfig();
	ACI_GATT_Init();
	ACI_GAP_Init();

	BLE_AddService();

	BLE_StartAdvertising();

	BLE_State = BLE_STATE_READY;
}


/* =========================================================
 * DEFAULT CALLBACKS (WEAK)
 * ========================================================= */

/* Temporary declaration. Will be declared and defined in BLE_Appl.c */

//__attribute__((weak)) void BLE_OnConnected(void) {}
//__attribute__((weak)) void BLE_OnDisconnected(void) {}
//__attribute__((weak)) void BLE_OnDataReceived(uint8_t *data, uint8_t len) {}
