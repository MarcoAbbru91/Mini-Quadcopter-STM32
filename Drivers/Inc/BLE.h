/*
 * BLE.h
 *
 *  Created on: 9 apr 2026
 *      Author: marco91
 */

  
#ifndef BLE_H
#define BLE_H


#include <stdint.h>
#include "gp_timer.h"
#include "debug.h"
#include "hci.h"
#include "GPIO.h"
#include "Timer.h"
#include "Sensor_service.h"



/****************************************************************************
DEFINES
****************************************************************************/
#define SYSCLK_FREQ 84000000

/* SPI CS Pin for BLE module (PB0) */
#define BLE_CS_HIGH()  (GPIOB_BSRR = (1UL << GPIOB_BSRR_BS_0_OFFSET))/* Set CS pin high */
#define BLE_CS_LOW()   (GPIOB_BSRR = (1UL << GPIOB_BSRR_BR_0_OFFSET))/* Set CS pin low */

#define STM32_UUID ((uint32_t *)0x1FFF7A10)
#define GAP_PERIPHERAL_ROLE_IDB05A1			(0x01)
#define MITM_PROTECTION_REQUIRED        (0x01)
#define OOB_AUTH_DATA_ABSENT		    (0x00)
#define USE_FIXED_PIN_FOR_PAIRING		(0x00)
#define BONDING				            (0x01)




/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/
extern volatile uint32_t HCI_ProcessEvent;



/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/
void Clear_SPI_EXTI_Flag(void);
void Enable_SPI_IRQ(void);
void Disable_SPI_IRQ(void);
void BLE_IRQ_SetAsOutput(void);
void BLE_IRQ_SetAsInput(void);

uint8_t BLE_Init(void);

uint8_t BlueNRG_DataPresent(void);

uint32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size);
uint32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2);

extern void Hal_Write_Serial(const void* data1, const void* data2, uint16_t n_bytes1, uint16_t n_bytes2);

 

#endif
