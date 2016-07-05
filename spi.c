//--------------------------------------------------------------------------------
// Name: Spi.c
//--------------------------------------------------------------------------------
// Company: Concordia University
// Author : MohammadHossein Askari-Hemmat
// 
// Create Date:    1/20/2014 
// Design Name: 	 SPI low level driver
// Target Devices: KL25Z128VLK4
// Platform: FRDM-KL25Z (Freescale Freedom Development Platform for Kinetis)
// Compiler Versions: Keil 5.1
// Version: 1.0
//----------------------------------------------------------------------------
#include "spi.h"

#include "common.h"
#include "freedom.h"

/*----------------------------------------------------------------------------
Initializes the SPI0 for:
	- Configure PTA15,PTA16,PTA17 as the SPI module pins (Second alternative)
	- Enables the SPI module by enabling clock gating to SPI and enables the SPI system 
	- MSB first 
	- Configure SPI as a MASTER
	- Disables the SPI transmit interrupt (SPTIE) 
	- Disables the SPI interrupt (SPIE)
	- Configures the SS pin as a general I/O pin (Not controlled by SPI) 
	- DMA request disabled
	- SPI uses separate pins for data input and data output (Is not bidirectional) 
	- SPI clocks stop when the MCU enters wait mode
	- Configures the Baudrate for 1MHZ operation 
Use the Following equation to find the proper Baudrate. If you are using the MCG
driver, the Busclock is 16MHZ. 
To use this driver qlong with NRF24L01 module, make sure that the SPCK is not more
than 10MHZ. Refer to the NRF24L01 datasheet for more information.

-Calculating the BaudRate:
	BaudRateDivisor = (SPPR + 1) * 2^(SPR + 1)
	BaudRate = BusClock / BaudRateDivisor
	Desired baudrate = 1MHZ, BusClock= 16MHZ  ----> BaudRateDivisor= 16 
	SPPR=1
	SPR=2
	
	This driver will use PTA14 as the CSN(Chip Select Not). The automatic triggering
	is disabled, so the code developer should take care of handling this pin.
*----------------------------------------------------------------------------*/

void spi_init(){
	
	en_SPI(); // Turns on the clock gating for SPI0 module
	SIM_SCGC5    |= SIM_SCGC5_PORTD_MASK;      /* Enable Clock to Port C */ 

	// PORTD_PCR8  = (1UL <<  8);                      /* (First  ALT) Pin PTC8  is GPIO (CE) */
	// PORTD_PCR9  = (1UL <<  8);                      /* (First  ALT) Pin PTC9  is GPIO (CSN) */
	// PORTD_PCR5  = (1UL <<  9);                      /* (Second ALT) Pin PTC5  is SPI0_SCK */
	// PORTD_PCR6  = (1UL <<  9);                      /* (Second ALT) Pin PTC6  is SPI0_MOSI */
	// PORTD_PCR7  = (1UL <<  9);                      /* (Second ALT) Pin PTC7  is SPI0_MISO */

	// PORTD_PCR0  = (1UL <<  8);                      /* (First  ALT) Pin PTC9  is GPIO (CSN) */
	PORTD_PCR1  = (1 <<  9);                      /* (Second ALT) Pin PTC5  is SPI0_SCK */
	PORTD_PCR2  = (1 <<  9);                      /* (Second ALT) Pin PTC6  is SPI0_MOSI */
	// PORTD_PCR3  = (1UL <<  9);                      /* (Second ALT) Pin PTC7  is SPI0_MISO */

	// FPTC_PDOR = (1UL << 8)|(1UL << 9);            /* switch Blue LED off  */
 //  FPTC_PDDR = (1UL << 8)|(1UL << 9);            /* enable PTD1 as Output */
	// EN_CSN;
	SPI0_C1 |= ((1<<SPI_C1_SPE_SHIFT)|(1<<SPI_C1_MSTR_SHIFT)|(1<<SPI_C1_CPHA_SHIFT));// Master mode is selected, SPI is Enabled, CPHA=1
	SPI0_C2 |= (1<<SPI_C2_SPISWAI_SHIFT);
	SPI0_BR = 0x00; // SPPR=1, SPR=2
}

/*----------------------------------------------------------------------------
SPI0 enable function:
	This function will enable the SPI0 module by enabling the SPI Clock Gating
in System Clock Gating Control Register 4. Refer to Page 204 for more information.
*----------------------------------------------------------------------------*/
void en_SPI(void){
		SIM_SCGC4 |= (1<<SIM_SCGC4_SPI0_SHIFT) | (SIM_SCGC4_SPI0_MASK);
}

/*----------------------------------------------------------------------------
SPI0 Read/Write function:
	Since there is only one register for reading and writing data to SPI, passing 
an argument to this function will return a data  
*----------------------------------------------------------------------------*/
uint8_t spi_send(uint8_t data){
	uint8_t RecData=0;
	while (((SPI0_S)&SPI_S_SPTEF_MASK)!= 0x20){
	}
	
	SPI0_D=data;
	
	while (((SPI0_S)&SPI_S_SPRF_MASK)!= 0x80){
	}
	
	RecData=SPI0_D;
	
	return RecData;
}
