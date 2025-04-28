/*
 * main.c
 *
 * Main program to demonstrate MPU6050 Angle Reading
 *
 * Created on: September 3rd, 2023
 *      Author: Claude AI / Modified by Gemini
 *
 */

//#include "TCS34727.h" // No longer using TCS34727 in this main
#include "MPU6050.h"    // Include MPU6050 driver
#include "I2C.h"
#include "UART0.h"
#include "util.h"
//#include "ModuleTest.h" // Not running ModuleTest
#include <stdio.h>     // For sprintf
#include <math.h>      // For atan2f, sqrtf (if MPU6050_Get_Angle uses them)

// Declare MPU6050 data structures globally or locally if preferred
MPU6050_ACCEL_t Accel_Instance;
MPU6050_GYRO_t  Gyro_Instance;
MPU6050_ANGLE_t Angle_Instance;

char printBuffer[100]; // Buffer for sprintf

int main(void){
    /* Initialize UART0 for communication */
    UART0_Init();
    UART0_OutString("MPU6050 Angle Reading Demo\r\n");
    
    /* Initialize I2C0 for communication with MPU6050 */
    I2C0_Init();
    
    /* Initialize MPU6050 */
    MPU6050_Init(); // Initialize the MPU6050 sensor
    
    /* Run the MPU6050 Reading Loop */
    // Module_Test(TCS34727_TEST); // Commented out TCS test
    
    while(1){
        // 1. Get Raw Sensor Data
        MPU6050_Get_Accel(&Accel_Instance);
        MPU6050_Get_Gyro(&Gyro_Instance); 
        
        // Optional: Process raw data into physical units (g's, deg/s)
        MPU6050_Process_Accel(&Accel_Instance);
        // MPU6050_Process_Gyro(&Gyro_Instance);

        // 2. Calculate Angles (using accelerometer data in this implementation)
        MPU6050_Get_Angle(&Accel_Instance, &Gyro_Instance, &Angle_Instance);
        
        // 3. Format and Print Angles via UART
        //    Using %f requires floating point support in printf/sprintf
        //    Ensure your linker settings/compiler options enable this.
        //    Also print Ay/Az inputs to atan2 for roll
        //    Also print Az_RAW to see the raw Z value
        sprintf(printBuffer, "Ay:%.2f Az:%.2f AzR:%d Roll:%.2f Pitch:%.2f\r\n", 
                Accel_Instance.Ay, Accel_Instance.Az, // Use processed Ay/Az 
                Accel_Instance.Az_RAW,               // Print Raw Z value
                Angle_Instance.ArX, Angle_Instance.ArY);
        UART0_OutString(printBuffer);
        
        // 4. Delay 
        DELAY_1MS(100); // Delay 100 ms (10 Hz update rate)
    }
    
    // This point should never be reached in this setup
    // return 0; // Typically unreachable in embedded while(1)
} 