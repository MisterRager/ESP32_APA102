#include "SpiDevice.h"

#include <algorithm>
#include <cstring>
#include <esp_heap_caps.h>
#include <esp_log.h>

#define TAG "spi_device"

namespace apa102 {

SpiDevice::SpiDevice(
    const std::shared_ptr<SpiBus> bus,
    const spi_device_interface_config_t &config) : bus(bus)
{
    ESP_ERROR_CHECK(spi_bus_add_device(bus->spiDevice, &config, &spi));
}

SpiDevice::~SpiDevice()
{
    ESP_ERROR_CHECK(spi_bus_remove_device(spi));
}

spi_device_interface_config_t SpiDevice::configure(int speed)
{
    spi_device_interface_config_t config = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = speed,
        .input_delay_ns = 0,
        .spics_io_num = -1,
        .flags = 0,
        .queue_size = 2,
        .pre_cb = nullptr,
        .post_cb = &SpiDevice::postTransactionHook};

    return config;
}

esp_err_t SpiDevice::sendData(
    size_t length,
    const uint8_t *buffer)
{
    ESP_LOGV(TAG, "Sending %d bytes of on SPI", length);

    spi_transaction_t *transaction = static_cast<spi_transaction_t *>(
        heap_caps_malloc(sizeof(spi_transaction_t), MALLOC_CAP_DMA));

    memset(transaction, 0, sizeof(spi_transaction_t));

    transaction->tx_buffer = buffer;
    transaction->length = length * 8;

    esp_err_t res = spi_device_queue_trans(spi, transaction, portMAX_DELAY);

    return res;
}

void SpiDevice::postTransactionHook(spi_transaction_t * transaction) {
    free(static_cast<void *>(transaction));
}

// namespace apa102
}
