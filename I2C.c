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
	I2C0_MCR_R = EN_I2C0_MASTER;            // Configure I2C0 as Master 
	
	/* Configuring I2C Clock Frequency to 100KHz */
	I2C0_MTPR_R = I2C_MTPR_TPR_VALUE;           // TPR = 19 for 100kHz at 40MHz system clock
	
	// Enable I2C0
	I2C0_MCR_R |= EN_I2C0_CLOCK;           // Enable I2C0 master
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
	while(I2C0_MCS_R & 0x01);
	
	/* Configure I2C0 Slave Address and Read Mode */
	I2C0_MSA_R = (slave_addr << 1);     // Slave Address is the 7 MSB
	I2C0_MDR_R = slave_reg_addr;        // Set Data Register to slave register address
	
	/* Initiate I2C by generating a START & RUN cmd */
	I2C0_MCS_R = 0x07;  // START + RUN
	
	/* Wait until write is done */
	while(I2C0_MCS_R & 0x01);
	
	/* Check for errors after first transaction */
	if(I2C0_MCS_R & 0x02) {
		I2C0_MCS_R = 0x04;  // Generate STOP condition
		return 0xFF;  // Return error code
	}
	
	/* Set I2C to Receive with Slave Address and change to Read */
	I2C0_MSA_R = (slave_addr << 1) | 0x01;  // Set read bit
	
	/* Initiate I2C by generating a repeated START, STOP, & RUN cmd */
	I2C0_MCS_R = 0x0B;  // START + STOP + RUN
	
	/* Wait until I2C bus is not busy */
	while(I2C0_MCS_R & 0x01);
	
	/* Check for any error */
	error = I2C0_MCS_R & 0x02;
	if(error != 0) {
		I2C0_MCS_R = 0x04;  // Generate STOP condition
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
	while(I2C0_MCS_R & 0x01);
	
	/* Configure I2C Slave Address, R/W Mode, and what to transmit */
	I2C0_MSA_R = (slave_addr << 1);  // Slave Address is the first 7 MSB
	I2C0_MSA_R &= ~I2C0_RW_PIN;      // Clear LSB to write
	I2C0_MDR_R = slave_reg_addr;      // Transmit register addr to interact
	
	/* Initiate I2C by generate a START bit and RUN cmd */
	I2C0_MCS_R = 0x07;  // START + RUN
	
	/* Wait until write has been completed */
	while(I2C0_MCS_R & 0x01);
	
	/* Update Data Register with data to be transmitted */
	I2C0_MDR_R = data;
	
	/* Initiate I2C by generating a STOP & RUN cmd */
	I2C0_MCS_R = 0x05;  // STOP + RUN
	
	/* Wait until write has been completed */
	while(I2C0_MCS_R & 0x01);
	
	/* Wait until bus isn't busy */
	while(I2C0_MCS_R & 0x01);
	
	/* Check for any error */
	error = I2C0_MCS_R & 0x02;
	if(error != 0)
		return error;
	else
		return 0;
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
	
	/* Check if I2C0 is busy */
	while(I2C0_MCS_R & 0x01);
	
	/* Configure I2C0 Slave Address and Read Mode */
	I2C0_MSA_R = (slave_addr << 1);  // Slave Address is the 7 MSB
	I2C0_MDR_R = slave_reg_addr;     // Set Data Register to slave register address
	
	/* Initiate I2C by generating a START & RUN cmd */
	I2C0_MCS_R = 0x07;  // START + RUN
	
	/* Wait until write is done */
	while(I2C0_MCS_R & 0x01);
	
	/* Set I2C to Receive with Slave Address and change to Read */
	I2C0_MSA_R = (slave_addr << 1) | 0x01;  // Set read bit
	
	/* Loop to receive multiple bytes */
	while(size > 1){
		/* Initiate I2C by generating a RUN cmd */
		I2C0_MCS_R = 0x01;  // RUN
		
		/* Wait until read is done */
		while(I2C0_MCS_R & 0x01);
		
		/* Store received data */
		*data++ = I2C0_MDR_R & 0xFF;
		size--;
	}
	
	/* Last byte with STOP condition */
	I2C0_MCS_R = 0x05;  // STOP + RUN
	
	/* Wait until read is done */
	while(I2C0_MCS_R & 0x01);
	
	/* Store last received data */
	*data = I2C0_MDR_R & 0xFF;
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
		return 0;
	
	/* Check if I2C0 is busy */
	while(I2C0_MCS_R & 0x01);
	
	/* Configure I2C Slave Address, R/W Mode, and what to transmit */
	I2C0_MSA_R = (slave_addr << 1);  // Slave Address is the first 7 MSB
	I2C0_MSA_R &= ~I2C0_RW_PIN;      // Clear LSB to write
	I2C0_MDR_R = slave_reg_addr;      // Transmit register addr to interact
	
	/* Initiate I2C by generate a START bit and RUN cmd */
	I2C0_MCS_R = 0x07;  // START + RUN
	
	/* Wait until write has been completed */
	while(I2C0_MCS_R & 0x01);
	
	/* Loop to Burst Transmit what is stored in data buffer */
	while(size > 1){
		I2C0_MDR_R = *data++;  // Load data and increment pointer
		I2C0_MCS_R = 0x01;     // RUN
		while(I2C0_MCS_R & 0x01);  // Wait until transmit is complete
		size--;
	}
	
	/* Last byte with STOP condition */
	I2C0_MDR_R = *data;  // Load last data byte
	I2C0_MCS_R = 0x05;   // STOP + RUN
	
	/* Wait until write has been completed */
	while(I2C0_MCS_R & 0x01);
	
	/* Wait until bus isn't busy */
	while(I2C0_MCS_R & 0x01);
	
	/* Check for any error */
	error = I2C0_MCS_R & 0x02;
	if(error != 0)
		return error;
	else
		return 0;
}
