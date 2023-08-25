## Description

This application demonstrates I2C communication between the MAX78000FTHR Application Platform and an ADXL359 Digital MEMS Accelerometer.  The application first configures the I2C peripheral instance, configures the ADXL359, and reads the data from the sensor.

## Software

### Project Usage

Universal instructions on building, flashing, and debugging this project can be found in the **[MSDK User Guide](https://analog-devices-msdk.github.io/msdk/USERGUIDE/)**.

### Project-Specific Build Notes

* This project comes pre-configured for the MAX78000EVKIT.  See [Board Support Packages](https://analog-devices-msdk.github.io/msdk/USERGUIDE/#board-support-packages) in the UG for instructions on changing the target board.

## Setup

### Prepare Hardware:

-   Connect ADXL359 I2C pins to the header pins for I2C1 labeled "SDA" and "SCL" respectively.  
-   Supply 3V power supply to the board.  
-   Connect SCLK pin to ground to disable SPI and enable I2C.  
-   Connect ASEL pin to ground to get the I2C device address as 0x1D  
Note:- This requires five pin connections with the board.

### Required Connections:

-   Connect a USB cable between the PC and the MAX78000FTHR CN1 (USB/PWR) connector.
-   Open a terminal application on the PC and connect to the MAX78000FTHR console UART at 115200, 8-N-1.

## Expected Output

The console UART of the MAX78000FTHR will output these messages:

```
MAX78000FTHR I2C ADXL359 demo.
x:-0.02  y: 0.02  z: 0.99
```
