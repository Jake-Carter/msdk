#ifndef __CAMERA_UTIL_H__
#define __CAMERA_UTIL_H__

#define CAMERA_FREQ (8.330 * 1000 * 1000)

#define IMAGE_XRES 320
#define IMAGE_YRES 240

#include "mxc.h"

int initialize_camera(void);
void display_camera(void);
void load_camera_input(void);
void stream_input_row(uint8_t *data, uint32_t w);

#endif