# I2C System for TM4C123 Launchpad

This project implements I2C communication on a TIVA TM4C123 Launchpad to interact with various peripherals.

## Hardware Connections

### I2C0

*   **SCL (Serial Clock):** `PB2`
*   **SDA (Serial Data):** `PB3`
*   **Pull-up Resistors:** 4.7kΩ resistors are required on both SCL and SDA lines, pulled up to 3.3V.

### Peripherals (Based on Project Description)

*   **TCS34725 RGB Color Sensor:** Connects to I2C0 (SCL, SDA).
*   **MPU6050 IMU:** Connects to I2C0 (SCL, SDA).
*   **16x2 LCD with I2C interface:** Connects to I2C0 (SCL, SDA).
*   **Angular Servo Motor:** Controlled via Hardware PWM (M0PWM0 - specific pin not detailed here).
*   **UART0:** Used for PC communication (Default pins are usually PA0/RX, PA1/TX).

## Project Structure

*(Add details about project files and organization here)*

## Building and Running

*(Add instructions on how to build and run the project here)* 