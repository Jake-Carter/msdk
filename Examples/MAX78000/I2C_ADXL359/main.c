/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mxc.h"
#include "adxl359.h"
#include "board.h"
#include "led.h"

#define I2C_INST MXC_I2C1
#define I2C_FREQ 400000

/*
  Print message and blink LEDs until reset.
*/
void blink_halt(char *msg)
{
    puts(msg);
    puts("Reset to restart application.");

    for (;;) {
        MXC_Delay(MXC_DELAY_MSEC(250));
        LED_Toggle(LED1);
    }
}

int adxl359_config(void)
{
    int result;

    result = 0;
    read_fifo_data();
    read_fifo_entries();
    result |= adxl359_enable_activity(0x07);
    //result |= adxl359_set_low_thresh(0x32);
    result |= adxl359_set_power_control(0x02);

    return result;
}

int main(void)
{
    int8_t axis_data[9];

    MXC_ICC_Enable(MXC_ICC0);
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    setbuf(stdout, NULL);
    printf("\nMAX78000FTHR I2C ADXL359 demo.\n");

    if (MXC_I2C_Init(I2C_INST, 1, 0) != E_NO_ERROR) {
        blink_halt("Trouble initializing I2C instance.");
    }

    MXC_I2C_SetFrequency(I2C_INST, I2C_FREQ);

    if (adxl359_init(I2C_INST) != E_NO_ERROR) {
        blink_halt("Trouble initializing ADXL359.");
    }

    if (adxl359_config() != E_NO_ERROR) {
        blink_halt("Trouble configuring ADXL359.");
    }

    uint8_t statval = 0x00;
    while(1){
        statval = read_status_register();
         MXC_Delay(MXC_DELAY_SEC(1));
        if (statval & 0x01){
            statval = 0x00;
            if (adxl359_get_axis_data(axis_data) != E_NO_ERROR) {
                blink_halt("Trouble reading ADXL359.");
            }
            printf("\rx:% d  y:%d  z:%d\n\r", axis_data[0], axis_data[3], axis_data[6]);
        }
    }
}
