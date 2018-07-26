#include "APA102.h"

APA102::APA102(uint8_t mosiPin, uint8_t clockPin, uint8_t ledsCount) {
    busConfig.miso_io_num = -1;
    busConfig.mosi_io_num = mosiPin;
    busConfig.sclk_io_num = clockPin;
    busConfig.max_transfer_sz = ledsCount * 4 + (ledsCount / 10);
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
    deviceConfig.queue_size = 2;
    deviceConfig.post_cb = NULL;
    deviceConfig.pre_cb = NULL;
}

void APA102::init() {
    esp_err_t ret;
    if (!hasInit) {
        ret = spi_bus_initialize(HSPI_HOST, &busConfig, 1);

        ESP_ERROR_CHECK(ret);

        ret = spi_bus_add_device(HSPI_HOST, &deviceConfig, &deviceHandle);

        hasInit = true;
    }
}

APA102::~APA102() {
    if (hasInit) {
        spi_bus_remove_device(deviceHandle);
    }
}
