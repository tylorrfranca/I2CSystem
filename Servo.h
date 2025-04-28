/*
 * Servo.h
 *
 *	Provides functions to interact with a standard angular Servo Motor
 *
 * Created on: June 11th, 2023
 *		Author: Jackie Huynh
 *
 */
 
 
#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include "util.h"

// USE HARDWARE PWM MODULE 0 GEN 0 CHANNEL 0 PB6

/* List of Fill In Macros */
#define EN_PWM0_GPIOB_CLOCK		(0x02)        // Enable GPIO Port B clock (bit 1)
#define PWM0_PIN							(0x40)        // PB6 for PWM0
#define CLEAR_ALT_FUNCTION		(0x0F000000)  // Clear bits 27-24 for PB6
#define PWM0_ALT_FUNCTION			(0x04000000)  // Set PWM alternate function for PB6
#define EN_PWM0_CLOCK					(0x01)        // Enable PWM0 Module
#define EN_USE_PWM_DIV				(0x00100000)  // Enable PWM divider
#define CLEAR_PWM_DIV					(0x000E0000)  // Clear PWM divider field
#define PWM0_DIV_VALUE				(0x00010000)  // Set PWM Divider to 2 (system clock/2)
#define PWM0_DEFAULT_CONFIG		(0x00)        // Count-down mode and auto-reload
#define PWM0_GEN_CONFIG				(0x0000008C)  // Low on LOAD, high on CMPA down match
#define PWM0_COUNTER					(160000)      // For 50Hz (20ms) with 16MHz/2 = 8MHz clock
#define PWM0_START						(0x01)        // Start PWM0 Gen 0
#define EN_PWM0_FUNCTION			(0x01)        // Enable PWM0 Output

/* Pulse Width Definitions - 8MHz clock (0.125μs per tick)
 * Center: 1.5ms = 12000 ticks
 * CW 45°: 1.0ms = 8000 ticks
 * CCW 45°: 2.0ms = 16000 ticks
 * CW 90°: 0.5ms = 4000 ticks
 * CCW 90°: 2.5ms = 20000 ticks
 */
#define SERVO_MIN_CNT					(4000)        // 0.5ms pulse (4000 out of 160000)
#define SERVO_MAX_CNT					(20000)       // 2.5ms pulse (20000 out of 160000)

/* Max Range of Either -90 to 90 or 0 to 180 */
#define SERVO_MIN_ANGLE				(-90)         // Minimum angle
#define SERVO_MAX_ANGLE				(90)          // Maximum angle

/*
 *	-------------------Servo_Init------------------
 *	Basic Servo Initialization function for PWM Generation
 *	Input: None
 *	Output: None
 */
void Servo_Init(void);

/*
 *	-------------------Drive_Servo------------------
 *	Drives the Servo Motor to a specified angle
 *	Input: Desired Angle
 *	Output: None
 */
void Drive_Servo(int16_t angle);

#endif