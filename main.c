/*
 * main.c
 *
 * Main program to demonstrate the TCS34727 RGB Color Sensor
 *
 * Created on: September 3rd, 2023
 *      Author: Claude AI
 *
 */

#include "TCS34727.h"
#include "I2C.h"
#include "UART0.h"
#include "util.h"
#include "ModuleTest.h"
#include <stdio.h>

int main(void){
    /* Initialize UART0 for communication */
    UART0_Init();
    UART0_OutString("TCS34727 RGB Color Sensor Demo\r\n");
    
    /* Initialize I2C0 for communication with TCS34727 */
    I2C0_Init();
    
    /* Initialize TCS34727 */
    TCS34727_Init();
    
    /* Run the TCS34727 test */
    Module_Test(TCS34727_TEST);
    
    /* This point should never be reached */
    while(1);
} 