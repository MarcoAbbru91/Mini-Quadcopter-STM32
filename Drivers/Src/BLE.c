/*
 * BLE.c
 *
 *  Created on: 9 apr 2026
 *      Author: marco91
 */


#include "BLE.h"



/****************************************************************************
GLOBAL VARIABLES
****************************************************************************/
uint16_t service_handle;
uint16_t dev_name_char_handle;
uint16_t appearance_char_handle;

volatile uint32_t HCI_ProcessEvent=0;




/* Resets the BlueNRG */
void BlueNRG_RST(void) 
{
	GPIOB_ODR &= ~(1UL<<2); 
	Delay_ms(5.0f);
	GPIOB_ODR |= (1UL<<2); 
	Delay_ms(5.0f);
}

uint8_t getBlueNRGVersion(uint8_t *hwVersion, uint16_t *fwVersion)
{
	uint8_t status;
	uint8_t hci_version = 0 ;
	uint8_t lmp_pal_version = 0;
	uint16_t hci_revision=0;
	uint16_t manufacturer_name=0;
	uint16_t lmp_pal_subversion=0;

	status = hci_le_read_local_version(&hci_version, &hci_revision, &lmp_pal_version,
				     &manufacturer_name, &lmp_pal_subversion);


	if (status == BLE_STATUS_SUCCESS) {
		*hwVersion = hci_revision >> 8;
		*fwVersion = (hci_revision & 0xFF) << 8; // Major Version Number
		*fwVersion |= ((lmp_pal_subversion >> 4) & 0xF) << 4; // Minor Version Number
		*fwVersion |= lmp_pal_subversion & 0xF; // Patch Version Number
	}

	return status;
}

/* Reports if the BlueNRG has data for the host micro */
uint8_t BlueNRG_DataPresent(void)
{
	return BLE_IRQ_ReadGPIO() ? 1 : 0;
}


/* Reads from BlueNRG SPI buffer and store data into local buffer */
uint32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size)
{
	uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
	uint8_t header_slave[5];
	uint16_t byte_count;
	uint8_t len = 0;


	BLE_CS_LOW();
	SPI1_TransferBuffer(header_master, header_slave, 5);  // la tua funzione

	if(header_slave[0] == 0x02)
	{
		byte_count = (header_slave[4] << 8) | header_slave[3];
		if(byte_count > 0)
		{
			if(byte_count > buff_size)
			{
				byte_count = buff_size;
			}

			uint8_t tx_ff[256];

			for(int i = 0; i < byte_count; i++)
			{
				tx_ff[i] = 0xFF;
			}

			SPI1_TransferBuffer(tx_ff, buffer, byte_count);
			len = byte_count;
		}
	}

	BLE_CS_HIGH();

	for(volatile int i = 0; i < 2; i++) __asm volatile("nop");

	return (len);
}

/* Writes data from local buffer to SPI */
uint32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{
	int32_t result = 0;
	uint8_t header_master[5] = {0x0a, 0x00, 0x00, 0x00, 0x00};
	uint8_t header_slave[5];
	uint8_t read_buf[256];

	Disable_SPI_IRQ();

	BLE_IRQ_SetAsOutput(); // SPI fix - IMPORTANT
	Delay_ms(0.15f); // 150us

	BLE_CS_LOW();
	SPI1_TransferBuffer(header_master, header_slave, 5);
	BLE_IRQ_SetAsInput();

	if(header_slave[0] == 0x02)
	{
		if(header_slave[1] >= (Nb_bytes1 + Nb_bytes2))
		{
			if(Nb_bytes1 > 0) SPI1_TransferBuffer(data1, read_buf, Nb_bytes1);
			if(Nb_bytes2 > 0) SPI1_TransferBuffer(data2, read_buf, Nb_bytes2);
		}
		else result = -2;
	}
	else result = -1;

	BLE_CS_HIGH();
	Enable_SPI_IRQ();
	return (result);
}

/* HAL hook used by hci_write() to push HCI packets out over SPI */
void Hal_Write_Serial(const void* data1, const void* data2, uint16_t n_bytes1, uint16_t n_bytes2)
{
	uint32_t counter = 42016806; // 5 seconds. TODO TMP code to be changed!!!!!!
	while(counter >= 0){
		if((BlueNRG_SPI_Write((uint8_t *)data1, (uint8_t *)data2, (uint8_t)n_bytes1, (uint8_t)n_bytes2)) == 0) break;
		if(counter == 0) break;
		counter--;
	}
}

/* BLE Initialization */
uint8_t BLE_Init(void)
{
	uint8_t ret = 1;
	uint8_t  hwVersion = 0U;
	uint16_t fwVersion = 0U;

	const char BoardName[11] = {'M', 'A', 'R', 'C', 'O', '_', 'D', 'R', 'O', 'N', 'E'};

	bdaddr[0] = (STM32_UUID[1] >> 24) & 0xFF;
	bdaddr[1] = (STM32_UUID[0]    ) & 0xFF;
	bdaddr[2] = (STM32_UUID[2] >> 8) & 0xFF;
	bdaddr[3] = (STM32_UUID[0] >> 16) & 0xFF;
	bdaddr[4] = (hwVersion > 0x30) ?
				((((0x34-48)*10) + (0x30-48) + 100) & 0xFF) :
				((((0x34-48)*10) + (0x30-48)) & 0xFF) ;
	bdaddr[5] = 0xC0; /* for a Legal BLE Random MAC */

	HCI_Init(); // Initializes event queue (SPI fix)

	BlueNRG_RST(); // Resets hardware

	ret= getBlueNRGVersion(&hwVersion, &fwVersion); // Reads version — Acts as first transaction

	BlueNRG_RST(); // ← SECOND reset, mandatory! (otherwise it will fail)

	ret = aci_gatt_init();
	ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07,
								&service_handle, &dev_name_char_handle, &appearance_char_handle);
	ret = hci_le_set_random_address(bdaddr); // MAC random valido

	ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
									7/*strlen(BoardName)*/, (uint8_t *)BoardName); // Board name
	ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
										OOB_AUTH_DATA_ABSENT,
										NULL, 7, 16,
										USE_FIXED_PIN_FOR_PAIRING, 123456,
										BONDING); // Authentication
	ret = aci_hal_set_tx_power_level(1, 4); // TX power

	ret = Add_ConsoleW2ST_Service();
	ret = Add_ConfigW2ST_Service();

	ret = Add_HWServW2ST_Service(); // GATT custom services

	ret = Add_ConsoleW2ST_Service();
	ret = Add_ConfigW2ST_Service();

	return (ret);
}

/* Set in Output mode the IRQ - REQUESTED BY SPI fix BlueNRG-MS*/
void BLE_IRQ_SetAsOutput(void)
{
	/* Clear and Set port to digital output mode */
	GPIOA_MODER &= ~(0x3UL << GPIOA_MODER_4_OFFSET);
	GPIOA_MODER |=  (0x1UL << GPIOA_MODER_4_OFFSET); // Output mode
	
	/* Force pin HIGH through BSRR (atomic set) - NECESSARY */
	GPIOA_BSRR = (1UL << 4);

	/* Output Type Register by default set to Push-pull */
}

/* Set the IRQ in input mode */
void BLE_IRQ_SetAsInput(void)
{
	/* Clear port to get digital input mode */
	GPIOA_MODER &= ~(0x3UL << GPIOA_MODER_4_OFFSET); // Input mode
}

/* Enable SPI IRQ */
void Enable_SPI_IRQ(void)
{
	*NVIC_ISER_BASEADDR |= (1UL << EXTI4_IRQn); 
}

/* Disable SPI IRQ */
void Disable_SPI_IRQ(void)
{
	*NVIC_ICER_BASEADDR |= (1UL << EXTI4_IRQn);
}

/* Clear EXTI (External Interrupt) line for SPI IRQ */
void Clear_SPI_EXTI_Flag(void)
{
	EXTI_PR = (1UL << EXTI_PR_PR4_OFFSET);
}

