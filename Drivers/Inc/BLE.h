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
#include <stdbool.h>

/* =========================
 * CONFIGURATION
 * ========================= */

/* Maximum size of SPI buffer (must cover largest BLE packet) */
#define BLE_MAX_BUFFER 128

/* =========================
 * BLE STATE MACHINE
 * ========================= */

/**
 * @brief Internal BLE state
 */
typedef enum
{
	BLE_STATE_IDLE = 0, // Not initialized
	BLE_STATE_READY,    // Initialized, advertising
	BLE_STATE_CONNECTED // Connected to central device
} BLE_State_t;

/* =========================
 * PUBLIC API
 * ========================= */

/**
 * @brief Initialize BLE module
 *
 * Performs:
 *  - Reset
 *  - Stack init (GATT + GAP)
 *  - Service creation
 *  - Start advertising
 */
void BLE_Init(void);

/**
 * @brief Process BLE events
 *
 * Must be called regularly (main loop or RTOS task).
 * Handles:
 *  - Reading events from SPI
 *  - Parsing events
 *  - Triggering callbacks
 */
void BLE_Process(void);

/**
 * @brief Send telemetry data via notification
 *
 * @param data Pointer to payload
 * @param len  Payload length
 *
 * Only works when connected.
 */
void BLE_SendTelemetry(uint8_t *data, uint8_t len);

/* =========================
 * CALLBACKS (APPLICATION)
 * ========================= */

/**
 * @brief Called when BLE connection is established
 */
void BLE_OnConnected(void);

/**
 * @brief Called when BLE is disconnected
 */
void BLE_OnDisconnected(void);

/**
 * @brief Called when data is received from central (phone)
 *
 * @param data Pointer to received payload
 * @param len  Payload length
 */
void BLE_OnDataReceived(uint8_t *data, uint8_t len);

#endif
