#include "APA102.h"

#define APA102_FIRST_LED_OFFSET 4
#define APA102_FRAME_PREAMBLE 224 // 11100000

APA102::APA102(uint8_t mosiPin, uint8_t clockPin, uint8_t ledsCount) {
    count = ledsCount;

    bufferSize = 
        4 + // Start Frame: 4 bytes of 0's
        count * 4 + // Each LED has 4 bytes of data
        (count / 4) + 1; // End frame of just 1's - needs to be at least n/2 bits

    busConfig.miso_io_num = -1;
    busConfig.mosi_io_num = mosiPin;
    busConfig.sclk_io_num = clockPin;
    busConfig.max_transfer_sz = bufferSize;
    busConfig.quadwp_io_num = -1;
    busConfig.quadhd_io_num = -1;

    deviceConfig.clock_speed_hz = 12 * 1000 * 1000; // Clock speed: 12MHz
    deviceConfig.mode = 0;
    deviceConfig.address_bits = 0;
    deviceConfig.command_bits = 0;
    deviceConfig.dummy_bits = 0;
    deviceConfig.cs_ena_posttrans = 0;
    deviceConfig.cs_ena_pretrans = 0;
    deviceConfig.spics_io_num = -1;
    deviceConfig.duty_cycle_pos = 0;
    deviceConfig.queue_size = 1;
    deviceConfig.post_cb = NULL;
    deviceConfig.pre_cb = NULL;

    transaction.length = bufferSize;
}

void APA102::init() {
    esp_err_t ret;
    if (!hasInit) {
        ret = spi_bus_initialize(HSPI_HOST, &busConfig, 1);
        ESP_ERROR_CHECK(ret);

        ret = spi_bus_add_device(HSPI_HOST, &deviceConfig, &deviceHandle);
        ESP_ERROR_CHECK(ret);

        outputBuffer = (uint8_t *) heap_caps_malloc(sizeof(uint8_t) * bufferSize, MALLOC_CAP_DMA);

        // Preamble: 4 bytes of 0
        outputBuffer[0] = 0;
        outputBuffer[1] = 0;
        outputBuffer[2] = 0;
        outputBuffer[3] = 0;

        // Postamble: at least n/2 bits of 1
        for (int k = APA102_FIRST_LED_OFFSET + 4 * count; k < bufferSize; k++) {
            outputBuffer[k] = 255; // 11111111
        }

        transaction.tx_buffer = outputBuffer;

        hasInit = true;
    }
}

APA102::~APA102() {
    if (hasInit) {
        spi_bus_remove_device(deviceHandle);
        heap_caps_free(outputBuffer);
    }
}

void APA102::setPixel(uint8_t index, PixelInfo pixel) {
    int bytesOffset = APA102_FIRST_LED_OFFSET + index * 4;
    
    outputBuffer[bytesOffset] = APA102_FRAME_PREAMBLE | pixel.brightness;
    outputBuffer[bytesOffset + 1] = pixel.blue;
    outputBuffer[bytesOffset + 2] = pixel.green;
    outputBuffer[bytesOffset + 3] = pixel.red;
}

void APA102::flush() {
    init();
    spi_device_queue_trans(deviceHandle, transaction, portMAX_DELAY);
}