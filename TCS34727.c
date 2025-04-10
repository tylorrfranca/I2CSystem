/*
 * TCS34727.c
 *
 *	Main implementation of the functions to interact with
 *	the TCS34727 RGB Color Sensor
 *
 * Created on: May 24th, 2023
 *		Author: Jackie Huynh
 *
 */

#include "TCS34727.h"
#include "I2C.h"
#include "UART0.h"
#include "util.h"
#include <stdio.h>
#include "tm4c123gh6pm.h"

/*	-------------------TCS34727_Init------------------
 *	Basic Initialization Function for TCS34727 at default settings
 *	Input: none
 *	Output: none
 */
void TCS34727_Init(void){
	uint8_t ret;																//Temp Variable to hold return values
	char printBuf[20];													//String buffer to print
	
	/* Check if RGB Color Sensor has been detected */
	ret = I2C0_Receive(TCS34727_ADDR, TCS34727_CMD|TCS34727_ID_R_ADDR);
	
	//Print ID or Error to Terminal
	sprintf(printBuf, "ID: %x\r\n", ret);
	UART0_OutString(printBuf);
	
	if(ret != TCS34727_ID){
		UART0_OutString("TCS34727 has not been Detected\r\n");
		return;
	}
	UART0_OutString("TCS34727 has been Detected\r\n");
	
	/* Set Integration Time to 2.4ms in timing register */
	ret = I2C0_Transmit(TCS34727_ADDR, TCS34727_CMD|TCS34727_TIMING_R_ADDR, TCS34727_ATIME_2_4_MS);
	if(ret != 0)
		UART0_OutString("Error on Transmit\r\n");
	else
		UART0_OutString("TCS34727 Integration Time Set\r\n");
	
	// Necessary Delay when setting integration time/wait time. 
	// This varies for which integration time is choosen.
	// This project chooses 2.4ms.
	DELAY_1MS(3);
	
	/* Setting Gain to 1X gain */
	ret = I2C0_Transmit(TCS34727_ADDR, TCS34727_CMD|TCS34727_CTRL_R_ADDR, TCS34727_CTRL_AGAIN_1);
	if(ret != 0)
		UART0_OutString("Error on Transmit\r\n");
	else
		UART0_OutString("TCS34727 Gain Set\r\n");
	
	/* Powering On Sensor at Enable register */
	ret = I2C0_Transmit(TCS34727_ADDR, TCS34727_CMD|TCS34727_ENABLE_R_ADDR, TCS34727_ENABLE_PON);
	if(ret != 0)
		UART0_OutString("Error on Transmit\r\n");
	else
		UART0_OutString("TCS34727 Power On\r\n");

	//Nessessary Delay When Powering On Module
	DELAY_1MS(3);
	
	/* Enabling RGBC 2-Channel ADC at Enable register */
	ret = I2C0_Transmit(TCS34727_ADDR, TCS34727_CMD|TCS34727_ENABLE_R_ADDR, TCS34727_ENABLE_PON |TCS34727_ENABLE_AEN);
	if(ret != 0)
		UART0_OutString("Error on Transmit\r\n");
	else
		UART0_OutString("TCS34727 RGBC On\r\n");
	
	//Integration Time Delay when Activating. Varies with Integration Time Choosen by User
	DELAY_1MS(3);
	
	UART0_OutString("TCS34727 Color Sensor Initialized\r\n");
	
}

/*	---------------TCS34727_GET_RAW_CLEAR-------------
 *	Receive RAW clear data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW clear data
 */
uint16_t TCS34727_GET_RAW_CLEAR(void){
	uint8_t CLEAR_LOW;
	uint8_t CLEAR_HIGH;
	uint16_t CLEAR_DATA;
	
	/* Use I2C to grab both HIGH and LOW data */
	CLEAR_LOW = I2C0_Receive(TCS34727_ADDR, TCS34727_CMD|TCS34727_CDATAL_R_ADDR);
	CLEAR_HIGH = I2C0_Receive(TCS34727_ADDR, TCS34727_CMD|TCS34727_CDATAH_R_ADDR);
	
	/* Concatanate into 16-bit value */
	//CODE_FILL: CLEAR_DATA=?
	
	//Integration Time Delay
	DELAY_1MS(3);
	
	return CLEAR_DATA;
}

/*	---------------TCS34727_GET_RAW_RED---------------
 *	Receive RAW red data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW red data
 */
uint16_t TCS34727_GET_RAW_RED(void){
	uint8_t RED_LOW;
	uint8_t RED_HIGH;
	uint16_t RED_DATA;
	
	/* Use I2C to grab both HIGH and LOW data */
	//CODE_FILL
	
	/* Concatanate into 16-bit value */
	//CODE_FILL
	
	//Integration Time Delay
	DELAY_1MS(3);
	
	return RED_DATA;
}

/*	---------------TCS34727_GET_RAW_GREEN-------------
 *	Receive RAW green data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW green data
 */
uint16_t TCS34727_GET_RAW_GREEN(void){
	uint8_t GREEN_LOW;
	uint8_t GREEN_HIGH;
	uint16_t GREEN_DATA;
	
	/* Use I2C to grab both HIGH and LOW data */
	//CODE_FILL
	
	/* Concatanate into 16-bit value */
	//CODE_FILL
	
	//Integration Time Delay
	DELAY_1MS(3);
	
	return GREEN_DATA;
}

/*	---------------TCS34727_GET_RAW_BLUE-------------
 *	Receive RAW blue data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW blue data
 */
uint16_t TCS34727_GET_RAW_BLUE(void){
	uint8_t BLUE_LOW;
	uint8_t BLUE_HIGH;
	uint16_t BLUE_DATA;
	
	/* Use I2C to grab both HIGH and LOW data */
	//CODE_FILL
	
	/* Concatanate into 16-bit value*/
	//CODE_FILL
	
	//Integration Time Delay
	DELAY_1MS(3);
	
	return BLUE_DATA;
}

/*	---------------TCS34727_GET_RGB------------------
 *	Normalize RAW data into RGB range (0-255)
 *	Input: RGB Color Struct User Instance
 *	Output: none
 */
void TCS34727_GET_RGB(RGB_COLOR_HANDLE_t* RGB_COLOR_Instance){
	
	/* Prevent Dividing by 0 by checking if the C_RAW value from struct is equal to 0 */
	if(CODE_FILL){
		RGB_COLOR_Instance->R = RGB_COLOR_Instance->G = RGB_COLOR_Instance->B = 0;
		return;
	}
	
	/*
	Divide all RGB value with their (RAW Value / (float)Clear Raw Value) and multiple everything with 255.0
	Store in RGB Color Instance Struct
	*/ 
	
	//CODE_FILL
	
}

/*	-----------------Detect_Color--------------------
 *	Detect which color is more prominant and returns that color
 *	Input: RGB Color User Instance Struct
 *	Output: COLOR_DETECTED enum value
 */
COLOR_DETECTED Detect_Color(RGB_COLOR_HANDLE_t* RGB_COLOR_Instance){
	
	/* Compare all values with eachother and return which color is prominent using enum type */
	if(CODE_FILL)
		return RED_DETECT;
	else if(CODE_FILL)
		return GREEN_DETECT;
	else if(CODE_FILL)
		return BLUE_DETECT;
	
	/* Otherwise no color is being detected */
	return NOTHING_DETECT;
}
