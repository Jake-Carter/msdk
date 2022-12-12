## Description

The example demonstartes the use of USB composite device with Mass Storage driver class and HID driver class. After doing the required connections given below, run the program and two new devices appear in the device manager, a Portable device and a HID keyboard device. The portable device can be read and written to. Open any text editor in the PC, pressing the switch SW1 on the evkit wil print out 'Maxim Integrated' in the editor one letter at a time.

## Setup
-   Connect a USB cable between the PC and the CN2 (USB/PWR/UART) connector.
-   Open a terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.
-   Connect a USB cable between the PC and the CN1 (USB/PWR) connector. 
-   Default EV kit hardware configuration.

## Expected Output

The Console UART of the device will output these messages:

```
Connect Port 0.7 to Port 4.0
***** MAX32690 USB Composite Device (Keyboard and Mass Storage) Example *****
Waiting for VBUS...
VBUS Connect
Suspended
Bus Reset
Bus Reset
Enumeration complete. Press SW2 to send character.
```