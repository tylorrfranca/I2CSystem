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
#define EN_PWM0_GPIOB_CLOCK		(CONSTANT_FILL)
#define PWM0_PIN							(CONSTANT_FILL)
#define CLEAR_ALT_FUNCTION		(CONSTANT_FILL)
#define PWM0_ALT_FUNCTION			(CONSTANT_FILL)
#define EN_PWM0_CLOCK					(CONSTANT_FILL)
#define EN_USE_PWM_DIV				(CONSTANT_FILL)
#define CLEAR_PWM_DIV					(CONSTANT_FILL)
#define PWM0_DIV_VALUE				(CONSTANT_FILL)
#define PWM0_DEFAULT_CONFIG		(CONSTANT_FILL)
#define PWM0_GEN_CONFIG				(CONSTANT_FILL)
#define PWM0_COUNTER					(CONSTANT_FILL)
#define PWM0_START						(CONSTANT_FILL)
#define EN_PWM0_FUNCTION			(CONSTANT_FILL)

/* 0.5ms (2.5%) - 2.5ms (12.5%) Duty Cycle */
#define SERVO_MIN_CNT					(CONSTANT_FILL)					
#define SERVO_MAX_CNT					(CONSTANT_FILL)					

/* Max Range of Either -90 to 90 or 0 to 180 */
#define SERVO_MIN_ANGLE				(CONSTANT_FILL)	
#define SERVO_MAX_ANGLE				(CONSTANT_FILL)

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