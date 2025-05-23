/*
 * ModuleTest.c
 *
 *	Provides the testing functions all of individual peripheral testing
 *	and full system testing
 *
 * Created on: September 3rd, 2023
 *		Author: Jackie Huynh
 *
 */
 
#include "ModuleTest.h"
#include "TCS34727.h"
#include "MPU6050.h"
#include "UART0.h"
#include "Servo.h"
#include "LCD.h"
#include "I2C.h"
#include "util.h"
#include "ButtonLED.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static char printBuf[100];
static char angleBuf[LCD_ROW_SIZE];
static char colorBuf[LCD_ROW_SIZE];
static char colorString[6];

/* Test Mode Variables */
uint8_t current_led = RED;    // Start with red LED
uint8_t current_test = 1;     // 1 for UART test, 2 for WTIMER0 test

/* RGB Color Struct Instance */
RGB_COLOR_HANDLE_t RGB_COLOR;
	
/* MPU6050 Struct Instance */
MPU6050_ACCEL_t Accel_Instance;
MPU6050_GYRO_t 	Gyro_Instance;
MPU6050_ANGLE_t Angle_Instance;

static void Test_Delay(void){
	static uint8_t led_state = 1;  // Track LED state (1 = on, 0 = off)
	
	if(led_state){
		LEDs = current_led;  // Turn on current LED
	} else {
		LEDs = DARK;         // Turn off LED
	}
	led_state ^= 1;         // Toggle state
	DELAY_1MS(125);         // 250ms delay for 2Hz frequency
}

static void Test_UART(void){
	char testString[100];
	float test_float = 45.67;
	static int counter = 0;
	
	// Create test string with incrementing counter and float
	sprintf(testString, "UART Test - Count: %d, Float: %.2f\r\n", counter++, test_float);
	UART0_OutString(testString);
	DELAY_1MS(1000);  // Update every 1 second
}

static void Test_I2C(void){
	uint8_t ret = I2C0_Receive(TCS34727_ADDR, TCS34727_CMD|TCS34727_ID_R_ADDR);
	sprintf(printBuf, "TCS34727 ID: 0x%02X\r\n", ret);
	UART0_OutString(printBuf);
	DELAY_1MS(1000);
}

static void Test_MPU6050(void){
	/* Grab Accelerometer and Gyroscope Raw Data*/
	MPU6050_Get_Accel(&Accel_Instance);
	MPU6050_Get_Gyro(&Gyro_Instance);
		
	/* Process Raw Accelerometer and Gyroscope Data */
	MPU6050_Process_Accel(&Accel_Instance);
	MPU6050_Process_Gyro(&Gyro_Instance);
		
	/* Calculate Tilt Angle */
	MPU6050_Get_Angle(&Accel_Instance, &Gyro_Instance, &Angle_Instance);
		
	/* Format buffer to print data and angle */
	sprintf(printBuf, "Accel: X=%.2f Y=%.2f Z=%.2f Angle: %.2f\r\n", 
		Angle_Instance.ArX, Angle_Instance.ArY, Angle_Instance.ArZ, Angle_Instance.ArY);
	UART0_OutString(printBuf);
	
	DELAY_1MS(10);
}

static void Test_TCS34727(void){
	/* Main test loop */
	while(1){
		/* Get raw color data */
		RGB_COLOR.C_RAW = TCS34727_GET_RAW_CLEAR();
		RGB_COLOR.R_RAW = TCS34727_GET_RAW_RED();
		RGB_COLOR.G_RAW = TCS34727_GET_RAW_GREEN();
		RGB_COLOR.B_RAW = TCS34727_GET_RAW_BLUE();
		
		/* Convert raw data to RGB values */
		TCS34727_GET_RGB(&RGB_COLOR);
		
		/* Detect the dominant color */
		COLOR_DETECTED detectedColor = Detect_Color(&RGB_COLOR);
		
		/* Set LED color based on detected color */
		switch(detectedColor){
			case RED_DETECT:
				LEDs = RED;
				break;
			case GREEN_DETECT:
				LEDs = GREEN;
				break;
			case BLUE_DETECT:
				LEDs = BLUE;
				break;
			default:
				LEDs = DARK;  // Turn off LEDs if no color detected
				break;
		}
		
		/* Print raw values */
		sprintf(printBuf, "Raw Values - Clear: %d, Red: %d, Green: %d, Blue: %d\r\n",
				RGB_COLOR.C_RAW, RGB_COLOR.R_RAW, RGB_COLOR.G_RAW, RGB_COLOR.B_RAW);
		UART0_OutString(printBuf);
		
		/* Print RGB values */
		sprintf(printBuf, "RGB Values - R: %.2f, G: %.2f, B: %.2f\r\n",
				RGB_COLOR.R, RGB_COLOR.G, RGB_COLOR.B);
		UART0_OutString(printBuf);
		
		/* Print detected color */
		UART0_OutString("Detected Color: ");
		switch(detectedColor){
			case RED_DETECT:
				UART0_OutString("RED\r\n");
				break;
			case GREEN_DETECT:
				UART0_OutString("GREEN\r\n");
				break;
			case BLUE_DETECT:
				UART0_OutString("BLUE\r\n");
				break;
			default:
				UART0_OutString("NO COLOR DETECTED\r\n");
				break;
		}
		
		/* Add a delay between readings */
		DELAY_1MS(1000);
	}
}

static void Test_Servo(void){
	Drive_Servo(0);
	DELAY_1MS(1000);
	
	Drive_Servo(-45);
	DELAY_1MS(1000);
	
	Drive_Servo(0);
	DELAY_1MS(1000);
	
	Drive_Servo(45);
	DELAY_1MS(1000);
	
	Drive_Servo(0);
	DELAY_1MS(1000);
	
	Drive_Servo(-90);
	DELAY_1MS(1000);
	
	Drive_Servo(0);
	DELAY_1MS(1000);
	
	Drive_Servo(90);
	DELAY_1MS(1000);
}

static void Test_LCD(void){
	LCD_Clear();
	DELAY_1MS(2);
	LCD_Set_Cursor(0, 4);
	LCD_Print_Str((uint8_t*)"CECS 447");
	DELAY_1MS(2);
	LCD_Set_Cursor(1, 4);
	LCD_Print_Str((uint8_t*)"I2C Project");
}

static void Test_Full_System(void){
	/* Grab Accelerometer and Gyroscope Raw Data*/
	MPU6050_Get_Accel(&Accel_Instance);
	MPU6050_Get_Gyro(&Gyro_Instance);
		
	/* Process Raw Accelerometer and Gyroscope Data */
	MPU6050_Process_Accel(&Accel_Instance);
	MPU6050_Process_Gyro(&Gyro_Instance);
		
	/* Calculate Tilt Angle */
	MPU6050_Get_Angle(&Accel_Instance, &Gyro_Instance, &Angle_Instance);
		
	/* Drive Servo Accordingly to Tilt Angle on X-Axis*/
	Drive_Servo(Angle_Instance.ArX);
		
	/* Format buffer to print MPU6050 data and angle */
	sprintf(printBuf, "Accel: X=%.2f Y=%.2f Z=%.2f Angle: %.2f\r\n", 
		Accel_Instance.Ax, Accel_Instance.Ay, Accel_Instance.Az, Angle_Instance.ArX);
	UART0_OutString(printBuf);
		
	/* Grab Raw Color Data From Sensor and Process it */
	RGB_COLOR.C_RAW = TCS34727_GET_RAW_CLEAR();
	RGB_COLOR.R_RAW = TCS34727_GET_RAW_RED();
	RGB_COLOR.G_RAW = TCS34727_GET_RAW_GREEN();
	RGB_COLOR.B_RAW = TCS34727_GET_RAW_BLUE();
	TCS34727_GET_RGB(&RGB_COLOR);
		
	/* Change Onboard RGB LED Color to Detected Color */
	switch(Detect_Color(&RGB_COLOR)){
		case RED_DETECT:
			LEDs = RED;
			strcpy(colorString, "RED");
			break;
		case GREEN_DETECT:
			LEDs = GREEN;
			strcpy(colorString, "GREEN");
			break;
		case BLUE_DETECT:
			LEDs = BLUE;
			strcpy(colorString, "BLUE");
			break;
		case NOTHING_DETECT:
			LEDs = DARK;
			strcpy(colorString, "NA");
			break;
	}
		
	/* Format String to Print RGB value*/
	sprintf(printBuf, "R=%.0f G=%.0f B=%.0f Color: %s\r\n", 
		RGB_COLOR.R, RGB_COLOR.G, RGB_COLOR.B, colorString);
		
	/* Print String to Terminal through USB */
	UART0_OutString(printBuf);
		
	/* Update LCD With Current Angle and Color Detected */
	sprintf(angleBuf, "Angle:%0.2f", Angle_Instance.ArX);
	sprintf(colorBuf, "Color:%s", colorString);
	
	LCD_Clear();
	DELAY_1MS(2);
	LCD_Set_Cursor(0, 0);
	LCD_Print_Str((uint8_t*)angleBuf);
	DELAY_1MS(2);
	LCD_Set_Cursor(1, 1);
	LCD_Print_Str((uint8_t*)colorBuf);
		
	DELAY_1MS(20);
}

// SW1 interrupt handler - Toggle between UART and WTIMER0 test
void GPIOPortF_Handler(void){
	if(SW1_FLAG){  // SW1 was pressed
		current_test = (current_test == 1) ? 2 : 1;  // Toggle between 1 and 2
		LEDs = DARK;  // Turn off all LEDs when switching modes
		PORTF_FLAGS = SW1_PIN;  // Clear SW1 interrupt flag
	}
	else if(SW2_FLAG && current_test == 2){  // SW2 was pressed during WTIMER0 test
		// Simple rotation: RED -> GREEN -> BLUE -> RED
		if(current_led == RED){          // If RED (0x02)
			current_led = GREEN;         // Set to GREEN (0x08)
		}
		else if(current_led == GREEN){   // If GREEN (0x08)
			current_led = BLUE;          // Set to BLUE (0x04)
		}
		else {                          // If BLUE or any other value
			current_led = RED;          // Set to RED (0x02)
		}
		PORTF_FLAGS = SW2_PIN;  // Clear SW2 interrupt flag
	}
}

void Module_Test(MODULE_TEST_NAME test){
	switch(test){
		case DELAY_TEST:
			if(current_test == 2){  // WTIMER0 test mode
				Test_Delay();
			}
			break;
		
		case UART_TEST:
			if(current_test == 1){  // UART test mode
				Test_UART();
			}
			break;
		
		case I2C_TEST:
			Test_I2C();
			break;
		
		case MPU6050_TEST:
			Test_MPU6050();
			break;
		
		case TCS34727_TEST:
			Test_TCS34727();
			break;
		
		case SERVO_TEST:
			Test_Servo();
			break;
		
		case LCD_TEST:
			Test_LCD();
			break;
			
		case FULL_SYSTEM_TEST:
			Test_Full_System();
			break;
		
		default:
			break;
	}
}

 