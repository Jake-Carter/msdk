#include "camera_utils.h"

#include "mxc.h"
#include "mxc_delay.h"
#include "camera.h"
#include "dma.h"
#include "led.h"

#include "tft_ssd2119.h"

int slaveAddress;
int id;
int dma_channel;

uint8_t *data = NULL;
uint8_t data565[IMAGE_XRES * 2];
stream_stat_t *stat;

int initialize_camera(void)
{
    int ret = 0;

    // Initialize DMA for camera interface
    MXC_DMA_Init();
    dma_channel = MXC_DMA_AcquireChannel();

    // Initialize the camera driver.
    camera_init(CAMERA_FREQ);
    printf("\n\nCamera Example\n");

    slaveAddress = camera_get_slave_address();
    printf("Camera I2C slave address: %02x\n", slaveAddress);

    // Obtain the manufacturer ID of the camera.
    ret = camera_get_manufacture_id(&id);

    if (ret != STATUS_OK) {
        printf("Error returned from reading camera id. Error %d\n", ret);
        return -1;
    }

    printf("Camera ID detected: %04x\n", id);

    printf("Init Camera\n");

    // Setup the camera image dimensions, pixel format and data acquiring details.
    ret = camera_setup(IMAGE_XRES, IMAGE_YRES, PIXFORMAT_RGB565, FIFO_FOUR_BYTE, STREAMING_DMA,
                       dma_channel);

    if (ret != STATUS_OK) {
        printf("Error returned from setting up camera. Error %d\n", ret);
        return -1;
    }

    MXC_Delay(SEC(1));

    camera_write_reg(0x11, 0x3); // (JC): Prescalers below 3 cause overflow.

// TODO(JC): Not sure what this does or if this is needed
//     // make the scale ratio of camera input size the same as output size, make is faster and regular
//     camera_write_reg(0xc8, 0x1);
//     camera_write_reg(0xc9, 0x60);
//     camera_write_reg(0xca, 0x1);
//     camera_write_reg(0xcb, 0x60);

    return ret;
}

void load_camera_input(void){
    uint32_t imgLen;
    uint32_t w, h;
    uint8_t *raw;

    LED_Off(LED2);

    union {
        uint32_t w;
        uint8_t b[4];
    } m;

    camera_start_capture_image();

    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &imgLen, &w, &h);
    printf("Capture Image Size: %dx%d\n", w, h);

    // Get image line by line
    for (int row = 0; row < h; row++) {
        // Wait until camera streaming buffer is full
        while ((data = get_camera_stream_buffer()) == NULL) {
            if (camera_is_image_rcv()) {
                break;
            }
        }

        // LED_Toggle(LED1);

        for (int i = 0; i < camera_get_stream_buffer_size(); i += 2) {
            // RGB565 to packed 24-bit RGB
            m.b[0] = (*(data + i) & 0xF8); // Red
            m.b[1] = (*(data + i) << 5) | ((*((data + i) + 1) & 0xE0) >> 3); // Green
            m.b[2] = (*((data + i) + 1) << 3); // Blue
            // Remove the following line if there is no risk that the source would overrun the FIFO:
            while (((*((volatile uint32_t *) 0x50000004) & 1)) != 0); // Wait for FIFO 0
            *((volatile uint32_t *) 0x50000008) = m.w ^ 0x00808080U; // Write FIFO 0
        }

        // LED_Toggle(LED1);
        release_camera_stream_buffer();
    }

    stat = get_camera_stream_statistic();
    if (stat->overflow_count > 0) {
        printf("OVERFLOW (CAMERA) = %d\n", stat->overflow_count);
        LED_On(LED2); // Turn on red LED if overflow detected

        while (1) {}
    }
}

void display_camera(void)
{
    uint32_t imgLen;
    uint32_t w, h;
    uint8_t *raw;

    LED_Off(LED2);

    camera_start_capture_image();

    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &imgLen, &w, &h);
    printf("Capture Image Size: %dx%d\n", w, h);

    // Get image line by line
    for (int row = 0; row < h; row++) {
        // Wait until camera streaming buffer is full
        while ((data = get_camera_stream_buffer()) == NULL) {
            if (camera_is_image_rcv()) {
                break;
            }
        }

        LED_Toggle(LED1);
        MXC_TFT_ShowImageCameraRGB565(0, row, data, w, 1);

        LED_Toggle(LED1);
        release_camera_stream_buffer();
    }

    stat = get_camera_stream_statistic();
    if (stat->overflow_count > 0) {
        printf("OVERFLOW DISP = %d\n", stat->overflow_count);
        LED_On(LED2); // Turn on red LED if overflow detected

        while (1) {}
    }
}