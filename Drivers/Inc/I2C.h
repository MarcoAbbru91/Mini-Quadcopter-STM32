/*
 * I2C.h
 *
 *  Created on: Jul 30, 2026
 *      Author: marco91
 */

#ifndef DRIVERS_INC_I2C_H_
#define DRIVERS_INC_I2C_H_


#include <stdint.h>
#include "RCC.h"
#include "GPIO.h"

/****************************************************************************
DEFINES
****************************************************************************/

#define I2C_Read_Operation   (0x01U) // LSB of the address byte set to 1 -> read operation
#define I2C_Write_Operation  (0xFEU) // 11111110 - keep LSB to 0 while keeping all other bits at 1 since it will be used with an "& operation"

/* Transfer direction, to be passed to I2C_SendAddress() */
#define I2C_Direction_Write  (0x00U)
#define I2C_Direction_Read   (0x01U)

/* Return values of the I2C functions */
#define I2C_OK               (0x00U)
#define I2C_ERROR            (0x01U) // NACK received or bus error or arbitration lost or timeout expired

/* Timeout expressed in while-loop iterations. Considering Clock=84MHz and each iteration
   taking around 5-10 clock cycles, 100000 iterations correspond to approx. 6-12ms,
   i.e. much longer than the transfer of a single byte (20us @400kHz) */
#define I2C_TIMEOUT          (100000UL)


/* I2C TIMING CONFIGURATION */

/* APB1 clock frequency (see RCC_Init(): SysClock=84MHz, PPRE1=2 -> PCLK1=42MHz) */
#define I2C_PCLK1_FREQ_HZ    (42000000UL)
/* Desired SCL frequency: <=100kHz -> Standard Mode, >100kHz (up to 400kHz) -> Fast Mode */
#define I2C_SCL_FREQ_HZ      (400000UL)

/* I2C CR2 FREQ bits: peripheral input clock expressed in MHz (must be >=2 in Sm, >=4 in Fm) */
#define I2C_CR2_FREQ_VAL     (I2C_PCLK1_FREQ_HZ / 1000000UL) // 42

/* Fast Mode with DUTY=0 (Tlow/Thigh = 2): Tscl = 3 * CCR * Tpclk1 */
#define I2C_CCR_VAL          (I2C_PCLK1_FREQ_HZ / (3UL * I2C_SCL_FREQ_HZ)) // 35 @400kHz
/* Maximum SCL rise time in Fast Mode is 300ns: TRISE = (300ns / Tpclk1) + 1 */
#define I2C_TRISE_VAL        (((I2C_PCLK1_FREQ_HZ / 1000000UL) * 300UL / 1000UL) + 1UL) // 13


/****************************************************************************
I2C REGISTERS
****************************************************************************/

/* Define base address specific to I2C1 - Pins PB6/PB8 (SCL) and PB7/PB9 (SDA) are used by TIM4 (PWM), so it cannot be used */
//#define I2C1_BASE_ADDRESS    (0x40005400UL)
/* Define base address specific to I2C2 (external devices) - Pins PB10 (SCL) and PB3 (SDA) are free */
#define I2C_BASE_ADDRESS    (0x40005800UL)
/* Define base address specific to I2C3 - Pin PA8 (SCL) is used as general purpose output, so it cannot be used */
//#define I2C3_BASE_ADDRESS    (0x40005C00UL)


/* I2C CR1 - Control Register 1 */
#define I2C_CR1_OFFSET         (0x00UL)/* I2C Control Register 1 address */
#define I2C_CR1_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_CR1_OFFSET)
#define I2C_CR1           (* (volatile uint32_t *)(I2C_CR1_BASE_ADDRESS)) // typecast and dereference
/* I2C Control Register 1 PE (Peripheral Enable) Register offset */
#define I2C_CR1_PE_OFFSET         (0UL)
/* I2C Control Register 1 NOSTRETCH (Clock Stretching Disable) Register offset */
#define I2C_CR1_NOSTRETCH_OFFSET  (7UL)
/* I2C Control Register 1 START (Start Generation) Register offset */
#define I2C_CR1_START_OFFSET      (8UL)
/* I2C Control Register 1 STOP (Stop Generation) Register offset */
#define I2C_CR1_STOP_OFFSET       (9UL)
/* I2C Control Register 1 ACK (Acknowledge Enable) Register offset */
#define I2C_CR1_ACK_OFFSET        (10UL)
/* I2C Control Register 1 POS (Acknowledge/PEC Position) Register offset */
#define I2C_CR1_POS_OFFSET        (11UL)
/* I2C Control Register 1 SWRST (Software Reset) Register offset */
#define I2C_CR1_SWRST_OFFSET      (15UL)


/* I2C CR2 - Control Register 2 */
#define I2C_CR2_OFFSET         (0x04UL)/* I2C Control Register 2 address */
#define I2C_CR2_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_CR2_OFFSET)
#define I2C_CR2           (* (volatile uint32_t *)(I2C_CR2_BASE_ADDRESS)) // typecast and dereference
/* I2C Control Register 2 FREQ (Peripheral Clock Frequency) Register offset */
#define I2C_CR2_FREQ_OFFSET    (0UL) // 6 bits
/* I2C Control Register 2 ITERREN (Error Interrupt Enable) Register offset */
#define I2C_CR2_ITERREN_OFFSET (8UL)
/* I2C Control Register 2 ITEVTEN (Event Interrupt Enable) Register offset */
#define I2C_CR2_ITEVTEN_OFFSET (9UL)
/* I2C Control Register 2 ITBUFEN (Buffer Interrupt Enable) Register offset */
#define I2C_CR2_ITBUFEN_OFFSET (10UL)


/* I2C OAR1 - Own Address Register 1 */
#define I2C_OAR1_OFFSET         (0x08UL)/* I2C Own Address Register 1 address */
#define I2C_OAR1_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_OAR1_OFFSET)
#define I2C_OAR1           (* (volatile uint32_t *)(I2C_OAR1_BASE_ADDRESS)) // typecast and dereference
/* I2C Own Address Register 1 ADDMODE (7-bit/10-bit Slave Address) Register offset */
#define I2C_OAR1_ADDMODE_OFFSET  (15UL)
/* I2C Own Address Register 1 bit 14 must always be kept at 1 by software (reserved) */
#define I2C_OAR1_KEEPAT1_OFFSET  (14UL)


/* I2C OAR2 - Own Address Register 2 */
#define I2C_OAR2_OFFSET         (0x0CUL)/* I2C Own Address Register 2 address */
#define I2C_OAR2_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_OAR2_OFFSET)
#define I2C_OAR2           (* (volatile uint32_t *)(I2C_OAR2_BASE_ADDRESS)) // typecast and dereference


/* I2C DR - Data Register */
#define I2C_DR_OFFSET         (0x10UL)/* I2C Data Register address */
#define I2C_DR_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_DR_OFFSET)
#define I2C_DR           (* (volatile uint32_t *)(I2C_DR_BASE_ADDRESS)) // Kept 32 bits long altough the data buffer is 8-bits long for alignment reasons. Will be casted inside the code.


/* I2C SR1 - Status Register 1 */
#define I2C_SR1_OFFSET         (0x14UL)/* I2C Status Register 1 address */
#define I2C_SR1_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_SR1_OFFSET)
#define I2C_SR1           (* (volatile uint32_t *)(I2C_SR1_BASE_ADDRESS)) // typecast and dereference
/* I2C Status Register 1 SB (Start Bit generated) Register offset */
#define I2C_SR1_SB_OFFSET     (0UL)
/* I2C Status Register 1 ADDR (Address sent/matched) Register offset */
#define I2C_SR1_ADDR_OFFSET   (1UL)
/* I2C Status Register 1 BTF (Byte Transfer Finished) Register offset */
#define I2C_SR1_BTF_OFFSET    (2UL)
/* I2C Status Register 1 STOPF (Stop detection) Register offset */
#define I2C_SR1_STOPF_OFFSET  (4UL)
/* I2C Status Register 1 RXNE (Data Register not empty) Register offset */
#define I2C_SR1_RXNE_OFFSET   (6UL)
/* I2C Status Register 1 TXE (Data Register empty) Register offset */
#define I2C_SR1_TXE_OFFSET    (7UL)
/* I2C Status Register 1 BERR (Bus Error) Register offset */
#define I2C_SR1_BERR_OFFSET   (8UL)
/* I2C Status Register 1 ARLO (Arbitration Lost) Register offset */
#define I2C_SR1_ARLO_OFFSET   (9UL)
/* I2C Status Register 1 AF (Acknowledge Failure, i.e. NACK received) Register offset */
#define I2C_SR1_AF_OFFSET     (10UL)
/* I2C Status Register 1 OVR (Overrun/Underrun) Register offset */
#define I2C_SR1_OVR_OFFSET    (11UL)


/* I2C SR2 - Status Register 2 */
#define I2C_SR2_OFFSET         (0x18UL)/* I2C Status Register 2 address */
#define I2C_SR2_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_SR2_OFFSET)
#define I2C_SR2           (* (volatile uint32_t *)(I2C_SR2_BASE_ADDRESS)) // typecast and dereference
/* I2C Status Register 2 MSL (Master/Slave mode) Register offset */
#define I2C_SR2_MSL_OFFSET    (0UL)
/* I2C Status Register 2 BUSY (Bus Busy) Register offset */
#define I2C_SR2_BUSY_OFFSET   (1UL)
/* I2C Status Register 2 TRA (Transmitter/Receiver) Register offset */
#define I2C_SR2_TRA_OFFSET    (2UL)


/* I2C CCR - Clock Control Register */
#define I2C_CCR_OFFSET         (0x1CUL)/* I2C Clock Control Register address */
#define I2C_CCR_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_CCR_OFFSET)
#define I2C_CCR           (* (volatile uint32_t *)(I2C_CCR_BASE_ADDRESS)) // typecast and dereference
/* I2C Clock Control Register CCR (Clock Control Divider) Register offset */
#define I2C_CCR_CCR_OFFSET    (0UL) // 12 bits
/* I2C Clock Control Register DUTY (Fast Mode Duty Cycle) Register offset */
#define I2C_CCR_DUTY_OFFSET   (14UL)
/* I2C Clock Control Register F/S (Standard/Fast Mode Selection) Register offset */
#define I2C_CCR_FS_OFFSET     (15UL)


/* I2C TRISE - Rise Time Register */
#define I2C_TRISE_OFFSET         (0x20UL)/* I2C Rise Time Register address */
#define I2C_TRISE_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_TRISE_OFFSET)
#define I2C_TRISE           (* (volatile uint32_t *)(I2C_TRISE_BASE_ADDRESS)) // typecast and dereference
/* I2C Rise Time Register TRISE Register offset */
#define I2C_TRISE_TRISE_OFFSET  (0UL) // 6 bits


/* I2C FLTR - Filter Register */
#define I2C_FLTR_OFFSET         (0x24UL)/* I2C Filter Register address */
#define I2C_FLTR_BASE_ADDRESS   (I2C_BASE_ADDRESS + I2C_FLTR_OFFSET)
#define I2C_FLTR           (* (volatile uint32_t *)(I2C_FLTR_BASE_ADDRESS)) // typecast and dereference
/* I2C Filter Register DNF (Digital Noise Filter) Register offset */
#define I2C_FLTR_DNF_OFFSET    (0UL) // 4 bits
/* I2C Filter Register ANOFF (Analog Noise Filter OFF) Register offset */
#define I2C_FLTR_ANOFF_OFFSET  (4UL)


/****************************************************************************
FUNCTIONS PROTOTYPES
****************************************************************************/

/* Initialize I2C peripheral */
void I2C_Init();

/* Generate a START (or a repeated START if the master is already on the bus) condition */
uint8_t I2C_Start(void);
/* Generate a STOP condition to release the bus */
void I2C_Stop(void);
/* Send the 7-bit slave address (right aligned, without the R/W bit) with the requested direction */
uint8_t I2C_SendAddress(uint8_t SlaveAddr, uint8_t Direction);
/* Clear the ADDR flag by reading SR1 followed by SR2 */
void I2C_ClearADDR(void);
/* Clear the error flags and generate a STOP condition to release the bus after a failed transfer */
void I2C_Abort(void);
/* I2C Transmit operation (writes one byte in DR once the TX buffer is empty) */
uint8_t I2C_Transmit(uint8_t Val);

/* I2C Write operation to configure slave's registers */
uint8_t I2C_Write(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t Data);
/* I2C Reads one register from the sensor */
uint8_t I2C_Read(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *Data);
/* I2C Reads Len consecutive registers from the sensor, starting from RegAddr */
uint8_t I2C_ReadBurst(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *Buffer, uint8_t Len);


#endif /* DRIVERS_INC_I2C_H_ */
