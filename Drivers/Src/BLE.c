/*
 * BLE.c
 *
 *  Created on: 9 apr 2026
 *      Author: marco91
 */

#include "BLE.h"

/* =========================================================
 * HARDWARE INTERFACE (TO BE IMPLEMENTED BY USER)
 * =========================================================
 *
 * These functions abstract your MCU hardware.
 * You must implement them elsewhere (e.g. SPI driver, GPIO).
 */

extern void SPI_TransmitReceive(uint8_t *tx, uint8_t *rx, uint16_t len);
extern void BLE_CS_LOW(void);     // Pull chip-select LOW
extern void BLE_CS_HIGH(void);    // Release chip-select
extern bool BLE_IRQ_Read(void);   // Read IRQ pin (data ready)
extern void Delay_ms(uint32_t ms);

/* =========================================================
 * INTERNAL STATE
 * ========================================================= */

/* Current BLE state */
static BLE_State_t BLE_State = BLE_STATE_IDLE;

/* IRQ flag set from interrupt */
static volatile bool BLE_irq_flag = false;

/* SPI buffers */
static uint8_t SPI_TX[BLE_MAX_BUFFER];
static uint8_t SPI_RX[BLE_MAX_BUFFER];

/* Connection handle (assigned after connection) */
static uint16_t Connection_Handle = 0;

/* =========================================================
 * IRQ HANDLER
 * ========================================================= */

/**
 * @brief Must be called from external interrupt (EXTI)
 *
 * IMPORTANT:
 * - Do NOT do SPI here
 * - Only set a flag
 */
void BLE_EXTI_IRQHandler(void)
{
	BLE_irq_flag = true;
}

/* =========================================================
 * SPI PROTOCOL DEFINITIONS
 * ========================================================= */

/* Control bytes for BlueNRG SPI */
#define BLE_SPI_WRITE 0x0A
#define BLE_SPI_READ  0x0B

/* =========================================================
 * SPI WRITE (MCU -> BLE)
 * ========================================================= */

static bool BLE_SPI_Write(uint8_t *data, uint16_t len)
{
	uint8_t Header_TX[5] = {BLE_SPI_WRITE, 0,0,0,0};
	uint8_t Header_RX[5];

	BLE_CS_LOW();

	/* Exchange header */
	SPI_TransmitReceive(Header_TX, Header_RX, 5);

	/* Check if BLE is ready */
	if (Header_RX[0] != 0x02)
	{
		BLE_CS_HIGH();
		return false;
	}

	/* Check available buffer space */
	if (Header_RX[1] < len)
	{
		BLE_CS_HIGH();
		return false;
	}

	/* Send payload */
	SPI_TransmitReceive(data, SPI_RX, len);

	BLE_CS_HIGH();
	return true;
}

/* =========================================================
 * SPI READ (BLE -> MCU)
 * ========================================================= */

static uint16_t BLE_SPI_Read(uint8_t *buffer)
{
	uint8_t Header_TX[5] = {BLE_SPI_READ,0,0,0,0};
	uint8_t Header_RX[5];

	BLE_CS_LOW();

	/* Read header */
	SPI_TransmitReceive(Header_TX, Header_RX, 5);

	if (Header_RX[0] != 0x02)
	{
		BLE_CS_HIGH();
		return 0;
	}

	/* Number of bytes available */
	uint8_t size = Header_RX[3];

	/* Send dummy bytes to receive data */
	for (uint8_t i=0;i<size;i++)
		SPI_TX[i] = 0;

	SPI_TransmitReceive(SPI_TX, buffer, size);

	BLE_CS_HIGH();

	return size;
}

/* =========================================================
 * GENERIC COMMAND SEND
 * ========================================================= */

/**
 * @brief Send HCI/ACI command (blocking retry)
 */
static void BLE_Send(uint8_t *cmd, uint8_t len)
{
	while (!BLE_SPI_Write(cmd, len));
}

/* =========================================================
 * BASIC BLE COMMANDS
 * ========================================================= */

/* Reset BLE chip */
static void HCI_Reset(void)
{
	uint8_t cmd[] = {0x01U, 0x03U, 0x0CU, 0x00U};

	BLE_Send(cmd, sizeof(cmd));
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

	BLE_Send(cmd, sizeof(cmd));
}

/* Initialize GATT layer */
static void ACI_GATT_Init(void)
{
	uint8_t cmd[] = {0x01U, 0x01U, 0xFDU, 0x00U};

	BLE_Send(cmd, sizeof(cmd));
}

/* Initialize GAP layer (peripheral role) */
static void ACI_GAP_Init(void)
{
	uint8_t cmd[] = {0x01U, 0x8AU, 0xFCU, 0x03U, 0x01U, 0x00U, 0x00U};

	BLE_Send(cmd, sizeof(cmd));
}

/* =========================================================
 * GATT SERVICE (DRONE INTERFACE)
 * ========================================================= */

/**
 * Service contains:
 *  - TX characteristic (notify): drone -> phone
 *  - RX characteristic (write):  phone -> drone
 */
static void BLE_AddService(void)
{
	/* Add service */
	uint8_t svc[] = {0x01U, 0x02U, 0xFDU, 0x05U, 0x01U, 0x34U, 0x12U, 0x07};

	BLE_Send(svc, sizeof(svc));

	/* TX characteristic (notify) */
	uint8_t tx[] = {0x01U, 0x04U, 0xFDU, 0x08U, 0U, 0U, 0x02U, 0x08U, 0x10U, 0U, 0U};

	BLE_Send(tx, sizeof(tx));

	/* RX characteristic (write) */
	uint8_t rx[] = {0x01U, 0x04U, 0xFDU, 0x08U, 0U, 0U, 0x02U, 0x08U, 0x08U, 0U, 0U};

	BLE_Send(rx, sizeof(rx));
}

/* =========================================================
 * ADVERTISING
 * ========================================================= */

static void BLE_StartAdvertising(void)
{
	uint8_t cmd[] = {0x01U, 0x0AU, 0x20U, 0x01U, 0x01U};

	BLE_Send(cmd, sizeof(cmd));
}

/* =========================================================
 * EVENT PARSER
 * ========================================================= */

/**
 * @brief Decode BLE events and trigger actions
 */
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

/* =========================================================
 * MAIN PROCESS FUNCTION
 * ========================================================= */

/**
 * @brief Must be called regularly
 *
 * Reads ALL pending events while IRQ is high.
 */
void BLE_Process(void)
{
	if (!BLE_irq_flag) return;

	BLE_irq_flag = false;

	/* Keep reading until IRQ is High, as per information from datasheet */
	while (BLE_IRQ_Read())
	{
		uint8_t buffer[BLE_MAX_BUFFER];

		uint16_t len = BLE_SPI_Read(buffer);

		if (len > 0)
		{
			BLE_ParseEvent(buffer, len);
		}
	}
}

/* =========================================================
 * INITIALIZATION
 * ========================================================= */

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
