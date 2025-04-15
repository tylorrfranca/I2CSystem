/*
 * ModuleTest.h
 *
 *	Provides the testing functions all of individual peripheral testing
 *	and full system testing
 *
 * Created on: September 3rd, 2023
 *		Author: Jackie Huynh
 *
 */
 
#ifndef MODULETEST_H_
#define MODULETEST_H_

#include <stdint.h>
#include "ButtonLED.h"

/* Test Mode Variables */
extern uint8_t current_led;    // Current LED color
extern uint8_t current_test;   // Current test mode (1 for UART, 2 for WTIMER0)

typedef enum{
	DELAY_TEST,
	UART_TEST,
	I2C_TEST,
	MPU6050_TEST,
	TCS34727_TEST,
	SERVO_TEST,
	LCD_TEST,
	FULL_SYSTEM_TEST
} MODULE_TEST_NAME;
 
void Module_Test(MODULE_TEST_NAME test);

#endif