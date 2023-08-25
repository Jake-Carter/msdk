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

/*
  adxl359.c

  Analog Devices ADXL359 driver.
*/

#include <stdint.h>
#include <stdlib.h>
#include "mxc.h"
#include "adxl359.h"

// I2C address with pin ALT_ADDRESS/SDO pulled low
#define adxl359_ADDR 0x1D

#define DEVID 0xAD

#define DEVID_REG 0x00
#define OFSX_REG 0x1E // X-axis offset
#define OFSY_REG 0x20 // Y-axis offset
#define OFSZ_REG 0x22 // Z-axis offset
#define DATAX0_REG 0x08 // X-Axis Data
#define POWER_CTL_REG 0x2D

static mxc_i2c_req_t i2c_req;

static inline int reg_write(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = { reg, val };

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = sizeof(buf);
    i2c_req.rx_len = 0;

    return MXC_I2C_MasterTransaction(&i2c_req);
}

static inline int reg_read(uint8_t reg, uint8_t *dat)
{
    uint8_t buf[1] = { reg };

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = sizeof(buf);
    i2c_req.rx_buf = dat;
    i2c_req.rx_len = 1;

    return MXC_I2C_MasterTransaction(&i2c_req);
}

int adxl359_get_axis_data(int8_t *ptr)
{
    uint8_t buf[1] = { DATAX0_REG };

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = sizeof(buf);
    i2c_req.rx_buf = (uint8_t *)ptr;
    i2c_req.rx_len = 9;

    return MXC_I2C_MasterTransaction(&i2c_req);
}


int adxl359_enable_activity(uint8_t en_act)
{
    int result;
    uint8_t reg;
    printf("en_act - 0x%X\n\r", en_act);
    if ((result = reg_read(0x24, &reg)) != E_NO_ERROR)
        return result;
    reg &= ~0xFF;
    reg |= en_act;
    return reg_write(0x24, reg);
}

int adxl359_set_low_thresh(uint8_t thresh)
{
    int result;
    uint8_t reg;
    printf("low thresh - 0x%X\n\r", thresh);
    if ((result = reg_read(0x26, &reg)) != E_NO_ERROR)
        return result;
    reg &= ~0xFF;
    reg |= thresh;
    return reg_write(0x26, reg);
}

void read_fifo_data()
{
    int result;
    uint8_t reg;

    (result = reg_read(0x11, &reg));
    printf("Reg value in Fifo DATA is 0x%X \n\r", reg);
}
uint8_t read_status_register(){
    int result;
    uint8_t reg;

    (result = reg_read(0x04, &reg));
    //printf("Reg value in Fifo DATA is 0x%X \n\r", reg);
    printf("Value read from the status register is 0x%x \n\r", reg);
    return reg;
}
void read_fifo_entries()
{
    int result;
    uint8_t reg;

    (result = reg_read(0x05, &reg));
    printf("Reg value in Fifo ENTRIES is 0x%X \n\r", reg);
}

int adxl359_set_power_control(uint8_t pwr)
{
    int result;
    uint8_t reg;

    if ((result = reg_read(POWER_CTL_REG, &reg)) != E_NO_ERROR)
        return result;
    reg &= ~0xFF;
    reg |= pwr;
    return reg_write(POWER_CTL_REG, reg);
}

int adxl359_set_offsets(const int16_t *offs)
{
    uint8_t buf[2] = { OFSX_REG, (offs[0]>>8)&0xFF};
    i2c_req.tx_buf = buf;
    i2c_req.tx_len = sizeof(buf);
    i2c_req.rx_len = 0;
    if (MXC_I2C_MasterTransaction(&i2c_req) != 0){
        printf("Error - 1 \n\r");
    }


    uint8_t buf1[2] = { OFSX_REG+1, (offs[0])&0xFF};
    i2c_req.tx_buf = buf1;
    i2c_req.tx_len = sizeof(buf1);
    i2c_req.rx_len = 0;
    if (MXC_I2C_MasterTransaction(&i2c_req) != 0){
        printf("Error - 2 \n\r");
    }



    uint8_t buf2[2] = { OFSY_REG, (offs[1]>>8)&0xFF};
    i2c_req.tx_buf = buf2;
    i2c_req.tx_len = sizeof(buf2);
    i2c_req.rx_len = 0;
    if (MXC_I2C_MasterTransaction(&i2c_req) != 0){
        printf("Error - 3 \n\r");
    }


    uint8_t buf3[2] = { OFSY_REG+1, (offs[1])&0xFF};
    i2c_req.tx_buf = buf3;
    i2c_req.tx_len = sizeof(buf3);
    i2c_req.rx_len = 0;
    if (MXC_I2C_MasterTransaction(&i2c_req) != 0){
        printf("Error - 4 \n\r");
    }

    uint8_t buf4[2] = { OFSZ_REG, (offs[2]>>8)&0xFF};
    i2c_req.tx_buf = buf4;
    i2c_req.tx_len = sizeof(buf4);
    i2c_req.rx_len = 0;
    if (MXC_I2C_MasterTransaction(&i2c_req) != 0){
        printf("Error - 5 \n\r");
    }


    uint8_t buf5[2] = { OFSZ_REG+1, (offs[2])&0xFF};
    i2c_req.tx_buf = buf5;
    i2c_req.tx_len = sizeof(buf5);
    i2c_req.rx_len = 0;
    if (MXC_I2C_MasterTransaction(&i2c_req) != 0){
        printf("Error - 6 \n\r");
    }

    return 0;
}

int adxl359_init(mxc_i2c_regs_t *i2c_inst)
{
    int result;
    uint8_t id;

    if (!i2c_inst){
        return E_NULL_PTR;
    }

    i2c_req.i2c = i2c_inst;
    i2c_req.addr = adxl359_ADDR;
    i2c_req.restart = 0;
    i2c_req.callback = NULL;

    if ((result = reg_read(DEVID_REG, &id)) != E_NO_ERROR){
        printf("2 - - \n\r");
        return result;
    }
    printf("ID - - %u\n\r", id);
    if (id != DEVID){
        printf("3 - - \n\r");
        return E_NOT_SUPPORTED;
    }
    return E_NO_ERROR;
}
