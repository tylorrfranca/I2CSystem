/*
 * I2C.h
 *
 *	Main implementation of the I2C Init, Read, and Write Function
 *
 * Created on: May 24th, 2023
 *		Author: Jackie Huynh
 *
 */
 
#include "I2C.h"
#include "tm4c123gh6pm.h"

/*
 *	-------------------I2C0_Init------------------
 *	Basic I2C Initialization function for master mode @ 100kHz
 *	Input: None
 *	Output: None
 */
void I2C0_Init(void){
	
	/* Enable Required System Clock */
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_I2C0;    // Enable I2C0 System Clock
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;   // Enable GPIOB System Clock
	
	// Wait Until GPIOB System Clock is enabled
	while((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOB) != SYSCTL_RCGC2_GPIOB);
	
	/* GPIOB I2C Alternate Function Setup */
	GPIO_PORTB_AMSEL_R &= ~I2C0_PINS;   // Disable Analog Mode
	GPIO_PORTB_AFSEL_R |= I2C0_PINS;    // Enable Alternate Function Selection
	GPIO_PORTB_DEN_R |= I2C0_PINS;      // Enable Digital I/O
	
	// Select I2C0 as the alternate function 
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & I2C0_ALT_FUNC_MSK) | I2C0_ALT_FUNC_SET;
	
	// Configure pins for I2C
	GPIO_PORTB_ODR_R |= I2C0_SDA_PIN;   // Enable Open Drain for SDA pin
	GPIO_PORTB_PUR_R |= I2C0_PINS;      // Enable pull-up resistors
	
	/* I2C0 Setup as Master Mode @ 100kBits */
	I2C0_MCR_R = 0; // Ensure module is disabled before configuration
	I2C0_MCR_R = EN_I2C0_MASTER; // Configure I2C0 as Master
	
	/* Configuring I2C Clock Frequency to 100KHz */
	I2C0_MTPR_R = I2C_MTPR_TPR_VALUE; // TPR = 7 for 100kHz at 16MHz system clock
	
	// Reset and Enable I2C0 Master
	I2C0_MCR_R &= ~I2C_MCR_MFE; // Disable Master Function
	I2C0_MCR_R |= I2C_MCR_MFE;  // Re-enable Master Function

	// Optional: Add a small delay after init/reset
	// for(volatile uint32_t i=0; i<100; i++); // Simple delay loop
}

/*
 *	-------------------I2C0_Receive------------------
 *	Polls to receive data from specified peripheral
 *	Input: Slave address & Slave Register Address
 *	Output: Returns 8-bit data that has been received
 */
uint8_t I2C0_Receive(uint8_t slave_addr, uint8_t slave_reg_addr){
	char error;
	
	/* Check if I2C0 is busy */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Configure I2C0 Slave Address and Read Mode */
	I2C0_MSA_R = (slave_addr << 1);     // Slave Address is the 7 MSB (Write mode)
	I2C0_MSA_R &= ~I2C_MSA_RS;          // Ensure write mode (RS=0)
	I2C0_MDR_R = slave_reg_addr;        // Set Data Register to slave register address
	
	/* Initiate I2C by generating a START & RUN cmd (NO STOP) */
	I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_START); // = 0x03
	
	/* Wait until write is done and bus is idle */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Check for errors after first transaction (ACK check) */
	error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
	if(error != 0) {
		I2C0_MCS_R = I2C_MCS_STOP;  // Generate STOP condition
		// Wait for STOP to finish
		while(I2C0_MCS_R & I2C_MCS_BUSY);
		return 0xFF;  // Return error code
	}
	
	/* Set I2C to Receive with Slave Address and change to Read */
	I2C0_MSA_R = (slave_addr << 1) | I2C_MSA_RS;  // Set read bit (RS=1)
	
	/* Initiate single byte read with repeated START, RUN, STOP */
	I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP); // = 0x07
	
	/* Wait until read is done and bus is idle */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Check for any error */
	error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
	if(error != 0) {
		// STOP was already sent, just return error
		return 0xFF;  // Return error code
	}
	else
		return I2C0_MDR_R & 0xFF;  // return I2C data register least significant 8 bits.
}

/*
 *	-------------------I2C0_Transmit------------------
 *	Transmit a byte of data to specified peripheral
 *	Input: Slave address, Slave Register Address, Data to Transmit
 *	Output: Any Errors if detected, otherwise 0
 */
uint8_t I2C0_Transmit(uint8_t slave_addr, uint8_t slave_reg_addr, uint8_t data){
	char error;  // Temp Variable to hold errors
	
	/* Check if I2C0 is busy */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Configure I2C Slave Address, R/W Mode, and what to transmit */
	I2C0_MSA_R = (slave_addr << 1);  // Slave Address is the first 7 MSB
	I2C0_MSA_R &= ~I2C_MSA_RS;      // Ensure write mode (RS=0)
	I2C0_MDR_R = slave_reg_addr;      // Transmit register addr to interact
	
	/* Initiate I2C by generate a START bit and RUN cmd (NO STOP) */
	I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_START); // = 0x03
	
	/* Wait until write has been completed and bus is idle */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Check for errors after sending register address */
	error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
	if (error != 0) {
		I2C0_MCS_R = I2C_MCS_STOP; // Generate STOP
		while(I2C0_MCS_R & I2C_MCS_BUSY); // Wait for STOP
		return error; // Return error code
	}
	
	/* Update Data Register with data to be transmitted */
	I2C0_MDR_R = data;
	
	/* Initiate I2C by generating a STOP & RUN cmd */
	I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_STOP); // = 0x05
	
	/* Wait until write has been completed and bus is idle */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Check for any error after sending data*/
	error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
	if(error != 0)
		return error; // STOP was already sent
	else
		return 0; // Success
}

/*
 *	----------------I2C0_Burst_Receive-----------------
 *	Polls to receive multiple bytes of data from specified
 *  peripheral by incrementing starting slave register address
 *	Input: Slave address, Slave Register Address, Data Buffer
 *	Output: None
 */
void I2C0_Burst_Receive(uint8_t slave_addr, uint8_t slave_reg_addr, uint8_t* data, uint32_t size){
	char error;
	
	if (size <= 0) return; // No bytes to receive
	
	/* Check if I2C0 is busy */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Configure I2C0 Slave Address and Read Mode */
	I2C0_MSA_R = (slave_addr << 1);  // Slave Address is the 7 MSB (Write mode)
	I2C0_MSA_R &= ~I2C_MSA_RS;       // Ensure write mode
	I2C0_MDR_R = slave_reg_addr;     // Set Data Register to slave register address
	
	/* Initiate I2C by generating a START & RUN cmd (NO STOP) */
	I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_START); // = 0x03
	
	/* Wait until write is done and bus is idle */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Check for errors after sending register address */
	error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
	if(error != 0) {
		I2C0_MCS_R = I2C_MCS_STOP;  // Generate STOP condition
		while(I2C0_MCS_R & I2C_MCS_BUSY);
		return; // Exit on error
	}
	
	/* Set I2C to Receive with Slave Address and change to Read */
	I2C0_MSA_R = (slave_addr << 1) | I2C_MSA_RS;  // Set read bit
	
	/* Check number of bytes to receive */
	if (size == 1) {
		// Single byte receive: START, RUN, STOP
		I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP); // = 0x07
		while(I2C0_MCS_R & I2C_MCS_BUSY); // Wait for completion
		error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
		if (error == 0) {
			*data = I2C0_MDR_R & 0xFF; // Store received data
		}
		// Error handling (optional): could set data to 0xFF or similar
		return;
	} else {
		// Multiple bytes receive
		// First byte: START, RUN, ACK
		I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_ACK); // = 0x0B
		while(I2C0_MCS_R & I2C_MCS_BUSY);
		error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
		if (error != 0) {
				I2C0_MCS_R = I2C_MCS_STOP; // Generate STOP
				while(I2C0_MCS_R & I2C_MCS_BUSY);
				return; // Exit on error
		}
		*data++ = I2C0_MDR_R & 0xFF; // Store first byte
		size--;
		
		// Middle bytes: RUN, ACK
		while(size > 1){
			I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_ACK); // = 0x09
			while(I2C0_MCS_R & I2C_MCS_BUSY);
			error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
			if (error != 0) {
					I2C0_MCS_R = I2C_MCS_STOP; // Generate STOP
					while(I2C0_MCS_R & I2C_MCS_BUSY);
					return; // Exit on error
			}
			*data++ = I2C0_MDR_R & 0xFF;
			size--;
		}
		
		// Last byte: RUN, STOP (Master sends NACK implicitly with STOP)
		I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_STOP); // = 0x05
		while(I2C0_MCS_R & I2C_MCS_BUSY);
		error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
		// Don't check error strictly here as NACK from slave is possible but maybe expected
		// But check ARBLST etc. If needed.
		*data = I2C0_MDR_R & 0xFF; // Store last byte regardless of final NACK
	}
}

/*
 *	----------------I2C0_Burst_Transmit-----------------
 *	Transmit multiple bytes of data to specified peripheral
 *  by incrementing starting slave address
 *	Input: Slave address, Slave Register Address, Data Buffer to transmit
 *	Output: None
 */
uint8_t I2C0_Burst_Transmit(uint8_t slave_addr, uint8_t slave_reg_addr, uint8_t* data, uint32_t size){
	char error;  // Temp Error Variable
	
	/* Asserting Param */
	if(size <= 0)
		return 0; // No data to send
	
	/* Check if I2C0 is busy */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Configure I2C Slave Address, R/W Mode, and what to transmit */
	I2C0_MSA_R = (slave_addr << 1);  // Slave Address is the first 7 MSB
	I2C0_MSA_R &= ~I2C_MSA_RS;      // Ensure write mode (RS=0)
	I2C0_MDR_R = slave_reg_addr;      // Transmit register addr to interact
	
	/* Initiate I2C by generate a START bit and RUN cmd (NO STOP) */
	I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_START); // = 0x03
	
	/* Wait until write has been completed and bus is idle */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Check for errors after sending register address */
	error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
	if (error != 0) {
		I2C0_MCS_R = I2C_MCS_STOP; // Generate STOP
		while(I2C0_MCS_R & I2C_MCS_BUSY); // Wait for STOP
		return error; // Return error code
	}
	
	/* Loop to Burst Transmit what is stored in data buffer */
	while(size > 1){
		I2C0_MDR_R = *data++;  // Load data and increment pointer
		I2C0_MCS_R = I2C_MCS_RUN;     // RUN (Continue transaction) = 0x01
		while(I2C0_MCS_R & I2C_MCS_BUSY);  // Wait until transmit is complete
		error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
		if (error != 0) {
			I2C0_MCS_R = I2C_MCS_STOP; // Generate STOP
			while(I2C0_MCS_R & I2C_MCS_BUSY); // Wait for STOP
			return error; // Return error code
		}
		size--;
	}
	
	/* Last byte with STOP condition */
	I2C0_MDR_R = *data;  // Load last data byte
	I2C0_MCS_R = (I2C_MCS_RUN | I2C_MCS_STOP);   // STOP + RUN = 0x05
	
	/* Wait until write has been completed and bus is idle */
	while(I2C0_MCS_R & I2C_MCS_BUSY);
	
	/* Check for any error after sending last byte */
	error = I2C0_MCS_R & (I2C_MCS_ERROR | I2C_MCS_ARBLST);
	if(error != 0)
		return error; // STOP was already sent
	else
		return 0; // Success
}
