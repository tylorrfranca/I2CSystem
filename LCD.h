/*
 * LCD.h
 *
 *	Provides LCD functions such as initialization, sending
 *	commands and data, and basic functionalities
 *
 * Created on: July 26th, 2023
 *		Author: Jackie Huynh
 *
 */
 
#ifndef LCD_H_
#define LCD_H_
#include "util.h"

/*************PCF8574A Register*************/
#define LCD_WRITE_ADDR			(0x3FU)
#define PCF8574A_REG				(0x00U)

/**************LCD CMD Register*************/
#define INIT_REG_CMD				(0x30U)
#define INIT_FUNC_CMD				(0x20U)

#define FUNC_MODE						(CONSTANT_FILL)
	#define FUNC_4_BIT				(CONSTANT_FILL)
	#define FUNC_2_ROW				(CONSTANT_FILL)
	#define FUNC_5_7					(CONSTANT_FILL)
	
#define DISP_CMD						(CONSTANT_FILL)
	#define DISP_ON						(CONSTANT_FILL)
	#define DISP_OFF					(CONSTANT_FILL)
	#define DISP_CURSOR_ON		(CONSTANT_FILL)
	#define DISP_CURSOR_OFF		(CONSTANT_FILL)
	#define DISP_BLINK_ON			(CONSTANT_FILL)
	#define DISP_BLINK_OFF		(CONSTANT_FILL)
	
#define CLEAR_DISP_CMD			(CONSTANT_FILL)

#define ENTRY_MODE_CMD			(CONSTANT_FILL)
	#define ENTRY_INC_CURSOR	(CONSTANT_FILL)
//	#define ENTRY_DISP_SHIFT	()
	
#define RETURN_HOME_CMD			(CONSTANT_FILL)

#define FIRST_ROW_CMD				(CONSTANT_FILL)
#define SECOND_ROW_CMD			(CONSTANT_FILL)

/* LCD Module Macros */
#define RS_Pin							(CONSTANT_FILL)
#define RW_Pin							(CONSTANT_FILL)
#define EN_Pin							(CONSTANT_FILL)
#define BACKLIGHT						(CONSTANT_FILL)

/* General Macros */
#define UPPER_NIBBLE_MSK		(CONSTANT_FILL)
#define NIBBLE_SHIFT				(CONSTANT_FILL)
#define ROW1								(0U)
#define ROW2								(1U)
#define LCD_ROW_SIZE				(CONSTANT_FILL)

#include <stdint.h>

/*
 *	-------------------LCD_Init------------------
 *	Basic LCD Initialization Function
 *	Input: None
 *	Output: None
 */
void LCD_Init(void);

/*
 *	-------------------LCD_Clear------------------
 *	Clear the LCD Display by passing a command
 *	Input: None
 *	Output: None
 */
void LCD_Clear(void);

/*
 *	----------------LCD_Set_Cursor----------------
 *	Set Cursor to Desire Place
 *	Input: Desired Row and Column to place Cursor
 *	Output: None
 */
void LCD_Set_Cursor(uint8_t row, uint8_t col);

/*
 *	---------------LCD_Reset_Cursor---------------
 *	Reset Cursor back to Row 1 and Column 0
 *	Input: None
 *	Output: None
 */
void LCD_Reset_Cursor(void);

/*
 *	----------------LCD_Print_Char----------------
 *	Prints a Character to LCD
 *	Input: Character Hex Value
 *	Output: None
 */
void LCD_Print_Char(uint8_t data);

/*
 *	----------------LCD_Print_Str-----------------
 *	Prints a string to LCD
 *	Input: Pointer to Character Array
 *	Output: None
 */
void LCD_Print_Str(uint8_t* str);

#endif