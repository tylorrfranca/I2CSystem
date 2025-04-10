/*
 * TCS34727.h
 *
 *	Provides functions to interact with TCS34727
 *	RGB color sensor
 *
 *	Datasheet Link: https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
 *
 * Created on: May 24th, 2023
 *		Author: Jackie Huynh
 *
 */
 
#ifndef TCS34727_H_
#define TCS34727_H_

#include <stdint.h>
#include "util.h"

/* List of Fill In Macros (Not all need to be filled)

IMPORTANT: BEFORE FILLING MACROS READ THE COMMENT BELOW

NOT ALL MACROS NEED TO BE FILLED OUT. REFERENCE TCS34727.c
TO FIGURE OUT WHAT'S NECESSARY (Filling it all out will not
incure any penelties but will be a waste of time)

Comment Out All Macros that are not used

*/

//Macros of TCS34727 device Address (Based on Datasheet)
#define TCS34727_ADDR							(CONSTANT_FILL)

/*************Command Register*************/
#define TCS34727_CMD							(CONSTANT_FILL)  // define the bit that indicates a command register

/*************Enable Registers*************/
#define TCS34727_ENABLE_R_ADDR		(CONSTANT_FILL)  // enable register address
	#define TCS34727_ENABLE_PON			(CONSTANT_FILL)
	#define TCS34727_ENABLE_AEN			(CONSTANT_FILL)
	#define TCS34727_ENABLE_WEN			(CONSTANT_FILL)
	#define TCS34727_ENABLE_AIEN		(CONSTANT_FILL)
		
/**********RGBC Timing Registers***********/
#define TCS34727_TIMING_R_ADDR				(CONSTANT_FILL)  // Define RGBC timing register address
	#define TCS34727_ATIME_2_4_MS				(CONSTANT_FILL)  // Set atime to 2.4ms

/************Control Registers*************/
#define TCS34727_CTRL_R_ADDR				(CONSTANT_FILL)  // Define control register address
	#define TCS34727_CTRL_AGAIN_1		(CONSTANT_FILL)
	
/**************ID Registers****************/
#define TCS34727_ID_R_ADDR			(CONSTANT_FILL)
	
/***********Color Data Register address definitions ***********/
#define TCS34727_CDATAL_R_ADDR 					(CONSTANT_FILL) 
#define TCS34727_CDATAH_R_ADDR 					(CONSTANT_FILL) 
#define TCS34727_RDATAL_R_ADDR 					(CONSTANT_FILL) 
#define TCS34727_RDATAH_R_ADDR 					(CONSTANT_FILL) 
#define TCS34727_GDATAL_R_ADDR 					(CONSTANT_FILL) 
#define TCS34727_GDATAH_R_ADDR 					(CONSTANT_FILL) 
#define TCS34727_BDATAL_R_ADDR 					(CONSTANT_FILL) 
#define TCS34727_BDATAH_R_ADDR 					(CONSTANT_FILL) 

/*************TCS34727 device ID Values**************/
#define TCS34727_ID			(CONSTANT_FILL)

/* Custom Return Type */
typedef enum{
	RED_DETECT 			= 0,
	GREEN_DETECT 		= 1,
	BLUE_DETECT 		= 2,
	NOTHING_DETECT	= 3
} COLOR_DETECTED;

/* Data Struct to store RGB color values */
typedef struct{
	uint16_t R_RAW;
	uint16_t G_RAW;
	uint16_t B_RAW;
	uint16_t C_RAW;
	
	float R;
	float G;
	float B;
} RGB_COLOR_HANDLE_t;

/*	-------------------TCS34727_Init------------------
 *	Basic Initialization Function for TCS34727 at default settings
 *	Input: none
 *	Output: none
 */
void TCS34727_Init(void);

/*	---------------TCS34727_GET_RAW_CLEAR-------------
 *	Receive RAW clear data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW clear data
 */
uint16_t TCS34727_GET_RAW_CLEAR(void);

/*	---------------TCS34727_GET_RAW_RED---------------
 *	Receive RAW red data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW red data
 */
uint16_t TCS34727_GET_RAW_RED(void);

/*	---------------TCS34727_GET_RAW_GREEN-------------
 *	Receive RAW green data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW green data
 */
uint16_t TCS34727_GET_RAW_GREEN(void);

/*	---------------TCS34727_GET_RAW_BLUE-------------
 *	Receive RAW blue data reading from the sensor
 *	Input: none
 *	Output: Returns 16-bit RAW blue data
 */
uint16_t TCS34727_GET_RAW_BLUE(void);

/*	---------------TCS34727_GET_RGB------------------
 *	Normalize RAW data into RGB range (0-255)
 *	Input: RGB Color User Instance Struct
 *	Output: none
 */
void TCS34727_GET_RGB(RGB_COLOR_HANDLE_t* RGB_COLOR_Instance);

/*	-----------------Detect_Color--------------------
 *	Detect which color is more prominant and returns that color
 *	Input: RGB Color User Instance Struct
 *	Output: COLOR_DETECTED enum value
 */
COLOR_DETECTED Detect_Color(RGB_COLOR_HANDLE_t* RGB_COLOR_Instance);

#endif
