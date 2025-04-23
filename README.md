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

# TCS34727 RGB Color Sensor Implementation

This project implements the TCS34727 RGB color sensor for the TM4C123GH6PM microcontroller. The TCS34727 is a color light-to-digital converter with an IR filter that can detect red, green, blue, and clear light.

## Hardware Setup

1. Connect the TCS34727 sensor to the TM4C123GH6PM microcontroller:
   - VDD to 3.3V
   - GND to GND
   - SCL to PB2 (I2C0_SCL)
   - SDA to PB3 (I2C0_SDA)
   - INT to an interrupt pin (optional)
   - NC (No Connect)

2. The TCS34727 uses I2C communication with the address 0x29.

## Software Implementation

The implementation consists of the following files:

- `TCS34727.h`: Header file with function declarations and register definitions
- `TCS34727.c`: Implementation of the TCS34727 functions
- `I2C.h` and `I2C.c`: I2C communication functions
- `ModuleTest.c`: Test function for the TCS34727 module
- `main.c`: Main program to demonstrate the TCS34727 module

## Functions

### Initialization

```c
void TCS34727_Init(void);
```

This function initializes the TCS34727 sensor with default settings:
- Sets integration time to 2.4ms
- Sets gain to 1x
- Powers on the sensor
- Enables the RGBC ADC

### Raw Data Retrieval

```c
uint16_t TCS34727_GET_RAW_CLEAR(void);
uint16_t TCS34727_GET_RAW_RED(void);
uint16_t TCS34727_GET_RAW_GREEN(void);
uint16_t TCS34727_GET_RAW_BLUE(void);
```

These functions retrieve the raw color data from the sensor. Each function returns a 16-bit value.

### RGB Conversion

```c
void TCS34727_GET_RGB(RGB_COLOR_HANDLE_t* RGB_COLOR_Instance);
```

This function converts the raw color data to RGB values in the range 0-255.

### Color Detection

```c
COLOR_DETECTED Detect_Color(RGB_COLOR_HANDLE_t* RGB_COLOR_Instance);
```

This function detects the dominant color based on the RGB values and returns an enum value:
- RED_DETECT
- GREEN_DETECT
- BLUE_DETECT
- NOTHING_DETECT

## Usage Example

```c
/* Initialize I2C0 for communication with TCS34727 */
I2C0_Init();

/* Initialize TCS34727 */
TCS34727_Init();

/* Get raw color data */
RGB_COLOR.C_RAW = TCS34727_GET_RAW_CLEAR();
RGB_COLOR.R_RAW = TCS34727_GET_RAW_RED();
RGB_COLOR.G_RAW = TCS34727_GET_RAW_GREEN();
RGB_COLOR.B_RAW = TCS34727_GET_RAW_BLUE();

/* Convert raw data to RGB values */
TCS34727_GET_RGB(&RGB_COLOR);

/* Detect the dominant color */
COLOR_DETECTED detectedColor = Detect_Color(&RGB_COLOR);
```

## Testing

The project includes a test function that demonstrates the TCS34727 module:

```c
Module_Test(TCS34727_TEST);
```

This function continuously reads color data from the sensor and prints the raw values, RGB values, and detected color to the UART terminal.

## References

- [TCS34725 Datasheet](https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf)
- TM4C123GH6PM Microcontroller Datasheet 