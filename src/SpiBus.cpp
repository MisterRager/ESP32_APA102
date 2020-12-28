#include "SpiBus.h"
#include "esp_log.h"

#define TAG "spi_bus"

namespace apa102 {

SpiBus::SpiBus(
    const spi_host_device_t spiDevice,
    const spi_bus_config_t &config,
    const int dmaChannel
) : spiDevice(spiDevice) {
    ESP_LOGV(TAG, "Bus is on [mosi %d, sclk %d]", config.mosi_io_num, config.sclk_io_num);
    ESP_ERROR_CHECK(spi_bus_initialize(spiDevice, &config, dmaChannel));
}

SpiBus::~SpiBus() {
    ESP_ERROR_CHECK(spi_bus_free(spiDevice));
}

spi_bus_config_t SpiBus::configure(int mosi, int sclk, int maxTransferSize) {
    spi_bus_config_t config = {
        .mosi_io_num = mosi,
        .miso_io_num = -1,
        .sclk_io_num = sclk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = maxTransferSize,
        .flags = 0,
        .intr_flags = 0};

    return config;
}

// namespace apa102
}
