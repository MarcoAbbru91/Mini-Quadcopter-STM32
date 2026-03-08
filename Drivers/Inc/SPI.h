/*
 * SPI.h
 *
 *  Created on: Mar 5, 2026
 *      Author: marco91
 */

#ifndef DRIVERS_INC_SPI_H_
#define DRIVERS_INC_SPI_H_


#include <stdint.h>

/****************************************************************************
DEFINES
****************************************************************************/

/* Define base address specific to SPI1 (Sensors) */
#define SPI1_BASE_ADDRESS    (0x40013000)
/* Define base address specific to SPI2 (BLE) */
#define SPI2_BASE_ADDRESS    (0x40003800)


/* SPI CR1 - Control Register 1 */
#define SPI1_CR1_OFFSET         (0x00UL)/* SPI Control Register 1 address */
#define SPI1_CR1_BASE_ADDRESS   (SPI1_BASE_ADDRESS + SPI1_CR1_OFFSET)
#define SPI1_CR1           (* (volatile uint32_t *)(SPI1_CR1_BASE_ADDRESS)) // typecast and dereference
/* SPI Control Register 1 CPHA Register offset */
#define SPI_CR1_CPHA_OFFSET  (0UL)
/* SPI Control Register 1 CPOL Register offset */
#define SPI_CR1_CPOL_OFFSET  (1UL)
/* SPI Control Register 1 MSTR Register offset */
#define SPI_CR1_MSTR_OFFSET  (2UL)
/* SPI Control Register 1 BR Register offset */
#define SPI_CR1_BR_OFFSET  (3UL) // 3 bits
/* SPI Control Register 1 SPE Register offset */
#define SPI_CR1_SPE_OFFSET  (6UL)
/* SPI Control Register 1 LSBFIRST Register offset */
#define SPI_CR1_LSBFIRST_OFFSET  (7UL)
/* SPI Control Register 1 SSI Register offset */
#define SPI_CR1_SSI_OFFSET  (8UL)
/* SPI Control Register 1 SSM Register offset */
#define SPI_CR1_SSM_OFFSET  (9UL)
/* SPI Control Register 1 RXONLY Register offset */
#define SPI_CR1_RXONLY_OFFSET  (10UL)
/* SPI Control Register 1 DFF Register offset */
#define SPI_CR1_DFF_OFFSET  (11UL)
/* SPI Control Register 1 BIDIMODE Register offset */
#define SPI_CR1_BIDIMODE_OFFSET  (15L)


/* SPI CR2 - Control Register 2 */
#define SPI1_CR2_OFFSET         (0x04UL)/* SPI Control Register 2 address */
#define SPI1_CR2_BASE_ADDRESS   (SPI1_BASE_ADDRESS + SPI1_CR2_OFFSET)
#define SPI1_CR2           (* (volatile uint32_t *)(SPI1_CR2_BASE_ADDRESS)) // typecast and dereference
/* SPI Control Register 2 RXNEIE Register offset */
#define SPI_CR2_RXNEIE_OFFSET  (6UL)


/* SPI SR - Status Register */
#define SPI1_SR_OFFSET         (0x08UL)/* SPI Status Register address */
#define SPI1_SR_BASE_ADDRESS   (SPI1_BASE_ADDRESS + SPI1_SR_OFFSET)
#define SPI1_SR           (* (volatile uint32_t *)(SPI1_SR_BASE_ADDRESS)) // typecast and dereference
/* SPI Status Register RXNE Register offset */
#define SPI_SR_RXNE_OFFSET  (0UL)


/* SPI DR - Data Register */
#define SPI1_DR_OFFSET         (0x0CUL)/* SPI Data Register address */
#define SPI1_DR_BASE_ADDRESS   (SPI1_BASE_ADDRESS + SPI1_DR_OFFSET)
#define SPI1_DR           (* (volatile uint32_t *)(SPI1_DR_BASE_ADDRESS)) // typecast and dereference


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Initialize SPI peripheral */
void SPI_Init();

void SPI_Transmit();

void SPI_Receive();

#endif /* DRIVERS_INC_SPI_H_ */
