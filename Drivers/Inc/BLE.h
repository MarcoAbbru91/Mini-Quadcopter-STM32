/*
 * BLE.h
 *
 *  Created on: 9 apr 2026
 *      Author: marco91
 */

/**
 * @file BLE.h
 * @brief BLE driver for BlueNRG (SPI-based)
 *
 * This module provides:
 *  - Initialization of BLE stack (GATT + GAP + advertising)
 *  - Event-driven processing (IRQ + SPI)
 *  - Simple GATT service (TX notify + RX write)
 *  - Callback interface for application layer
 *
 * Usage:
 *  - Call BLE_Init() once at startup
 *  - Call BLE_Process() in main loop or task
 *  - Implement hardware functions (SPI, GPIO)
 */

#ifndef BLE_H
#define BLE_H

#include <stdint.h>
#include "GPIO.h"
#include "Timer.h"
#include "BLE_Appl.h"


/****************************************************************************
DEFINES
****************************************************************************/

/* Maximum size of SPI buffer (it must cover the largest BLE packet) */
#define BLE_MAX_BUFFER 128


/* SPI CS Pin for BLE module */
#define BLE_CS_HIGH()  (GPIOB_BSRR |= (1UL << GPIOB_BSRR_BS_0_OFFSET))/* Set CS pin high */
#define BLE_CS_LOW()   (GPIOB_BSRR |= (1UL << GPIOB_BSRR_BR_0_OFFSET))/* Set CS pin low */

/* Control bytes for BlueNRG SPI */
#define BLE_SPI_WRITE 0x0A
#define BLE_SPI_READ  0x0B


/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/

/* Internal BLE state machine */
typedef enum
{
	BLE_STATE_IDLE = 0, // Not initialized
	BLE_STATE_READY,    // Initialized, advertising
	BLE_STATE_CONNECTED // Connected to central device
} BLE_State_t;

/* SPI buffers */
uint8_t SPI_TX[BLE_MAX_BUFFER];
uint8_t SPI_RX[BLE_MAX_BUFFER];




/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* IRQ handler called by external interrupt (EXTI) */
void BLE_EXTI_IRQHandler(void);

/* BLE module initialization */
void BLE_Init(void);

uint16_t BLE_SPI_Read(uint8_t *buffer);

void BLE_SPI_Write(uint8_t *data, uint16_t len);

/* Process regular BLE events (events from SPI, parsing of events, triggering of callbacks) */
void BLE_Process(void);


#endif
