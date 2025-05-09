/*
 * LCD.c
 *
 *	Main Implementation of LCD functions such as initialization, sending
 *	commands and data, and basic functionalities
 *
 * Created on: July 26th, 2023
 *		Author: Jackie Huynh
 *
 */
 
#include "LCD.h"
#include "tm4c123gh6pm.h"
#include "util.h"
#include "I2C.h"

/*
 *	-------------------LCD_Send_CMD------------------
 *	Local LCD send commands function
 *	Input: Command to send
 *	Output: None
 */
static void LCD_Send_CMD(uint8_t cmd){
	
	/* Temp Variables to hold upper and lower value */
	uint8_t cmd_upper, cmd_lower;
	uint8_t cmd_array[4];								//Command Array to Burst Transmit
	
	/* Seperate Upper and Lower Nibble */
	cmd_upper = (cmd & UPPER_NIBBLE_MSK);
	cmd_lower = ((cmd << NIBBLE_SHIFT) & UPPER_NIBBLE_MSK);
	
	/* LCD I2C Message Pattern */
	cmd_array[0] = cmd_upper | (BACKLIGHT|EN_Pin);  // Send upper nibble with EN high
	DELAY_1MS(1);                                    // Short delay
	cmd_array[1] = cmd_upper | BACKLIGHT;           // Send upper nibble with EN low
	DELAY_1MS(1);                                    // Short delay
	cmd_array[2] = cmd_lower | (BACKLIGHT|EN_Pin);  // Send lower nibble with EN high
	DELAY_1MS(1);                                    // Short delay
	cmd_array[3] = cmd_lower | BACKLIGHT;           // Send lower nibble with EN low
	
	/* I2C Burst Transmit Command Array to LCD */
	I2C0_Burst_Transmit(LCD_WRITE_ADDR, PCF8574A_REG, cmd_array, sizeof(cmd_array));
	DELAY_1MS(2);                                    // Delay after command
}

/*
 *	------------------LCD_Send_Data------------------
 *	Local LCD send data function
 *	Input: Data to send
 *	Output: None
 */
static void LCD_Send_Data(uint8_t data){
	
	/* Temp Variables to hold upper and lower value */
	uint8_t data_upper, data_lower;
	uint8_t data_array[4];							//Data Array to Burst Transmit
	
	/* Seperate Upper and Lower Nibble */
	data_upper = (data & UPPER_NIBBLE_MSK);
	data_lower = ((data << NIBBLE_SHIFT) & UPPER_NIBBLE_MSK);
	
	/* LCD I2C Message Pattern */
	data_array[0] = data_upper | (BACKLIGHT|EN_Pin|RS_Pin);  // Send upper nibble with EN high
	DELAY_1MS(1);                                             // Short delay
	data_array[1] = data_upper | (BACKLIGHT|RS_Pin);         // Send upper nibble with EN low
	DELAY_1MS(1);                                             // Short delay
	data_array[2] = data_lower | (BACKLIGHT|EN_Pin|RS_Pin);  // Send lower nibble with EN high
	DELAY_1MS(1);                                             // Short delay
	data_array[3] = data_lower | (BACKLIGHT|RS_Pin);         // Send lower nibble with EN low
	
	/* I2C Burst Transmit Data Array to LCD */
	I2C0_Burst_Transmit(LCD_WRITE_ADDR, PCF8574A_REG, data_array, sizeof(data_array));
	DELAY_1MS(2);                                             // Delay after data
}

/*
 *	-------------------LCD_Init------------------
 *	Basic LCD Initialization Function
 *	Input: None
 *	Output: None
 */
void LCD_Init(void){
	
	/* Power up delay */
	DELAY_1MS(100);  // Increased initial delay for power stabilization
	
	/* Magic LCD Initialization Sequence */
	LCD_Send_CMD(0x30);  // First initialization command
	DELAY_1MS(10);
	LCD_Send_CMD(0x30);  // Second initialization command
	DELAY_1MS(10);
	LCD_Send_CMD(0x30);  // Third initialization command
	DELAY_1MS(10);
	
	/* Set to 4-bit mode */
	LCD_Send_CMD(0x20);  // Function set: 4-bit mode
	DELAY_1MS(10);
	
	/* Configure display settings */
	LCD_Send_CMD(0x28);  // Function set: 4-bit, 2-line, 5x8 dots
	DELAY_1MS(10);
	
	/* Display control */
	LCD_Send_CMD(0x0C);  // Display on, cursor off, blink off
	DELAY_1MS(10);
	
	/* Clear display */
	LCD_Send_CMD(0x01);  // Clear display
	DELAY_1MS(20);       // Increased delay for clear command
	
	/* Entry mode set */
	LCD_Send_CMD(0x06);  // Increment cursor, no display shift
	DELAY_1MS(10);
	
	/* Turn on display with cursor */
	LCD_Send_CMD(0x0E);  // Display on, cursor on, blink off
	DELAY_1MS(10);
	
	/* Clear display */
	LCD_Send_CMD(0x01);  // Clear display
	DELAY_1MS(20);       // Increased delay for clear command
}

/*
 *	-------------------LCD_Clear------------------
 *	Clear the LCD Display by passing a command
 *	Input: None
 *	Output: None
 */
void LCD_Clear(void){
	LCD_Send_CMD(CLEAR_DISP_CMD);
	DELAY_1MS(1);
}

/*
 *	----------------LCD_Set_Cursor----------------
 *	Set Cursor to Desire Place
 *	Input: Desired Row and Column to place Cursor
 *	Output: None
 */
void LCD_Set_Cursor(uint8_t row, uint8_t col){
	
	/* Switch case based of row choice */
	switch(row){
		case ROW1:
			col |= FIRST_ROW_CMD;
			break;
		case ROW2:
			col |= SECOND_ROW_CMD;
			break;
		default:
			col |= FIRST_ROW_CMD;
			break;
	}
	
	/* Send Command to set Row and Column */
	LCD_Send_CMD(col);
	DELAY_1MS(2);
	
}

/*
 *	---------------LCD_Reset_Cursor---------------
 *	Reset Cursor back to Row 1 and Column 0
 *	Input: None
 *	Output: None
 */
void LCD_Reset_Cursor(void){
	LCD_Send_CMD(RETURN_HOME_CMD);
	DELAY_1MS(1);
}

/*
 *	----------------LCD_Print_Char----------------
 *	Prints a Character to LCD
 *	Input: Character Hex Value
 *	Output: None
 */
void LCD_Print_Char(uint8_t data){
	LCD_Send_Data(data);
	DELAY_1MS(1);
}

/*
 *	----------------LCD_Print_Str-----------------
 *	Prints a string to LCD
 *	Input: Pointer to Character Array
 *	Output: None
 */
void LCD_Print_Str(uint8_t* str){
	while(*str){
		LCD_Send_Data(*str++);
		DELAY_1MS(2);
	}
}
